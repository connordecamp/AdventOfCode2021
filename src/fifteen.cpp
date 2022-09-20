/*
 *
 */

#include <bits/stdc++.h>

typedef std::vector<std::vector<uint8_t>> AdjMatrix;

class Graph {
    int numVertices;
    std::list<std::pair<int, int>>* adjacencyList;

public:
    Graph(int numVertices) : numVertices(numVertices) {
        adjacencyList = new std::list<std::pair<int, int>>[numVertices];
    }

    void addEdge(int u, int v, int w) {
        adjacencyList[u].push_back(std::make_pair(v, w));
    }

    void calculateShortestPaths(int src, int dest) {
        typedef std::pair<int, int> int_pair;
        std::priority_queue<
            int_pair, std::vector<int_pair>, std::greater<int_pair> > pq;
        
        std::vector<int> distances(numVertices, INT_MAX);
        pq.push(std::make_pair(0, src));
        distances[src] = 0;

        while(!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for(const auto& iter : adjacencyList[u]) {
                int v = iter.first;
                int weight = iter.second;

                if(distances[v] > distances[u] + weight) {
                    distances[v] = distances[u] + weight;
                    pq.push(std::make_pair(distances[v], v));
                }
            }
        }

        std::cout << "Shortest path from node " << src << " to node " << dest << " is: ";
        std::cout << distances[numVertices - 1] << '\n';
    }
};

bool isValidMatrixIndex(size_t i, size_t j, const AdjMatrix& m) {
    bool validRow = (i >= 0 && i < m.size());
    bool validCol = (j >= 0 && j < m[0].size());

    return validRow && validCol;
}

void dijkstra(const AdjMatrix& graph)
{
    auto convertToIndex = [&graph](size_t i, size_t j) {
        return i * graph[0].size() + j;
    };

    auto indexToBoardPos = [&graph](size_t index) {
        std::pair<size_t, size_t> pos;

        pos.first = index / graph[0].size();
        pos.second = index % graph[0].size();

        return pos;
    };

    auto isValidIndex = [&graph](size_t i, size_t j) {
        bool validRow = (i >= 0 && i < graph.size());
        bool validCol = (j >= 0 && j < graph[0].size());

        return validRow && validCol;
    };

    size_t numVertices = graph.size() * graph[0].size();

    constexpr int Undefined = -1;

    std::vector<int> distances(numVertices, INT_MAX);
    std::vector<int> previous(numVertices, Undefined);
    std::unordered_set<int> vertices;
    for(size_t i = 0; i < graph.size(); i++) {
        for(size_t j = 0; j < graph[0].size(); j++){
            size_t index = convertToIndex(i, j);
            vertices.insert(index);
        }
    }
    
    distances[0] = 0;
    
    const int Directions[4][2] {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    while(!vertices.empty()) {
        int u = Undefined;
        int minDistance = INT_MAX;
        for(int v : vertices) {
            if(distances[v] < minDistance) {
                minDistance = distances[v];
                u = v;
            }
        }

        auto pair = indexToBoardPos(u);
        if(pair.first == graph.size() - 1 && pair.second == graph[0].size() - 1)
            break;
        vertices.erase(u);

        for(const auto& dir : Directions) {
            size_t newI = pair.first + dir[0];
            size_t newJ = pair.second + dir[1];

            int alt = INT_MAX;

            if(isValidIndex(newI, newJ)) {
                size_t v = convertToIndex(newI, newJ);
                bool foundIndex = (vertices.find(v) != vertices.end());
                if(foundIndex) {
                    alt = distances[u] + graph[newI][newJ];
                    if(alt < distances[v]) {
                        distances[v] = alt;
                        previous[v] = u;
                    }
                }
            }
        }

    }

    size_t u = convertToIndex(graph.size() - 1, graph[0].size() - 1);
    int cost = 0;
    while(u > 0) {
        auto pair = indexToBoardPos(u);
        cost += graph[pair.first][pair.second];
        u = previous[u];
    }

    std::cout << "Cost = " << cost << '\n';
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected = 2;
    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    std::string line;

    AdjMatrix data;
    while(std::getline(infile, line)) {
        std::vector<uint8_t> temp;
        for(const char c : line) {
            temp.push_back((int)(c - '0'));
        }
        data.push_back(std::move(temp));
    }

    // Need to create full map
    std::map<size_t, AdjMatrix> tilemap;
    // Copying big data, ouch
    tilemap[0] = data;

    constexpr unsigned BoardSize = 5;

    auto incrementMapTile = [](AdjMatrix& data) {
        for(auto& row : data) {
            for(uint8_t& val : row) {
                val++;
                if(val > 9)
                    val = 1;
            }
        }
    };

    for(size_t i = 1; i < 2*BoardSize; i++) {
        // Have the same matrix at 0,1 and 1,0
        // Same at 0,2 and 1,1 and 2, 0, etc.
        tilemap[i] = tilemap[i - 1];
        incrementMapTile(tilemap[i]);
    }


    // Have to fill a new adjacency matrix
    AdjMatrix graph;

    // 0 1 2 3 4
    // 1 2 3 4 5
    // 2 3 4 5 6
    // 3 4 5 6 7
    // 4 5 6 7 8

    size_t min = 0;
    size_t max = 4;
    const size_t InitialNumRows {data.size()};
    const size_t InitialRowLength {data[0].size()};
    const size_t FinalRowLength {InitialRowLength * 5};

    while(max < 2 * BoardSize - 1) {
        // For each index in range [min, max]
        for(size_t i = 0; i < InitialNumRows; i++)
        {
            std::vector<uint8_t> row;

            for(size_t curIndex = min; curIndex <= max; curIndex++) {
                for(int val : tilemap[curIndex][i]) {
                    row.push_back(val);
                }
            }
            graph.push_back(row);

        }
        
        min++;
        max++;
    }

    for(const auto& row : graph) {
        for(auto val : row) std::cout << +val;
        std::cout << '\n';
    }

    // Part one
    // dijkstra(graph);

    // Part two
    size_t m = graph.size();
    size_t n = graph[0].size();
    Graph g(m*n);
    const int Directions[4][2] {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    auto convertToIndex = [&graph](size_t i, size_t j) {
        return i * graph[0].size() + j;
    };

    for(size_t i = 0; i < m; i++) {
        for(size_t j = 0; j < n; j++) {
            size_t currentVertexID = convertToIndex(i, j);
            for(const auto& dir : Directions) {
                size_t newI = i + dir[0];
                size_t newJ = j + dir[1];

                if(isValidMatrixIndex(newI, newJ, graph)) {
                    size_t adjVertexID = convertToIndex(newI, newJ);
                    g.addEdge(currentVertexID, adjVertexID, graph[newI][newJ]);
                    g.addEdge(adjVertexID, currentVertexID, graph[i][j]);
                }
            }
        }
    }

    g.calculateShortestPaths(0, m*n-1);

    return 0;
}