#ifndef DEEP_TOOLKIT_MOBILE_FACENET_H
#define DEEP_TOOLKIT_MOBILE_FACENET_H

#include "toolkit/onnx/core/onnx_core.h"
#include "toolkit/onnx/core/onnx_utils.h"
#include "toolkit/onnx/core/model_data.h"

namespace onnx_cv
{
  using toolkit::onnx::core::ModelData;

  class DEEP_TOOLKIT_EXPORTS MobileFaceNet : public core::BasicONNXHandler
  {
  public:
    explicit MobileFaceNet(const std::string &_onnx_path,
                           size_t _num_threads = 1)
        : BasicONNXHandler(_onnx_path, _num_threads) {}

    explicit MobileFaceNet(const ModelData &_model_data, size_t _num_threads = 1);
    inline ~MobileFaceNet()
    {
#if ENABLE_ORT_DEBUG
      std::cout << "Destroy MobileFaceNet object" << std::endl;
#endif
    }

  private:
    static constexpr const float mean_val = 127.5f;
    static constexpr const float scale_val = 1.f / 128.0f;

  private:
    Ort::Value transform(const cv::Mat &mat) override;

  public:
    /**
       @param mat
       @param face_content
    */
    void detect(const cv::Mat &mat, types::FaceContent &face_content);
  };
}

#endif
