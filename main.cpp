#include <iostream>
#include <iomanip>
#include <fstream>
#include "FiniteAuto.h"
#include "Lexer.h"

void printHeading(std::ofstream ofstream);

int main()
{
    bool saveToFile = false;
    lexer::Lexer lexer1;
    auto lexerCompleted = lexer1.getAllTokens("./input.hs");
    if (lexerCompleted)
    {
        lexer1.printResult("./output.txt");
        std::cout << "Finished!";
    }
    else {
        std::cerr << "An error occured during lexer execution";
    }
    return 0;
}

void printHeading(std::ofstream ofstream) {

}