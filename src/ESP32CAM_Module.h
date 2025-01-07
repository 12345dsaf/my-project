#ifndef ESP32CAM_MODULE_H
#define ESP32CAM_MODULE_H

#include <Arduino.h>

// 函數聲明
namespace ESP32CAM_Module {
    void initCamera();                             // 初始化攝影頭
    void connectCameraWiFi();                     // 連接 Wi-Fi
    bool captureImage(signed char *image_buffer, int buffer_size); // 擷取影像數據
}

#endif
