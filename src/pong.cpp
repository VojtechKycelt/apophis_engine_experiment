//
// Created by vojta on 17.09.2024.
//

#include "pong.h"
#include <iostream>
#include "util.h"

void Pong::check_goals(float deltaTime) {
    if (ball.position_.x + ball.shape_.getSize().x < 0) {
        bot_score += 1;
        ball.position_ = ball.start_position_;
        ball.velocity_ = sf::Vector2f(400.0, 0.0);
    }
    if (ball.position_.x + ball.velocity_.x * deltaTime > renderer_.WINDOW_SIZE.x) {
        player_score += 1;
        ball.position_ = ball.start_position_;
        ball.velocity_ = sf::Vector2f(-400.0, 0.0);
    }
}

void Pong::check_wall_collision(float deltaTime) {
    if (ball.position_.y + ball.velocity_.y * deltaTime < 0 || ball.position_.y + ball.velocity_.y * deltaTime >
        renderer_.WINDOW_SIZE.y - ball.shape_.getSize().y)
        ball.velocity_.y *= -1;
}

void Pong::check_paddle_single_axis_collision(const Paddle &paddle, const char &axis, float deltaTime) {
    if (axis == 'x') {
        if (detect_rect_collision_2d(player_paddle.position_,
                                  sf::Vector2f(ball.position_.x + ball.velocity_.x * deltaTime, ball.position_.y),
                                  player_paddle.shape_.getSize(), ball.shape_.getSize())) {
            ball.velocity_.x *= -ball_velocity_multiplier;
            ball.velocity_.y = randomf(-4.0, 4.0) * 100;
                                  } else {
                                      ball.position_.x += ball.velocity_.x * deltaTime;
                                  }
    }
    if (axis == 'y') {

    }

}


