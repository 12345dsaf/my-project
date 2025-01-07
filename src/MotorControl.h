#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

class MotorControl {
public:
    static void init();                // 初始化伺服馬達
    static void openLid(int binType);  // 打開垃圾桶蓋
};

#endif
