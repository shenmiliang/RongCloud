#include "config.h"
#include "model_data.h"

namespace core
{
    using toolkit::onnx::core::ModelData;

    class DEEP_TOOLKIT_EXPORTS BasicONNXHandler
    {
    protected:
        Ort::Env ort_env;
        std::unique_ptr<Ort::Session> ort_session;

        const char *input_name = nullptr;

        std::vector<const char *> input_node_names;
        std::vector<int64_t> input_node_dims; 

        std::size_t input_tensor_size = 1;
        std::vector<float> input_values_handler;
        Ort::MemoryInfo memory_info_handler = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        std::vector<const char *> output_node_names;
        std::vector<std::vector<int64_t>> output_node_dims;

        const DEEP_TOOLKIT_CHAR *onnx_path = nullptr;
        const char *log_id = nullptr;
        int num_outputs = 1;

    protected:
        const size_t num_threads; 

    protected:
        explicit BasicONNXHandler(const std::string &_onnx_path, size_t _num_threads = 1);
        explicit BasicONNXHandler(const ModelData& _modelData , size_t _num_threads = 1);
        
        virtual ~BasicONNXHandler();
    protected:
        BasicONNXHandler(const BasicONNXHandler &) = delete;
        BasicONNXHandler(BasicONNXHandler &&) = delete;
        BasicONNXHandler &operator=(const BasicONNXHandler &) = delete;
        BasicONNXHandler &operator=(BasicONNXHandler &&) = delete;

    protected:
        virtual Ort::Value transform(const cv::Mat &mat) = 0;

    private:
        void print_debug_string();
        Ort::SessionOptions createSessionOptions();
        void initializeModelFields();
    };
}
