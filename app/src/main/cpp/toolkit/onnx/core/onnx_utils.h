#ifndef DEEP_TOOLKIT_ONNX_CORE_UTILS_H
#define DEEP_TOOLKIT_ONNX_CORE_UTILS_H

#include "config.h"

namespace onnx_cv
{
  // specific utils for ONNXRuntime
  namespace utils
  {
    namespace transform
    {
      enum
      {
        CHW = 0, HWC = 1
      };

      /**
       * @param mat CV:Mat with type 'CV_32FC3|2|1'
       * @param tensor_dims e.g {1,C,H,W} | {1,H,W,C}
       * @param memory_info It needs to be a global variable in a class
       * @param tensor_value_handler It needs to be a global variable in a class
       * @param data_format CHW | HWC
       * @return
       */
      DEEP_TOOLKIT_EXPORTS Ort::Value create_tensor(const cv::Mat &mat, const std::vector<int64_t> &tensor_dims,
                                               const Ort::MemoryInfo &memory_info_handler,
                                               std::vector<float> &tensor_value_handler,
                                               unsigned int data_format = CHW) throw(std::runtime_error);

      DEEP_TOOLKIT_EXPORTS cv::Mat normalize(const cv::Mat &mat, float mean, float scale);

      DEEP_TOOLKIT_EXPORTS cv::Mat normalize(const cv::Mat &mat, const float mean[3], const float scale[3]);

      DEEP_TOOLKIT_EXPORTS void normalize(const cv::Mat &inmat, cv::Mat &outmat, float mean, float scale);

      DEEP_TOOLKIT_EXPORTS void normalize_inplace(cv::Mat &mat_inplace, float mean, float scale);

      DEEP_TOOLKIT_EXPORTS void normalize_inplace(cv::Mat &mat_inplace, const float mean[3], const float scale[3]);
    }

  } // NAMESPACE UTILS
} // NAMESPACE 

#endif //DEEP_TOOLKIT_ONNX_CORE_UTILS_H