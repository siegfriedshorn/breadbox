#include "modes/ProjectM.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

ProjectM::ProjectM(socd::SocdType socd_type, ProjectMOptions options) {
    _socd_pair_count = 4;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{ &InputState::left,   &InputState::right,   socd::SOCD_2IP_NO_REAC},
        socd::SocdPair{ &InputState::down,   &InputState::up,      socd::SOCD_2IP_NO_REAC},
        socd::SocdPair{ &InputState::c_left, &InputState::c_right, socd_type},
        socd::SocdPair{ &InputState::c_down, &InputState::c_up,    socd_type},
    };

    _options = options;
    _horizontal_socd = false;
}

void ProjectM::HandleSocd(InputState &inputs) {
    _horizontal_socd = inputs.left && inputs.right;
    InputMode::HandleSocd(inputs);
}

void ProjectM::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;
    outputs.x = inputs.x;
    outputs.y = inputs.y;
    // True Z press vs macro lightshield + A.
    if (_options.true_z_press || inputs.mod_x) {
        outputs.buttonR = inputs.z;
    } else {
        outputs.a = inputs.a || inputs.z;
    }
    if (inputs.nunchuk_connected) {
        outputs.triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs.triggerLDigital = inputs.l;
    }
    outputs.triggerRDigital = inputs.r;
    outputs.start = inputs.start;

    // Activate D-Pad layer by holding Mod X + Mod Y or Nunchuk C button.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }

    // Don't override dpad up if it's already pressed using the MX + MY dpad
    // layer.
    outputs.dpadUp = outputs.dpadUp || inputs.midshield;

    if (inputs.select)
        outputs.dpadLeft = true;
    if (inputs.home)
        outputs.dpadRight = true;
}

void ProjectM::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up,
        inputs.c_left,
        inputs.c_right,
        inputs.c_down,
        inputs.c_up,
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    bool shield_button_pressed = inputs.l || inputs.lightshield;

     if (directions.diagonal) {
        if (directions.y == 1) {
            outputs.leftStickX = 128 + (directions.x * 83);
            outputs.leftStickY = 128 + (directions.y * 93);
        }
    }

    if (inputs.mod_x) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 70);
        }
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 60);
        }

        if (directions.cx != 0) {
            outputs.rightStickX = 128 + (directions.cx * 65);
            outputs.rightStickY = 128 + (directions.y * 23);
        }

        if (directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 70);
            outputs.leftStickY = 128 + (directions.y * 60);

            if (inputs.r) {
                outputs.leftStickX = 128 + (directions.x * 93);
                outputs.leftStickY = 128 + (directions.y * 28);
            }
            
            if (inputs.l) {
                outputs.leftStickX = 128 + (directions.x * 93);
                outputs.leftStickY = 128 + (directions.y * 28);
            }

            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 83);
                outputs.leftStickY = 128 + (directions.y * 55);
            }

            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 66);
                outputs.leftStickY = 128 + (directions.y * 28);
            }

            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 53);
                outputs.leftStickY = 128 + (directions.y * 29);
            }

            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 77);
                outputs.leftStickY = 128 + (directions.y * 64);
            }         
            
            /* Extended Up B Angles */
            if (inputs.b) {
                
                outputs.leftStickX = 128 + (directions.x * 93);
                outputs.leftStickY = 128 + (directions.y * 28);
                
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 88);
                    outputs.leftStickY = 128 + (directions.y * 36);
                }
        
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 84);
                    outputs.leftStickY = 128 + (directions.y * 45);
                }
                
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 83);
                    outputs.leftStickY = 128 + (directions.y * 55);
                }
                
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 77);
                    outputs.leftStickY = 128 + (directions.y * 64);
                }
            }
        }
    }

    if (inputs.mod_y) {
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 35);
        }
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 61);
        }

        if (directions.diagonal) {
            outputs.leftStickX = 128 + (directions.x * 28);
            outputs.leftStickY = 128 + (directions.y * 62);

            if (inputs.r) {
                outputs.leftStickX = 128 + (directions.x * 45);
                outputs.leftStickY = 128 + (directions.y * 84);
            }

            if (inputs.c_up) {
                outputs.leftStickX = 128 + (directions.x * 55);
                outputs.leftStickY = 128 + (directions.y * 83);
            }

            if (inputs.c_down) {
                outputs.leftStickX = 128 + (directions.x * 28);
                outputs.leftStickY = 128 + (directions.y * 66);
            }

            if (inputs.c_left) {
                outputs.leftStickX = 128 + (directions.x * 29);
                outputs.leftStickY = 128 + (directions.y * 53);
            }

            if (inputs.c_right) {
                outputs.leftStickX = 128 + (directions.x * 64);
                outputs.leftStickY = 128 + (directions.y * 77);
            } 
            
            /* Extended Up B Angles */
            if (inputs.b) {
                // 67.0362 - 3875 9125 = 31 73
                outputs.leftStickX = 128 + (directions.x * 28);
                outputs.leftStickY = 128 + (directions.y * 93);
                // 62.62896 - 4500 8750 (62.8) = 36 70
                if (inputs.c_down) {
                    outputs.leftStickX = 128 + (directions.x * 36);
                    outputs.leftStickY = 128 + (directions.y * 88);
                }
                // 58.22172 - 5250 8500 (58.3) = 42 68
                if (inputs.c_left) {
                    outputs.leftStickX = 128 + (directions.x * 45);
                    outputs.leftStickY = 128 + (directions.y * 84);
                }
                // 53.81448 - 5875 8000 (53.7) = 47 64
                if (inputs.c_up) {
                    outputs.leftStickX = 128 + (directions.x * 55);
                    outputs.leftStickY = 128 + (directions.y * 83);
                }
                // 49.40724 - 5875 7125 (50.49) = 47 57
                if (inputs.c_right) {
                    outputs.leftStickX = 128 + (directions.x * 64);
                    outputs.leftStickY = 128 + (directions.y * 77);
                }
            }
        }
    }

    // C-stick ASDI Slideoff angle overrides any other C-stick modifiers (such as
    // angled fsmash).
    // We don't apply this for c-up + c-left/c-right in case we want to implement
    // C-stick nair somehow.
    if (directions.cx != 0 && directions.cy == -1) {
        // 3000 9875 = 30 78
        outputs.rightStickX = 128 + (directions.cx * 35);
        outputs.rightStickY = 128 + (directions.cy * 98);
    }
    
    if ( inputs.l && inputs.mod_x) {
        outputs.rightStickX = 128 + (directions.cx * 40);
        outputs.rightStickY = 128 + (directions.cy * 40);
    }

    if (inputs.lightshield) {
        outputs.triggerRAnalog = 49;
    }

    // Send lightshield input if we are using Z = lightshield + A macro.
    if (inputs.z && !(inputs.mod_x || _options.true_z_press)) {
        outputs.triggerRAnalog = 49;
    }

    if (outputs.triggerLDigital) {
        outputs.triggerLAnalog = 140;
    }

    if (outputs.triggerRDigital) {
        outputs.triggerRAnalog = 140;
    }

    // Shut off C-stick when using D-Pad layer.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
