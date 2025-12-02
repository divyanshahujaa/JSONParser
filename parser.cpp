#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <map>
#include "tokeniser.h"
/*
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
enum class JSONValueType{
    STRING,
    INT,
    FLOAT,
    BOOL,
    NULLOBJ,
    JSON,
    UNKNOWN
};

class JSONValue{
public:
    std::variant<std::string, int, float, bool, std::nullptr_t, std::map<std::string, JSONValue>> _value;
    JSONValueType type_id;

    JSONValue(){
        this->type_id = JSONValueType::UNKNOWN;
    }

    JSONValue(JSONValue &x) = delete;
    
    JSONValue(JSONValue &&x): _value(std::move(_value)), type_id(x.type_id){
        x.type_id = JSONValueType::UNKNOWN;
    }
    JSONValue& operator=(JSONValue &&x){
        _value = std::move(x._value);
        type_id = x.type_id;
        return *this;
    }


};

class JSONObject{
public:
    std::map<std::string, JSONValue> objl;
};

void runParser(JSONObject& x, std::vector<Token> tokens){
    bool isFirstOpenBrace = true;
    bool isReadingKey = true;
    std::string currKey = "";
    for(auto token : tokens){
        switch (token.type)
        {
        case OPENBRACE:
            {if(isFirstOpenBrace){
                isFirstOpenBrace = false;
            }
            else{
                JSONValue t;
                t.type_id = JSONValueType::JSON;
                t._value = std::map<std::string, JSONValue>();
                x.objl[currKey] = std::move(t);
            }}
            break;
        
        case CLOSEDBRACE:
            
            break;
        
        case COLON:
            isReadingKey = false;
            break;
        
        case STRING:
            {if(isReadingKey){
                currKey = std::get<std::string>(token.val);
            }
            else{
                JSONValue t;
                t.type_id = JSONValueType::STRING;
                t._value = std::get<std::string>(token.val);
                x.objl[currKey] = std::move(t);
            }}
            break;
        
        case INTEGER:
            {JSONValue t;
            t.type_id = JSONValueType::INT;
            t._value = std::get<int>(token.val);
            x.objl[currKey] = std::move(t);}
            break;

        case FLOAT:
            {JSONValue t;
            t.type_id = JSONValueType::FLOAT;
            t._value = std::get<float>(token.val);
            x.objl[currKey] = std::move(t);}
            break;

        case BOOL:
            {JSONValue t;
            t.type_id = JSONValueType::STRING;
            t._value = std::get<std::string>(token.val);
            x.objl[currKey] = std::move(t);}
            break;

        case NULLOBJ:
            {JSONValue t;
            t.type_id = JSONValueType::NULLOBJ;
            t._value = std::nullptr_t();
            x.objl[currKey] = std::move(t);}
            break;

        case DELIMITER:
            
            break;

        case INVALID:
            /* code */
            break;

        case UNKNOWN:
            /* code */
            break;

        default:
            break;
        }
    }
    return;
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
/*task 4: parse the tokens to get the json structure and write it to the JSON object*/
int main(int argc, char* argv[]){
    JSONObject x; //the goal is to build the json object.
    std::cout<<x.objl.size()<<std::endl;
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

    std::vector<Token> tokens;
    runTokeniser(pFile, tokens);
    runParser(x, tokens);


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
    


    return 0;
}