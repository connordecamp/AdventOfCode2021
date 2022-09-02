/* Day 5: Hydrothermal Venture
 *
 * Part 1
 */

#include <bits/stdc++.h>

struct Point {
    int x {0};
    int y {0};
};

struct Line {
    Point start {0, 0};
    Point stop {0, 0};
};

unsigned countOverlappingLines(const std::vector<Line>& lines)
{
    // Given entries that represent vent lines, like
    // 0,9 -> 5,9
    // 8,0 -> 0,8
    // etc.
    // Determine the number of points where two or more lines overlap

    // Algorithm
    // Create a 2d matrix of size MAX_X by MAX_Y with every value
    // initialized to zero
    // For each line in the input:
    //     Determine if line is going horizontal or vertical
    //     along the horizontal/vertical path, increment each cell by 1
    // Iterate through the entire array and count the number of places where
    // the entry is >= 2

    int maxX {0};
    int maxY {0};


    for(const auto& line : lines) {
        maxX = std::max(maxX, std::max(line.start.x, line.stop.x));
        maxY = std::max(maxY, std::max(line.start.y, line.stop.y));
    }

    // Have to include 0 so both sizes increase by 1
    maxX++;
    maxY++;

    std::vector<std::vector<int>> grid(maxY, std::vector<int>(maxX, 0));

    for(const auto& line : lines) {
    
        // Find slope, avoid division
        int dy {line.stop.y - line.start.y};
        int dx {line.stop.x - line.start.x};

        bool slopeIsOne {dx == dy};
        bool slopeIsNegativeOne {dx == -dy || -dx == dy};
        bool slopeIsZero {dy == 0};
        bool slopeIsUndefined {dx == 0};

        if(slopeIsOne) {

            // Prefer leftmost point
            Point start {line.start};
            Point end {line.stop};
            if(line.stop.x < line.start.x){
                start = line.stop;
                end = line.start;
            }

            int x {start.x};
            int y {start.y};

            while(x <= end.x && y <= end.y) {
                grid[y][x]++;
                x++;
                y++;
            }

            assert(x > end.x && y > end.y);
        }  
        else if(slopeIsNegativeOne) {

            // We know to go down and right
            // Prefer leftmost point
            Point start {line.start};
            Point end {line.stop};
            if(line.stop.x < line.start.x){
                start = line.stop;
                end = line.start;
            }

            int x {start.x};
            int y {start.y};

            while(x <= end.x && y >= end.y) {
                grid[y][x]++;
                x++;
                y--;
            }

            assert(x > end.x && y < end.y);
            
        }
        else if(slopeIsZero) {
            int start = std::min(line.start.x, line.stop.x);
            int end = std::max(line.start.x, line.stop.x);

            for(size_t i = start; i <= end; i++) {
                grid[line.start.y][i]++;
            }
        }
        else if(slopeIsUndefined){
            int start = std::min(line.start.y, line.stop.y);
            int end = std::max(line.start.y, line.stop.y);

            for(size_t i = start; i <= end; i++) {
                grid[i][line.start.x]++;
            }
        }
    }

    unsigned overlappingLineCount {0};

    for(const auto& row : grid) {
        for(int val : row) {
            if(val >= 2) 
                overlappingLineCount++;
        }
    }

    return overlappingLineCount;
}

Line parseInputLine(const std::string& line)
{
    // a,b -> c,d
    // Find comma, then ' -> ', then next comma
    std::string temp{line};
    Point start;

    // Get x value
    auto it = std::find(temp.begin(), temp.end(), ',');
    int len = std::distance(temp.begin(), it);
    start.x = std::stoi(temp.substr(0, len));
    temp.erase(temp.begin(), temp.begin() + len + 1);

    // Get first y value
    it = std::find(temp.begin(), temp.end(), ' ');
    len = std::distance(temp.begin(), it);
    start.y = std::stoi(temp.substr(0, len));    
    temp.erase(temp.begin(), temp.begin() + len + 1);

    // Erase arrow
    it = std::find(temp.begin(), temp.end(), ' ');
    len = std::distance(temp.begin(), it);
    temp.erase(temp.begin(), temp.begin() + len + 1);

    Point end;
    it = std::find(temp.begin(), temp.end(), ',');
    len = std::distance(temp.begin(), it);
    end.x = std::stoi(temp.substr(0, len));
    temp.erase(temp.begin(), temp.begin() + len + 1);
    end.y = std::stoi(temp);
    
    return {start, end};
}

int main(int argc, char** argv)
{
    constexpr unsigned NumArgsExpected {2};

    if(argc != NumArgsExpected) {
        std::cerr << "Usage: " << argv[0] << " <infile>\n";
        return -1;
    }

    std::ifstream infile(argv[1]);
    std::string line{};
    std::vector<Line> lines{};
    
    while(std::getline(infile, line)) {
        lines.push_back(parseInputLine(line));
    }

    std::cout << countOverlappingLines(lines) << '\n';

    return 0;
}