//
// Created by DefTruth on 2021/11/29.
//

#ifndef LITE_AI_TOOLKIT_TNN_CV_TNN_FAST_STYLE_TRANSFER_H
#define LITE_AI_TOOLKIT_TNN_CV_TNN_FAST_STYLE_TRANSFER_H

#include "lite/tnn/core/tnn_core.h"

namespace tnncv {
class TNNFastStyleTransfer : public BasicTNNHandler
{
public:
  explicit TNNFastStyleTransfer(const std::string& _proto_path, const std::string& _model_path,
                                unsigned int _num_threads = 1);   //
  ~TNNFastStyleTransfer() override = default;

private:
  // In TNN: x*scale + bias
  std::vector<float> scale_vals = {1.0f, 1.0f, 1.0f};
  std::vector<float> bias_vals  = {0.f, 0.f, 0.f};

private:
  void transform(const cv::Mat& mat_rs) override;   //

public:
  void detect(const cv::Mat& mat, types::StyleContent& style_content);
};
}   // namespace tnncv


#endif   // LITE_AI_TOOLKIT_TNN_CV_TNN_FAST_STYLE_TRANSFER_H
