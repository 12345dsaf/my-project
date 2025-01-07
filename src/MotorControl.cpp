#include "MotorControl.h"
#include <ESP32Servo.h>

Servo servoBottle;
Servo servoTrash;

void MotorControl::init() {
    servoBottle.attach(14); // GPIO14
    servoTrash.attach(15);  // GPIO15
}

void MotorControl::openLid(int binType) {
    if (binType == 1) {
        servoBottle.write(90); // 打開
        delay(1000);
        servoBottle.write(0);  // 關閉
    } else if (binType == 2) {
        servoTrash.write(90); // 打開
        delay(1000);
        servoTrash.write(0);  // 關閉
    }
}
