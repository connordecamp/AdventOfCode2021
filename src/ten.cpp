#include <bits/stdc++.h>

constexpr char OK {'!'};

const std::unordered_map<char, char> rightBraceMap {
    {')', '('},
    {']', '['},
    {'}', '{'},
    {'>', '<'}
};

bool isChunkCorrupted(const std::string& chunk)
{   
    std::stack<char> leftBraceStack;

    for(const char& c : chunk) {
        if(c == '(' || c == '[' || c == '{' || c == '<') {
            leftBraceStack.push(c);
        }
        else {
            bool isValidChunk {leftBraceStack.top() == rightBraceMap.at(c)};

            if(!isValidChunk) {
                return true;
            }

            leftBraceStack.pop();
        }
    }

    return false;
}

bool isChunkIncomplete(const std::string& chunk)
{   
    std::stack<char> leftBraceStack;

    for(const char& c : chunk) {
        if(c == '(' || c == '[' || c == '{' || c == '<') {
            leftBraceStack.push(c);
        }
        else {
            bool isValidChunk {leftBraceStack.top() == rightBraceMap.at(c)};

            if(!isValidChunk) {
                return false;
            }

            leftBraceStack.pop();
        }
    }

    return leftBraceStack.size() > 0;
}

uint64_t partOne(const std::vector<std::string>& corruptChunks)
{
    auto getFirstInvalidCharacter = [](const std::string& s) {
        std::stack<char> leftBraceStack;

        for(char c : s) {
            bool shouldAddToStack {
                c == '(' || c == '<' || c == '{' || c == '['
            };
            if(shouldAddToStack)
                leftBraceStack.push(c);
            else {
                if(leftBraceStack.top() != rightBraceMap.at(c)) {
                    return c;
                }
                leftBraceStack.pop();
            }
        }

        return OK;
    };

    /* Problem definition
       ): 3 points.
       ]: 57 points.
       }: 1197 points.
       >: 25137 points.
    */
    const std::map<char, uint64_t> rightBraceScore {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137}
    };

    uint64_t result {0};
    for(const auto& chunk : corruptChunks) {
        char firstInvalidCharacter = getFirstInvalidCharacter(chunk);
        if(firstInvalidCharacter != OK) {
            result += rightBraceScore.at(firstInvalidCharacter);
        }
    }

    return result;
}

uint64_t partTwo(const std::vector<std::string>& input)
{
    const std::unordered_map<char, char> leftBraceMap {
        {'(', ')'},
        {'[', ']'},
        {'{', '}'},
        {'<', '>'}
    };

    const std::unordered_map<char, int> rightBraceScore {
        {')', 1},
        {']', 2},
        {'}', 3},
        {'>', 4},  
    };

    std::vector<uint64_t> scores;
    for(const std::string& chunk : input) {

        uint64_t totalScore {0};
        std::stack<char> leftBraceStack;

        for(const char& c : chunk) {
            if(c == '(' || c == '[' || c == '{' || c == '<') {
                leftBraceStack.push(c);
            }
            else {
                bool isValidChunk {leftBraceStack.top() == rightBraceMap.at(c)};

                if(!isValidChunk) {
                    while(!leftBraceStack.empty())
                        leftBraceStack.pop();
                    break;
                }

                leftBraceStack.pop();
            }
        }

        bool shouldPush {false};
        while(leftBraceStack.size() > 0) {
            shouldPush = true;
            char newestLeftBrace {leftBraceStack.top()};
            char matchingRightBrace {leftBraceMap.at(newestLeftBrace)};

            totalScore *= 5;
            totalScore += rightBraceScore.at(matchingRightBrace);

            leftBraceStack.pop();
        }

        if(shouldPush)
            scores.push_back(totalScore);
    }

    std::sort(scores.begin(), scores.end());

    return scores[scores.size() / 2];
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

    const std::unordered_map<char, int> SyntaxScore {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137}
    };
    
    std::vector<std::string> corruptChunks;
    std::vector<std::string> incompleteChunks;

    while(!infile.eof()) {
        std::getline(infile, line);
        
        if(isChunkCorrupted(line))
            corruptChunks.push_back(std::move(line));
        else if(isChunkIncomplete(line))
            incompleteChunks.push_back(std::move(line));
    }

    std::cout << "Part one: " << partOne(corruptChunks) << '\n';
    std::cout << "Part two: " << partTwo(incompleteChunks) << '\n';

    return 0;
}