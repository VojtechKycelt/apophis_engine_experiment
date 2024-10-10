//
// Created by Vojtěch Kycelt on 08.10.2024.
//

#include "Scene.h"
#include "util.h"
#include <iostream>
#include <map>
#include <set>

void Scene::update(float deltaTime, bool game_paused) {
    if (!initialized_) {
        init();
        initialized_ = true;
    }
    //input manager - detect input press
    input_manager_.detect_input_press(renderer_, game_paused);

    //update everything

    //clear canvas
    renderer_.window.clear(sf::Color::Blue);

    //draw everything again
    draw_graph();

    //show update in window
    renderer_.window.display();

}

void Scene::init() {
    init_graph();
    auto it1 = graph.nodes.find(0);
    auto it2 = graph.nodes.find(9);
    if (it1 != graph.nodes.end() && it2 != graph.nodes.end()) {
        astar(&it1->second, &it2->second);
    } else {
        std::cout << "node not found" << std::endl;
    }
}


void print_final_path(Node *start, Node *end, const std::vector<int> &final_path) {
    std::cout << "start: " << end->ID_ << ", ";
    for (auto it = final_path.begin(); it != final_path.end(); ++it) {
        std::cout << *it << ", ";
    }
    std::cout << start->ID_ << " - END" << std::endl;
}

std::vector<int> construct_final_path(const std::map<int, Node *> &path, Node* current, Node* start){
    std::vector<int> final_path;
    std::cout << "FOUND path" << std::endl;
    auto it_curr = path.find(current->ID_);
    if (it_curr != path.end()){
        final_path.push_back(it_curr->second->ID_);
        while (it_curr->first != start->ID_) {
            auto it2 = it_curr->second;
            it_curr = path.find(it2->ID_);
            if (it_curr == path.end()) break;
            final_path.push_back(it_curr->second->ID_);
        }
    };
    return final_path;
}

class Compare {
public:
    bool operator()(Node* a, Node* b){
        float dist1 = get_distance(a->position_, end_pos);
        float dist2 = get_distance(b->position_, end_pos);
        return dist1 < dist2;
    }

sf::Vector2f end_pos;
};

std::vector<int> Scene::astar(Node *start, Node *end) {
    std::vector<int> final_path;

    std::vector<Node *> adjacent_nodes = find_all_edges_from_node(start);
    //priority queue - fringe that returns node with smallest distance
    auto cmp = [end](Node* left, Node* right){return (get_distance(left->position_,end->position_)) < (get_distance(right->position_,end->position_));};
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> fringe(cmp);
    //std::priority_queue<Node *, Compare> fringe;
    //TODO sort fringe to choose the point with heuristics - closest dist to end node
    for (auto &&an: adjacent_nodes) fringe.push(an);
    std::map<int, float> visited{{start->ID_,0.0}};
    std::map<int, Node *> path;

    while (!fringe.empty()) {
        Node *current = fringe.top();
        std::cout << fringe.top()->ID_ << ", ";
        fringe.pop();
        if (current == end) {
            final_path = construct_final_path(path, current, start);
            //print_final_path(start, end, final_path);
            break;
        }
        auto it = visited.find(current->ID_);
        visited.insert({current->ID_, get_distance(current->position_,start->position_)+it->second});
        adjacent_nodes = find_all_edges_from_node(current);
        for (auto &&an: adjacent_nodes) {
            auto it_visited = visited.find(an->ID_);
            if (visited.count(an->ID_) != 0) {
                float dist = get_distance(an->position_,current->position_) + it->second;
                if (it_visited->second > dist){
                    visited[current->ID_] = dist;
                }
            } else {
                fringe.push(an);
                path.insert({an->ID_, current});
            }
        }
    }
    return final_path;
}


std::vector<Node *> Scene::find_all_edges_from_node(Node *node) {
    std::vector<Node *> adjacent_nodes;
    for (auto it = graph.edges.begin(); it != graph.edges.end(); ++it) {
        if (it->ID1_ == node->ID_) {
            auto it2 = graph.nodes.find(it->ID2_);
            adjacent_nodes.push_back(&it2->second);
        } else if (it->ID2_ == node->ID_) {
            auto it2 = graph.nodes.find(it->ID1_);
            adjacent_nodes.push_back(&it2->second);
        }
    }
    return adjacent_nodes;
}

