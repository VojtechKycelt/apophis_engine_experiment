#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "util.h"

int main() {
    //TODO make cpp file just for Pong game and use Renderer header

    //declare renderer with window
    Renderer renderer(
        sf::Vector2i(1280, 720),
        "APOPHIS ENGINE EXPERIMENT",
        240);

    //declare rectangle
    sf::RectangleShape rect;
    sf::Vector2f rect_start_position(540, 250);
    sf::Vector2f rect_position(rect_start_position);
    sf::Vector2f rect_start_velocity(-400.0, 0.0);
    sf::Vector2f rect_velocity(rect_start_velocity);
    rect.setSize(sf::Vector2f(100, 100));
    rect.setFillColor(sf::Color(250, 250, 250));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(0);
    rect.setPosition(rect_position);

    //declare player paddle
    sf::RectangleShape player_paddle;
    sf::Vector2f player_paddle_position(20, renderer.WINDOW_SIZE.y / 2);
    float player_paddle_speed = 200.0;
    player_paddle.setSize(sf::Vector2f(20, 150));
    player_paddle.setFillColor(sf::Color::Blue);
    player_paddle.setPosition(player_paddle_position);

    //declare bot paddle
    sf::RectangleShape bot_paddle;
    sf::Vector2f bot_paddle_position(renderer.WINDOW_SIZE.x - 40, renderer.WINDOW_SIZE.y / 2);
    float bot_paddle_speed = 200.0;
    bot_paddle.setSize(sf::Vector2f(20, 150));
    bot_paddle.setFillColor(sf::Color::Red);
    bot_paddle.setPosition(bot_paddle_position);

    //smooth input declaration
    bool key_up_pressed = false;
    bool key_down_pressed = false;

    int player_score = 0;
    int bot_score = 0;
    float ball_velocity_multiplier = 1.05;

    //keep track of time between frames
    sf::Clock deltaClock;
    float deltaTime;

    bool game_paused = false;
    bool pause_key_pressed = false;

    sf::Font font;
    if (!font.loadFromFile("../../src/spaceranger.ttf"))
        return EXIT_FAILURE;

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


        if (!game_paused) {
            if (key_up_pressed && player_paddle_position.y > player_paddle.getOutlineThickness()) {
                sf::Vector2f new_pos(player_paddle_position.x,
                                     player_paddle_position.y - player_paddle_speed * deltaTime);
                if (!detect_rect_collision_2d(new_pos, rect_position, player_paddle.getSize(), rect.getSize())) {
                    player_paddle_position.y -= player_paddle_speed * deltaTime;
                }
            }
            if (key_down_pressed && player_paddle_position.y < renderer.WINDOW_SIZE.y - player_paddle.getSize().y -
                player_paddle.getOutlineThickness()) {
                sf::Vector2f new_pos(player_paddle_position.x,
                                     player_paddle_position.y + player_paddle_speed * deltaTime);
                if (!detect_rect_collision_2d(new_pos, rect_position, player_paddle.getSize(), rect.getSize())) {
                    player_paddle_position.y += player_paddle_speed * deltaTime;
                }
            }

            //physics
            if (rect_position.x + rect.getSize().x < 0) {
                bot_score += 1;
                rect_position = rect_start_position;
                rect_velocity = sf::Vector2f(400.0, 0.0);
            }
            if (rect_position.x + rect_velocity.x * deltaTime > renderer.WINDOW_SIZE.x) {
                player_score += 1;
                rect_position = rect_start_position;
                rect_velocity = sf::Vector2f(-400.0, 0.0);
            }

            if (rect_position.y + rect_velocity.y * deltaTime < 0 || rect_position.y + rect_velocity.y * deltaTime >
                renderer.WINDOW_SIZE.y - rect.getSize().y)
                rect_velocity.y *= -1;

            if (!detect_rect_collision_2d(player_paddle_position,
                                          sf::Vector2f(rect_position.x + rect_velocity.x * deltaTime, rect_position.y),
                                          player_paddle.getSize(), rect.getSize())) {
                rect_position.x += rect_velocity.x * deltaTime;
            } else {
                rect_velocity.x *= -ball_velocity_multiplier;
                rect_velocity.y = randomf(-4.0, 4.0) * 100;
            }
            if (!detect_rect_collision_2d(player_paddle_position,
                                          sf::Vector2f(rect_position.x, rect_position.y + rect_velocity.y * deltaTime),
                                          player_paddle.getSize(), rect.getSize())) {
                rect_position.y += rect_velocity.y * deltaTime;
            } else { rect_velocity.y *= -1; }

            if (detect_rect_collision_2d(bot_paddle_position,
                                         sf::Vector2f(rect_position.x + rect_velocity.x * deltaTime, rect_position.y),
                                         bot_paddle.getSize(), rect.getSize())) {
                rect_velocity.x *= -ball_velocity_multiplier;
                rect_velocity.y = randomf(-4.0, 4.0) * 100;
            }
            if (detect_rect_collision_2d(bot_paddle_position,
                                         sf::Vector2f(rect_position.x, rect_position.y + rect_velocity.y * deltaTime),
                                         bot_paddle.getSize(), rect.getSize())) { rect_velocity.y *= -1; }

            //BOT AI
            float ball_center_pos_y = rect.getPosition().y + rect.getSize().y / 2;
            if (bot_paddle_position.y + bot_paddle.getSize().y / 2 > ball_center_pos_y) {
                if (!detect_rect_collision_2d(bot_paddle_position,
                                              sf::Vector2f(rect_position.x,
                                                           rect_position.y + rect_velocity.y * deltaTime),
                                              bot_paddle.getSize(), rect.getSize()) && bot_paddle_position.y > 0) {
                    bot_paddle_position.y -= bot_paddle_speed * deltaTime;
                }
            }
            if (bot_paddle_position.y + bot_paddle.getSize().y / 2 < ball_center_pos_y) {
                if (!detect_rect_collision_2d(bot_paddle_position,
                                              sf::Vector2f(rect_position.x,
                                                           rect_position.y + rect_velocity.y * deltaTime),
                                              bot_paddle.getSize(),
                                              rect.getSize()) && bot_paddle_position.y + bot_paddle.getSize().y <
                    renderer.
                    window.getSize().y) {
                    bot_paddle_position.y += bot_paddle_speed * deltaTime;
                }
            }

            rect.setPosition(rect_position);
            player_paddle.setPosition(player_paddle_position);
            bot_paddle.setPosition(bot_paddle_position);


            //display score

            std::string player_score_string = std::to_string(player_score);
            std::string bot_score_string = std::to_string(bot_score);
            sf::Text scoreboard(player_score_string += " : " + bot_score_string, font, 100);
            scoreboard.setFillColor(sf::Color::White);
            scoreboard.setOutlineColor(sf::Color::Black);
            //scoreboard.setOutlineThickness(1);
            sf::FloatRect textRect = scoreboard.getLocalBounds();
            scoreboard.setOrigin(textRect.left + textRect.width / 2.0f,
                                 textRect.top + textRect.height / 2.0f);
            scoreboard.setPosition(sf::Vector2f(renderer.window.getSize().x / 2.0f, 50));

            //display rectangle
            renderer.window.clear(sf::Color::Green);
            renderer.window.draw(rect);
            renderer.window.draw(player_paddle);
            renderer.window.draw(bot_paddle);
            renderer.window.draw(scoreboard);
            renderer.window.display();

        } else {
            //render paused text
            sf::Text pause_text("PAUSED",font, 200);
            pause_text.setFillColor(sf::Color::Black);
            pause_text.setOutlineColor(sf::Color::White);
            pause_text.setOutlineThickness(1);
            sf::FloatRect textRect = pause_text.getLocalBounds();
            pause_text.setOrigin(textRect.left + textRect.width / 2.0f,
                                 textRect.top + textRect.height / 2.0f);
            pause_text.setPosition(sf::Vector2f(renderer.window.getSize().x / 2.0f, renderer.window.getSize().y / 2.0f));
            renderer.window.draw(pause_text);
            renderer.window.display();

        }

    }
}
