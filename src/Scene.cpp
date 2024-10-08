//
// Created by Vojtěch Kycelt on 08.10.2024.
//

#include "Scene.h"

void Scene::update(float deltaTime, bool game_paused) {
    //input manager - detect input press
    input_manager_.detect_input_press(renderer_,game_paused);

    //update everything

    //clear canvas
    //draw everything again
}
