#include "Lexer.h"
#include "CharFunctions.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

namespace lexer
{

    void Lexer::getOperatorToken() {
        std::pair<TokenType, size_t> autoToken = operatorsAuto.checkValue(currentLine.line, currentLine.column);
        if (autoToken.first == TokenType::INVALID)
            processBadToken(autoToken.second - currentLine.column);
        else
        {
            addToken(autoToken.first, autoToken.second - currentLine.column);
        }
    }

    std::pair<TokenType, size_t> Lexer::getKeyWordToken() {
        return keyWords.checkValue(currentLine.line, currentLine.column);
    }

    void Lexer::getSingleLineComment(TokenType const type) {
        addToken(TokenType::SingleLineComment, currentLine.line.length() - currentLine.column);
    }

    void Lexer::getMultilineComment() {
        currentState = LongComment;
        char currentSymbol;
        size_t forward = 0;
        while (!currentLine.ended(forward + 1)) {
            forward++;
            currentSymbol = currentLine.getNextSymbol(forward);
            if (currentState == LongComment) {
                if (currentSymbol == '-') {
                    currentState = LongCommentEnding;
                }
                continue;
            }
            else if (currentState == LongCommentEnding) {
                if (currentSymbol == '}') {
                    addLongToken(TokenType::MultiLineComment, forward + 1);
                    return;
                }
                else if (currentSymbol != '-') {
                    currentState = LongComment;
                }
            }
        }
        if (cache.length() > 0) cache += "\\n";
        cache += currentLine.line.substr(currentLine.column, forward);
        currentLine.column += forward + 1;
        currentState = LongComment;
    }

    void Lexer::processOneQuoteChar() {
        const auto getNextSymbol = [this]() { return currentLine.getNextSymbol(); };
        const auto getNextSymbolWithOffset = [this](size_t offset) { return currentLine.getNextSymbol(offset); };
        const auto addCharValueToken = [this](size_t length) { addToken(TokenType::CharValueOneQuote, length); };
        const auto processBadToken = [this]() { processBadToken(1); };
        currentState = Char;
        const char c = getNextSymbol();
        if (c == '\\' && !currentLine.ended(3)) {
            const char next = getNextSymbolWithOffset(3);
            if (next == '\'') {
                addCharValueToken(4);
                return;
            }
        }
        else if (!currentLine.ended(2) && getNextSymbolWithOffset(2) == '\'') {
            addCharValueToken(3);
            return;
        }
        processBadToken();
    }





    void Lexer::processTwoQuoteString() {
        currentState = String;
        size_t forward = 0;
        const auto getNextSymbol = [this](size_t forward) { return currentLine.getNextSymbol(forward); };
        const auto addStringValueToken = [this, forward]() { addToken(TokenType::StringValueInTwoQuotes, forward + 1); };
        const auto getErrorText = [this, forward]() -> std::string {
            return "No matching \" found to this quotation mark: (" + std::to_string(currentLine.row) +
                ", " + std::to_string(currentLine.column) + ")";
        };
        const auto getErrorLine = [this, forward]() -> std::string {
            return currentLine.line.substr(currentLine.column, forward);
        };
        const auto getErrorRow = [this]() { return currentLine.row; };
        const auto getErrorColumn = [this, forward]() { return currentLine.column; };
        const auto registerError = [this](const std::string& errorText, const std::string& errorLine, size_t errorRow, size_t errorColumn) {
            this->errors.emplace_back(errorText, errorLine, errorRow, errorColumn);
        };
        while (!currentLine.ended(forward + 1)) {
            forward++;
            const char currentSymbol = getNextSymbol(forward);
            if (currentState == String) {
                if (currentSymbol == '\\') {
                    currentState = StringSlash;
                }
                else if (currentSymbol == '\"') {
                    addStringValueToken();
                    return;
                }
            }
            else if (currentState == StringSlash) {
                currentState = String;
            }
        }
        registerError(getErrorText(), getErrorLine(), getErrorRow(), getErrorColumn());
    }


