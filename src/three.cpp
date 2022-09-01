#include <bits/stdc++.h>

int calculateOxygenGeneratorRating(const std::vector<std::string>& vec)
{
    std::vector<size_t> indicesToUse(vec.size());
    std::iota(indicesToUse.begin(), indicesToUse.end(), 0);

    for(size_t bit = 0; bit < vec[0].size(); bit++) {
    
        unsigned numOneBits {0};
        for(size_t index : indicesToUse) {
            if(vec[index][bit] == '1') numOneBits++;
        }
        
        bool oneIsMostCommonBit {2 * numOneBits >= indicesToUse.size()};

        char toLookFor = '0';
        if(oneIsMostCommonBit) {
            toLookFor = '1';
        }

        // Remove indices where the character at position [bit]
        // is not the same as char toLookFor
        auto it = indicesToUse.begin();
        while(it != indicesToUse.end() && indicesToUse.size() > 1) {
            if(vec[*it][bit] != toLookFor) {
                it = indicesToUse.erase(it);
            }
            else {
                it++;
            }
        }

        if(indicesToUse.size() == 1)
            break;
    }

    assert(indicesToUse.size() == 1);

    return std::stoi(vec[indicesToUse[0]], 0, 2);
}

int calculateC02ScrubberRating(const std::vector<std::string>& vec)
{
    std::vector<size_t> indicesToUse(vec.size());
    std::iota(indicesToUse.begin(), indicesToUse.end(), 0);

    for(size_t bit = 0; bit < vec[0].size(); bit++) {
    
        unsigned numOneBits {0};
        for(size_t index : indicesToUse) {
            if(vec[index][bit] == '1') numOneBits++;
        }
        
        // We want the LEAST common bit
        bool oneIsLeastCommonBit {2 * numOneBits < indicesToUse.size()};

        char toLookFor = '0';
        if(oneIsLeastCommonBit) {
            toLookFor = '1';
        }

        // Remove indices where the character at position [bit]
        // is not the same as char toLookFor
        auto it = indicesToUse.begin();
        while(it != indicesToUse.end() && indicesToUse.size() > 1) {
            if(vec[*it][bit] != toLookFor) {
                it = indicesToUse.erase(it);
            }
            else {
                it++;
            }
        }

        if(indicesToUse.size() == 1)
            break;
    }

    assert(indicesToUse.size() == 1);

    return std::stoi(vec[indicesToUse[0]], 0, 2);
}

int main(int argc, char** argv)
{
    std::ifstream inFile(argv[1]);

    std::string binaryString;
    std::vector<std::string> binaryStrings;

    while(!inFile.eof()) {
        inFile >> binaryString;
        binaryStrings.push_back(binaryString);
    }

    int gammaRate {0};
    int epsilonRate {0};

    unsigned multiplier {1};

    for(int bit = binaryString.length() - 1; bit >= 0; bit--) {

        unsigned numOnBits {0};
        for(size_t i = 0; i < binaryStrings.size(); i++) {
            if(binaryStrings[i][bit] == '1') numOnBits++;
        }

        if(2 * numOnBits > binaryStrings.size()) {
            gammaRate += multiplier;
        }
        else {
            epsilonRate += multiplier;
        }

        multiplier *= 2;
    }

    std::cout << std::setw(20) << "Gamma Rate";
    std::cout << std::setw(20) << "Epsilon Rate";
    std::cout << std::setw(20) << "Power Consumption" << '\n';
    std::cout << std::setw(20) << gammaRate << std::setw(20) << epsilonRate;
    std::cout << std::setw(20) << gammaRate * epsilonRate << '\n';

    int oxygenRating {calculateOxygenGeneratorRating(binaryStrings)};
    int c02Rating {calculateC02ScrubberRating(binaryStrings)};

    std::cout << oxygenRating * c02Rating << '\n';

    return 0;
}