//
// Created by Vojtěch Kycelt on 08.10.2024.
//

#ifndef APOPHISENGINEPROTOTYPE_GRAPH_H
#define APOPHISENGINEPROTOTYPE_GRAPH_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
class Node {
public:
    Node(int ID, const sf::Vector2f &position): ID_(ID), position_(position){};
    int ID_;
    sf::Vector2f position_;
};

class Edge {
public:
    Edge(int ID1, int ID2): ID1_(ID1), ID2_(ID2){}
    int ID1_;
    int ID2_;
};

class Graph {

public:
    Graph(){};
    std::unordered_map<int, Node> nodes;
    //std::vector<Node> nodes;
    std::vector<Edge> edges;

};


#endif //APOPHISENGINEPROTOTYPE_GRAPH_H
