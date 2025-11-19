#include <string>
#include <vector>
#include <unordered_map>

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

};
int main(){
    JSONObject x;
    return 0;
}