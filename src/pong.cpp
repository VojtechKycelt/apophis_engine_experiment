//
// Created by vojta on 17.09.2024.
//

#include "pong.h"


void update_game(const Pong& pong) {
    //check for collisions
        //goal
        //paddle
        //walls

    //update ball position

    //update paddle position

    //draw everything

};

void init_game(const sf::Vector2f& ball_start_pos, const sf::Vector2f& ball_start_velocity){
    Pong pong_;
    pong_.ball_pos_ = ball_start_pos;
    pong_.ball_velocity_ = ball_start_velocity;
    pong_.game_over_ = false;

    while (!pong_.game_over_) {
        update_game(pong_);
    }
};