    void Lexer::processMultilineMode() {
        if (currentState == LongComment)
            getMultilineComment();
        else
            processBadToken(0);
    }

    void Lexer::processNumber() {
        currentState = Integer;
        char currentSymbol = currentLine.getNextSymbol();
        int forward = 0;

        while (!currentLine.ended(forward + 1) && isDigit(currentSymbol)) {
            forward++;
            currentSymbol = currentLine.getNextSymbol(forward);
        }

        if (isDot(currentSymbol)) {
            currentState = Float;
            forward++;
            currentSymbol = currentLine.getNextSymbol(forward);
        }

        while (!currentLine.ended(forward + 1) && isDigit(currentSymbol)) {
            forward++;
            currentSymbol = currentLine.getNextSymbol(forward);
        }

        if (currentLine.ended(forward + 1) || isEndOfToken(currentSymbol)) {
            addToken(currentState == Integer ? TokenType::IntValue : TokenType::FloatValue, forward + 1);
            return;
        }
        else {
            processBadToken(forward);
        }
    }

    void Lexer::registerError(const std::string& errorName, const std::string& errorLexeme, size_t row, size_t col) {
        errors.emplace_back(errorName, errorLexeme, row, col);
        currentLine.column += col + 1;
        currentState = Default;
    }

    void Lexer::processBadToken(size_t forward) {
        currentState = Error;
        char currentSymbol = currentLine.getNextSymbol(forward);
        while (!currentLine.ended(forward + 1) || isEndOfToken(currentSymbol)) {
            forward++;
            currentSymbol = currentLine.getNextSymbol(forward);
        }
        registerError("Invalid token ",
            currentLine.line.substr(currentLine.column, forward),
            currentLine.row,
            currentLine.column);

    }

    void Lexer::processComment() {
        char c = currentLine.getNextSymbol();;
        if (c == '-')
            getSingleLineComment(TokenType::SingleLineComment);
        else {
            getMultilineComment();
        }
    }

    void Lexer::processWord()
    {
        size_t forward = 0;
        char currentSymbol;
        do {
            forward++;
            currentSymbol = currentLine.getNextSymbol(forward);
            if (isEndOfToken(currentSymbol)) {
                auto pair = getKeyWordToken();
                if (pair.first == TokenType::INVALID) {
                    addToken(TokenType::Identifier, forward);
                }
                else {
                    addToken(pair.first, forward);
                }
                return;
            }
            if (currentState == Identifier) {
                if (currentSymbol == '\\') {
                    currentState = StringSlash;
                }
                else if (currentSymbol == '\"') {

                }
            }
        } while (!currentLine.ended(forward + 1));
        auto pair = getKeyWordToken();
        if (pair.first == TokenType::INVALID) {
            addToken(TokenType::Identifier, forward + 1);
        }
        else {
            addToken(pair.first, forward + 1);
        }
    }

    void Lexer::addEmptyToken(TokenType token, size_t size) {
        tokens.emplace_back(token, currentLine.row, currentLine.column);
        currentLine.column += size;
        currentState = Default;
    }

    void Lexer::addToken(TokenType token, size_t size) {
        int index = symbolTable.size();
        tokens.emplace_back(token, currentLine.row, currentLine.column, index);
        symbolTable.emplace_back(currentLine.line.substr(currentLine.column, size));
        currentLine.column += size;
        currentState = Default;
    }

    void Lexer::addLongToken(TokenType token, size_t size) {
        int index = symbolTable.size();
        tokens.emplace_back(token, currentLine.row, currentLine.column, index);
        symbolTable.emplace_back(cache + currentLine.line.substr(currentLine.column, size));
        currentLine.column += size;
        cache.clear();
        currentState = Default;
    }