void Scene::init_graph() {

    Node n0(0, sf::Vector2f(300, 300));
    Node n1(1, sf::Vector2f(130, 100));
    Node n2(2, sf::Vector2f(400, 100));
    Node n3(3, sf::Vector2f(150, 300));
    Node n4(4, sf::Vector2f(200, 400));
    Node n5(5, sf::Vector2f(700, 300));
    Node n6(6, sf::Vector2f(renderer_.window.getSize().x - 100, 200));
    Node n7(7, sf::Vector2f(renderer_.window.getSize().x - 100, renderer_.window.getSize().y - 100));
    Node n8(8, sf::Vector2f(renderer_.window.getSize().x - 300, renderer_.window.getSize().y - 20));
    Node n9(9, sf::Vector2f(renderer_.window.getSize().x / 2, renderer_.window.getSize().y - 400));
    Node n10(10, sf::Vector2f(80, renderer_.window.getSize().y - 50));

    graph.nodes.insert({n0.ID_, n0});
    graph.nodes.insert({n1.ID_, n1});
    graph.nodes.insert({n2.ID_, n2});
    graph.nodes.insert({n3.ID_, n3});
    graph.nodes.insert({n4.ID_, n4});
    graph.nodes.insert({n5.ID_, n5});
    graph.nodes.insert({n6.ID_, n6});
    graph.nodes.insert({n7.ID_, n7});
    graph.nodes.insert({n8.ID_, n8});
    graph.nodes.insert({n9.ID_, n9});
    graph.nodes.insert({n10.ID_, n10});


    Edge e0(0, 1);
    Edge e1(1, 2);
    Edge e2(1, 3);
    Edge e3(3, 0);
    Edge e4(3, 4);
    Edge e5(5, 2);
    Edge e6(6, 2);
    Edge e7(6, 8);
    Edge e8(7, 9);
    Edge e10(10, 9);
    Edge e12(8, 4);
    Edge e13(10, 1);


    graph.edges.push_back(e0);
    graph.edges.push_back(e1);
    graph.edges.push_back(e2);
    graph.edges.push_back(e3);
    graph.edges.push_back(e4);
    graph.edges.push_back(e5);
    graph.edges.push_back(e6);
    graph.edges.push_back(e7);
    graph.edges.push_back(e8);
    graph.edges.push_back(e10);
    graph.edges.push_back(e12);
    graph.edges.push_back(e13);
}

void Scene::draw_graph() {

    for (auto it = graph.edges.begin(); it != graph.edges.end(); ++it) {
        sf::Vector2f pos1 = graph.nodes.find(it->ID1_)->second.position_;
        sf::Vector2f pos2 = graph.nodes.find(it->ID2_)->second.position_;
        sf::Vertex line[] =
                {
                        sf::Vertex(pos1),
                        sf::Vertex(pos2)
                };
        renderer_.window.draw(line, 2, sf::Lines);
    }

    for (auto &&node: graph.nodes) {
        sf::RectangleShape shape_;
        shape_.setSize(sf::Vector2f(10, 10));
        shape_.setPosition(node.second.position_ - sf::Vector2f(5, 5));
        renderer_.window.draw(shape_);

        std::string id = std::to_string(int(node.second.ID_));
        std::string px = std::to_string(int(node.second.position_.x));
        std::string py = std::to_string(int(node.second.position_.y));
        sf::Text position_text(id + ": [" + px += " , " + py + "]", font_, 20);
        position_text.setFillColor(sf::Color::Red);
        position_text.setOutlineColor(sf::Color::Black);
        position_text.setOutlineThickness(1);
        sf::FloatRect textRect = position_text.getLocalBounds();
        position_text.setOrigin(textRect.left + textRect.width / 2.0f,
                                textRect.top + textRect.height / 2.0f);
        position_text.setPosition(sf::Vector2f(node.second.position_.x, node.second.position_.y - 15));
        position_text.setStyle(0);
        renderer_.window.draw(position_text);
    }
}
