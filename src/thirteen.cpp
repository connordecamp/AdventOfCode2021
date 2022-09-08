/* Day 13: Transparent Origami
 *
 */

#include <bits/stdc++.h>

enum class FoldType {
    BottomUp,
    RightLeft
};

struct Fold {
    int value;
    FoldType foldType;
    Fold(int v, FoldType type) :
        value(v),
        foldType(type){}
};

template<typename T>
void printMatrix(const std::vector<std::vector<T>>& matrix)
{
    size_t i = 0;
    for(const auto& row : matrix) {
        //std::cout << i << ": ";
        for(const auto& item : row) {
            std::cout << item;
        }
        std::cout << '\n';
        i++;
    }
}

void foldBottomHalfUp(std::vector<std::vector<char>>& board, const Fold& fold)
{
    // Use two pointer approach
    // Here, we modify the row and iterate through each column
    // Have upper and lower pointer
    // upper = fold.value - 1
    // lower = fold.value
    // board[upper][i] = board[lower][i]
    // upper--, lower++

    int upper = fold.value - 1;
    int lower = fold.value + 1;

    while(upper >= 0 && lower < board.size()) {

        for(size_t i = 0; i < board[upper].size(); i++) {
            char upperChar = board[upper][i];
            char lowerChar = board[lower][i];

            bool topOrBottomHasDot = (upperChar != lowerChar);

            if(topOrBottomHasDot) {
                board[upper][i] = '#';
            }

        }
        upper--;
        lower++;
    }
}

void foldRightHalfLeft(std::vector<std::vector<char>>& board, const Fold& fold)
{
    int left = fold.value - 1;
    int right = fold.value + 1;

    while(left >= 0 && right < board[0].size()) {

        for(size_t i = 0; i < board.size(); i++) {
            char leftChar = board[i][left];
            char rightChar = board[i][right];

            // Characters can only be # or .
            // If they are not equal, one must be #
            // which will always be seen after folding
            bool leftOrRightHasDot = (leftChar != rightChar);

            if(leftOrRightHasDot)
                board[i][left] = '#';

        }
        left--;
        right++;
    }
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected = 2;
    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    std::string line;

    typedef std::pair<int, int> Pair;
    std::vector<Pair> orderedPairs;
    std::vector<Fold> folds;

    while(std::getline(infile, line)) {
        
        size_t commaIndex = line.find(',');
        bool isOrderedPair = (commaIndex != std::string::npos);
        bool isFold = (line.find("fold") != std::string::npos);

        if(isOrderedPair) {
            // Ordered pairs are always formatted as "x,y"
            int x = std::stoi(line.substr(0, commaIndex));
            int y = std::stoi(line.substr(commaIndex + 1));

            orderedPairs.push_back({x, y});

        } else if(isFold){
            size_t equalsSignIndex = line.find("=");
            int value = std::stoi(line.substr(equalsSignIndex + 1));

            // Look for x and y in the string
            // Fold lines are always formatted as
            // fold [x|y]=[number]

            size_t yIndex = line.find("y");
            // A line with the equation y=constant is horizontal
            bool isHorizontalLine = (yIndex != std::string::npos);

            if(isHorizontalLine)
                folds.push_back(Fold(value, FoldType::BottomUp));
            else 
                folds.push_back(Fold(value, FoldType::RightLeft));
        }

    }

    std::cout << "Initial number of dots: " << orderedPairs.size() << '\n';
    
    auto pairWithHighestXValue = std::max_element(
        orderedPairs.begin(), 
        orderedPairs.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
        }
    );

    int highestXValue = pairWithHighestXValue->first;

    auto pairWithHighestYValue = std::max_element(
        orderedPairs.begin(), 
        orderedPairs.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        }
    );

    int highestYValue = pairWithHighestYValue->second;
    constexpr char Empty = '.';
    constexpr char Dot = '#';

    // Coordinates go from [0,0] to [maxX, maxY]
    // This is a range of (maxX + 1, maxY + 1)
    std::vector<std::vector<char>> board(
        highestYValue + 1, 
        std::vector<char>(highestXValue + 1, Empty)
    );

    for(const Pair& p : orderedPairs) {
        board[p.second][p.first] = Dot;
    }

    for(const Fold& fold : folds)  {

        if(fold.foldType == FoldType::BottomUp) {
            std::cout << "Folding along y=" << fold.value << '\n';
            foldBottomHalfUp(board, fold);

            // Bottom half was folded up, so remove those
            // rows from the board
            board.resize(fold.value);
        }
        else {
            std::cout << "Folding along x=" << fold.value << '\n';
            foldRightHalfLeft(board, fold);

            // Right half was folded to the left, so remove the
            // extra columns from the board
            for(size_t i = 0; i < board.size(); i++) {
                board[i].resize(fold.value);
            }
        }
    }

    unsigned numDots = 0;
    
    printMatrix(board);

    for(const auto& row : board) {
        for(const auto& c : row) {
            if(c == Dot) {
                numDots++;
            }
        }
    }
    std::cout << "Number of dots after folding: " << numDots << '\n';

    return 0;
}