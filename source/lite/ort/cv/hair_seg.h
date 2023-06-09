//
// Created by DefTruth on 2022/6/19.
//

#ifndef LITE_AI_TOOLKIT_ORT_CV_HAIR_SEG_H
#define LITE_AI_TOOLKIT_ORT_CV_HAIR_SEG_H

#include "lite/ort/core/ort_core.h"

namespace ortcv {
class HairSeg : public BasicOrtHandler
{
public:
  explicit HairSeg(const std::string& _onnx_path, unsigned int _num_threads = 1)
    : BasicOrtHandler(_onnx_path, _num_threads){};

  ~HairSeg() override = default;

private:
  Ort::Value transform(const cv::Mat& mat) override;

  void generate_mask(std::vector<Ort::Value>& output_tensors, const cv::Mat& mat,
                     types::HairSegContent& content, float score_threshold = 0.0f,
                     bool remove_noise = false);

public:
  void detect(const cv::Mat& mat, types::HairSegContent& content, float score_threshold = 0.0f,
              bool remove_noise = false);
};
}   // namespace ortcv

#endif   // LITE_AI_TOOLKIT_ORT_CV_HAIR_SEG_H
