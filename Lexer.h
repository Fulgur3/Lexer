#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <vector>
#include <memory>

#include "FiniteAuto.h"
#include "CurrentLineWrapper.h"

namespace lexer
{

    class Lexer
    {
        enum LexerState {
            Default,
            Error,
            LongComment,
            LongCommentEnding,
            Integer,
            Float,
            Char,
            String,
            StringSlash,
            Identifier
        };


        std::vector<std::string> symbolTable;
        std::vector<Token> tokens;
        std::vector<InvalidToken> errors;
        lexer::CurrentLineWrapper currentLine;
        LexerState currentState = Default;
        std::string cache;
        FiniteAuto operatorsAuto;
        FiniteAuto keyWords;

        void getOperatorToken();
        void addEmptyToken(TokenType token, size_t size);

        void getSingleLineComment(TokenType type);
        void getMultilineComment();
        void processMultilineMode();
        void processNumber();
        void processComment();
        void processWord();
        void processOneQuoteChar();
        void processTwoQuoteString();
        void processNextToken();

    public:

        bool printWhiteSpaces = false;

        Lexer();
        static std::vector<std::string> getKeywordsFromFile(std::string const& pathToFile);

        bool getAllTokens(std::string const& pathToFile);
        void addToken(TokenType token, size_t size);
        void processBadToken(size_t forward);
        void registerError(const std::string& errorName, const std::string& errorLexeme, size_t row, size_t col);
        std::pair<TokenType, size_t> getKeyWordToken();
        void addLongToken(TokenType token, size_t size);

        void printResult(const std::string& pathToFile) const;
    };
}




#endif //LEXER_LEXER_H