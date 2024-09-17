//
// Created by vojta on 17.09.2024.
//

#ifndef PONG_H
#define PONG_H
#include "SFML/System/Vector2.hpp"


class Pong {
public:
    sf::Vector2f ball_pos_;
    sf::Vector2f ball_velocity_;
    bool game_over_ = false;
};


#endif //PONG_H
