#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <cstdio>
#include <string>
#include <limits>
#include "TokenSamples.h"
#define TokenList std::vector<std::pair<std::string, TokenType> >


namespace lexer
{
    const size_t SYMBOL_TABLE_MAX = std::numeric_limits<size_t>::max();

    struct Token {
        TokenType type;
        size_t row_pos;
        size_t column_pos;
        size_t symbolTableIndex;

        Token()
        {
            setInvalid();
        }

        Token(TokenType type, size_t row, size_t col, size_t symbol_table_index = SYMBOL_TABLE_MAX)
        {
            this->type = type;
            this->row_pos = row;
            this->column_pos = col;
            this->symbolTableIndex = symbol_table_index;
        }

        void setInvalid()
        {
            type = TokenType::INVALID;
            row_pos = column_pos = symbolTableIndex = SYMBOL_TABLE_MAX;
        }
    };

    struct InvalidToken
    {
        std::string error_message;
        std::string error_symbol;
        size_t row_pos;
        size_t column_pos;

        InvalidToken() {}

        InvalidToken(std::string message, std::string symbol, size_t row, size_t col)
        {
            this->error_message = message;
            this->error_symbol = symbol;
            this->row_pos = row;
            this->column_pos = col;
        }
    };
}

#endif //LEXER_TOKEN_H