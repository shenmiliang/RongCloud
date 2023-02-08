#ifndef TOOLKIT_ONNX_CORE_MODEL_DATA_H
#define TOOLKIT_ONNX_CORE_MODEL_DATA_H

#include <string>
#include <iostream>
#include <fstream>

namespace toolkit
{
    namespace onnx
    {
        namespace core
        {
            struct ModelData
            {
                const char *m_model_name;
                char *m_buffer;
                size_t m_buffer_size;

                /**
                 * @brief Construct a new Model Data object.
                 * Returned value of <code>mode_path.data()</code> will be assigned to m_model_name.
                 * @param model_path Path of model file
                 */
                explicit ModelData(const std::string &model_path) noexcept;

                /**
                 * @brief Construct a new Model Data object.
                 * For Android platform initialize.
                 * 
                 * @param _model_name 
                 * @param _model_data 
                 * @param _buffer_size 
                 */
                ModelData(const char *_model_name, char *_model_data, size_t _buffer_size);

                explicit ModelData(ModelData &&) = default;
                ModelData &operator=(ModelData &&other) = default;

                // Deleted constructors
                ModelData(const ModelData &other) = delete;
                ModelData(ModelData &other) = delete;
                ModelData &operator=(ModelData &other) = delete;
                ModelData &operator=(const ModelData &other) = delete;

                ~ModelData()
                {
                    if (m_buffer != nullptr)
                    {
                        delete[] m_buffer;
                        std::cout << "delete ModelData buffer " << m_model_name << std::endl;
                    }

                    m_buffer = nullptr;
                }
            };
        }
    }
}
#endif