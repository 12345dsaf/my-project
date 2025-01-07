#ifndef THINGSPEAK_MODULE_H
#define THINGSPEAK_MODULE_H
#include <Arduino.h>
#include <ThingSpeak.h>

class ThingSpeakModule {
public:
    static void init();                      // 初始化 ThingSpeak
    static void uploadTrashLevel(int binID, int capacity); // 上傳垃圾桶容量
};

#endif
