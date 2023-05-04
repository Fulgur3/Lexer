#include <stdint.h>
#include <vector>



namespace lexer
{
    enum class TokenType : size_t
    {
        Identifier, // 1
        Include,                   //
        Import,
        If,
        Then,
        Else,
        Class,
        Where,
        Do,
        Case,
        Of,
        Error,
        Deriving,
        Echo,
        Print,
        Int, //types
        Integer,
        Bool,
        Scientific,
        Char,
        String,
        Integral,
        Ratio,
        Float,
        Fractional,
        Double,
        Complex,  //type classes
        Eq,
        Num,
        Ord,
        Show,
        Bounded,
        Enum,
        Read,
        Monad,
        Maybe,
        Functor,
        Either,
        Ordering,
        Nothing,
        Just,
        Left,
        Right,
        LT,
        EQ,
        GT,
        True,
        False,
        StringValueInTwoQuotes,
        IntValue,
        FloatValue,
        CharValueOneQuote,
        Dollar, // arithmetic tokens 14
        Add,
        Sub
        , Multi
        , Div
        , Mod
        , Exp
        , Assignment
        , Equal
        , NotEqualQuest
        , NotEqualBracket
        , GreaterThan
        , LessThan
        , GreaterThanEqual
        , LessThanEqual
        , Concat
        , Ampersand
        , AndSymb
        , OrSymb
        , Not
        , BitwiseOr
        , BitwiseXor
        , LeftShift
        , RightShift
        , BitwiseOrAssignment
        , BitwiseXorAssignment
        , LeftShiftAssignment
        , RightShiftOrAssignment    //special
        , MinusLess
        , MinusMinusLess
        , Dot
        , QuestMark
        , Colon
        , NullCoalescing
        , ArrayKey
        , VarReference,
        DoubleColon,   //comments
        Comma
        , Semicolon
        , LParen
        , RParen
        , LBracket
        , RBracket
        , LBrace
        , RBrace
        , Cat
        , Dash
        , DotDotDot
        , BackwardArror
        , SingleLineComment       //punkuation
        , MultiLineComment
        , MultiLineArray,
        WhiteSpace,
        Tab,
        INVALID,
        TOKEN_NUMBER
    };









    std::string const TokenValue[static_cast<size_t>(TokenType::TOKEN_NUMBER)] =
    {
        "Identifier", //1
        "include",   //2              // Include //14
        "import",       //3          // Import
        "if",                      // If
        "then",                    // Else
        "else",                  // ElseIf
        "class",                // Function
        "where",                  // Return
        "do",                    // Define
        "case",                    // Static
        "of",                    // Global
        "error",                    // Global
        "deriving",                    // Global
        "echo",                      // Echo
        "print",                     // Print
        "Int", //12
        "Integer",
        "Bool",
        "Scientific",
        "Char",
        "String",
        "Integral",
        "Ratio",
        "Float",
        "Fractional",
        "Double",
        "Complex",
        "Eq",
        "Num",
        "Ord",
        "Show",
        "Bounded",
        "Enum",
        "Read",
        "Monad",
        "Maybe",
        "Functor",
        "Either",
        "Ordering",
        "Nothing",
        "Just",
        "Left",
        "Right",
        "LT",
        "EQ",
        "GT",
        "True",
        "False",
        "StringValueInTwoQuotes",
        "IntValue",
        "FloatValue",
        "CharValueOneQuote",
        "$",                       // DollarIdentifier
        "+",                        // Add
        "-",                        // Sub
        "*",                        // Multi
        "/",                        // Div
        "%",                        // Mod
        "**",                       // Exp
        "=",                        // Assignment
        "==",                       // Equal
        "/=",                       // NotEqualQuest
        "<>",                       // NotEqualBracket
        ">",                        // GreaterThan
        "<",                        // LessThan
        ">=",                       // GreaterThanEqual
        "<=",                       // LessThanEqual
        "++",
        "&",                        // Ampersand
        "&&",                       // AndSymb
        "||",                       // OrSymb
        "!",                        // Not
        "|",                        // BitwiseOr
        "^",                        // BitwiseXor
        "<<",                       // LeftShift
        ">>",                       // RightShift
        "|=",                       // BitwiseOrAssignment
        "^=",                       // BitwiseXorAssignment
        "<<=",                      // LeftShiftAssignment
        ">>=",                      // RightShiftOrAssignment
        "-<",                      // MinusLess
        "--<",                      // MinusMinusLess
        ".",                        // Dot
        "?",                        // QuestMark
        ":",                        // Colon
        "??",                       // NullCoalescing
        "=>",                       // ArrayKey
        "->",                       // VarReference,
        "::",                       // DoubleColon

        ",",                        // Comma
        ";",                        // Semicolon
        "(",                        // LParen
        ")",                        // RParen
        "[",                        // LBracket
        "]",                        // RBracket
        "{",                        // LBrace
        "}",                        // RBrace
        "@",                        // Cat
        "_",                        // Dash
        "...",                        // DotDotDot
        "<-",                        // DotDotDot
        "SingleLineComment",                       // SingleLineComment
        "MultiLineComment",                    // MultiLineComment
        "[|, |]",                    // MultiLineArray
        "Whitespace",
        "Tab",
        "INVALID"
    };








    /*
    26 |
    27 ~
    28 as
    29 case, of
    30 class
    31 data
    32 data family
    33 data instance
    34 default
    35 deriving
    36 deriving instance
    37 do
    38 forall
    39 foreign
    40 hiding
    41 if, then, else
    42 import
    43 infix, infixl, infixr
    44 instance
    45 let, in
    46 mdo
    47 module
    48 newtype
    49 proc
    50 qualified
    51 rec
    52 type
    53 type family
    54 type instance
    55 where
    */
}
