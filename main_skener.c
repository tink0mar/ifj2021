#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "error.h"

const char* tks[] = {

    "TT_EOF",                 // End of file
    "TT_EOL",                 //  \n        // idk ci to potrebujeme
    
    "TT_PLUS",                //  +
    "TT_MINUS",               //  -
    "TT_MUL",                 //  *
    "TT_DIV",                 //  /
    "TT_FLOOR_DIV",           //  //
    "TT_HASHTAG",             //  #
    
    "TT_GREATER",             //  >
    "TT_LESS",                //  <
    "TT_GREATER_OR_EQ",       //  >=
    "TT_LESS_OR_EQ",          //  <=
    "TT_NOT_OR_EQ",           //  ~=
    "TT_EQ",                  //  ==
    
    "TT_ASSIGN",              //  =
    "TT_LEFT_PAR",            //  () //???
    "TT_RIGHT_PAR",           //  )
    "TT_COLON",               //  :
    "TT_CONCAT",              //  ..
    "TT_COMMA",               //  ,

    "TT_NUMBER",   // uvidime ci to je potrebne zadanie 8 strana        
    "TT_INTEGER",
    "TT_STRING",

    "TT_IDENTIFIER",
    "TT_KW_DO",
    "TT_KW_ELSE",
    "TT_KW_END",
    "TT_KW_FUNCTION",
    "TT_KW_GLOBAL",
    "TT_KW_IF",
    "TT_KW_INTEGER",
    "TT_KW_LOCAL",
    "TT_KW_NIL",
    "TT_KW_NUMBER",
    "TT_KW_REQUIRE",
    "TT_KW_RETURN",
    "TT_KW_STRING",
    "TT_KW_THEN",
    "TT_KW_WHILE"

};


int main (){

    Token *token = malloc(sizeof(Token));

    while(1){
        get_token(token);
      

        if (num_error != 0){
            error_mess(num_error);
            return 0;
        }
        
        if (token->type == TT_EOF){
            printf("KONIEC");
            return 0;
        }
        printf("%s\n", tks[token->type]);
        printf("string: %s\n", token->attribs.string);
        printf("first:%d second:%f",token->attribs.integer, token->attribs.number);

    }

}