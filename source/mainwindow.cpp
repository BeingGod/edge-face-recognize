#include "mainwindow.h"
#include "request/request.h"
#include "status_code.h"
#include "ui_mainwindow.h"
#include "utils/base64.h"
#include "utils/utils.h"

#include <QDateTime>
#include <QDebug>
#include <QImage>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

#include <cjson/cJSON.h>
#include <glog/logging.h>


static std::string formatEmbedding(const std::vector<float>& embedding)
{
  std::string s("");
  for (size_t i = 0; i < embedding.size(); i++) {
    if (i != embedding.size()) {
      s += std::to_string(embedding[i]) + ",";
    }
    else {
      s += std::to_string(embedding[i]);
    }
  }

  return s;
}

static std::string getFormatDate()
{
  time_t     now;
  struct tm* tm_now;
  char       datetime[200];

  time(&now);
  tm_now = localtime(&now);
  strftime(datetime, 200, "%Y-%m-%d %H:%M:%S", tm_now);

  return std::string(datetime);
}

// Mat转成QImage
static QImage MatImageToQt(const cv::Mat& src)
{
  // CV_8UC1 8位无符号的单通道---灰度图片
  if (src.type() == CV_8UC1) {
    // 使用给定的大小和格式构造图像
    // QImage(int width, int height, Format format)
    cv::Mat dst(cv::Size(640, 480), CV_8UC3);
    cvtColor(src, dst, cv::COLOR_GRAY2RGB);
    QImage qImage((const uchar*)dst.data, dst.cols, dst.rows, QImage::Format_RGB888);
    return qImage.rgbSwapped();
  }
  // 为3通道的彩色图片
  else if (src.type() == CV_8UC3) {
    // 得到图像的的首地址
    const uchar* pSrc = (const uchar*)src.data;
    // 以src构造图片
    QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_RGB888);
    // 在不改变实际图像数据的条件下，交换红蓝通道
    return qImage.rgbSwapped();
  }
  // 四通道图片，带Alpha通道的RGB彩色图像
  else if (src.type() == CV_8UC4) {
    const uchar* pSrc = (const uchar*)src.data;
    QImage       qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
    // 返回图像的子区域作为一个新图像
    return qImage.copy();
  }
  else {
    return QImage();
  }
}

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  int classroom_id = getClassroomId();
  ui->lblClassroomId->setText(QString::fromStdString(std::to_string(classroom_id)));

  const int threads = 1;

  std::string detect_model_path, recognize_model_path;
  // TODO
  loadConfig("./config.json");

  LOG(INFO) << "========================================";
  LOG(INFO) << "Initing face detect model";
  initFaceDetectModel(threads);
  LOG(INFO) << "Face detect model init success!";
  LOG(INFO) << "========================================";


  LOG(INFO) << "========================================";
  LOG(INFO) << "Initing face recognize model";
  initFaceRecognizeModel(threads);
  LOG(INFO) << "Face recognize model init success!";
  LOG(INFO) << "========================================";


  LOG(INFO) << "===========GUI Init Success!============";
}

MainWindow::~MainWindow()
{
  if (capture.isOpened()) capture.release();
  if (_face_detect_model) {
    delete _face_detect_model;
    _face_detect_model = nullptr;
  }
  if (_face_recognize_model) {
    delete _face_recognize_model;
    _face_recognize_model = nullptr;
  }
}

void MainWindow::on_btnOpenCam_clicked()
{
  if (capture.open(0, cv::CAP_V4L2)) {
    capture.set(cv::CAP_PROP_FPS, 30);
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    LOG(INFO) << "cam is opened";
    ui->lblCapStatus->setText("opened");

    //        timer->start(33);
  }
  else {
    LOG(INFO) << "cam is not opened";
    capture.release();
  }
}


void MainWindow::on_btnCloseCam_clicked()
{
  ui->lblCapStatus->setText("closed");
  capture.release();
}

void MainWindow::on_btnStartCapture_clicked()
{
  //    timer->start(500);
  cv::Mat frame;
  QImage  qimage;
  if (!capture.isOpened()) {
    QMessageBox::critical(NULL, "error", "not open camera", QMessageBox::Yes, QMessageBox::Yes);
    return;
  }

  capture >> frame;
  qimage = MatImageToQt(frame);
  ui->lblImage->setPixmap(QPixmap::fromImage(qimage));
  cv::Mat                  vis;
  std::vector<std::string> signStudents;
  detect(frame, vis, signStudents);

  std::string response_msg = getResonseMsg();

  qimage = MatImageToQt(vis);
  ui->lblImage->setPixmap(QPixmap::fromImage(qimage));
  ui->lblResponseMsg->setText(QString::fromStdString(response_msg));
  ui->lblSignStudentNum->setText(QString::fromStdString(std::to_string(signStudents.size())));
}

void MainWindow::slot_on_timer_timeout()
{
  //    LOG(INFO) << "timer..."

  cv::Mat frame;
  capture >> frame;

  if (frame.empty()) {
    LOG(INFO) << "frame is empty";
  }
  else {
    LOG(INFO) << frame.size;
    QImage qimage = MatImageToQt(frame);
    ui->lblImage->setPixmap(QPixmap::fromImage(qimage));
  }
}

