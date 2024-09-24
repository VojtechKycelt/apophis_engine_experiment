#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>

class Renderer {
public:
    Renderer(const Renderer &);
    Renderer(const sf::Vector2i& window_size, const std::string& window_name, const int& fps_limit) : WINDOW_SIZE(window_size),
        window_name_(window_name) {
        window.setFramerateLimit(fps_limit);
    };
    const sf::Vector2i WINDOW_SIZE;
    std::string window_name_;
    sf::RenderWindow window = sf::RenderWindow(
        sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y),
        window_name_
    );

    void render(const sf::RectangleShape& rect) {
        window.clear(sf::Color::Green);
        window.draw(rect);
        window.display();
    }

};

#endif //RENDERER_H
