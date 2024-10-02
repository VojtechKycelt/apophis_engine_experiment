//
// Created by vojta on 17.09.2024.
//

#include "pong.h"
#include <iostream>
#include "util.h"

void Pong::check_goals() {
    if (ball.position_.x + ball.shape_.getSize().x < 0) {
        bot_score += 1;
        ball.position_ = ball.start_position_;
        ball.velocity_ = sf::Vector2f(400.0, 0.0);
    }
    if (ball.position_.x + ball.velocity_.x * delta_time > renderer_.WINDOW_SIZE.x) {
        player_score += 1;
        ball.position_ = ball.start_position_;
        ball.velocity_ = sf::Vector2f(-400.0, 0.0);
    }
}

void Pong::check_wall_collision() {
    if (ball.position_.y + ball.velocity_.y * delta_time < 0 || ball.position_.y + ball.velocity_.y * delta_time >
        renderer_.WINDOW_SIZE.y - ball.shape_.getSize().y)
        ball.velocity_.y *= -1;
}

void Pong::check_paddle_single_axis_collision(const Paddle &paddle, char axis) {
    if (axis == 'x' && detect_rect_collision_2d(paddle.position_,
                                                sf::Vector2f(ball.position_.x + ball.velocity_.x * delta_time,
                                                             ball.position_.y),
                                                paddle.shape_.getSize(), ball.shape_.getSize())) {
        ball.velocity_.x *= -ball_velocity_multiplier;
        ball.velocity_.y = randomf(-4.0, 4.0) * 100;
    }
    if (axis == 'y' && detect_rect_collision_2d(paddle.position_,
                                                sf::Vector2f(ball.position_.x,
                                                             ball.position_.y + ball.velocity_.y * delta_time),
                                                paddle.shape_.getSize(), ball.shape_.getSize())) {
        ball.velocity_.y *= -ball_velocity_multiplier;
    }
}

void Pong::move_ball() {
    ball.position_.x += ball.velocity_.x * delta_time;
    ball.position_.y += ball.velocity_.y * delta_time;
}

void Pong::move_bot_paddle() {
    //BOT AI
    float ball_center_pos_y = ball.shape_.getPosition().y + ball.shape_.getSize().y / 2;
    if ((bot_paddle.position_.y + bot_paddle.shape_.getSize().y / 2 > ball_center_pos_y) && (!detect_rect_collision_2d(
                bot_paddle.position_,
                sf::Vector2f(ball.position_.x,
                             ball.position_.y + ball.velocity_.y * delta_time),
                bot_paddle.shape_.getSize(),
                ball.shape_.getSize()) && bot_paddle.position_.y > 0)) {
        bot_paddle.position_.y -= bot_paddle.speed_ * delta_time;
    }
    if ((bot_paddle.position_.y + bot_paddle.shape_.getSize().y / 2 < ball_center_pos_y) && !detect_rect_collision_2d(
            bot_paddle.position_,
            sf::Vector2f(ball.position_.x,
                         ball.position_.y + ball.velocity_.y * delta_time),
            bot_paddle.shape_.getSize(),
            ball.shape_.getSize()) && bot_paddle.position_.y + bot_paddle.shape_.getSize()
        .y <
        renderer_.
        window.getSize().y) {
        bot_paddle.position_.y += bot_paddle.speed_ * delta_time;
    }
}

void Pong::move_player_paddle_up(bool key_up_pressed) {
    sf::Vector2f new_pos(player_paddle.position_.x,
                         player_paddle.position_.y - player_paddle.speed_ * delta_time);
    if ((key_up_pressed && player_paddle.position_.y > player_paddle.shape_.getOutlineThickness()) && (!
            detect_rect_collision_2d(new_pos, ball.position_, player_paddle.shape_.getSize(),
                                     ball.shape_.getSize())
        )) {
        player_paddle.position_.y -= player_paddle.speed_ * delta_time;
    }
}

void Pong::move_player_paddle_down(bool key_down_pressed) {
    sf::Vector2f new_pos(player_paddle.position_.x,
                         player_paddle.position_.y + player_paddle.speed_ * delta_time);
    if ((key_down_pressed && player_paddle.position_.y < renderer_.WINDOW_SIZE.y - player_paddle.shape_.getSize().
         y -
         player_paddle.shape_.getOutlineThickness()
        ) && (!detect_rect_collision_2d(new_pos, ball.position_, player_paddle.shape_.getSize(),
                                        ball.shape_.getSize()))) {
        player_paddle.position_.y += player_paddle.speed_ * delta_time;
    }
}

void Pong::render_unpaused() {
    //goal check
    check_goals();

    //top and bottom walls collision check
    check_wall_collision();

    //check if ball collides with PLAYER paddle and either speed up and bounce back or move with velocity
    //separate player x axis check
    check_paddle_single_axis_collision(player_paddle, 'x');
    //separate player y axis check
    check_paddle_single_axis_collision(player_paddle, 'y');
    //separate bot x axis check
    check_paddle_single_axis_collision(bot_paddle, 'x');
    //separate bot y axis check
    check_paddle_single_axis_collision(bot_paddle, 'y');

    //move player paddle if there is no ball colliding up
    move_player_paddle_up(input_manager_.is_key_pressed("UP"));

    //move player paddle if there is no ball colliding down
    move_player_paddle_down(input_manager_.is_key_pressed("DOWN"));

    //move bot AI
    move_bot_paddle();

    //move ball
    move_ball();

    //set positions
    ball.shape_.setPosition(ball.position_);
    player_paddle.shape_.setPosition(player_paddle.position_);
    bot_paddle.shape_.setPosition(bot_paddle.position_);

    //declare UI
    std::string player_score_string = std::to_string(player_score);
    std::string bot_score_string = std::to_string(bot_score);
    sf::Text scoreboard(player_score_string += " : " + bot_score_string, font_, 100);
    scoreboard.setFillColor(sf::Color::White);
    scoreboard.setOutlineColor(sf::Color::Black);
    //scoreboard.setOutlineThickness(1);
    sf::FloatRect textRect = scoreboard.getLocalBounds();
    scoreboard.setOrigin(textRect.left + textRect.width / 2.0f,
                         textRect.top + textRect.height / 2.0f);
    scoreboard.setPosition(sf::Vector2f(renderer_.window.getSize().x / 2.0f, 50));

    //draw everything
    renderer_.window.clear(sf::Color::Green);
    renderer_.window.draw(ball.shape_);
    renderer_.window.draw(player_paddle.shape_);
    renderer_.window.draw(bot_paddle.shape_);
    renderer_.window.draw(scoreboard);
    renderer_.window.display();
}

void Pong::render_paused() const {
    //declare and draw paused text
    sf::Text pause_text("PAUSED", font_, 200);
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

void Pong::update_game(float deltaTime, bool game_paused) {
    delta_time = deltaTime;
    if (!game_paused) {
        render_unpaused();
    } else {
        render_paused();
    }
}


void Pong::init_game() {

}
