#include "LineNotify.h"
#include <HTTPClient.h>

const char* lineToken = "YOUR_LINE_NOTIFY_TOKEN";

void LineNotify::init() {
    Serial.println("LINE Notify initialized.");
}

void LineNotify::sendNotification(String message) {
    HTTPClient http;
    http.begin("https://notify-api.line.me/api/notify");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Bearer " + String(lineToken));

    String payload = "message=" + message;
    int response = http.POST(payload);

    if (response == 200) {
        Serial.println("Notification sent successfully.");
    } else {
        Serial.println("Failed to send notification.");
    }

    http.end();
}
