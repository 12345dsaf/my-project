#include "ThingSpeakModule.h"
#include <WiFi.h>
#include <ThingSpeak.h>
#include "WiFiConfig.h"

// 全域變數
WiFiClient client;
const char* apiKey = "VKLNR93VCAR8GW6I"; // 替換為正確的 API 金鑰
const long channelID = 2791455;           // 替換為正確的頻道 ID

void ThingSpeakModule::init() {
    ThingSpeak.begin(client); // 初始化 ThingSpeak
}

void ThingSpeakModule::uploadTrashLevel(int binID, int capacity) {
    ThingSpeak.setField(1, binID);     // 設置字段 1 為垃圾桶 ID
    ThingSpeak.setField(2, capacity); // 設置字段 2 為垃圾桶容量

    if (ThingSpeak.writeFields(channelID, apiKey) == 200) {
        Serial.println("Data uploaded successfully.");
    } else {
        Serial.println("Failed to upload data.");
    }
}
