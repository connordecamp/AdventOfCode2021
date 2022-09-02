#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iomanip>

using std::setw;

uint64_t approach2(const std::vector<int>& input, const unsigned numDays)
{
    // Have to track fish state at 0, 1, 2, 3, 4, 5, 6, 7, 8 days remaining
    constexpr uint64_t NumStates {9};
    std::array<uint64_t, NumStates> histogram{};

    // Setup initial state
    for(uint64_t num : input) {
        histogram[num]++;
    }


    for(uint64_t day = 1; day <= numDays; day++) {
        uint64_t fishToAdd {histogram[0]};
       
        for(size_t i = 0; i < NumStates - 1; i++) {
            histogram[i] = histogram[i + 1];
        }

        histogram[6] += fishToAdd;
        histogram[8] = fishToAdd;
    }

    return std::accumulate(histogram.begin(), histogram.end(), static_cast<uint64_t>(0));
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected {3};

    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <infile> <number of days>\n";
        return -1;
    }

    std::ifstream file(argv[1]);

    std::vector<int> lanternFish{};
    char delim{','};
    int value{0};

    while(!file.eof()) {
        file >> value >> delim;
        lanternFish.push_back(value);
    }

    constexpr unsigned FishResetState {6};
    constexpr unsigned NewFishState {8};
    unsigned numDays {atoi(argv[2])};

    // for(size_t day = 1; day <= numDays; day++) {
    //     std::cout << "Day " << day << '\n';

    //     unsigned fishToAdd{0};

    //     for(int& fish : lanternFish) {
    //         if(fish > 0) {
    //             fish--;
    //         }
    //         else if(fish == 0) {
    //             fishToAdd++;
    //             fish = FishResetState;
    //         }
    //     }

    //     // std::cout << "After " << day << " days: ";
    //     // for(int x : lanternFish) {
    //     //     std::cout << x << ' ';
    //     // }
    //     // std::cout << '\n';

    //     for(size_t i = 0; i < fishToAdd; i++)
    //         lanternFish.push_back(NewFishState);

    // }

    std::cout << "Number of fish on day " << numDays << ": " << approach2(lanternFish, numDays) << '\n';

    return 0;
}