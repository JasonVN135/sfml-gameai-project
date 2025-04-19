#ifndef GRAPH_H
#define GRAPH_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <cstdlib> 
#include <queue>
#include <fstream>
#include <sstream>
#include <cmath>
#include <functional>

// Define a Vertex structure
struct Vertex {
    int id;
    sf::Vector2f position; // SFML position
};

// Graph class
class Graph {
private:
    int vertices;
    bool isDirected;
    std::unordered_map<int, std::list<std::pair<int, float>>> adjList; // Adjacency list (vertex -> (neighbor, weight))
    std::unordered_map<int, Vertex> vertexPositions; // Store vertex positions

    std::function<float(const sf::Vector2f&, const sf::Vector2f&)> heuristicFunc;


public:
    // Constructor
    Graph(std::string vertexPath, std::string edgePath, bool directed = false);

    Graph(int V, bool directed = false);

    Graph(bool directed = false);

    void readVertices(std::string vertexPath);

    void readEdges(std::string edgePath);

    // Generate random positions for vertices
    void generateRandomPositions(int count);

    int getVertexCount();

    Vertex getVertex(int idx);

    void addVertex(float x, float y);

    // Add edge with weight
    void addEdge(int u, int v, float weight = 1.0f);

    void removeVertex(int u);

    // Remove edge between two vertices
    void removeEdge(int u, int v);

    // Draw the graph using SFML
    void drawGraph(sf::RenderWindow &window);

    // Get position of a vertex
    sf::Vector2f getPosition(int v);

    // Get the closest vertex
    Vertex getClosestVertex(int x, int y);

    int calculateDistance(Vertex u, Vertex v);

    int dijkstra(int start, int goal);

    int astar(int start, int goal);

    float heuristic(int start, int goal);

    static float manhattanHeuristic(const sf::Vector2f& a, const sf::Vector2f& b);

    static float euclideanHeuristic(const sf::Vector2f& a, const sf::Vector2f& b);

    static float squaredEuclideanHeuristic(const sf::Vector2f& a, const sf::Vector2f& b);

    void setHeuristic(std::function<float(const sf::Vector2f&, const sf::Vector2f&)> func);

};


#endif