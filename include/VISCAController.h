#pragma once

#include <Arduino.h>
#include <ButtonArray.h>
#include <ControllerMode.h>
#include <LEDs.h>
#include <Sticks.h>

class VISCAController : CanHandleButtonEvent {

    private:
        char* _button_labels;
        char  _stick_labels[3] = { 'P', 'T', 'Z' };

        uint8_t _num_presets;
        ButtonArray* _buttons;
        LEDs*        _leds;
        Sticks*      _sticks;

        ControllerMode _mode = ControllerMode::NORMAL;

        void _setSticks();

    public:
        VISCAController(
            uint8_t* button_x_pins, uint8_t button_x_pins_count, uint8_t* button_y_pins, uint8_t button_y_pins_count,
            uint8_t* led_pins,
            uint8_t* stick_pins,
            uint8_t num_presets = 9
        );
        
        void begin();
        void loop();

        void handleButtonEvent(char key, ButtonEventType type);

        ControllerMode getMode();
        void setMode(ControllerMode mode);

        void goToHome();
        void goToPreset(uint8_t preset);
        void storePreset(uint8_t preset);

};