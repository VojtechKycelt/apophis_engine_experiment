//
// Created by vojta on 20.09.2024.
//
#include <SFML/Graphics.hpp>
#include "util.h"
#include <random>

//returns true if 2 rectangles are colliding
bool detect_rect_collision_2d(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &a_size,
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

//returns random float number value
float randomf(float min, float max) {
    std::random_device rd; // Získání náhodného semene
    std::mt19937 gen(rd()); // Mersenne Twister generátor s náhodným semenem
    std::uniform_real_distribution<> dist(min, max); // Rozmezí 1.0 - 2.0

    // Generování náhodného float čísla
    float randomFloat = dist(gen);

    return randomFloat;
}

float get_distance(const sf::Vector2f p1, const sf::Vector2f p2) {
    float distance = pow(fabs(p1.x - p2.x),2)  + pow(fabs(p1.y - p2.y),2);
    distance = pow(distance,0.5);
    return distance;
}
