#include "load_assets.h"

using toolkit::onnx::core::ModelData;

std::unique_ptr<ModelData> ray::LoadAssets::Load(JNIEnv* env, jobject& assetManager, const char *fileName) {
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);

    if (mgr == nullptr) {
        __android_log_print(ANDROID_LOG_DEBUG, "MATTING", ":%s", "AAssetManager==NULL");
        return nullptr;
    }

    AAsset* asset = AAssetManager_open(mgr, fileName, AASSET_MODE_UNKNOWN);
    if (asset == nullptr) {
        __android_log_print(ANDROID_LOG_DEBUG, "MATTING ", ":%s", "AAssetManager==NULL");
        return nullptr;
    }

    unsigned long bufferSize = AAsset_getLength(asset);
    __android_log_print(ANDROID_LOG_DEBUG, "MATTING", "file size : %ld \n", bufferSize);

    char* data = new char[bufferSize];
    int readSize = AAsset_read(asset, data, bufferSize);
    if (readSize != bufferSize) {
        __android_log_print(ANDROID_LOG_DEBUG, "MATTING", "Total size: %ld bytes; Read size: %d bytes \n", bufferSize, readSize);
        AAsset_close(asset);
        delete[] data;
        return nullptr;
    }

    __android_log_print(ANDROID_LOG_DEBUG, "MATTING", "Total size: %ld bytes == Read size: %d bytes \n", bufferSize, readSize);
    std::unique_ptr<ModelData> modelData{new ModelData(fileName, data, static_cast<size_t>(bufferSize))};
    AAsset_close(asset);
    return modelData;
}