void Pong::render_unpaused(float deltaTime, bool key_up_pressed, bool key_down_pressed) {
    //check for collisions
    //move player paddle if there is no ball colliding up
    if (key_up_pressed && player_paddle.position_.y > player_paddle.shape_.getOutlineThickness()) {
        sf::Vector2f new_pos(player_paddle.position_.x,
                             player_paddle.position_.y - player_paddle.speed_ * deltaTime);
        if (!detect_rect_collision_2d(new_pos, ball.position_, player_paddle.shape_.getSize(),
                                      ball.shape_.getSize())) {
            player_paddle.position_.y -= player_paddle.speed_ * deltaTime;
        }
    }
    //move player paddle if there is no ball colliding down
    if (key_down_pressed && player_paddle.position_.y < renderer_.WINDOW_SIZE.y - player_paddle.shape_.getSize().
        y -
        player_paddle.shape_.getOutlineThickness()
    ) {
        sf::Vector2f new_pos(player_paddle.position_.x,
                             player_paddle.position_.y + player_paddle.speed_ * deltaTime);
        if (!detect_rect_collision_2d(new_pos, ball.position_, player_paddle.shape_.getSize(),
                                      ball.shape_.getSize())) {
            player_paddle.position_.y += player_paddle.speed_ * deltaTime;
        }
    }

    //goal check
    check_goals(deltaTime);

    //top and bottom walls collision check
    check_wall_collision(deltaTime);

    //check if ball collides with PLAYER paddle and either speed up and bounce back or move with velocity
    //separate x axis check

    if (detect_rect_collision_2d(player_paddle.position_,
                                 sf::Vector2f(ball.position_.x + ball.velocity_.x * deltaTime, ball.position_.y),
                                 player_paddle.shape_.getSize(), ball.shape_.getSize())) {
        ball.velocity_.x *= -ball_velocity_multiplier;
        ball.velocity_.y = randomf(-4.0, 4.0) * 100;
    } else {
        ball.position_.x += ball.velocity_.x * deltaTime;
    }
    //separate y axis check
    if (detect_rect_collision_2d(player_paddle.position_,
                                 sf::Vector2f(ball.position_.x, ball.position_.y + ball.velocity_.y * deltaTime),
                                 player_paddle.shape_.getSize(), ball.shape_.getSize())) {
        ball.velocity_.y *= -1;
    } else { ball.position_.y += ball.velocity_.y * deltaTime; }

    //check for bot paddle collision
    if (detect_rect_collision_2d(bot_paddle.position_,
                                 sf::Vector2f(ball.position_.x + ball.velocity_.x * deltaTime, ball.position_.y),
                                 bot_paddle.shape_.getSize(), ball.shape_.getSize())) {
        ball.velocity_.x *= -ball_velocity_multiplier;
        ball.velocity_.y = randomf(-4.0, 4.0) * 100;
    }
    if (detect_rect_collision_2d(bot_paddle.position_,
                                 sf::Vector2f(ball.position_.x, ball.position_.y + ball.velocity_.y * deltaTime),
                                 bot_paddle.shape_.getSize(), ball.shape_.getSize()))
        { ball.velocity_.y *= -1; }

    //BOT AI
    float ball_center_pos_y = ball.shape_.getPosition().y + ball.shape_.getSize().y / 2;
    if (bot_paddle.position_.y + bot_paddle.shape_.getSize().y / 2 > ball_center_pos_y) {
        if (!detect_rect_collision_2d(bot_paddle.position_,
                                      sf::Vector2f(ball.position_.x,
                                                   ball.position_.y + ball.velocity_.y * deltaTime),
                                      bot_paddle.shape_.getSize(),
                                      ball.shape_.getSize()) && bot_paddle.position_.y > 0) {
            bot_paddle.position_.y -= bot_paddle.speed_ * deltaTime;
        }
    }
    if (bot_paddle.position_.y + bot_paddle.shape_.getSize().y / 2 < ball_center_pos_y) {
        if (!detect_rect_collision_2d(bot_paddle.position_,
                                      sf::Vector2f(ball.position_.x,
                                                   ball.position_.y + ball.velocity_.y * deltaTime),
                                      bot_paddle.shape_.getSize(),
                                      ball.shape_.getSize()) && bot_paddle.position_.y + bot_paddle.shape_.getSize()
            .y <
            renderer_.
            window.getSize().y) {
            bot_paddle.position_.y += bot_paddle.speed_ * deltaTime;
        }
    }

    //set positions
    ball.shape_.setPosition(ball.position_);
    player_paddle.shape_.setPosition(player_paddle.position_);
    bot_paddle.shape_.setPosition(bot_paddle.position_);

    //draw everything
    std::string player_score_string = std::to_string(player_score);
    std::string bot_score_string = std::to_string(bot_score);
    sf::Text scoreboard(player_score_string += " : " + bot_score_string, font, 100);
    scoreboard.setFillColor(sf::Color::White);
    scoreboard.setOutlineColor(sf::Color::Black);
    //scoreboard.setOutlineThickness(1);
    sf::FloatRect textRect = scoreboard.getLocalBounds();
    scoreboard.setOrigin(textRect.left + textRect.width / 2.0f,
                         textRect.top + textRect.height / 2.0f);
    scoreboard.setPosition(sf::Vector2f(renderer_.window.getSize().x / 2.0f, 50));

    //display rectangle
    renderer_.window.clear(sf::Color::Green);
    renderer_.window.draw(ball.shape_);
    renderer_.window.draw(player_paddle.shape_);
    renderer_.window.draw(bot_paddle.shape_);
    renderer_.window.draw(scoreboard);
    renderer_.window.display();
}

void Pong::render_paused() {
    //render paused text
    sf::Text pause_text("PAUSED", font, 200);
    pause_text.setFillColor(sf::Color::Black);
    pause_text.setOutlineColor(sf::Color::White);
    pause_text.setOutlineThickness(1);
    sf::FloatRect textRect = pause_text.getLocalBounds();
    pause_text.setOrigin(textRect.left + textRect.width / 2.0f,
                         textRect.top + textRect.height / 2.0f);
    pause_text.setPosition(sf::Vector2f(renderer_.window.getSize().x / 2.0f,
                                        renderer_.window.getSize().y / 2.0f));
    renderer_.window.draw(pause_text);
    renderer_.window.display();
}

void Pong::update_game(float deltaTime, bool key_up_pressed, bool key_down_pressed, bool game_paused) {
    if (!game_paused) {
        render_unpaused(deltaTime, key_up_pressed, key_down_pressed);
    } else {
        render_paused();
    }
}


void Pong::init_game() {
    if (!font.loadFromFile("../../src/spaceranger.ttf"))
        std::cerr << "FONT NOT LOADED" << std::endl;
    //return EXIT_FAILURE;
}
