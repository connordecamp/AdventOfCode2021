/* Bingo with a Squid
 * We are given a random set of integers and a random set of
 * puzzle boards.
 * 
 * Based on the set of integers, determine which board will win
 * first and return the sum of all UNMARKED numbers on that board
 * multiplied by the last number called.
 */

#include <bits/stdc++.h>

std::vector<int> parseStringToVec(const std::string& input, const char delimiter=' ')
{
    const size_t len {input.length()};
    assert(len > 0);
    std::vector<int> resultVec {};

    size_t start {0};
    for(size_t i = 0; i < len; i++) {
        if(input[i] == delimiter) {
            
            size_t substrLength {i - start};

            if(substrLength == 0)
                continue;

            std::string number {input.substr(start, i - start)};
            int value {std::stoi(number)};
            resultVec.push_back(value);
            start = i + 1;
        }

    }
    // Get last number from line
    resultVec.push_back(std::stoi(input.substr(start)));
    return resultVec;
}

bool boardHasBingo(const std::vector<std::vector<int>>& vec)
{
    for(size_t row = 0; row < vec.size(); row++) {
        bool rowHasBingo {true};
        bool colHasBingo {true};

        for(size_t col = 0; col < vec[row].size(); col++) {
            if(vec[row][col] >= 0) {
                rowHasBingo = false;
            }

            if(vec[col][row] >= 0) {
                colHasBingo = false;
            }

            if(!rowHasBingo && !colHasBingo) break;
        }

        if(rowHasBingo || colHasBingo)
            return true;
    }

    return false;
}


int main(int argc, char** argv)
{
    constexpr unsigned NumExpectedArguments {2};

    if(argc != NumExpectedArguments) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    std::string line {};
    std::getline(infile, line);

    std::vector<int> bingoNumbers {parseStringToVec(line, ',')};

    // Clear empty line to allow bingo board parsing
    std::getline(infile, line);

    // Bingo Boards are square matrices
    typedef std::vector<std::vector<int>> Board;
    Board defaultBoard {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    std::vector<Board> boards{defaultBoard};
    size_t currentBoard {0};
    size_t currentRow {0};

    /* Parse all bingo boards and store as 2D vectors
     * in the vector "boards"
     */
    while(std::getline(infile, line)) {
        bool newBoardStarting {line == ""};
        
        if(newBoardStarting) {
            boards.push_back(defaultBoard);
            currentBoard++;
            currentRow = 0;
        }
        else {
            // Parse line to get ints
            std::vector<int> numbers {parseStringToVec(line)};
            for(size_t i = 0; i < numbers.size(); i++) {
                boards[currentBoard][currentRow][i] = numbers[i];
            }
            currentRow++;
        }
    }

    // Now we can begin bingo
    // How do we tell if a board wins?
    // We need to search every board for each number
    // If we find the number, we can make it negative
    // After searching/marking each board, we check if the board
    // has a win by checking all rows/columns for bingo
    // "Marking" a tile will be setting the value of the cell to -1
    constexpr int Marked {-1};
    bool foundBingoBoard {false};
    size_t lastWinnerIndex {0};
    int lastNumberCalled {0};
    int numWinners {0};

    std::unordered_map<int, bool> winners;

    for(int number : bingoNumbers) {

        lastNumberCalled = number;
        std::cout << "LAST NUMBER: " << lastNumberCalled << '\n';

        for(size_t i = 0; i < boards.size(); i++) {
            for(auto& row : boards[i]) {
                for(int& val : row) {
                    if(val == number) {
                        val = Marked;
                        break;
                    }
                }
            }

            if(!winners[i] && boardHasBingo(boards[i])) {
                winners[i] = true;
                numWinners++;

                if(numWinners == boards.size()) {
                    lastWinnerIndex = i;
                    foundBingoBoard = true;
                    break;
                }
            }

            if(foundBingoBoard)
                break;
        }

        if(foundBingoBoard)
            break;
    }

    std::cout << "Last board to get bingo was board " << lastWinnerIndex << '\n';
    
    int sum {0};
    for(const auto& row : boards[lastWinnerIndex]) {
        for(const int val : row) {
            if(val != Marked) {
                sum += val;
            }
        }
    }

    std::cout << sum << ' ' << lastNumberCalled << ' ' << sum * lastNumberCalled << '\n';

    return 0;
}