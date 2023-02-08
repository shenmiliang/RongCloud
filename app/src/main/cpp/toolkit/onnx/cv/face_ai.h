#ifndef TOOLKIT_ONNX_CV_FACE_AI_H
#define TOOLKIT_ONNX_CV_FACE_AI_H

#include <memory>
#include "toolkit/headers.h"
#include "toolkit/toolkit.h"
#include "toolkit/onnx/core/onnx_core.h"
#include "toolkit/onnx/core/model_data.h"

namespace toolkit
{
    using toolkit::onnx::face::detect::SCRFD;
    using toolkit::onnx::faceid::MobileFaceNet;
    using toolkit::onnx::core::ModelData;


    class FaceAI
    {
    public:
        FaceAI(const ModelData& scrfd_model_data, const ModelData& face_net_model_data, size_t num_threads = 1);

        FaceAI &operator=(FaceAI &) = delete;
        FaceAI &operator=(FaceAI &&) = delete;
        FaceAI(FaceAI &) = delete;
        FaceAI(FaceAI &&) = delete;

        /**
         * @brief 
         * 
         * @param first 
         * @param second 
         * @param outputs 
         */
        void Recognition(const cv::Mat &first, const cv::Mat &second, std::vector<float> &outputs);

        /**
         * @brief Detects how many faces in the given image.
         * 
         * @param scrfd_model_data Model data
         * @param src Image will be detected
         * @param num_threads 
         * @return size_t Number of faces
         */
        static size_t Detect(const ModelData& scrfd_model_data, const cv::Mat &src, unsigned int num_threads);

    private:
        void getFaces(const cv::Mat &img, std::vector<cv::Mat> &outputs);


    private:
        std::unique_ptr<SCRFD> m_scrfd;
        std::unique_ptr<MobileFaceNet> m_face_net;
    };
}
#endif