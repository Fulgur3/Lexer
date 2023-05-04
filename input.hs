import System.Environment

-- Define the token type
data Token = NumberToken Int | PlusToken | MinusToken | TimesToken | DivToken deriving (Show, Eq)

-- Define the lexer function
lexer :: String -> [Token]
lexer [] = []
lexer (c:cs)
  | isDigit c = lexNumber (c:cs)
  | isSpace c = lexer cs
  | c == '+'  = PlusToken : lexer cs
  | c == '-'  = MinusToken : lexer cs
  | c == '*'  = TimesToken : lexer cs
  | c == '/'  = DivToken : lexer cs
  | otherwise = error $ "Invalid character: " ++ [c]
  where
    lexNumber cs = NumberToken (read num) : lexer rest
      where (num,rest) = span isDigit cs

-- Define a function to parse arithmetic expressions
parseExpr :: [Token] -> Int
parseExpr tokens = case parseTerm tokens of
  (term, []) -> term
  _          -> error "Invalid expression"
  where
    parseTerm :: [Token] -> (Int, [Token])
    parseTerm tokens = case parseFactor tokens of
      (factor, TimesToken:rest) -> let (term, tokens') = parseTerm rest in (factor * term, tokens')
      (factor, DivToken:rest)   -> let (term, tokens') = parseTerm rest in (factor `div` term, tokens')
      (factor, tokens')         -> (factor, tokens')

    parseFactor :: [Token] -> (Int, [Token])
    parseFactor (NumberToken n:rest) = (n, rest)
    parseFactor (PlusToken:rest)     = parseFactor rest
    parseFactor (MinusToken:rest)    = let (n, tokens) = parseFactor rest in (-n, tokens)
    parseFactor tokens              = error $ "Unexpected token: " ++ show tokens

-- Define the main function
main :: IO ()
main = do
  args <- getArgs
  case args of
    [filename] -> do
      input <- readFile filename
      let tokens = lexer input
      let result = parseExpr tokens
      putStrLn $ "Result: " ++ show result
    _ -> putStrLn "Usage: lexer-test <input-file>"