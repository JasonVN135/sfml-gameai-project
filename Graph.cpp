#include "Graph.h"

using namespace std;

// Constructor

Graph::Graph(std::string vertexPath, std::string edgePath, bool directed) : vertices(0), isDirected(directed), heuristicFunc(&Graph::euclideanHeuristic){

    readVertices(vertexPath);
    readEdges(edgePath);
}

Graph::Graph(int V, bool directed) : vertices(0), isDirected(directed), heuristicFunc(&Graph::euclideanHeuristic) {
  generateRandomPositions(V);
}


Graph::Graph(bool directed) : vertices(0), isDirected(directed), heuristicFunc(&Graph::euclideanHeuristic) {
}

void Graph::readVertices(std::string vertexPath) {
    // Read a vertices
    std::ifstream file(vertexPath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << vertexPath <<  std::endl;
        return;
    }
    std::string line;

    // Get each line in the file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<float> row;

        while (std::getline(ss, cell, ',')) {
            try {
            row.push_back(std::stof(cell));
            } catch (const std::exception& e) {
            std::cerr << "Conversion Error: " << e.what() << " for values: " << cell << std::endl;
            }
            
        }

        // Check if the row has the correct number of elements (at least 3: ID, X, Y)
        if (row.size() >= 3) {
            float x = row[1];  // X-coordinate
            float y = row[2];  // Y-coordinate

            // Add the vertex using the addVertex function
            addVertex(x, y);
        }
    }

    file.close();
}


void Graph::readEdges(std::string edgePath) {

    // Read edges
    std::ifstream file(edgePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << edgePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<float> row;

        while (std::getline(ss, cell, ',')) {
            try {
                row.push_back(std::stof(cell));
            } catch (const std::exception& e) {
                std::cerr << "Conversion Error: " << e.what() << " for value: " << cell << std::endl;
                return; // Return empty if conversion fails
            }
        }

        if (row.size() >= 3) {
            int uID = static_cast<int>(row[0]);
            int vID = static_cast<int>(row[1]);
            int distance = static_cast<int>(row[2]);

            if (vertexPositions.find(uID) == vertexPositions.end() || vertexPositions.find(vID) == vertexPositions.end()) {
                std::cerr << "Vertex ID not found: " << uID << " or " << vID << std::endl;
                continue;
            }
            addEdge(uID, vID, distance);
        }
    }

    file.close();
}

// Generate random positions for vertices
void Graph::generateRandomPositions(int count) {
    for (int i = 0; i < count; i++) {
        float x = rand() % 960 + 20;
        float y = rand() % 960 + 20;

        addVertex(x, y);
    }

    int totalEdges = (int) ((((float) (rand() % 100) / 100) + 3) * count); // Generate 3.0-4.0x edges
    std::unordered_map<int, int> edgeCount; // Track edges per vertex

    while (totalEdges > 0) {
        
        int u = rand() % vertices;
        int v = rand() % vertices;

        if (u != v && edgeCount[u] < 10 && edgeCount[v] < 10) { 
            int weight = calculateDistance(getVertex(u), getVertex(v));
            edgeCount[u] += 1;
            edgeCount[v] += 1;
            
            addEdge(u, v, weight);
            totalEdges--;
        }
    }
}

int Graph::getVertexCount() {
    return vertices;
}

Vertex Graph::getVertex(int idx) {
    return vertexPositions[idx];
}

void Graph::addVertex(float x, float y) {

    vertexPositions[vertices] = {vertices, sf::Vector2f(x, y)};
    vertices += 1;
}


// Add edge with weight
void Graph::addEdge(int u, int v, float weight) {
    adjList[u].push_back({v, weight});
    if (!isDirected) {
        adjList[v].push_back({u, weight});
    }
}

void Graph::removeVertex(int u) {
    // Step 1: Remove the vertex from the adjacency list of all other vertices
    for (auto& [id, neighbors] : adjList) {
        neighbors.remove_if([u](const std::pair<int, float>& edge) {
            return edge.first == u;  // Remove all occurrences of vertexId
        });
    }

    // Step 2: Remove the vertex itself from the adjacency list
    adjList.erase(u);
}

void Graph::removeEdge(int u, int v) {
    // Step 1: Remove edge u -> v
    auto& neighborsU = adjList[u];
    neighborsU.remove_if([v](const std::pair<int, float>& edge) {
        return edge.first == v;// Check if the neighbor is v
    });  

    // Step 2: If undirected, also remove v -> u
    auto& neighborsV = adjList[v];
    neighborsV.remove_if([u](const std::pair<int, float>& edge) {
        return edge.first == u;  // Check if the neighbor is u
    });

}

