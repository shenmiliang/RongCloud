#ifndef DEEP_TOOLKIT_UTILS_H
#define DEEP_TOOLKIT_UTILS_H

#include "types.h"

namespace toolkit
{
    namespace utils
    {
        static constexpr const float _PI{3.1415926f};

        // String Utils
        DEEP_TOOLKIT_EXPORTS std::wstring to_wstring(const std::string &str);

        DEEP_TOOLKIT_EXPORTS std::string to_string(const std::wstring &wstr);

        // Drawing Utils
        DEEP_TOOLKIT_EXPORTS cv::Mat draw_axis(const cv::Mat &mat, const types::EulerAngles &euler_angles, float size = 50.f, int thickness = 2);

        DEEP_TOOLKIT_EXPORTS cv::Mat draw_boxes(const cv::Mat &mat, const std::vector<types::Boxf> &boxes);

        DEEP_TOOLKIT_EXPORTS cv::Mat draw_landmarks(const cv::Mat &mat, types::Landmarks &landmarks);

        DEEP_TOOLKIT_EXPORTS cv::Mat draw_age(const cv::Mat &mat, types::Age &age);

        DEEP_TOOLKIT_EXPORTS cv::Mat draw_gender(const cv::Mat &mat, types::Gender &gender);

        DEEP_TOOLKIT_EXPORTS cv::Mat draw_emotion(const cv::Mat &mat, types::Emotions &emotions);

        DEEP_TOOLKIT_EXPORTS cv::Mat draw_boxes_with_landmarks(const cv::Mat &mat, const std::vector<types::BoxfWithLandmarks> &boxes_kps, bool text = false);

        DEEP_TOOLKIT_EXPORTS void draw_boxes_inplace(cv::Mat &mat_inplace, const std::vector<types::Boxf> &boxes);

        DEEP_TOOLKIT_EXPORTS void draw_axis_inplace(cv::Mat &mat_inplace, const types::EulerAngles &euler_angles, float size = 50.f, int thickness = 2);

        DEEP_TOOLKIT_EXPORTS void draw_landmarks_inplace(cv::Mat &mat, types::Landmarks &landmarks);

        DEEP_TOOLKIT_EXPORTS void draw_age_inplace(cv::Mat &mat_inplace, types::Age &age);

        DEEP_TOOLKIT_EXPORTS void draw_gender_inplace(cv::Mat &mat_inplace, types::Gender &gender);

        DEEP_TOOLKIT_EXPORTS void draw_emotion_inplace(cv::Mat &mat_inplace, types::Emotions &emotions);

        DEEP_TOOLKIT_EXPORTS void draw_boxes_with_landmarks_inplace(cv::Mat &mat_inplace, const std::vector<types::BoxfWithLandmarks> &boxes_kps, bool text = false);

        // Object Detection Utils
        DEEP_TOOLKIT_EXPORTS void hard_nms(std::vector<types::Boxf> &input, std::vector<types::Boxf> &output, float iou_threshold, size_t topk);

        DEEP_TOOLKIT_EXPORTS void blending_nms(std::vector<types::Boxf> &input, std::vector<types::Boxf> &output, float iou_threshold, size_t topk);

        DEEP_TOOLKIT_EXPORTS void offset_nms(std::vector<types::Boxf> &input, std::vector<types::Boxf> &output, float iou_threshold, size_t topk);

        // Matting & Segmentation Utils
        DEEP_TOOLKIT_EXPORTS void swap_background(const cv::Mat &fgr_mat, const cv::Mat &pha_mat, const cv::Mat &bgr_mat, cv::Mat &out_mat, bool fgr_is_already_mul_pha = false);

        DEEP_TOOLKIT_EXPORTS void remove_small_connected_area(cv::Mat &alpha_pred, float threshold = 0.05f);

        namespace math
        {
            template <typename T = float>
            std::vector<float> softmax(const std::vector<T> &logits, size_t &max_id);

            template DEEP_TOOLKIT_EXPORTS std::vector<float> softmax(const std::vector<float> &logits, size_t &max_id);

            template <typename T = float>
            std::vector<float> softmax(const T *logits, size_t _size, size_t &max_id);

