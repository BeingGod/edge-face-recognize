//
// Created by DefTruth on 2021/11/27.
//

#ifndef LITE_AI_TOOLKIT_MNN_CV_MNN_REXNET_EMOTION7_H
#define LITE_AI_TOOLKIT_MNN_CV_MNN_REXNET_EMOTION7_H

#include "lite/mnn/core/mnn_core.h"

namespace mnncv {
class MNNReXNetEmotion7 : public BasicMNNHandler
{
public:
  explicit MNNReXNetEmotion7(const std::string& _mnn_path, unsigned int _num_threads = 1);   //
  ~MNNReXNetEmotion7() override = default;

private:
  const float mean_vals[3]     = {255.f * 0.485f, 255.f * 0.456, 255.f * 0.406f};
  const float norm_vals[3]     = {1 / (255.f * 0.229f), 1 / (255.f * 0.224f), 1 / (255.f * 0.225f)};
  const char* emotion_texts[7] = {
    "angry", "disgust", "fear", "happiness", "neutral", "sadness", "surprise"};

private:
  void initialize_pretreat();   //

  void transform(const cv::Mat& mat) override;   // padding & resize & normalize.

public:
  void detect(const cv::Mat& mat, types::Emotions& emotions);
};
}   // namespace mnncv

#endif   // LITE_AI_TOOLKIT_MNN_CV_MNN_REXNET_EMOTION7_H