// Draw the graph using SFML
void Graph::drawGraph(sf::RenderWindow &window) {

    // Draw edges
    for (auto &node : adjList) {
        int u = node.first;
        sf::Vector2f posU = vertexPositions[u].position;
        for (auto &neighbor : node.second) {
            int v = neighbor.first;
            sf::Vector2f posV = vertexPositions[v].position;

            sf::Vertex line[] = {sf::Vertex(posU, sf::Color::Blue), sf::Vertex(posV, sf::Color::Blue)};
            window.draw(line, 3, sf::Lines);
        }
    }

    // Draw vertices
    for (auto &node : vertexPositions) {

        int id = node.first;
        sf::Vector2f pos = node.second.position;

        sf::CircleShape circle(8);

        circle.setFillColor(sf::Color::Yellow);
        circle.setPosition(pos.x - 8, pos.y - 8); // Center the circle
        window.draw(circle);

        // Draw text label
        sf::Text text;
        text.setString(to_string(id));
        text.setCharacterSize(7);
        text.setFillColor(sf::Color::Black);
        text.setPosition(pos.x - 8, pos.y - 8);
        window.draw(text);
    }
}

int Graph::calculateDistance(Vertex u, Vertex v) {

  float xDistance = abs(u.position.x - v.position.x);
  float yDistance = abs(u.position.y - v.position.y);
  
  float squared = pow(xDistance, 2) + pow(yDistance, 2);
  float root = sqrt(squared);

  return round(root);

}


// Get position of a vertex
sf::Vector2f Graph::getPosition(int v) {
    return vertexPositions[v].position;
}

Vertex Graph::getClosestVertex(int xPos, int yPos) {

    float shortestDistance = std::numeric_limits<float>::infinity();;
    Vertex closestVertex = vertexPositions[0];

    for (auto& [pos, vertex] : vertexPositions) {
        float xDistance = abs(vertex.position.x - xPos);
        float yDistance = abs(vertex.position.y - yPos);
        
        float squared = pow(xDistance, 2) + pow(yDistance, 2);
        float root = sqrt(squared);

        if (root < shortestDistance) {
            shortestDistance = root;
            closestVertex = vertex;
        }
    }

    return closestVertex;
}


int Graph::dijkstra(int start, int end) {
  
  std::unordered_map<int, float> distances;
  std::unordered_map<int, int> prev; // Store previous node in path
  std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<>> pq;

  // Initialize distances as infinity
  for (const auto& [node, _] : adjList) {
      distances[node] = std::numeric_limits<float>::infinity();
  }

  distances[start] = 0;
  pq.emplace(0, start);

  while (!pq.empty()) {
      auto [currDistance, currNode] = pq.top();
      pq.pop();

      // If we reached the end node, break early
      if (currNode == end) break;

      // If distance is greater than recorded shortest distance, skip
      if (currDistance > distances[currNode]) continue;

      // Traverse neighbors
      for (const auto& [neighbor, weight] : adjList.at(currNode)) {
          float newDist = currDistance + weight;

          if (newDist < distances[neighbor]) {
              distances[neighbor] = newDist;
              prev[neighbor] = currNode; // Store previous node
              pq.emplace(newDist, neighbor);
          }
      }
  }

  // If the end node was never reached, return std::nullopt
  if (prev.find(end) == prev.end()) return -1;

  // Reconstruct path by backtracking from end to start
  int current = end;
  int firstStep = end; // Default to end if no valid path

  while (prev.find(current) != prev.end()) {
      if (prev[current] == start) {
          firstStep = current; // First move from start
          break;
      }
      current = prev[current];
  }

  return firstStep;
}

int Graph::astar(int start, int end) {
    std::unordered_map<int, float> gScore; // Cost from start to this node
    std::unordered_map<int, float> fScore; // Estimated total cost to goal
    std::unordered_map<int, int> prev; // Stores previous node in path
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<>> pq;

    // Initialize distances as infinity
    for (const auto& [node, _] : adjList) {
        gScore[node] = std::numeric_limits<float>::infinity();
        fScore[node] = std::numeric_limits<float>::infinity();
    }

    gScore[start] = 0;
    fScore[start] = heuristic(start, end); // Initial heuristic estimate
    pq.emplace(fScore[start], start);

    while (!pq.empty()) {
        auto [currFScore, currNode] = pq.top();
        pq.pop();

        if (currNode == end) break; // Reached destination

        for (const auto& [neighbor, weight] : adjList.at(currNode)) {
            float tentative_gScore = gScore[currNode] + weight;

            if (tentative_gScore < gScore[neighbor]) {
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + heuristic(neighbor, end);
                prev[neighbor] = currNode;
                pq.emplace(fScore[neighbor], neighbor);
            }
        }
    }

    // If goal was never reached
    if (prev.find(end) == prev.end()) return -1;

    // Reconstruct path (return the first step from start to end)
    int current = end;
    int firstStep = end;

    while (prev.find(current) != prev.end()) {
        if (prev[current] == start) {
            firstStep = current;
            break;
        }
        current = prev[current];
    }

    std::cout << start << "->" << firstStep << ": " << heuristic(start, firstStep) << std::endl;

    return firstStep;
}

float Graph::heuristic(int node, int goal) {
    return heuristicFunc(getVertex(node).position, getVertex(goal).position);
}

float Graph::manhattanHeuristic(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

float Graph::euclideanHeuristic(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

float Graph::squaredEuclideanHeuristic(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

void Graph::setHeuristic(std::function<float(const sf::Vector2f&, const sf::Vector2f&)> func) {
    heuristicFunc = func;
}

