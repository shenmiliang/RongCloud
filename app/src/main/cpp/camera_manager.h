#ifndef CAMERAAPPLICATION_CAMERA_MANAGER_H
#define CAMERAAPPLICATION_CAMERA_MANAGER_H
#include <string>
#include <vector>
#include <map>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraMetadataTags.h>

class CameraId;
class NDKCamera {
public:
    NDKCamera();
    ~NDKCamera();
};
#endif //CAMERAAPPLICATION_CAMERA_MANAGER_H
