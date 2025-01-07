#include <Arduino.h>
#include "ESP32CAM_Module.h"
#include "MotorControl.h"
#include "UltrasonicModule.h"
#include "ThingSpeakModule.h"
#include "LineNotify.h"
#include "WiFiConfig.h"
#include "tony01180000089-project-1_inferencing.h"

// 超聲波模組腳位
#define TRIG_PIN_BIN1 12
#define ECHO_PIN_BIN1 13
#define TRIG_PIN_BIN2 14
#define ECHO_PIN_BIN2 15

// 超聲波模組實例
UltrasonicModule ultrasonicBin1(TRIG_PIN_BIN1, ECHO_PIN_BIN1);
UltrasonicModule ultrasonicBin2(TRIG_PIN_BIN2, ECHO_PIN_BIN2);

// 緩存影像數據
static signed char image_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// 將模型分類結果映射為「Recyclable」或「Trash」
String mapToCategory(String modelLabel) {
    if (modelLabel == "plastic" || modelLabel == "glass" || modelLabel == "metal") {
        return "Recyclable"; // 可回收
    } else if (modelLabel == "trash" || modelLabel == "cardboard" || modelLabel == "paper") {
        return "Trash"; // 一般垃圾
    } else {
        return "Unknown"; // 未知類別
    }
}

// 影像分類函數
String classifyImage() {
    if (!ESP32CAM_Module::captureImage(image_buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE)) {
        Serial.println("Failed to capture image.");
        return "Unknown";
    }

    // 將 signed char 轉換為 float 並標準化到 [-1, 1]
    float float_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
    for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i++) {
        float_buffer[i] = static_cast<float>(image_buffer[i]) / 128.0f;
    }

    // 創建信號
    signal_t signal;
    int err = numpy::signal_from_buffer(float_buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        Serial.println("Failed to create signal from buffer.");
        return "Unknown";
    }

    // 執行分類
    ei_impulse_result_t result = {0};
    err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
        Serial.printf("Classification failed (%d)\n", err);
        return "Unknown";
    }

    // 解析分類結果
    for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > 0.8) { // 信心值大於 80%
            return mapToCategory(String(result.classification[i].label)); // 映射分類結果
        }
    }
    return "Unknown";
}

void setup() {
    Serial.begin(115200);

    // 初始化 Wi-Fi
    ESP32CAM_Module::connectCameraWiFi();

    // 初始化攝影機模組
    ESP32CAM_Module::initCamera();

    // 初始化其他模組
    MotorControl::init();
    ultrasonicBin1.init();
    ultrasonicBin2.init();
    ThingSpeakModule::init();
    LineNotify::init();

    Serial.println("System initialized successfully.");
}

void loop() {
    // 步驟 1: 執行影像分類
    String category = classifyImage();
    Serial.println("Category: " + category);

    // 步驟 2: 根據分類結果控制垃圾桶蓋
    if (category == "Recyclable") {
        MotorControl::openLid(1); // 開啟可回收垃圾桶的蓋子
        Serial.println("Recyclable bin lid opened.");
    } else if (category == "Trash") {
        MotorControl::openLid(2); // 開啟一般垃圾桶的蓋子
        Serial.println("Trash bin lid opened.");
    } else {
        Serial.println("No valid classification result.");
    }

    // 步驟 3: 測量垃圾桶容量
    int capacityBin1 = ultrasonicBin1.measureDistance();
    int capacityBin2 = ultrasonicBin2.measureDistance();
    Serial.println("Recyclable bin capacity: " + String(capacityBin1) + " cm");
    Serial.println("Trash bin capacity: " + String(capacityBin2) + " cm");

    // 步驟 4: 上傳數據到 ThingSpeak
    ThingSpeakModule::uploadTrashLevel(1, capacityBin1);
    ThingSpeakModule::uploadTrashLevel(2, capacityBin2);

    // 步驟 5: 發送 LINE 通知（如果容量過高）
    if (capacityBin1 < 10) { // 假設 10 cm 表示可回收垃圾桶快滿
        LineNotify::sendNotification("Recyclable bin is almost full!");
    }
    if (capacityBin2 < 10) { // 假設 10 cm 表示一般垃圾桶快滿
        LineNotify::sendNotification("Trash bin is almost full!");
    }

    delay(5000); // 每 5 秒執行一次
}
