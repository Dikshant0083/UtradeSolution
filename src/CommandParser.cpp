#include "../include/CommandParser.h"
#include <sstream>

std::vector<std::string> parseCommand(std::string input) {

    std::stringstream ss(input);

    std::vector<std::string> tokens;

    std::string word;

    while(ss >> word)
        tokens.push_back(word);

    return tokens;
}