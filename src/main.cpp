#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "pong.h"
#include "renderer.h"
#include "util.h"
#include "input_manager.h"

int main() {
    //declare renderer with window
    Renderer renderer(
            sf::Vector2i(1280, 720),
            "APOPHIS ENGINE EXPERIMENT",
            60);
    sf::Font font;
    if (!font.loadFromFile("../../src/spaceranger.ttf"))
        return EXIT_FAILURE;

    //keep track of time between frames
    sf::Clock deltaClock;
    float deltaTime;

    //smooth input declaration
    bool game_paused = false;
    //TODO put input manager into the Pong game
    InputManager input_manager;
    Pong pong(renderer,input_manager, font);
    //pong.init_game();


    //engine loop
    while (renderer.window.isOpen()) {
        //get delta time for same speed of the engine with different fps
        deltaTime = deltaClock.restart().asSeconds();

        input_manager.detect_input_press(renderer, game_paused);

        pong.update_game(deltaTime,game_paused);
    }
}
