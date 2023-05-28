#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lite/lite.h"

#include <glog/logging.h>

#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <string>


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
  void onBtnOpenCamClicked();

  void onBtnCloseCamClicked();

  void onBtnStartCaptureClicked();

  void onTimerTimeout();

private:
  Ui::MainWindow*  ui;
  cv::VideoCapture capture;
  QTimer*          timer;

public:
  bool detect(cv::Mat& frame, cv::Mat& vis, std::string& student_name,
              std::string& attendence_status);

  int getClassroomId() { return _classroom_id; }

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

  void loadConfig(const std::string& config_path);

  void initFaceDetectModel(const int thread_nums);

  void initFaceRecognizeModel(const int thread_nums);

  void detectFace(const cv::Mat& img, cv::Mat& vis, std::vector<lite::types::Boxf>& detected_boxes);

  void encodeFace(const cv::Mat& img, const lite::types::Boxf& detected_box,
                  lite::types::FaceContent& face_content);

  bool requestUpdateAttendenceStatus(const lite::types::FaceContent& face_content,
                                     std::string& student_name, std::string& attendence_status);

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
