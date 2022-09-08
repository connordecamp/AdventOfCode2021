/* Day 12: Passage Pathing
 *
 */

#include <bits/stdc++.h>

struct Node {
    std::string id;
    bool canUseAgain;

    std::vector<Node*> adjacent;

    Node(std::string _id) : 
        id(_id), 
        canUseAgain(true) {}
    

};

bool isSmallCave(const std::string& s) {
    for(const char& c : s) {
        if(c < 'a' || c > 'z')
            return false;
    }
    return true;
}

unsigned numPaths = 0;

void dfs(Node* current, Node* target, std::vector<Node*>& currentPath)
{

    currentPath.push_back(current);

    if(current == target) {
        numPaths++;
        return;
    }

    for(Node* adj : current->adjacent) {
        if(adj->canUseAgain) {
            if(adj->id != "end" && isSmallCave(adj->id))
                adj->canUseAgain = false;

            dfs(adj, target, currentPath);
            currentPath.pop_back();
            adj->canUseAgain = true;
        }
    }
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected {2};
    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    std::string line{};
    std::unordered_map<std::string, Node*> nodeMap;

    // All edges are formatted as: src-dest
    constexpr char Delimiter {'-'};

    while(!infile.eof()) {
        std::getline(infile, line);

        auto delimPos {std::find(line.begin(), line.end(), Delimiter)};
        auto delimIndex {std::distance(line.begin(), delimPos)};

        std::string src {line.substr(0, delimIndex)};
        bool srcNodeExists {nodeMap.find(src) != nodeMap.end()};
        if(!srcNodeExists) {
            nodeMap[src] = new Node(src);
        }

        std::string dest {line.substr(delimIndex + 1)};
        bool destNodeExists {nodeMap.find(dest) != nodeMap.end()};
        if(!destNodeExists) {
            nodeMap[dest] = new Node(dest);
        }

        nodeMap[src]->adjacent.push_back(nodeMap[dest]);
        nodeMap[dest]->adjacent.push_back(nodeMap[src]);
    }

    auto printAdjacenyList = [nodeMap]() {
        for(const auto& pair : nodeMap) {
            std::cout << pair.first << ": ";
            for(const auto& adj : pair.second->adjacent) {
                std::cout << adj->id << ' ';
            }
            std::cout << '\n';
        }
    };

    Node* current {nodeMap["start"]};
    current->canUseAgain = false;

    Node* end {nodeMap["end"]};

    std::vector<Node*> paths;
    dfs(current, end, paths);

    std::cout << "Num paths: " << numPaths << '\n';

    return 0;
}