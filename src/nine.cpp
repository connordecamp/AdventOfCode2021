#include <bits/stdc++.h>

typedef std::pair<size_t, size_t> MapIndex;
typedef std::vector<std::string> Heightmap;

template<typename T>
bool isValidIndex(size_t row, size_t column, const std::vector<T>& container)
{
    bool validRow {row >= 0 && row < container.size()};
    bool validColumn {column >= 0 && column < container[0].size()};

    return validRow && validColumn;
}

std::vector<MapIndex> findBasins(const Heightmap& heightmap)
{
    const int Directions[4][2] {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    std::vector<MapIndex> basinIndices{};
    for(size_t i = 0; i < heightmap.size(); i++) {
        for(size_t j = 0; j < heightmap[i].size(); j++) {
            bool isBasin {true};

            for(const auto& dir : Directions) {
                size_t newI {i + dir[0]};
                size_t newJ {j + dir[1]};

                if(isValidIndex(newI, newJ, heightmap)) {
                    isBasin &= (heightmap[i][j] < heightmap[newI][newJ]);
                }
            }

            if(isBasin) {
                basinIndices.push_back({i,j});
            }
        }
    }

    return basinIndices;
}

int partOne(const Heightmap& heightmap)
{
    std::map<MapIndex, bool> shouldCheck;
    for(size_t i = 0; i < heightmap.size(); i++) {
        for(size_t j = 0; j < heightmap[i].size(); j++) {
            shouldCheck[{i,j}] = true;
        }
    }

    auto isValidIndex = [&heightmap](size_t i, size_t j) {
        bool rowValid {i >= 0 && i < heightmap.size()};
        bool colValid (j >= 0 && j < heightmap[0].size());

        return rowValid && colValid;
    };

    unsigned numLowPoints {0};
    unsigned riskLevel {0};
    const int Directions[4][2] {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    for(size_t i = 0; i < heightmap.size(); i++) {
        for(size_t j = 0; j < heightmap[i].size(); j++) {

            if(shouldCheck[{i, j}]) {
                bool result {true};
                for(const auto& dir : Directions) {
                    size_t newI {i + dir[0]};
                    size_t newJ {j + dir[1]};

                    if(isValidIndex(newI, newJ)) {
                        result &= (heightmap[i][j] < heightmap[newI][newJ]);
                    }
                }

                if(result) {
                    for(const auto& dir : Directions) {
                        size_t newI {i + dir[0]};
                        size_t newJ {j + dir[1]};

                        if(isValidIndex(newI, newJ)) {
                            shouldCheck[{newI, newJ}] = false;
                        }
                    }

                    numLowPoints++;
                    // Problem definition
                    // Risk level is height level + 1
                    riskLevel += (heightmap[i][j] - '0' + 1);
                }
                else {
                    shouldCheck[{i, j}] = false;
                }

            }

        }
    }

    return riskLevel;
}

unsigned calculateBasinSize(
    const Heightmap& heightmap, 
    size_t i,
    size_t j, 
    std::map<MapIndex, bool>& checked)
{
    if(!isValidIndex(i, j, heightmap)) return 0;

    // Max height is 9 which will never be part of any basin
    constexpr unsigned StopValue {'9'};
    if(heightmap[i][j] == StopValue) {
        return 0;
    }
    //std::cout << "Not equal to " << StopValue << '\n';

    //std::cout << "Valid index\n";
    if(checked[{i,j}]) return 0;
    checked[{i,j}] = true;

    //std::cout << "Not checked before\n";
    
    const int Directions[4][2] {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    unsigned answer {1};

    for(const auto& dir : Directions) {
        size_t newI {i + dir[0]};
        size_t newJ {j + dir[1]};
        answer += calculateBasinSize(heightmap, newI, newJ, checked);
    }

    return answer;
}

unsigned partTwo(const Heightmap &heightmap)
{
    std::vector<MapIndex> basinIndices {findBasins(heightmap)};
    std::priority_queue<unsigned> basinSizeHeap;

    std::map<MapIndex, bool> checked;
    for(size_t i = 0; i < heightmap.size(); i++) {
        for(size_t j = 0; j < heightmap[i].size(); j++) {
            checked[{i, j}] = false;
        }
    }   
    
    for(const MapIndex& index : basinIndices) {

        unsigned basinSize {calculateBasinSize(heightmap, index.first, index.second, checked)};
        basinSizeHeap.push(basinSize);
    }

    unsigned answer {1};
    constexpr unsigned NumBasinSizes {3};
    for(size_t i = 0; i < NumBasinSizes; i++) {
        answer *= basinSizeHeap.top();
        basinSizeHeap.pop();
    }

    return answer;
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected {2};
    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <infile>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    std::string line{};
    std::vector<std::string> heightmap;

    while(!infile.eof()) {
        std::getline(infile, line);
        heightmap.push_back(std::move(line));
    }

    std::cout << "Answer for part one: " << partOne(heightmap) << '\n';
    std::cout << "Answer for part two: " << partTwo(heightmap) << '\n';

    return 0;
}