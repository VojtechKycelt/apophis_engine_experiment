#include <iostream>
#include <SFML/Graphics.hpp>
#include "renderer.h"


bool detect_collision_2d(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &a_size,
                         const sf::Vector2f &b_size) {
    float ax = a.x;
    float ay = a.y;
    float axe = a.x + a_size.x;
    float aye = a.y + a_size.y;

    float bx = b.x;
    float by = b.y;
    float bxe = b.x + b_size.x;
    float bye = b.y + b_size.y;

    if (ax < bx) {
        if (axe < bx) { return false; } //
        if (aye < by) { return false; } //
        if (ay > bye) { return false; } //
        return true;
    }
    if (ax > bxe) { return false; }
    if (aye < by) { return false; }
    if (ay > bye) { return false; }

    return true;
}

int main() {
    //TODO make cpp file just for Pong game and use Renderer header

    //declare renderer with window
    Renderer renderer(
        sf::Vector2i(1280, 720),
        "APOPHIS ENGINE EXPERIMENT",
        240);

    //declare rectangle
    sf::RectangleShape rect;
    sf::Vector2f rectanglePosition(540, 250);
    sf::Vector2f rect_velocity(0.0, 0.0);
    rect.setSize(sf::Vector2f(100, 100));
    rect.setFillColor(sf::Color(250, 250, 250));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(0);
    rect.setPosition(rectanglePosition);

    //declare paddle
    sf::RectangleShape player_paddle;
    sf::Vector2f player_paddle_position(20, renderer.WINDOW_SIZE.y / 2);
    float player_paddle_speed = 300.0;
    player_paddle.setSize(sf::Vector2f(20, 200));
    player_paddle.setFillColor(sf::Color::Red);
    player_paddle.setOutlineColor(sf::Color(0, 0, 250));
    player_paddle.setOutlineThickness(0);
    player_paddle.setPosition(player_paddle_position);

    bool key_up_pressed = false;
    bool key_down_pressed = false;
    bool key_left_pressed = false;
    bool key_right_pressed = false;

    //keep track of time between frames
    sf::Clock deltaClock;
    float deltaTime;

    //engine loop
    while (renderer.window.isOpen()) {
        //get delta time for same speed of the engine with different fps
        deltaTime = deltaClock.restart().asSeconds();

        //handle input
        for (auto event = sf::Event{}; renderer.window.pollEvent(event);) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                renderer.window.
                        close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::Scan::Up) {
                    key_up_pressed = true;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Down) {
                    key_down_pressed = true;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Right) {
                    key_right_pressed = true;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Left) {
                    key_left_pressed = true;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.scancode == sf::Keyboard::Scan::Up) {
                    key_up_pressed = false;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Down) {
                    key_down_pressed = false;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Right) {
                    key_right_pressed = false;
                }
                if (event.key.scancode == sf::Keyboard::Scan::Left) {
                    key_left_pressed = false;
                }
            }
        }

        if (key_up_pressed && player_paddle_position.y > player_paddle.getOutlineThickness()) {
            player_paddle_position.y -= player_paddle_speed * deltaTime;
        }
        if (key_down_pressed && player_paddle_position.y < renderer.WINDOW_SIZE.y - player_paddle.getSize().y -
            player_paddle.getOutlineThickness()) {
            player_paddle_position.y += player_paddle_speed * deltaTime;
        }
        if (key_right_pressed) {
            player_paddle_position.x += player_paddle_speed * deltaTime;
        }
        if (key_left_pressed) {
            player_paddle_position.x -= player_paddle_speed * deltaTime;
        }

        if (rectanglePosition.x < player_paddle_position.x + player_paddle.getSize().x + player_paddle.
            getOutlineThickness()) {
            if ((rectanglePosition.y < player_paddle_position.y
                 && rectanglePosition.y + rect.getSize().y > player_paddle_position.y)
                || (rectanglePosition.y > player_paddle_position.y && rectanglePosition.y + rect.getSize().y
                    < player_paddle_position.y + player_paddle.getSize().y + rect.getSize().y)) {
                rect_velocity.x *= -1;
            }
        }


        //physics
        if (rectanglePosition.x + rect_velocity.x * deltaTime < 0 || rectanglePosition.x + rect_velocity.x * deltaTime >
            renderer.WINDOW_SIZE.x - rect.getSize().x)
            rect_velocity.x *= -1;
        if (rectanglePosition.y + rect_velocity.y * deltaTime < 0 || rectanglePosition.y + rect_velocity.y * deltaTime >
            renderer.WINDOW_SIZE.y - rect.getSize().y)
            rect_velocity.y *= -1;
        rectanglePosition.x += rect_velocity.x * deltaTime;
        rectanglePosition.y += rect_velocity.y * deltaTime;

        rect.setPosition(rectanglePosition);
        player_paddle.setPosition(player_paddle_position);

        //display rectangle
        renderer.window.clear(sf::Color::Green);
        renderer.window.draw(rect);
        renderer.window.draw(player_paddle);
        renderer.window.display();
    }
}
