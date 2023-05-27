//
// Created by DefTruth on 2021/7/31.
//

#ifndef LITE_AI_ORT_CV_RETINAFACE_H
#define LITE_AI_ORT_CV_RETINAFACE_H

#include "lite/ort/core/ort_core.h"

namespace ortcv {
// reference: Pytorch_Retinaface python implementation.
// https://github.com/biubug6/Pytorch_Retinaface/blob/master/layers/functions/prior_box.py
class RetinaFace : public BasicOrtHandler
{
public:
  explicit RetinaFace(const std::string& _onnx_path, unsigned int _num_threads = 1)
    : BasicOrtHandler(_onnx_path, _num_threads){};

  ~RetinaFace() override = default;

private:
  // nested classes
  struct RetinaAnchor
  {
    float cx;
    float cy;
    float s_kx;
    float s_ky;
  };

private:
  const float                   mean_vals[3]  = {104.f, 117.f, 123.f};   // bgr order
  const float                   scale_vals[3] = {1.f, 1.f, 1.f};
  const float                   variance[2]   = {0.1f, 0.2f};
  std::vector<int>              steps         = {8, 16, 32};
  std::vector<std::vector<int>> min_sizes     = {{16, 32}, {64, 128}, {256, 512}};

  enum NMS
  {
    HARD   = 0,
    BLEND  = 1,
    OFFSET = 2
  };
  static constexpr const unsigned int max_nms = 30000;

private:
  Ort::Value transform(const cv::Mat& mat) override;   //

  void generate_anchors(const int target_height, const int target_width,
                        std::vector<RetinaAnchor>& anchors);

  void generate_bboxes(std::vector<types::Boxf>& bbox_collection,
                       std::vector<Ort::Value>& output_tensors, float score_threshold,
                       float img_height,
                       float img_width);   // rescale & exclude

  void nms(std::vector<types::Boxf>& input, std::vector<types::Boxf>& output, float iou_threshold,
           unsigned int topk, unsigned int nms_type);

public:
  void detect(const cv::Mat& mat, std::vector<types::Boxf>& detected_boxes,
              float score_threshold = 0.25f, float iou_threshold = 0.45f, unsigned int topk = 400,
              unsigned int nms_type = NMS::HARD);
};
}   // namespace ortcv


#endif   // LITE_AI_ORT_CV_RETINAFACE_H
