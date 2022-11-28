#include <Arduino.h>
#include <VISCAController.h>

VISCAController controller = VISCAController(
    (uint8_t[4]){ 5, 4, 3, 2 }, 4,
    (uint8_t[3]){ 6, 8, 7 }, 3,
    (uint8_t[10]){ 9, 10, 11, 12, 13, A0, A1, A2, A3, A4 },
    (uint8_t[3]){ A5, A6, A7 },
    9
);

void setup() {
    Serial.begin(115200);
    controller.begin();
}

void loop() {
    controller.loop();
}