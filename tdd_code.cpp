//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Yelyzaveta Ovsiannikova  <xovsia00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"
#include <set>
using namespace std;

Graph::Graph(){
}

Graph::~Graph(){
    clear();
}

vector<Node*> Graph::nodes() {
    return NodeList;
}

vector<Edge> Graph::edges() const{
    return EdgeList;
}

Node* Graph::addNode(size_t nodeId)
{
    for (auto& node : NodeList)
    {
        if (node->id == nodeId){
            return nullptr;
        }
    }
    auto newNode = new Node;
    newNode->id = nodeId;
    NodeList.push_back(newNode);
    return newNode;
}

bool Graph::addEdge(const Edge& edge)
{
    if (getNode(edge.b) == nullptr) {
        addNode(edge.b);
    }
    if (getNode(edge.a) == nullptr) {
        addNode(edge.a);
    }
    if (edge.b == edge.a) {
        return false;
    }
    for (auto& existingEdge : EdgeList)
    {
        if (existingEdge == edge) {
            return false; // edge already exists
        }
    }
    EdgeList.push_back(edge);
    return true;
}


void Graph::addMultipleEdges(const vector<Edge>& edges)
{
    for (auto &edge : edges) {
        addEdge(edge);
    }
}

Node* Graph::getNode(size_t nodeId)
{
    for (auto& node : NodeList)
    {
        if (node->id == nodeId){
            return node;
        }
    }
    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const
{
    for (auto existingEdge : EdgeList)
    {
        if (existingEdge == edge) {
            return true;
        }
    }
    return false;
}

void Graph::removeNode(size_t nodeId)
{
    auto n = NodeList.begin();
    while (n != NodeList.end()) {
        if ((*n)->id == nodeId) { // Remove edges connected to the node
            auto e = EdgeList.begin();
            while (e != EdgeList.end()) {
                if (e->a == nodeId) {
                    e = EdgeList.erase(e);
                    e--;
                } else {
                    if (e->b == nodeId) {
                        e = EdgeList.erase(e);
                        e--;
                    } else {
                        ++e;
                    }
                }
            }
            delete *n;// Delete the node and erase it from the graph
            n = NodeList.erase(n);
            break;
        } else {
            ++n;
        }
    }
    if (n == NodeList.end()) {
        throw out_of_range("Node does not exist");
    }
}

void Graph::removeEdge(const Edge& edge)
{
    auto e = EdgeList.begin();
    while (e != EdgeList.end()) {
        if (*e == edge) {
            EdgeList.erase(e);
            return;
        }
        ++e;
    }
    if (e == EdgeList.end()) {
        throw out_of_range("Edge does not exist");
    }
}

size_t Graph::nodeCount() const{
    return NodeList.size();
}

size_t Graph::edgeCount() const{
    return EdgeList.size();
}

size_t Graph::nodeDegree(size_t nodeId) const
{
    size_t degree = 0;
    for (auto& edge : EdgeList)
    {
        if (edge.a == nodeId) {
            degree++;
        } else {
            if (edge.b == nodeId) {
                degree++;
            }
        }
    }
    if (degree == 0) {
        throw out_of_range("Node does not exist");
    }
    return degree;
}

size_t Graph::graphDegree() const
{

    int maxGraphDegree = 0;
    for (auto& node : NodeList)
    {
        if (nodeDegree(node->id) > maxGraphDegree) {
            maxGraphDegree = nodeDegree(node->id);
        }
    }
    return maxGraphDegree;
}

void Graph::coloring()
{
    for (auto& node : NodeList) {
        node->color = 0; // Set all nodes to have no color
    }
    for (auto& node : NodeList) {
        set<int> usedColors; // Set of colors already used by neighbors
        for (auto& edge : EdgeList) {
            if (edge.a == node->id) { // If the node is the first node in the edge
                auto neighbor = getNode(edge.b); // Get the second node in the edge
                if (neighbor != nullptr && neighbor->color != 0) { // If the neighbor exists and has a color
                    usedColors.insert(neighbor->color); // Add the color to the set of used colors
                }
            } else if (edge.b == node->id) { // If the node is the second node in the edge
                auto neighbor = getNode(edge.a); // Get the first node in the edge
                if (neighbor != nullptr && neighbor->color != 0) { // If the neighbor exists and has a color
                    usedColors.insert(neighbor->color); // Add the color to the set of used colors
                }
            }
        }
        int color = 1;// Find the first unused color
        while (usedColors.find(color) != usedColors.end()) {
            color++;
        }
        node->color = color; // Set the node's color to the first unused color
    }
}

void Graph::clear()
{
    for (auto &node : NodeList) {
        delete node;
    }
    NodeList.clear();
    EdgeList.clear();
}

/*** Konec souboru tdd_code.cpp ***/
