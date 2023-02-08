#include "helper.h"
#include "matting.h"

#ifdef __cplusplus

/**
 * @brief Create a Background object
 *
 * @param width
 * @param height
 * @param data
 * @return cv::Mat Background mat
 */
const cv::Mat createBackground(int width, int height, char *data)
{
    return cv::Mat(height, width, CV_8UC3, data);
}

extern "C"
{

    /**
     * @brief
     *
     * @param src byte[] from java(YUV N21 format)
     * @param width
     * @param height
     * @return uchar* of byte[] (YUV N21 format)
     */
    char *matting_test(const char *src, int width, int height);

    /**
     * @brief 
     * 
     * @param model_name 
     * @param model_data 
     * @param buffer_size 
     * @param src 
     * @param width 
     * @param height 
     * @param bg_width 
     * @param bg_height 
     * @param bg 
     * @param num_threads 
     * @return char* 
     */
    char* matting(const char *model_name,
                  char *model_data,
                  size_t buffer_size,
                  char *src,
                  int width,
                  int height,
                  int bg_width,
                  int bg_height,
                  char *bg,
                  size_t num_threads = 2);

#endif
#ifdef __cplusplus
}
#endif