void MainWindow::loadConfig(const std::string& config_path)
{
  char* txt = utils::readFile(config_path);

  cJSON* root = cJSON_Parse(txt);

  if (root == NULL) {
    LOG(ERROR) << "Read config.json failed!";
    exit(EXIT_CONFIG_LOAD_FAIL);
  }

  cJSON* detect_model_node    = cJSON_GetObjectItem(root, "detect_model");
  cJSON* recognize_model_node = cJSON_GetObjectItem(root, "recognize_model");
  cJSON* classroom_id_node    = cJSON_GetObjectItem(root, "classroom_id");
  cJSON* base_url_node        = cJSON_GetObjectItem(root, "base_url");

  _detect_model_config.path =
    std::string(cJSON_GetObjectItem(detect_model_node, "path")->valuestring);
  _detect_model_config.conf_thres =
    cJSON_GetObjectItem(detect_model_node, "conf_thres")->valuedouble;
  _detect_model_config.iou_thres = cJSON_GetObjectItem(detect_model_node, "iou_thres")->valuedouble;
  _detect_model_config.topk      = cJSON_GetObjectItem(detect_model_node, "topk")->valueint;
  _detect_model_config.width     = cJSON_GetObjectItem(detect_model_node, "width")->valueint;
  _detect_model_config.height    = cJSON_GetObjectItem(detect_model_node, "height")->valueint;

  _recognize_model_config.path =
    std::string(cJSON_GetObjectItem(recognize_model_node, "path")->valuestring);
  _recognize_model_config.width  = cJSON_GetObjectItem(recognize_model_node, "width")->valueint;
  _recognize_model_config.height = cJSON_GetObjectItem(recognize_model_node, "height")->valueint;
  _recognize_model_config.sim_threshold =
    cJSON_GetObjectItem(recognize_model_node, "sim_threshold")->valuedouble;

  _classroom_id = classroom_id_node->valueint;
  _base_url     = std::string(base_url_node->valuestring);

  _detect_model_config.print();
  _recognize_model_config.print();

  cJSON_Delete(root);
  root = nullptr;

  free(txt);
  txt = nullptr;
}

void MainWindow::initFaceDetectModel(const int threads)
{
  std::string param_path = _detect_model_config.path + ".param";
  std::string bin_path   = _detect_model_config.path + ".bin";

  _face_detect_model = new lite::ncnn::cv::face::detect::RetinaFace(
    param_path, bin_path, threads, _detect_model_config.width, _detect_model_config.height);
}

void MainWindow::initFaceRecognizeModel(const int threads)
{
  std::string param_path = _recognize_model_config.path + ".param";
  std::string bin_path   = _recognize_model_config.path + ".bin";

  _face_recognize_model = new lite::ncnn::cv::faceid::MobileFaceNet(param_path, bin_path, threads);
}

void MainWindow::detectFace(const cv::Mat& img, cv::Mat& vis,
                            std::vector<lite::types::Boxf>& detected_boxes)
{
  // LOG(INFO) << "Detecting face..."
  _face_detect_model->detect(img,
                             detected_boxes,
                             _detect_model_config.conf_thres,
                             _detect_model_config.iou_thres,
                             _detect_model_config.topk);

  vis = lite::utils::draw_boxes(img, detected_boxes);

  LOG(INFO) << "Retinaface Detected Face Num: " << detected_boxes.size();
}

void MainWindow::encodeFace(const cv::Mat& img, const lite::types::Boxf& detected_box,
                            lite::types::FaceContent& face_content)
{
  LOG(INFO) << "Encoding face...";
  cv::Rect rect = detected_box.rect();

  // 边界处理
  rect.x = rect.x < 0 ? 0 : rect.x;
  rect.y = rect.y < 0 ? 0 : rect.y;
  rect.x = rect.x >= img.cols ? img.cols : rect.x;
  rect.y = rect.y >= img.rows ? img.rows : rect.y;

  cv::Mat face = img(rect);

  _face_recognize_model->detect(face, face_content);
}

bool MainWindow::requestUpdateAttendenceStatus(
  const std::vector<lite::types::FaceContent>& face_contents, std::vector<std::string> students)
{
  const std::string url = "/attendance/update";

  LOG(INFO) << "===========Requesting============";

  std::vector<std::string> base64_face_contents;

  for (auto& face_content : face_contents) {
    const std::string formatedEmbedding = formatEmbedding(face_content.embedding);
    // 将embedding进行base64编码
    base64_face_contents.emplace_back(
      base64_encode(reinterpret_cast<const unsigned char*>(formatedEmbedding.c_str()),
                    formatedEmbedding.length(),
                    false));
  }

  std::string now = getFormatDate();
  LOG(INFO) << "Request Date: " << now;

  cJSON* root = cJSON_CreateObject();
  cJSON_AddArrayToObject(root, "faceIds");
  cJSON_AddStringToObject(root, "date", now.c_str());
  cJSON_AddNumberToObject(root, "classroomId", _classroom_id);
  cJSON_AddNumberToObject(root, "threshold", _recognize_model_config.sim_threshold);

  cJSON* face_ids_node = cJSON_GetObjectItem(root, "faceIds");

  for (std::string& base64_face_content : base64_face_contents) {
    cJSON_AddItemToArray(face_ids_node, cJSON_CreateString(base64_face_content.c_str()));
  }



  LOG(INFO) << "=================================";

  return true;
}

bool MainWindow::detect(cv::Mat& frame, cv::Mat& vis, std::vector<std::string>& sign_students)
{
  std::vector<lite::types::Boxf>        detected_boxes;
  std::vector<lite::types::FaceContent> face_contents;

  detectFace(frame, vis, detected_boxes);

  if (detected_boxes.size() == 0) {   // 检测人脸
    LOG(WARNING) << "Not detected face !";
    return false;
  }

  for (auto& box : detected_boxes) {   // 遍历人脸并进行编码
    lite::types::FaceContent face_content;
    encodeFace(frame, box, face_content);
    face_contents.emplace_back(face_content);
  }

  // 发送HTTP请求
  return requestUpdateAttendenceStatus(face_contents, sign_students);
}