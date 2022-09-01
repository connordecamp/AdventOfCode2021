#include <bits/stdc++.h>

int main(int argc, char** argv)
{
    std::ifstream inFile(argv[1]);
    std::vector<int> measurements;

    int inputValue {0};
    while(!inFile.eof()) {
        inFile >> inputValue;
        measurements.push_back(inputValue);
    }


    std::cout << "Number of measurements: " << measurements.size() << '\n';

   

    unsigned windowsLargerThanPrevious = 0;
    int currentSum = 0;
    for(size_t i = 0; i < 3; i++) {
        currentSum += measurements[i];
    }

    std::vector<int> newMeasurements;

    for(size_t i = 3; i < measurements.size(); i++) {
        newMeasurements.push_back(currentSum);

        currentSum -= measurements[i - 3];
        currentSum += measurements[i];
    }

    // Count how many measurements are larger than the previous measurement
    unsigned amountLargerThanPrevious = 0;
    for(size_t i = 1; i < newMeasurements.size(); i++) {
        if(newMeasurements[i] > newMeasurements[i - 1])
            amountLargerThanPrevious++;
    }

    std::cout << amountLargerThanPrevious << "\n";

    return 0;
}