#pragma once

#include <string>
#include <fstream>
#include "toolkit/toolkit.h"
#include "debug.h"

namespace sdk
{
    class DEEP_TOOLKIT_EXPORTS Helper
    {
    public:

        /**
         * @brief 
         * 
         * @param input byte[] from java
         * @param width 
         * @param height 
         * @return const cv::Mat 
         */
        static const cv::Mat NV212BGR(const char *input, int width, int height);

        /**
         * @brief 
         * 
         * @param input cv::Mat which format is BGR had matted 
         * @return const cv::Mat which format is yuv21
         */
        static const cv::Mat BGR2NV21(const cv::Mat& input);
    private:
        static char *ReadAllBytes(const char *filename);
        static void WriteToFile(uchar* input, int width, int height);
    };
} // namespace ryman