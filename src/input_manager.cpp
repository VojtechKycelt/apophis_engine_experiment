//
// Created by Vojtěch Kycelt on 01.10.2024.
//

#include "input_manager.h"
#include <iostream>
bool InputManager::is_key_pressed(const std::string &key) {
    auto it = keys_pressed.find(key);
    if (it != keys_pressed.end()){
        return it->second;
    }
    return false;
}

void InputManager::add_key(const std::string &key) {
    keys_pressed.insert(std::pair(key,false));
}

void InputManager::set_key_pressed(const std::string &key, bool pressed) {
    auto it = keys_pressed.find(key);
    if (it != keys_pressed.end()){
        it->second = pressed;
    } else std::cerr << "KEY NOT FOUND" << std::endl;
}

void InputManager::detect_input_press(Renderer& renderer, bool &game_paused) {
    if (!is_key_pressed("PAUSE")) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            game_paused = !game_paused;
            set_key_pressed("PAUSE", true);
        };
    }

    //handle input
    for (auto event = sf::Event{}; renderer.window.pollEvent(event);) {
        if (!is_key_pressed("PAUSE")) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                game_paused = !game_paused;
                set_key_pressed("PAUSE", true);
            };
        }
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.scancode == sf::Keyboard::Scan::P) {
                set_key_pressed("PAUSE", false);
            }
        }
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            renderer.window.close();

        if (game_paused) {
            set_key_pressed("UP", false);
            set_key_pressed("DOWN", false);
            break;
        };

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.scancode == sf::Keyboard::Scan::Up) {
                set_key_pressed("UP", true);

            }
            if (event.key.scancode == sf::Keyboard::Scan::Down) {
                set_key_pressed("DOWN", true);
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.scancode == sf::Keyboard::Scan::Up) {
                set_key_pressed("UP", false);
            }
            if (event.key.scancode == sf::Keyboard::Scan::Down) {
                set_key_pressed("DOWN", false);
            }
        }
    }
}

