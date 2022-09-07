/* Advent of Code 2022
 * Day 11: Dumbo Octopuses
 */

#include <bits/stdc++.h>

void incrementAllEnergyLevels(std::vector<std::vector<int>>& energyLevels)
{
    for(size_t i = 0; i < energyLevels.size(); i++) {
        for(size_t j = 0; j < energyLevels[i].size(); j++) {
            energyLevels[i][j]++;
        }
    }
}

void flash(
    std::vector<std::vector<int>>& energyLevels,
    size_t i, 
    size_t j)
{
    auto isValidIndex = [&energyLevels](size_t i, size_t j){
        bool rowValid {i >= 0 && i < energyLevels.size()};
        bool colValid {j >= 0 && j < energyLevels[0].size()};

        return rowValid && colValid;
    };    

    if(!isValidIndex(i, j))
        return;

    const short Directions[8][2] {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1},
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1}
    };

    energyLevels[i][j] = 0;

    for(const auto& dir : Directions) {
        size_t newI {i + dir[0]};
        size_t newJ {j + dir[1]};

        if(isValidIndex(newI, newJ)) {
            energyLevels[newI][newJ]++;
        }
    }
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected {3};

    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <input-file> <number of steps>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    std::string line;
    std::vector<std::vector<int>> energyLevels;
    unsigned numOctopus {0};

    while(!infile.eof()) {
        std::getline(infile, line);

        std::vector<int> energy;
        for(char c : line) {
            energy.push_back(c - '0');
        }
        numOctopus += energy.size();
        energyLevels.push_back(std::move(energy));
    }
    
    auto printMatrix = [](std::vector<std::vector<int>>& vec) {
        for(auto row : vec) {
            for(auto num : row) {
                std::cout << num;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    };
    //printMatrix(energyLevels);

    // Increment all energy levels by 1
    // while a 10 is in matrix
    //  Iterate through matrix
    //      if energy level >= 10
    //          flash

    unsigned numSteps {atoi(argv[2])};
    constexpr unsigned EnergyLevelToFlash {10};
    uint64_t totalFlashes {0};


    typedef std::pair<size_t, size_t> PairIndex;
    for(size_t step = 0; step < numSteps; step++) {
        incrementAllEnergyLevels(energyLevels);

        std::vector<PairIndex> cellsToFlash;
        std::set<PairIndex> cellsFlashed;
        
        bool keepSearching {true};
        unsigned flashesThisStep {0};

        unsigned firstStepWhereAllFlashed {0};

        while(keepSearching) {
            keepSearching = false;
            for(size_t i = 0; i < energyLevels.size(); i++) {
                for(size_t j = 0; j < energyLevels.size(); j++) {
                    bool shouldFlash {energyLevels[i][j] >= EnergyLevelToFlash};
                    bool hasFlashedBefore {cellsFlashed.find({i, j}) != cellsFlashed.end()};

                    if(!hasFlashedBefore && shouldFlash) {
                        cellsToFlash.push_back({i, j});
                        keepSearching = true;
                    }
                }
            }

            for(const auto& pair : cellsToFlash) {
                cellsFlashed.insert(pair);
                flash(energyLevels, pair.first, pair.second);
                flashesThisStep++;
            }

            cellsToFlash.clear();
        }
        
        if(flashesThisStep == numOctopus) {
            firstStepWhereAllFlashed = step;
            std::cout << "First step where all jellyfish flashed: " << firstStepWhereAllFlashed + 1 << '\n';
            return 0;
        }

        totalFlashes += flashesThisStep;

        for(const auto& pair : cellsFlashed) {
            energyLevels[pair.first][pair.second] = 0;
        }

        //printMatrix(energyLevels);
    }

    std::cout << "Flashes after " << numSteps << " steps: " << totalFlashes << '\n';

    return 0;
}