//
// Created by vojta on 20.09.2024.
//

#ifndef UTIL_H
#define UTIL_H

class util {

};

bool detect_rect_collision_2d(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &a_size,
                              const sf::Vector2f &b_size);
float randomf(float min, float max);

float get_distance(const sf::Vector2f p1, const sf::Vector2f p2);
#endif //UTIL_H
