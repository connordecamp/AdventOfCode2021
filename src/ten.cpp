#include <bits/stdc++.h>

constexpr char OK {'!'};

char findFirstInvalid(const std::string& chunk)
{
    std::stack<char> leftBraceStack;

    std::unordered_map<char, char> rightBraceMap;

    rightBraceMap[')'] = '(';
    rightBraceMap[']'] = '[';
    rightBraceMap['}'] = '{';
    rightBraceMap['>'] = '<';

    for(const char& c : chunk) {
        if(c == '(' || c == '[' || c == '{' || c == '<') {
            leftBraceStack.push(c);
        }
        else {
            bool isValidChunk {leftBraceStack.top() == rightBraceMap[c]};

            if(!isValidChunk) {
                return c;
            }

            leftBraceStack.pop();
        }
    }

    if(leftBraceStack.size() > 0) {
        // NYI
        return OK;
    }


    return OK;
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
    
    int totalSyntaxScore {0};
    while(!infile.eof()) {
        std::getline(infile, line);

        char firstInvalidCharacter = findFirstInvalid(line);

        if(firstInvalidCharacter != OK) {
            totalSyntaxScore += SyntaxScore.find(firstInvalidCharacter)->second;
        }
    }

    std::cout << "Part one: " << totalSyntaxScore << '\n';

    return 0;
}