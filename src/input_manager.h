//
// Created by Vojtěch Kycelt on 01.10.2024.
//

#ifndef APOPHISENGINEPROTOTYPE_INPUT_MANAGER_H
#define APOPHISENGINEPROTOTYPE_INPUT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "renderer.h"

class InputManager {
public:
    //TODO make enum of used keys
    InputManager(){
        add_key("UP");
        add_key("DOWN");
        add_key("PAUSE");
    }
    void detect_input_press(Renderer& renderer, bool &game_paused);
    bool is_key_pressed(const std::string &key);
    void add_key(const std::string &key);
    void set_key_pressed(const std::string &key, bool pressed);
private:
    std::map<std::string, bool> keys_pressed;
};


#endif //APOPHISENGINEPROTOTYPE_INPUT_MANAGER_H
