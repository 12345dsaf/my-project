#ifndef ULTRASONIC_MODULE_H
#define ULTRASONIC_MODULE_H

#include <Arduino.h>

class UltrasonicModule {
public:
    UltrasonicModule(int trigPin, int echoPin);
    void init();                  // 初始化超聲波模組
    int measureDistance();        // 測量距離（厘米）
private:
    int _trigPin;
    int _echoPin;
};

#endif
