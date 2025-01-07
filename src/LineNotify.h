#ifndef LINE_NOTIFY_H
#define LINE_NOTIFY_H

#include <Arduino.h>

class LineNotify {
public:
    static void init();                          // 初始化 LINE Notify
    static void sendNotification(String message); // 發送通知
};

#endif
