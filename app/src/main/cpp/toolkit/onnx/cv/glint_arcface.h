#ifndef DEEP_TOOLKIT_GLINT_ARCFACE_H
#define DEEP_TOOLKIT_GLINT_ARCFACE_H

#include "toolkit/onnx/core/onnx_core.h"

namespace onnx_cv
{
    class DEEP_TOOLKIT_EXPORTS GlintArcFace : public core::BasicONNXHandler
    {
    public:
        explicit GlintArcFace(const std::string &_onnx_path, unsigned int _num_threads = 1)
            : BasicONNXHandler(_onnx_path, _num_threads)
        {
        }
        
        ~GlintArcFace() = default;

    private:
        static constexpr const float mean_val = 127.5f;
        static constexpr const float scale_val = 1.f / 127.5f;

    private:
        Ort::Value transform(const cv::Mat &mat) override;

    public:
        void detect(const cv::Mat &mat, types::FaceContent &face_content);
    };

} // namespace onnx_cv

#endif