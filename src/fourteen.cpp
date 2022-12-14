/* Day 14: Extended Polymerization
 *
 */

#include <bits/stdc++.h>

std::unordered_map<char, uint64_t> getLetterFrequencies(const std::string& s)
{
    std::unordered_map<char, uint64_t> frequencyMap;
    for(char c : s)
        frequencyMap[c]++;

    return frequencyMap;
}

uint64_t partTwo(const char* filename)
{
    std::ifstream infile(filename);
    std::string line;
    std::unordered_map<std::string, std::pair<std::string, std::string>> insertionRules;
    std::unordered_map<std::string, uint64_t> polymer;

    const std::string Arrow {" -> "};
    char firstChar = '-';

    while(std::getline(infile, line)) {

        if(line.size() == 0)
            continue;

        size_t arrowIndex = line.find(Arrow);
        bool isInsertionRule {arrowIndex != std::string::npos};

        if(isInsertionRule) {
            std::string key {line.substr(0, arrowIndex)};
            char value = line.substr(arrowIndex + Arrow.length())[0];

            std::string opt1 {{key[0], value}};
            std::string opt2 {{value, key[1]}};

            insertionRules.insert({key, {opt1, opt2}});
        }
        else {
            
            // Create polymer
            firstChar = line[0];
            constexpr unsigned WindowSize {2};
            for(size_t i = 0; i <= line.size() - WindowSize; i++) {
                std::string key {line.substr(i, WindowSize)};
                polymer[key]++;
            }
        }
    }

    constexpr unsigned Steps = 40;

    for(size_t i = 0; i < Steps; i++) {
        std::unordered_map<std::string, uint64_t> newPolymer;

        for(const auto& iter : polymer) {
            std::string key = iter.first;
            
            bool isInsertionRule = (insertionRules.find(key) != insertionRules.end());
            uint64_t amountInPolymer = polymer[key];
            
            if(isInsertionRule) {
                std::pair<std::string, std::string> newStrings {
                    insertionRules.find(key)->second};

                newPolymer[newStrings.first] += amountInPolymer;
                newPolymer[newStrings.second] += amountInPolymer;
            }
            else {
                newPolymer[key] = amountInPolymer;
            }
        }

        polymer = std::move(newPolymer);
    }

    std::unordered_map<char, uint64_t> frequency;
    for(const auto& pair : polymer) {
        frequency[pair.first[0]] += pair.second;
        frequency[pair.first[1]] += pair.second;
    }


    for(auto& iter : frequency) {
        iter.second /= 2;
    }


    for(auto& iter : frequency) {
        std::cout << iter.first << ' ' << iter.second << '\n';
    }

    typedef std::pair<char, uint64_t> pair_type;
    auto comparator = [](const pair_type& p1, const pair_type& p2){
        return p1.second < p2.second;
    };

    uint64_t highestFrequency = std::max_element(
        frequency.begin(),
        frequency.end(),
        comparator
    )->second;

    uint64_t lowestFrequency = std::min_element(
        frequency.begin(),
        frequency.end(),
        comparator
    )->second;

    return (highestFrequency - lowestFrequency);
}

int main(int argc, char** argv)
{
    constexpr unsigned NumExpectedArgs = 2;
    if(argc != NumExpectedArgs) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return -1;
    }

    std::cout << "Answer: " << partTwo(argv[1]) << '\n';

    return 0;
    // TODO
    // Fix off by 1 error
    std::ifstream infile(argv[1]);
    std::string line;

    std::string polymerTemplate;
    std::unordered_map<std::string, char> polymerMap;

    const std::string Arrow {" -> "};

    while(std::getline(infile, line)) {

        if(line.size() == 0)
            continue;

        size_t arrowIndex = line.find(Arrow);
        bool isInsertionRule {arrowIndex != std::string::npos};

        if(isInsertionRule) {
            std::string key {line.substr(0, arrowIndex)};
            char value = line.substr(arrowIndex + Arrow.length())[0];
            polymerMap.insert({std::move(key), value});
        }
        else {
            polymerTemplate = std::move(line);
        }
    }

    // If Template is ABC
    // and AB -> D and BC -> F
    std::cout << "Initial template: " << polymerTemplate << '\n';

    constexpr size_t NumCharacters = 2;
    for(size_t numIters = 0; numIters < 10; numIters++) {

        std::string nextTemplate {polymerTemplate};
        size_t left = 0;
        size_t numInsertedCharacters = 0;
        
        while(left + NumCharacters <= polymerTemplate.size()) {

            std::string key = polymerTemplate.substr(left, NumCharacters);
            bool isValidKey {polymerMap.find(key) != polymerMap.end()};

            if(isValidKey) {
                std::string toInsert {polymerMap.at(key)};
                size_t pos = left + NumCharacters + numInsertedCharacters - 1;
                nextTemplate = nextTemplate.insert(pos, toInsert);
                numInsertedCharacters++;
            }

            left++;
        }

        polymerTemplate = nextTemplate;
        nextTemplate.clear();

        std::cout << polymerTemplate << "\n\n";
    }

    std::cout << "Polymer now has length " << polymerTemplate.length() << '\n';

    std::unordered_map<char, uint64_t> letterFrequency {
        getLetterFrequencies(polymerTemplate)};

    typedef std::pair<char, uint64_t> pair_type;
    auto compareFunc = [](const pair_type& p1, const pair_type& p2) {
        return p1.second < p2.second;
    };

    uint64_t highestFrequency = std::max_element(
        letterFrequency.begin(),
        letterFrequency.end(),
        compareFunc
    )->second;

    uint64_t lowestFrequency = std::min_element(
        letterFrequency.begin(),
        letterFrequency.end(),
        compareFunc
    )->second;

    std::cout << highestFrequency << " - " << lowestFrequency << " = " << (int64_t)(highestFrequency - lowestFrequency) << '\n';

    return 0;
}