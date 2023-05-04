#include "CurrentLineWrapper.h"

bool lexer::CurrentLineWrapper::ended(size_t forward) const {
    return line.length() <= column + forward;
}

char lexer::CurrentLineWrapper::getNextSymbol(size_t forward) const {
    return line[column + forward];
}