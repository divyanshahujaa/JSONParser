#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <map>
#include "tokeniser.h"

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

void runParser(JSONObject& x, std::vector<Token> tokens, int idx){
    bool isFirstOpenBrace = true;
    bool isReadingKey = true;
    std::string currKey = "";
    for(int i=idx; i<tokens.size();i++){
        auto token = tokens[i];
        switch (token.type)
        {
        case OPENBRACE:
            {if(isFirstOpenBrace){
                isFirstOpenBrace = false;
            }
            else{
                JSONValue t;
                t.type_id = JSONValueType::JSON;
                // t._value = std::map<std::string, JSONValue>();
                JSONObject k;
                runParser(k, tokens, i);
                t._value = std::move(k.objl);
                x.objl[currKey] = std::move(t);
            }}
            break;
        
        case CLOSEDBRACE:
            return;
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
            if(isReadingKey){
                std::cout<<"error parsing";
                return;
            }
            {JSONValue t;
            t.type_id = JSONValueType::INT;
            t._value = std::get<int>(token.val);
            x.objl[currKey] = std::move(t);}
            break;

        case FLOAT:
            if(isReadingKey){
                std::cout<<"error parsing";
                return;
            }
            {JSONValue t;
            t.type_id = JSONValueType::FLOAT;
            t._value = std::get<float>(token.val);
            x.objl[currKey] = std::move(t);}
            break;

        case BOOL:
            if(isReadingKey){
                std::cout<<"error parsing";
                return;
            }
            {JSONValue t;
            t.type_id = JSONValueType::STRING;
            t._value = std::get<std::string>(token.val);
            x.objl[currKey] = std::move(t);}
            break;

        case NULLOBJ:
            if(isReadingKey){
                std::cout<<"error parsing";
                return;
            }
            {JSONValue t;
            t.type_id = JSONValueType::NULLOBJ;
            t._value = std::nullptr_t();
            x.objl[currKey] = std::move(t);}
            break;

        case DELIMITER:
            if(isReadingKey){
                std::cout<<"error parsing";
                return;
            }
            isReadingKey = true;
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
    runParser(x, tokens, 0);
    std::cout<<x.objl.size()<<std::endl;

    fclose(pFile);
    


    return 0;
}