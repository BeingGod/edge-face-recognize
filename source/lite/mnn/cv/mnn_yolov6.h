//
// Created by DefTruth on 2022/6/25.
//

#ifndef LITE_AI_TOOLKIT_MNN_CV_MNN_YOLOV6_H
#define LITE_AI_TOOLKIT_MNN_CV_MNN_YOLOV6_H

#include "lite/mnn/core/mnn_core.h"

namespace mnncv {
class MNNYOLOv6 : public BasicMNNHandler
{
public:
  explicit MNNYOLOv6(const std::string& _mnn_path, unsigned int _num_threads = 1);   //
  ~MNNYOLOv6() override = default;

private:
  // nested classes
  typedef struct GridAndStride
  {
    int grid0;
    int grid1;
    int stride;
  } YOLOv6Anchor;

  // nested classes
  typedef struct
  {
    float r;
    int   dw;
    int   dh;
    int   new_unpad_w;
    int   new_unpad_h;
    bool  flag;
  } YOLOv6ScaleParams;

private:
  const float mean_vals[3]    = {0.f, 0.f, 0.f};   // RGB
  const float norm_vals[3]    = {1.0 / 255.f, 1.0 / 255.f, 1.0 / 255.f};
  const char* class_names[80] = {"person",        "bicycle",      "car",
                                 "motorcycle",    "airplane",     "bus",
                                 "train",         "truck",        "boat",
                                 "traffic light", "fire hydrant", "stop sign",
                                 "parking meter", "bench",        "bird",
                                 "cat",           "dog",          "horse",
                                 "sheep",         "cow",          "elephant",
                                 "bear",          "zebra",        "giraffe",
                                 "backpack",      "umbrella",     "handbag",
                                 "tie",           "suitcase",     "frisbee",
                                 "skis",          "snowboard",    "sports ball",
                                 "kite",          "baseball bat", "baseball glove",
                                 "skateboard",    "surfboard",    "tennis racket",
                                 "bottle",        "wine glass",   "cup",
                                 "fork",          "knife",        "spoon",
                                 "bowl",          "banana",       "apple",
                                 "sandwich",      "orange",       "broccoli",
                                 "carrot",        "hot dog",      "pizza",
                                 "donut",         "cake",         "chair",
                                 "couch",         "potted plant", "bed",
                                 "dining table",  "toilet",       "tv",
                                 "laptop",        "mouse",        "remote",
                                 "keyboard",      "cell phone",   "microwave",
                                 "oven",          "toaster",      "sink",
                                 "refrigerator",  "book",         "clock",
                                 "vase",          "scissors",     "teddy bear",
                                 "hair drier",    "toothbrush"};

  enum NMS
  {
    HARD   = 0,
    BLEND  = 1,
    OFFSET = 2
  };
  static constexpr const unsigned int max_nms = 30000;

private:
  void initialize_pretreat();   //

  void transform(const cv::Mat& mat_rs) override;   // without resize

  void resize_unscale(const cv::Mat& mat, cv::Mat& mat_rs, int target_height, int target_width,
                      YOLOv6ScaleParams& scale_params);

  void generate_anchors(const int target_height, const int target_width, std::vector<int>& strides,
                        std::vector<YOLOv6Anchor>& anchors);

  void generate_bboxes(const YOLOv6ScaleParams&                   scale_params,
                       std::vector<types::Boxf>&                  bbox_collection,
                       const std::map<std::string, MNN::Tensor*>& output_tensors,
                       float score_threshold, int img_height,
                       int img_width);   // rescale & exclude

  void nms(std::vector<types::Boxf>& input, std::vector<types::Boxf>& output, float iou_threshold,
           unsigned int topk, unsigned int nms_type);

public:
  void detect(const cv::Mat& mat, std::vector<types::Boxf>& detected_boxes,
              float score_threshold = 0.25f, float iou_threshold = 0.45f, unsigned int topk = 100,
              unsigned int nms_type = NMS::OFFSET);
};
}   // namespace mnncv

#endif   // LITE_AI_TOOLKIT_MNN_CV_MNN_YOLOV6_H
