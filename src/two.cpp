#include <bits/stdc++.h>

int main(int argc, char** argv)
{
    const std::string FORWARD {"forward"};
    const std::string DOWN {"down"};
    const std::string UP {"up"};

    int horizontal {0};
    int depth {0};
    int aim {0};

    std::ifstream infile(argv[1]);

    std::string command;
    int value;
    std::cout << std::setw(20) << "Horizontal" << std::setw(20) << "Depth" << '\n';
    while(!infile.eof()) {
        infile >> command >> value;

        if(command == FORWARD) {
            horizontal += value;
            depth += (aim * value);
        }
        else if(command == UP) aim -= value;
        else if(command == DOWN) aim += value;

        std::cout << std::setw(20) << horizontal << std::setw(20) << depth << '\n';
    }

    std::cout << "Horizontal\tDepth\tAnswer\n" << horizontal << '\t' << depth << '\t' << horizontal * depth << '\n';

    return 0;
}