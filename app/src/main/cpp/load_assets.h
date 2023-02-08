#ifndef ASSET_MANAGER_LOAD_ASSETS_H
#define ASSET_MANAGER_LOAD_ASSETS_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <memory>
#include "toolkit/onnx/core/model_data.h"

namespace ray{
    class LoadAssets {
    public:
        /**
         * Create a ModelData object by AssetManager.
         * @param env
         * @param assetManager
         * @param fileName
         * @return A ModelData unique pointer.
         */
        static std::unique_ptr<toolkit::onnx::core::ModelData> Load(JNIEnv* env, jobject& assetManager, const char* fileName);
    };
}
#endif //ASSET_MANAGER_LOAD_ASSETS_H
