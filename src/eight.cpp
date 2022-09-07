#include <bits/stdc++.h>

bool isSubset(const std::string& s1, const std::string& s2) 
{
    size_t s1Len {s1.length()};
    size_t s2Len {s2.length()};
    // Returns true if s1 is a subset of s2
    if(s1Len > s2Len)
        return false;
    
    // If the strings are the same length, sort and return comparison
    if(s1Len == s2Len) {
        std::sort(s1.begin(), s1.end());
        std::sort(s2.begin(), s2.end());

        return s1 == s2;
    }

    // Otherwise, see if all characters in s1 are found in s2
    
    std::unordered_map<char, unsigned> s2Frequency;
    for(const char c : s2) {
        s2Frequency[c]++;
    }

    for(auto c : s1) {
        s2Frequency[c]--;

        if(s2Frequency[c] < 0) {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected {2};

    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return -1;
    }


    // Letters are placed in ascending order for easy comparison
    // in part two
    const std::string zero  {"abcefg"};
    const std::string one   {"cf"};
    const std::string two   {"acdeg"};
    const std::string three {"acdfg"};
    const std::string four  {"bcdf"};
    const std::string five  {"abdfg"};
    const std::string six   {"abdefg"};
    const std::string seven {"acf"};
    const std::string eight {"abcdefg"};
    const std::string nine  {"abcdfg"};

    unsigned answer {0};
    constexpr unsigned NumberOfSignalPatterns {10};
    constexpr unsigned NumberOfDigitPatterns {4};
    constexpr char Delimiter {'|'};

    
    std::ifstream infile(argv[1]);
    std::string line{};

    /* How can we find out which letters map to which segment?
     * If two letters, whatever they are must map to number 1, which is the 
     * two vertical rightmost lines. Both of these must be in the three letter
     * segment (number 7), with one extra that MUST be the topmost segment
     * 
     * Number 4 uses four segments - the two vertical from number 1, a horizontal
     * middle segment, and the left vertical segment
     *      
     * Number 8 uses all seven segments. It at least tells us which 2 are possibly
     * the bottom left and bottom middle
     *     
     */

    while(!infile.eof()) {
        for(size_t i = 0; i < NumberOfSignalPatterns; i++) {
            infile >> line;
        }

        // get delimiter
        infile >> line;

        for(size_t i = 0; i < NumberOfDigitPatterns; i++) {
            infile >> line;
            switch(line.length()) {
            case 2:
            case 3:
            case 4:
            case 7:
                answer++;
                break;
            }
        }
    }


    std::cout << "Part one answer is: " << answer << '\n';

    return 0;
}