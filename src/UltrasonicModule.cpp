#include "UltrasonicModule.h"

UltrasonicModule::UltrasonicModule(int trigPin, int echoPin) {
    _trigPin = trigPin;
    _echoPin = echoPin;
}

void UltrasonicModule::init() {
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}

int UltrasonicModule::measureDistance() {
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);

    long duration = pulseIn(_echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    return distance; // 返回距離（厘米）
}
