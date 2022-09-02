#include <bits/stdc++.h>

/* Possible solutions
 *
 * Could brute force and do O(n^2) solution
 * for i in input
 *      for j in input
 *          if i == j continue
 *          else
 *              temp += abs(input[j] - input[i])
 *      lowest = min(lowest, temp)
 */


int solvePartOne(const std::vector<int>& numbers)
{
    // Check for minimum energy from 0->max element
    auto it = std::max_element(numbers.begin(), numbers.end());
    int maxDistance {*it};
    
    int best = INT_MAX;
    for(int dist = 0; dist <= maxDistance; dist++) {
        int energy{0};
        for(int num : numbers) {
            energy += abs(num - dist);
        }
        best = std::min(best, energy);
    }
    
    return best;
}

int solvePartTwo(const std::vector<int>& numbers)
{
    // Check for minimum energy from 0->max element
    auto it = std::max_element(numbers.begin(), numbers.end());
    int maxDistance {*it};
    
    auto sumFromOneToN = [](int x) {
        return x*(x + 1) / 2;
    };

    int best = INT_MAX;
    for(int dist = 0; dist <= maxDistance; dist++) {
        int energy{0};
        for(int num : numbers) {
            int toMove {abs(num - dist)};
            energy += sumFromOneToN(toMove);
        }
        best = std::min(best, energy);
    }
    
    return best;
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected {2};

    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    char delim{','};
    int value{0};

    std::vector<int> positions{};

    while(!infile.eof()) {
        infile >> value >> delim;
        positions.push_back(value);
    }

    unsigned answer1 {solvePartOne(positions)};
    unsigned answer2 {solvePartTwo(positions)};

    std::cout << "Answer to part 1: " << answer1 << '\n';
    std::cout << "Answer to part 2: " << answer2 << '\n';

    return 0;
}