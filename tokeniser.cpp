#include <string>
#include <vector>

enum TokenType{
    OPENBRACE,
    CLOSEDBRACE,
    COLON,
    KEY,
    VALUE,
    DELIMITER
};
class JSONToken{
public:
    std::string data;
    TokenType type;

    JSONToken(TokenType type){
        this->type = type;
    }

    JSONToken(std::string data, TokenType type){
        this->data = data;
        this->type = type;
    }
};


enum TokeniserState{
    START,
    PARSINGKEY,
    PARSINGVALUE,
    ParsingKeyInQuotes,
    ParsingValInQuotes,
    OUTQUOTES
};

/*{key : value, key2 : value2, ...}
ignore spaces, int only parsing*/
std::vector<JSONToken> tokenise(std::string json){
    int n = json.length();
    std::vector<JSONToken> tokenList;
    TokeniserState state = START;
    std::string data = "";
    for(char c: json){
        switch (c)
        {
        case ' ':
            
            break;
        
        case '{':
            state = PARSINGKEY;
            tokenList.push_back(JSONToken(OPENBRACE));
            break;
        case '}':
            //state = CLOSEBRACKET;
            tokenList.push_back(JSONToken(CLOSEDBRACE));
            break;
        case '"':
            if(state == PARSINGKEY){
                state = ParsingKeyInQuotes;
            }
            else if(state == PARSINGVALUE){
                state = ParsingValInQuotes;
            }
            else if(state == ParsingKeyInQuotes){
                state = PARSINGKEY;
                tokenList.push_back(JSONToken(data, KEY));
                data = "";
            }
            else if(state == ParsingValInQuotes){
                state = PARSINGVALUE;
                tokenList.push_back(JSONToken(data, VALUE));
                data = "";
            }
            break;
        case ':':
            state = PARSINGVALUE;
            tokenList.push_back(JSONToken(COLON));
            break;
        case ',':
            state = PARSINGKEY;
            tokenList.push_back(JSONToken(DELIMITER));
            break;
        default:
            data += c;
            break;
        }
    }
    return tokenList;
}

int main(){
    return 0;
}