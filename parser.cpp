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
        std::string key;
        std::string value;
        bool isReadingKey = true;
        while((currChar = fgetc(pFile)) != EOF){
            switch (currChar)
            {
            case '{':
                if(!isReadingKey){
                    perror("Invalid Format");
                    return 1;
                }
                break;
            case '}':
                if(isReadingKey){
                    perror("Invalid Format");
                    return 1;
                }
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
        std::cout<<key<<" "<<value<<std::endl;
        fclose(pFile);
    }


    return 0;
}