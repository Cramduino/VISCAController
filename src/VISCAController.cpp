#include <VISCAController.h>

void VISCAController::_setSticks() {
    switch (this->_mode) {
        case ControllerMode::CALIBRATION_LIMITS:
            this->_sticks->setMode(AxisMode::CALIBRATE_LIMITS);
            break;
        case ControllerMode::CALIBRATION_DEADZONE:
            this->_sticks->setMode(AxisMode::CALIBRATE_DEADZONE);
            break;
        default:
            this->_sticks->setMode(AxisMode::NORMAL);
            break;
    }
}

VISCAController::VISCAController(
    uint8_t* button_x_pins, uint8_t button_x_pins_count, uint8_t* button_y_pins, uint8_t button_y_pins_count,
    uint8_t* led_pins,
    uint8_t* stick_pins,
    uint8_t num_presets
) {
    this->_num_presets = num_presets;

    // Prepare button labels
    this->_button_labels = (char*)malloc(sizeof(char) * (num_presets + 2));
    for (uint8_t i = 0; i < num_presets; i++) { this->_button_labels[i] = '0' + i + 1; }
    this->_button_labels[num_presets] = 'H';
    this->_button_labels[num_presets + 1] = 'C';

    this->_buttons = new ButtonArray(
        this->_button_labels, num_presets + 2,
        button_x_pins, button_x_pins_count,
        button_y_pins, button_y_pins_count
    );

    this->_leds = new LEDs(led_pins, num_presets + 1);

    this->_sticks = new Sticks(3, stick_pins, this->_stick_labels);
}

void VISCAController::begin() {
    
    this->_buttons->begin();
    this->_buttons->setHandlerClass(this);

    this->_leds->begin();
    this->_leds->setFastFlashRate(100);
    this->_leds->setSlowFlashRate(500);
    this->_leds->setAll(LEDMode::FLASH_FAST);
    this->_leds->queueChangeAll(LEDMode::OFF, 1000);

    this->_sticks->begin();
}

void VISCAController::loop() {
    this->_buttons->loop();
    this->_leds->loop();
    this->_sticks->loop();

    this->_setSticks();
}

void VISCAController::handleButtonEvent(char key, ButtonEventType type) {
    
    // Press and hold 'C' to start calibration
    if (key == 'C' && type == ButtonEventType::PRESS_AND_HOLD) {
        this->setMode(ControllerMode::CALIBRATION_START);
        return;
    }

    // During calibration...
    if (this->getMode() >= ControllerMode::CALIBRATION_START) {

        // ... press and hold 'H' to restart calibration
        if (key == 'H' && type == ButtonEventType::PRESS_AND_HOLD) {
            this->setMode(ControllerMode::CALIBRATION_START);
            return;
        }

        // ... press and hold '1' to progress to next step
        if (key == '1' && type == ButtonEventType::PRESS_AND_HOLD) {
            ControllerMode nextMode = this->getMode() < ControllerMode::CALIBRATION_FINISH ? 
                (ControllerMode)((uint8_t)this->getMode() + 1) :
                ControllerMode::NORMAL;
            this->setMode(nextMode);
            return;
        }
    }

    if (this->getMode() == ControllerMode::NORMAL) {

        uint8_t preset = key - '0';
        if (preset > 0 && preset <= this->_num_presets) {
            if (type == ButtonEventType::PRESS_AND_RELEASE) this->goToPreset(preset);
            else if (type == ButtonEventType::PRESS_AND_HOLD) this->storePreset(preset);
            return;
        }

        if (key == 'H') {
            if (type == ButtonEventType::PRESS) this->goToHome();
            return;
        }

    }
}

ControllerMode VISCAController::getMode() { return this->_mode; }

void VISCAController::setMode(ControllerMode mode) {
    
    this->_mode = mode;
    switch (this->_mode) {
        case ControllerMode::NORMAL:
            this->_leds->setAll(LEDMode::OFF);
            break;
        case ControllerMode::CALIBRATION_START:
            this->_leds->setAll(LEDMode::FLASH_FAST);
            break;
        case ControllerMode::CALIBRATION_LIMITS:
            this->_leds->setAll(LEDMode::OFF);
            this->_leds->setMode(0, LEDMode::FLASH_SLOW);
            break;
        case ControllerMode::CALIBRATION_DEADZONE:
            this->_leds->setAll(LEDMode::OFF);
            this->_leds->setMode(1, LEDMode::FLASH_SLOW);
            break;
        case ControllerMode::CALIBRATION_FINISH:
            this->_leds->setAll(LEDMode::ON);
            break;
        default: break;
    }

}

void VISCAController::goToHome() {
    // TODO: VISCA
    this->_leds->setAll(LEDMode::OFF);
    this->_leds->setMode(this->_num_presets, LEDMode::ON);
}

void VISCAController::goToPreset(uint8_t preset) {
    // TODO: VISCA
    this->_leds->setAll(LEDMode::OFF);
    this->_leds->setMode(preset - 1, LEDMode::ON);
}

void VISCAController::storePreset(uint8_t preset) {
    // TODO: VISCA
    this->_leds->setAll(LEDMode::OFF);
    this->_leds->setMode(preset - 1, LEDMode::FLASH_FAST);
    this->_leds->queueChange(preset - 1, LEDMode::ON, 500);
}