//
// Created by Vojtěch Kycelt on 08.10.2024.
//

#ifndef APOPHISENGINEPROTOTYPE_SCENE_H
#define APOPHISENGINEPROTOTYPE_SCENE_H


#include "input_manager.h"
#include "Graph.h"

class Scene {
public:
    Scene(Renderer &renderer, sf::Font &font): renderer_(renderer), font_(font), initialized_(false){};
    void init();
    void init_graph();
    void draw_graph();
    void update(float deltaTime, bool game_paused);
    std::vector<int> astar(Node* start, Node* end);
    std::vector<Node *> find_all_edges_from_node(Node* node);
    Graph graph;
    sf::Font font_;
    bool initialized_;

private:
    Renderer &renderer_;
    InputManager input_manager_;
};


#endif //APOPHISENGINEPROTOTYPE_SCENE_H
