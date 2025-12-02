#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <variant>
#include <map>
#include "tokeniser.h"



std::map<int, std::string> translator = {{OPENBRACE, "OPENBRACE"}, {CLOSEDBRACE, "CLOSEDBRACE"},
    {COLON, "COLON"},
    {STRING, "STRING"},
    {INTEGER, "INTEGER"},
    {FLOAT, "FLOAT"},
    {BOOL, "BOOL"},
    {NULLOBJ, "NULLOBJ"},
    {DELIMITER, "DELIMITER"},
    {INVALID, "INVALID"},
    {UNKNOWN, "UNKNOWN"}};




Token getToken(FILE* pFile){
    char currChar;
    Token t;
    bool isIgnoreSpaces = true;
    bool isReadingString = false;
    bool isPreDecimal = true;
    int mayBeNull = 0;
    int mayBeTrue = 0;
    int mayBeFalse = 0;
    TokenType currTokenType = UNKNOWN;

    while((currChar = fgetc(pFile))!=EOF){
        // std::cout<<currChar<<std::endl;
        switch (currChar)
        {
        case '{':
            if(currTokenType==UNKNOWN){
                t.type = OPENBRACE;
                return t;
            }
            else if(currTokenType==STRING){
                t.val = std::get<std::string>(t.val) +currChar;
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
                t.val = std::get<std::string>(t.val) +currChar;
            }
            else{
                // perror("INVALID TOKEN");
                // t.type = INVALID;
                ungetc(currChar, pFile);
                return t;
            }
            break;
        case ':':
            if(currTokenType==UNKNOWN){
                t.type = COLON;
            return t;
            }
            else if(currTokenType==STRING){
                t.val = std::get<std::string>(t.val) +currChar;
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
                t.val = std::get<std::string>(t.val) +currChar;
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
                    t.val = std::get<std::string>(t.val) +currChar;
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
            break;
        case '.':
            if(currTokenType == STRING){
                t.val = std::get<std::string>(t.val) +currChar;
            }
            else{
                currTokenType = FLOAT;
                t.type = FLOAT;
                isPreDecimal = false;
            }
            break;
        
        default:
            // std::cout<<"am i here";
            if(currTokenType!=STRING){
                
                if(currTokenType != FLOAT)
                {
                    
                    if(isdigit(currChar)){
                        if(currTokenType==UNKNOWN){
                            // std::cout<<"am i here";
                            currTokenType = INTEGER;
                            t.val = 0;
                            t.type = INTEGER;
                        }
                        currTokenType = INTEGER;
                        t.type = INTEGER;
                        int x = currChar - '0';
                        // t.val.integer = t.val.integer*10 + x;
                        t.val = std::get<int>(t.val)*10 +x;
                    }
                }
                else{
                    
                    if(isPreDecimal){
                        int x = currChar - '0';
                        // t.val.integer = t.val.integer*10 + x;
                        
                        t.val = std::get<int>(t.val)*10 +x;
                    }
                    else{
                        float x = currChar - '0';
                        // t.val.fl = t.val.fl + x/10;
                        if(std::holds_alternative<int>(t.val)){
                            t.val = static_cast<float>(std::get<int>(t.val));
                        }
                        t.val = std::get<float>(t.val) +x/10;
                    }
                }

                if(currTokenType == UNKNOWN){
                    if(currChar=='n'){
                        mayBeNull = 1;
                        continue;
                    }
                    if(currChar=='t'){
                        mayBeTrue = 1;
                        continue;
                    }
                    if(currChar=='f'){
                        mayBeFalse = 1;
                        continue;
                    }
                }

            }
            else{
                
                t.val = std::get<std::string>(t.val) +currChar;
            }
            if(currTokenType == UNKNOWN){
                if(mayBeNull!=0){
                    if((mayBeNull==1) && (currChar=='u')){
                        mayBeNull++;
                        continue;
                    }
                    if((mayBeNull==2) && (currChar=='l')){
                        mayBeNull++;
                        continue;
                    }
                    if((mayBeNull==3) && (currChar=='l')){
                        mayBeNull++;
                        t.type = NULLOBJ;
                        return t;
                    }
                }
                if(mayBeTrue!=0){
                    if(mayBeTrue==1 && currChar=='r'){
                        mayBeTrue++;
                        continue;
                    }
                    if(mayBeTrue==2 && currChar=='u'){
                        mayBeTrue++;
                        continue;
                    }
                    if(mayBeTrue==3 && currChar=='e'){
                        mayBeTrue++;
                        t.type = BOOL;
                        t.val = true;
                        return t;
                    }
                }
                if(mayBeFalse!=0){
                    if(mayBeFalse==1 && currChar=='a'){
                        mayBeFalse++;
                    }
                    if(mayBeFalse==2 && currChar=='l'){
                        mayBeFalse++;
                    }
                    if(mayBeFalse==3 && currChar=='s'){
                        mayBeFalse++;
                    }
                    if(mayBeFalse==4 && currChar=='e'){
                        mayBeFalse++;
                        t.type = BOOL;
                        t.val = false;
                        return t;
                    }
                }
            }
            break;
        }
    }
    return t;
}

void printToken(Token t){
    std::cout<<translator[t.type]<<std::endl;
}

void parseTokens(FILE* pFile){
    std::vector<Token> tokens;
    char currChar;
    while((currChar = fgetc(pFile))!=EOF){
        ungetc(currChar, pFile);
        Token token = getToken(pFile);
        tokens.push_back(token);
        printToken(token);
    }
    // for(auto t : tokens){
    //     printToken(t);
    // }
    
}
