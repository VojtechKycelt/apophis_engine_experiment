//
// Created by Vojtěch Kycelt on 08.10.2024.
//

#ifndef APOPHISENGINEPROTOTYPE_SCENE_H
#define APOPHISENGINEPROTOTYPE_SCENE_H


#include "input_manager.h"

class Scene {
public:
    Scene(Renderer &renderer): renderer_(renderer){};
    void update(float deltaTime, bool game_paused);
private:
    Renderer &renderer_;
    InputManager input_manager_;
};


#endif //APOPHISENGINEPROTOTYPE_SCENE_H