            template DEEP_TOOLKIT_EXPORTS std::vector<float> softmax(const float *logits, size_t _size, size_t &max_id);

            template <typename T = float>
            std::vector<size_t> argsort(const std::vector<T> &arr);

            template DEEP_TOOLKIT_EXPORTS std::vector<size_t> argsort(const std::vector<float> &arr);

            template <typename T = float>
            std::vector<size_t> argsort(const T *arr, size_t _size);

            template DEEP_TOOLKIT_EXPORTS std::vector<size_t> argsort(const float *arr, size_t _size);

            template <typename T = float>
            float cosine_similarity(const std::vector<T> &a, const std::vector<T> &b);

            template DEEP_TOOLKIT_EXPORTS float cosine_similarity(const std::vector<float> &a, const std::vector<float> &b);
        }

    } // namespace utils
} // namespace toolkit

template <typename T>
std::vector<float> toolkit::utils::math::softmax(const T *logits, size_t _size, size_t &max_id)
{
    types::__assert_type<T>();
    if (_size == 0 || logits == nullptr)
        return {};

    float max_prob{0.f}, total_exp{0.f};

    std::vector<float> softmax_probs(_size);
    for (size_t i = 0; i < _size; ++i)
    {
        softmax_probs[i] = std::exp((float)logits[i]);
        total_exp += softmax_probs[i];
    }
    for (size_t i = 0; i < _size; ++i)
    {
        softmax_probs[i] = softmax_probs[i] / total_exp;
        if (softmax_probs[i] > max_prob)
        {
            max_id = i;
            max_prob = softmax_probs[i];
        }
    }
    return softmax_probs;
}

template <typename T>
std::vector<float> toolkit::utils::math::softmax(const std::vector<T> &logits, size_t &max_id)
{
    types::__assert_type<T>();
    if (logits.empty())
        return {};
    const size_t _size = logits.size();
    float max_prob = 0.f, total_exp = 0.f;
    std::vector<float> softmax_probs(_size);
    for (size_t i = 0; i < _size; ++i)
    {
        softmax_probs[i] = std::exp((float)logits[i]);
        total_exp += softmax_probs[i];
    }
    for (size_t i = 0; i < _size; ++i)
    {
        softmax_probs[i] = softmax_probs[i] / total_exp;
        if (softmax_probs[i] > max_prob)
        {
            max_id = i;
            max_prob = softmax_probs[i];
        }
    }
    return softmax_probs;
}

template <typename T>
std::vector<size_t> toolkit::utils::math::argsort(const std::vector<T> &arr)
{
    types::__assert_type<T>();
    if (arr.empty())
        return {};
    const size_t _size = arr.size();
    std::vector<size_t> indices;
    for (size_t i = 0; i < _size; ++i)
        indices.push_back(i);
    std::sort(indices.begin(), indices.end(),
              [&arr](const size_t a, const size_t b)
              { return arr[a] > arr[b]; });
    return indices;
}

template <typename T>
std::vector<size_t> toolkit::utils::math::argsort(const T *arr, size_t _size)
{
    types::__assert_type<T>();

    if (_size == 0 || arr == nullptr)
        return {};

    std::vector<size_t> indices;
    for (size_t i = 0; i < _size; ++i)
        indices.push_back(i);
    std::sort(indices.begin(), indices.end(),
              [arr](const size_t a, const size_t b)
              { return arr[a] > arr[b]; });
    return indices;
}

template <typename T>
float toolkit::utils::math::cosine_similarity(const std::vector<T> &a, const std::vector<T> &b)
{
    types::__assert_type<T>();

    float zero_value = 0.f;
    if (a.empty() || b.empty() || (a.size() != b.size()))
        return zero_value;

    const size_t _size = a.size();
    float mul_a = zero_value, mul_b = zero_value, mul_ab = zero_value;
    for (size_t i = 0; i < _size; ++i)
    {
        mul_a += (float)a[i] * (float)a[i];
        mul_b += (float)b[i] * (float)b[i];
        mul_ab += (float)a[i] * (float)b[i];
    }

    if (mul_a == zero_value || mul_b == zero_value)
        return zero_value;

    return mul_ab / (std::sqrt(mul_a) * std::sqrt(mul_b));
}

#endif