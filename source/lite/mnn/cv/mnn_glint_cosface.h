//
// Created by DefTruth on 2021/11/13.
//

#ifndef LITE_AI_TOOLKIT_MNN_CV_MNN_GLINT_COSFACE_H
#define LITE_AI_TOOLKIT_MNN_CV_MNN_GLINT_COSFACE_H

#include "lite/mnn/core/mnn_core.h"

namespace mnncv {
class MNNGlintCosFace : public BasicMNNHandler
{
public:
  explicit MNNGlintCosFace(const std::string& _mnn_path, unsigned int _num_threads = 1);   //
  ~MNNGlintCosFace() override = default;

private:
  const float mean_vals[3] = {127.5f, 127.5f, 127.5f};   // RGB
  const float norm_vals[3] = {1.f / 127.5f, 1.f / 127.5f, 1.f / 127.5f};

private:
  void initialize_pretreat();   //

  void transform(const cv::Mat& mat) override;   //

public:
  void detect(const cv::Mat& mat, types::FaceContent& face_content);
};
}   // namespace mnncv


#endif   // LITE_AI_TOOLKIT_MNN_CV_MNN_GLINT_COSFACE_H
