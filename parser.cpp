#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

/*enum TokenType{
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

enum JSONValueType{
    JSONOBJECT,
    JSONVALUE
};

class JSONObject{
public:
    JSONValueType type;
    std::unordered_map<std::string, JSONObject> obj;
};


class JSONObjectValue : public JSONObject{
public:

    std::string data;
    JSONObjectValue(std::string s){
        this->data = s;
    }
};
enum JSONParserState{
    PARSESTART, PARSEKEY, PARSEVALUE
};
JSONObject parseTokenList(std::vector<JSONToken> tokenList){
    JSONObject parsedJson;
    JSONParserState state = PARSESTART;
    std::string key = "";
    JSONObject value;
    for(auto x : tokenList){
        switch (x.type)
        {
        case OPENBRACE:
            state = PARSEKEY;
            break;
        case CLOSEDBRACE:
            parsedJson.obj[key] = value;
            break;
        case KEY:
            key = x.data;
            break;
        case VALUE:
            value = JSONObjectValue(x.data);
            break;
        default:
            break;
        }
    }
    return parsedJson;

};*/

class JSONObject{
public:
    std::vector<std::pair<std::string,std::string>> objl;
};

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

union TokenVal
{
    std::string str;
    int integer;
    float fl;
    bool trufal;

};

class Token
{
public:
    TokenType type;
    TokenVal val;
};


Token getToken(FILE* pFile){
    char currChar;
    Token t;
    bool isIgnoreSpaces = true;
    bool isReadingString = false;
    bool isPreDecimal = true;
    TokenType currTokenType = UNKNOWN;

    while((currChar = fgetc(pFile))!=EOF){
        switch (currChar)
        {
        case '{':
            if(currTokenType==UNKNOWN){
                t.type = OPENBRACE;
                return t;
            }
            else if(currTokenType==STRING){
                t.val.str += currChar;
            }
            else{
                perror("INVALID TOKEN");
                t.type = INVALID;
                return t;
            }
            break;
        case '}':
            if(currTokenType==UNKNOWN){
                t.type = CLOSEDBRACE;
                return t;
            }
            else if(currTokenType==STRING){
                t.val.str += currChar;
            }
            else{
                perror("INVALID TOKEN");
                t.type = INVALID;
                return t;
            }
            break;
        case ':':
            if(currTokenType==UNKNOWN){
                t.type = COLON;
            return t;
            }
            else if(currTokenType==STRING){
                t.val.str += currChar;
            }
            else{
                perror("INVALID TOKEN");
                t.type = INVALID;
                return t;
            }
            break;
        case ',':
            if(currTokenType==UNKNOWN){
                t.type = DELIMITER;
                return t;
                //ungetc()
            }
            else if(currTokenType==STRING){
                t.val.str += currChar;
            }
            else{
                ungetc(currChar, pFile);
                return t;
            }
            
            break;
        case ' ':
            if(isIgnoreSpaces){
                continue;
            }
            else{
                if(currTokenType == STRING){
                    t.val.str += currChar;
                }
                else{
                    perror("INVALID TOKEN");
                    t.type = INVALID;
                    return t;
                }
            }
            break;
        case '"':
            if(isReadingString){
                isReadingString = false;
                return t;
            }
            else{
                isReadingString = true;
                currTokenType = STRING;
                t.type = STRING;
            }
        
        case '.':
            if(currTokenType == STRING){
                t.val.str += currChar;
            }
            else{
                currTokenType = FLOAT;
                t.type = FLOAT;
                isPreDecimal = false
            }
        
        default:
            if(currTokenType!=STRING){
                if(currTokenType != FLOAT)
                {
                    if(isdigit(currChar)){
                        currTokenType = INTEGER;
                        int x = currChar - '0';
                        t.val.integer = t.val.integer*10 + x;
                    }
                }
                else{
                    if(isPreDecimal){
                        int x = currChar - '0';
                        t.val.integer = t.val.integer*10 + x;
                    }
                    else{
                        float x = currChar - '0';
                        t.val.fl = t.val.fl + x/10;
                    }
                }
            }
            else{
                t.val.str += currChar;
            }
            break;
        }
    }
    
}

void printToken(Token t){
    std::cout<<t.type<<std::endl;
}

void parseTokens(FILE* pFile){
    std::vector<Token> tokens;
    char currChar;
    while((currChar = fgetc(pFile))!=EOF){
        ungetc(currChar, pFile);
        Token token = getToken(pFile);
        tokens.push_back(token);
    }
    for(auto t : tokens){
        printToken(t);
    }
    
}
/* 
    Holds a list of key-value pairs in sequence
    key is a string
    value can be either of:
        string
        int
        float
        JSONObject
*/
/*task 1 is to parse single key value(strings only) pair*/
/*task 2 is to parse multiple key value(strings only) pairs*/
/*task 3 is to parse multiple objects: string, int, float, nullobj, bool*/
int main(int argc, char* argv[]){
    JSONObject x; //the goal is to build the json object.
    if(argc!=2){
        std::cout<<"Incorrect Usage\n";
    }

    FILE *pFile;
    //char buffer[100];
    char currChar;
    pFile = fopen(argv[1], "r");
    if(pFile == nullptr){
        perror("Error opening file");
        return 1;
    }
    else{
        parseTokens(pFile);
        /*std::string key;
        std::string value;
        bool isReadingKey = true;
        bool isReadingString = false;
        bool isIgnoreSpaces = false;
        while((currChar = fgetc(pFile)) != EOF){
            switch (currChar)
            {
            case '{':
                if(!isReadingKey){
                    perror("Invalid Format");
                    return 1;
                }
                isIgnoreSpaces = true;
                break;
            case '}':
                if(isReadingKey){
                    perror("Invalid Format");
                    return 1;
                }
                break;
            case '"':
                if(!isReadingKey)
                    isReadingString = !isReadingString;
                break;
            case ' ':
                break;
            case ':':
                if(!isReadingKey){
                    perror("Invalid Format");
                    return 1;
                }
                isReadingKey = false;
                break;
            case ',':
                if(isReadingKey){
                    perror("Invalid Format");
                    return 1;
                }
                isReadingKey = true;
                x.objl.push_back({key, value});
                key = "";
                value = "";
                break;
            default:
                if(isReadingKey){
                    key = key + currChar;
                }
                else{
                    value = value + currChar;
                }
                break;
            }
            std::cout<<currChar;
        }
        std::cout<<"\n";
        if(isReadingKey){
            perror("Invalid Format");
            return 1;
        }
        x.objl.push_back({key, value});
        std::cout<<key<<" "<<value<<std::endl;*/
        fclose(pFile);
    }


    return 0;
}