    void Lexer::processNextToken()
    {
        if (currentLine.column > currentLine.line.length())
            return;

        if (currentState != Default) {
            processMultilineMode();
            return;
        }

        char currentSymbol = currentLine.getNextSymbol();;

        if (isWhitespace(currentSymbol)) {
            addToken(TokenType::WhiteSpace, 1);
            return;
        }

        if (isTab(currentSymbol)) {
            addToken(TokenType::Tab, 1);
            return;
        }

        if (isDigit(currentSymbol))
        {
            processNumber();
            return;
        }

        if (currentLine.column + 1 < currentLine.line.length() &&
            isComment(currentSymbol, currentLine.line[currentLine.column + 1]))
        {
            processComment();
            return;
        }

        if (isFirstIdentifierSymbol(currentSymbol))
        {
            processWord();
            return;
        }

        if (isSingleQuote(currentSymbol))
        {
            processOneQuoteChar();
            return;
        }
        if (isDoubleQuote(currentSymbol))
        {
            processTwoQuoteString();
            return;
        }
        if (isOperator(currentSymbol))
        {
            getOperatorToken();
            return;
        }
        //   if (isPunctuation(currentSymbol))
        //   {
        //       processPunctuation();
        //       return;
        //   }
        std::string symbol;
        symbol += currentLine.getNextSymbol();
        errors.emplace_back("Invalid symbol", symbol, currentLine.row, currentLine.column);
        currentLine.column += 1;
    }

    bool Lexer::getAllTokens(std::string const& pathToFile)
    {
        std::ifstream ifs(pathToFile);
        if (!ifs)
        {
            std::cerr << "\tFile doesn't exists!\n";
            return false;
        }
        while (!ifs.eof())
        {
            std::getline(ifs, currentLine.line);
            currentLine.column = 0;
            while (!currentLine.ended())
            {
                processNextToken();
            }
            currentLine.row += 1;
        }

        return true;
    }

    void Lexer::printResult(std::string const& pathToFile) const
    {
        std::ofstream ofs(pathToFile);

        ofs << "---------------------------Tokens list---------------------------\n";
        ofs << std::left << std::setw(22) << "Token type";
        ofs << std::left << std::setw(4) << "|row" << "|";
        ofs << std::left << std::setw(6) << "col|";
        ofs << std::left << "symbol table index and value\n";
        ofs << "-----------------------------------------------------------------\n";
        for (auto token : tokens)
        {
            if (printWhiteSpaces || token.type != TokenType::WhiteSpace) {
                ofs << std::left << std::setw(22) << TokenValue[static_cast<size_t>(token.type)];
                ofs << std::right << std::setw(4) << token.row_pos << "|";
                ofs << std::left << std::setw(6) << token.column_pos;


                if (token.symbolTableIndex != SYMBOL_TABLE_MAX)
                    ofs << " " << symbolTable[token.symbolTableIndex] << "";
                ofs << '\n';
            }
        }
        ofs << "\n---------------------------Invalid tokens---------------------------\n";
        ofs << std::left << std::setw(2) << "|row" << "|";
        ofs << std::left << std::setw(8) << "col|";
        ofs << "Error explanation";
        ofs << "\n--------------------------------------------------------------------\n";
        for (const auto& error : errors)
        {
            ofs << std::right << std::setw(2) << error.row_pos << "|";
            ofs << std::left << std::setw(8) << error.column_pos;
            ofs << error.error_message + " |" + error.error_symbol + "|\n";
        }
        ofs << "\n---------------------------Symbol table---------------------------\n";
        for (int i = 0; i < symbolTable.size(); ++i)
        {
            ofs << i << ") |" << symbolTable[i] << "|\n";
        }
    }

    std::vector<std::string> Lexer::getKeywordsFromFile(std::string const& pathToFile) {
        std::vector<std::string> result;
        std::ifstream ifs(pathToFile);
        std::string s;
        while (!ifs.eof()) {
            std::getline(ifs, s);
            result.push_back(s);
        }
        return result;
    }

    Lexer::Lexer()
    {
        std::vector<std::string> operators = getKeywordsFromFile("./operators.txt");
        std::vector<std::string> keywords = getKeywordsFromFile("./keywords.txt");
        operatorsAuto.setupAuto(operators, 52);
        keyWords.setupAuto(keywords, 1);
    }
}
