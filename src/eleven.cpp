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

int flash(
    std::vector<std::vector<int>>& energyLevels,
    std::map<std::pair<size_t, size_t>, bool>& flashed, 
    size_t i, 
    size_t j)
{
    auto isValidIndex = [&energyLevels](size_t i, size_t j){
        bool rowValid {i >= 0 && i < energyLevels.size()};
        bool colValid {j >= 0 && j < energyLevels[0].size()};

        return rowValid && colValid;
    };    

    if(!isValidIndex(i, j))
        return 0;

    if(flashed[{i, j}]) return 0;
    flashed[{i, j}] = true;
    energyLevels[i][j] = 0;
    unsigned numFlashes {1};

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


    for(const auto& dir : Directions) {
        size_t newI {i + dir[0]};
        size_t newJ {j + dir[1]};

        if(isValidIndex(newI, newJ)) {
            energyLevels[newI][newJ]++;
            
            std::pair<size_t, size_t> index {newI, newJ};
            if(!flashed[index] && energyLevels[newI][newJ] >= 10) {
                numFlashes += flash(energyLevels, flashed, newI, newJ);
            }
        }
    }

    return numFlashes;
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

    while(!infile.eof()) {
        std::getline(infile, line);

        std::vector<int> energy;
        for(char c : line) {
            energy.push_back(c - '0');
        }
        energyLevels.push_back(std::move(energy));
    }
    
    // Increment all energy levels by 1
    // while a 10 is in matrix
    //  Iterate through matrix
    //      if energy level >= 10
    //          flash

    unsigned numSteps {atoi(argv[2])};
    constexpr unsigned EnergyLevelToFlash {10};
    uint64_t totalFlashes {0};

    auto printMatrix = [](std::vector<std::vector<int>>& vec) {
        for(auto row : vec) {
            for(auto num : row) {
                std::cout << num;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    };

    printMatrix(energyLevels);

    for(size_t i = 0; i < numSteps; i++) {
        bool keepSearching {false};
        incrementAllEnergyLevels(energyLevels);

        std::map<std::pair<size_t, size_t>, bool> flashed;
        do {
            keepSearching = false;
            for(size_t i = 0; i < energyLevels.size(); i++) {
                for(size_t j = 0; j < energyLevels[i].size(); j++) {
                    if(energyLevels[i][j] >= EnergyLevelToFlash) {
                        keepSearching = true;
                        totalFlashes += flash(energyLevels, flashed, i, j);
                    }
                }
            }

        }while(keepSearching);

        printMatrix(energyLevels);
    }

    std::cout << "Flashes after " << numSteps << " steps: " << totalFlashes << '\n';

    return 0;
}