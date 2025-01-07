#include "ESP32CAM_Module.h"
#include "camera_pins.h"
#include <WiFi.h>
#include "WiFiConfig.h"
#include "esp_camera.h"

namespace ESP32CAM_Module {

void initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_GRAYSCALE;

    if (esp_camera_init(&config) != ESP_OK) {
        Serial.println("Camera init failed!");
        return;
    }
    Serial.println("Camera initialized.");
}

void connectCameraWiFi() {
    WiFi.begin(cam_ssid, cam_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");
    Serial.println("Camera IP address: " + WiFi.localIP().toString());
}

bool captureImage(signed char *image_buffer, int buffer_size) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return false;
    }

    // 確保緩存大小正確並將數據填充到緩存中
    if (fb->len > buffer_size) {
        Serial.println("Buffer size is too small for the captured image");
        esp_camera_fb_return(fb);
        return false;
    }
    memcpy(image_buffer, fb->buf, fb->len);
    esp_camera_fb_return(fb);

    Serial.println("Image captured successfully.");
    return true;
}

} // namespace ESP32CAM_Module
