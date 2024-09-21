//
// Created by vojta on 17.09.2024.
//

#ifndef PONG_H
#define PONG_H
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include "renderer.h"




class Paddle {
public:
    explicit Paddle(
        const sf::Vector2f &size,
        const sf::Vector2f &position,
        const sf::Color &fill_color,
        const float &speed)
        : size_(size), position_(position), fill_color_(fill_color), speed_(speed) {
        shape_.setSize(size);
        shape_.setPosition(position);
        shape_.setFillColor(fill_color);
    };

    sf::RectangleShape shape_;
    sf::Vector2f size_;
    sf::Vector2f position_;
    sf::Color fill_color_;
    float speed_;
};

class Ball {
public:
    explicit Ball(
        const sf::Vector2f &size,
        const sf::Vector2f &position,
        const sf::Vector2f &velocity,
        const sf::Color &fill_color)
        : size_(size), start_position_(position), position_(position), start_velocity_(velocity), velocity_(velocity),
          fill_color_(fill_color) {
        shape_.setSize(size);
        shape_.setPosition(position);
        shape_.setFillColor(fill_color);
    };

    sf::RectangleShape shape_;
    sf::Vector2f size_;
    sf::Vector2f start_position_;
    sf::Vector2f position_;
    sf::Vector2f start_velocity_;
    sf::Vector2f velocity_;
    sf::Color fill_color_;

};

class Pong {
private:
    Renderer& renderer_;
public:
    Pong(Renderer& renderer): renderer_(renderer) {};
    Ball ball = Ball(sf::Vector2f(100, 100),sf::Vector2f(540, 250), sf::Vector2f(-400.0, 0.0),sf::Color::White);
    Paddle player_paddle = Paddle(sf::Vector2f(20, 150), sf::Vector2f(20, renderer_.WINDOW_SIZE.y / 2), sf::Color::Blue, 200.0);
    Paddle bot_paddle = Paddle(sf::Vector2f(20, 150), sf::Vector2f(renderer_.WINDOW_SIZE.x - 40, renderer_.WINDOW_SIZE.y / 2),sf::Color::Red, 200.0);
    bool game_over_ = false;
    int player_score = 0;
    int bot_score = 0;
    float ball_velocity_multiplier = 1.05;
    void init_game();
    //TODO add class input mapper/manager and inside all key presses and change param to only Input class
    void update_game(float deltaTime,bool key_up_pressed, bool key_down_pressed, bool game_paused);
    sf::Font font;

};


#endif //PONG_H
