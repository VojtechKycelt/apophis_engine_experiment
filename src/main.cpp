#include <SFML/Graphics.hpp>
#include "renderer.h"

int main()
{
    //declare renderer with window
    Renderer renderer(sf::Vector2i(1280, 720), "APOPHIS ENGINE EXPERIMENT", 240);

    //declare rectangle
    sf::RectangleShape rect;
    sf::Vector2f rectanglePosition(540,250);
    sf::Vector2f rect_velocity(200.0, 200.0);
    rect.setSize(sf::Vector2f(100, 100));
    rect.setFillColor(sf::Color(250,250,250));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(10);
    rect.setPosition(rectanglePosition);

    //keep track of time between frames
    sf::Clock deltaClock;
    float deltaTime;

    //engine loop
    while (renderer.window.isOpen()) {

        //get delta time for same speed of the engine with different fps
        deltaTime = deltaClock.restart().asSeconds();

        //handle input
        for (auto event = sf::Event{}; renderer.window.pollEvent(event);) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) renderer.window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                rect_velocity.x *= 2;
                rect_velocity.y *= 2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                rect_velocity.x /= 2;
                rect_velocity.y /= 2;
            }
        }

        //physics
        if (rectanglePosition.x + rect_velocity.x * deltaTime < 0 || rectanglePosition.x + rect_velocity.x * deltaTime > renderer.WINDOW_SIZE.x - rect.getSize().x) rect_velocity.x *= -1;
        if (rectanglePosition.y + rect_velocity.y * deltaTime < 0 || rectanglePosition.y + rect_velocity.y * deltaTime > renderer.WINDOW_SIZE.y - rect.getSize().y) rect_velocity.y *= -1;
        rectanglePosition.x += rect_velocity.x * deltaTime;
        rectanglePosition.y += rect_velocity.y * deltaTime;
        rect.setPosition(rectanglePosition);

        //display rectangle
        renderer.render(rect);
    }
}


