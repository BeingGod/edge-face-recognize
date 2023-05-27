#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lite/lite.h"

#include <glog/logging.h>

#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);

  ~MainWindow();

private slots:
  void on_btnOpenCam_clicked();

  void on_btnCloseCam_clicked();

  void on_btnStartCapture_clicked();

  void slot_on_timer_timeout();

private:
  Ui::MainWindow*  ui;
  cv::VideoCapture capture;
  //    QTimer *timer;

public:
  /**
   * @brief 主函数
   *
   * @param frame 输入图像
   * @param vis 可视化结果
   * @param sign_students 到场学生
   * @return true
   * @return false
   */
  bool detect(cv::Mat& frame, cv::Mat& vis, std::vector<std::string>& sign_students);

  /**
   * @brief Get the Classroom Id object
   *
   * @return int
   */
  int getClassroomId() { return _classroom_id; }

  /**
   * @brief Get the Resonse Msg object
   *
   * @return std::string&
   */
  std::string& getResonseMsg() { return _response_msg; }

private:
  typedef struct DetectModelConfig
  {
    std::string path;
    float       conf_thres;   // 置信度阈值
    float       iou_thres;    // IOU阈值
    int         topk;         // 保留box数
    int         width;
    int         height;

    void print()
    {
      LOG(INFO) << "===========Detect Model Config==========";
      LOG(INFO) << "Model Path: " << path;
      LOG(INFO) << "Confidence Threshold: " << conf_thres;
      LOG(INFO) << "IOU Threshold: " << iou_thres;
      LOG(INFO) << "TopK: " << topk;
      LOG(INFO) << "Width: " << width;
      LOG(INFO) << "Height: " << height;
      LOG(INFO) << "========================================";
    }
  } DetectModelConfig;   // 人脸检测模型配置

  typedef struct RecognizeModelConfig
  {
    std::string path;
    int         width;
    int         height;
    float       sim_threshold;   // 相似度阈值
    void        print()
    {
      LOG(INFO) << "=========Recognize Model Config=========";
      LOG(INFO) << "Model Path: " << path;
      LOG(INFO) << "Width: " << width;
      LOG(INFO) << "Height: " << height;
      LOG(INFO) << "Similarity Threshold: " << sim_threshold;
      LOG(INFO) << "========================================";
    }
  } RecognizeModelConfig;   // 人脸识别模型配置

  /**
   * @brief 读取配置文件
   *
   * @param config_path
   */
  void loadConfig(const std::string& config_path);

  /**
   * @brief 初始化人脸检测模型
   *
   * @param thread_nums
   */
  void initFaceDetectModel(const int thread_nums);

  /**
   * @brief 初始化人脸识别模型
   *
   * @param thread_nums
   */
  void initFaceRecognizeModel(const int thread_nums);

  /**
   * @brief 检测人脸
   *
   * @param img
   * @param vis
   * @param detected_boxes
   */
  void detectFace(const cv::Mat& img, cv::Mat& vis, std::vector<lite::types::Boxf>& detected_boxes);

  /**
   * @brief 人脸编码
   *
   * @param img
   * @param detected_box
   * @param face_content
   */
  void encodeFace(const cv::Mat& img, const lite::types::Boxf& detected_box,
                  lite::types::FaceContent& face_content);

  /**
   * @brief 更新考勤状态
   *
   * @param face_contents
   * @param students
   * @return true
   * @return false
   */
  bool requestUpdateAttendenceStatus(const std::vector<lite::types::FaceContent>& face_contents,
                                     std::vector<std::string>                     students);

private:
  uint32_t _classroom_id;

  std::string _response_msg;

  std::string _base_url;

  RecognizeModelConfig _recognize_model_config;

  DetectModelConfig _detect_model_config;

  lite::ncnn::cv::face::detect::RetinaFace* _face_detect_model = nullptr;

  lite::ncnn::cv::faceid::MobileFaceNet* _face_recognize_model = nullptr;
};

#endif   // MAINWINDOW_H
