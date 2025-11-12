#include "inputValidation.h"

bool is_number(const std::string& input)
{
    if (std::all_of(input.begin(), input.end(), ::isdigit)){
        return true;
    } else {
        return false;
    }
    
}

bool vector_contains(const std::vector<std::string>& vec, const std::string& value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

int getIntInput(int min, int max) {
    int input;
    bool valid = false;
    
    while (!valid) {
        std::cin >> input;
        if(std::cin.fail() || input < min || input > max){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n Please enter a number " << min << "-" << max << ": ";
        } else { 
            valid = true;
        }
    }
    
    return input;
}

char getYesNo() {
    std::string input;
    char inputChar;

    while (true) {
        std::cout << " Select (y/n): ";
        std::getline(std::cin, input); // getting entire line here to avoid repeating error messages

        if (input.empty()) continue;  // user pressed Enter

        inputChar = toupper(input[0]);  // take first character only

        if (inputChar == 'Y' || inputChar == 'N')
            return inputChar;

        std::cout << " Invalid input. Please enter y or n.\n";
    }
}


std::string stringToLower(std::string str) {
    for (char &c : str) {
        c = std::tolower(c);
    }
    return str;
}
