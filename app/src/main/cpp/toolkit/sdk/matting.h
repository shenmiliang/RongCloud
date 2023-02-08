#pragma once

#include "toolkit/toolkit.h"
#include "toolkit/onnx/core/model_data.h"
#include "toolkit/onnx/cv/rvm.h"
#include "toolkit/types.h"
#include "opencv2/core/core_c.h"

namespace sdk
{
    class DEEP_TOOLKIT_EXPORTS Matting
    {
    private:
        std::string _model_path;
        std::unique_ptr<toolkit::onnx::matting::RobustVideoMatting> _rvm;
        cv::Mat _bg;
        
    public:
        /**
         * @brief Construct a new Matting object
         * 
         * @param model_path 
         * @param num_threads 
         */
        explicit Matting(const std::string &model_path, int num_threads = 2);

        /**
         * @brief Construct a new Matting object
         * 
         * @param model_name 
         * @param model_data 
         * @param buffer_size 
         * @param num_threads 
         */
        Matting(const char* model_name, const char* model_data, size_t buffer_size, size_t num_threads = 2);

        /**
         * @brief 
         * 
         * @param src cv::Mat will be matted 
         * @return cv::Mat&& matted data
         */
        cv::Mat doMatting(const cv::Mat& src);


        /**
         * @brief Set the Background object
         * 
         * @param bg 
         */
        inline void setBackground(const cv::Mat& bg) {
             _bg = bg.clone();
        }
    };

} // namespace ryman