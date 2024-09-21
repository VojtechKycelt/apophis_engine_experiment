#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "pong.h"
#include "renderer.h"
#include "util.h"

int main() {
    //declare renderer with window
    Renderer renderer(
        sf::Vector2i(1280, 720),
        "APOPHIS ENGINE EXPERIMENT",
        240);

    sf::Font font;
    if (!font.loadFromFile("../../src/spaceranger.ttf"))
        return EXIT_FAILURE;

    //keep track of time between frames
    sf::Clock deltaClock;
    float deltaTime;

    //smooth input declaration
    bool key_up_pressed = false;
    bool key_down_pressed = false;
    bool pause_key_pressed = false;
    bool game_paused = false;

    Pong pong(renderer);
    pong.init_game();

    //engine loop
    while (renderer.window.isOpen()) {
        //get delta time for same speed of the engine with different fps
        deltaTime = deltaClock.restart().asSeconds();

        if (!pause_key_pressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                game_paused = !game_paused;
                pause_key_pressed = true;
            };
        }

        //handle input
        for (auto event = sf::Event{}; renderer.window.pollEvent(event);) {
            if (!pause_key_pressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                    game_paused = !game_paused;
                    pause_key_pressed = true;
                };
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.scancode == sf::Keyboard::Scan::P) {
                    pause_key_pressed = false;
                }
            }
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                renderer.window.close();

            if (game_paused) {
                key_up_pressed = false;
                key_down_pressed = false;
                break;
            };

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::Scan::Up) {
                    key_up_pressed = true;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Down) {
                    key_down_pressed = true;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.scancode == sf::Keyboard::Scan::Up) {
                    key_up_pressed = false;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Down) {
                    key_down_pressed = false;
                }
            }
        }

        pong.update_game(deltaTime,key_up_pressed,key_down_pressed,game_paused);
    }
}
