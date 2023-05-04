#include <cstdio>
#include "Token.h"

namespace lexer {
    class CurrentLineWrapper {
    public:
        size_t column{ 0 };
        size_t row{ 0 };
        std::string line;

        bool ended(size_t forward = 0) const;
        char getNextSymbol(size_t forward = 0) const;
    };
}
