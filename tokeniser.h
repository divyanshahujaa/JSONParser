#include <variant>
#include <string>

enum TokenType{
    OPENBRACE,
    CLOSEDBRACE,
    COLON,
    STRING,
    INTEGER,
    FLOAT,
    BOOL,
    NULLOBJ,
    DELIMITER,
    INVALID,
    UNKNOWN
};


typedef std::variant<std::string, int, float, bool> TokenVal;

class Token
{
public:
    TokenType type;
    TokenVal val;
};

Token getToken(FILE* pFile);

void runTokeniser(FILE* pFile, std::vector<Token>& tokens);