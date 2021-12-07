/**
 * @file scanner.h
 * @brief Interface of scanner.c
 * @author Adam Juliš
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

/**
 * @enum Types of tokens
 */

#ifndef SCANNER_H_
#define SCANNER_H_

typedef enum {
    TT_EOF,                 // End of file
    TT_EOL,                 //  \n        // idk ci to potrebujeme
    
    TT_PLUS,                //  +
    TT_MINUS,               //  -
    TT_MUL,                 //  *
    TT_DIV,                 //  /
    TT_FLOOR_DIV,           //  //
    TT_HASHTAG,             //  #
    
    TT_GREATER,             //  >
    TT_LESS,                //  <
    TT_GREATER_OR_EQ,       //  >=
    TT_LESS_OR_EQ,          //  <=
    TT_NOT_EQ,           //  ~=
    TT_EQ,                  //  ==
    
    TT_ASSIGN,              //  =
    TT_LEFT_PAR,            //  () //???
    TT_RIGHT_PAR,           //  )
    TT_COLON,               //  :
    TT_CONCAT,              //  ..
    TT_COMMA,               //  ,

    TT_NUMBER,   // uvidime ci to je potrebne zadanie 8 strana        
    TT_INTEGER,
    TT_STRING,

    TT_IDENTIFIER,
    TT_KW_DO,
    TT_KW_ELSE,
    TT_KW_END,
    TT_KW_FUNCTION,
    TT_KW_GLOBAL,
    TT_KW_IF,
    TT_KW_INTEGER,
    TT_KW_LOCAL,
    TT_KW_NIL,
    TT_KW_NUMBER,
    TT_KW_REQUIRE,
    TT_KW_RETURN,
    TT_KW_STRING,
    TT_KW_THEN,
    TT_KW_WHILE,

} TokenType;



/**
 * @enum Enumerate for states 
 */

typedef enum {

    S_START,            //co toto to?
    S_EOF,
    
    //operators
    S_PLUS,
    S_MINUS_COMMENT, // moze z toho vzinknut --
    S_MUL,
    S_DIV,
    S_FLOOR_DIV,
    S_HASHTAG,
    
    S_GREATER,
    S_LESS,
    S_GREATER_OR_EQ,
    S_LESS_OR_EQ,
    S_TILDE,
    S_NOT_OR_EQ,
    S_EQ,

    S_ASSIGN,
    S_LEFT_PAR,
    S_RIGHT_PAR,
    S_COLON,
    S_CONCAT,
    S_COMMA,

    //numbers
    S_INTEGER,
    S_DOT,

    S_NUMBER_NO_EX,
    S_EXPONENT,
    S_EX_PLUS_MINUS,
    S_NUMBER,
    
    //string
    S_STRING,
    S_ESCAPE,               // symbol '/' was loaded
    S_ESC_NUM_ZERO,         // 0 was loaded after '/'
    S_ESC_NUM_ONE,          // 1 was loaded after '/'
    S_ESC_NUM_TWO,          // 2 was loaded after '/'
    S_ESC_NUM_ZERO_ZERO,    // 0 was loaded after 0
    S_ESC_NUM_REST,         // rest of numbers loaded after 0,1 or 2 
    S_ESC_NUM_TWO_FIVE,     // 5 was loaded after 2
    S_STRING_END,
    
    //comments

    S_LINE_COMMENT,
    S_LINE_COMMENT_CHECK,
    S_DOUBLE_BRACKET,
    S_MULT_COMMENT,

    S_IDENTIFIER

} StateType;



/**
 * @struct Structure for Token attributes
 */

typedef struct {

    char *string; // TODO dynamicky string 
    int integer;
    double number;

} TokenAttributes;

/**
 * @struct Structure for Token
 */

typedef struct {

    TokenType type;
    TokenAttributes attribs;

} Token;

/**
 * @brief Get the token object
 * 
 * @return TToken* 
 */

void get_token(Token *token);

/**
 * @brief Appends char to string
 * 
 * char *string = NULL;
 * expand_string(&string,'a');
 * 
 * @param string string where the c will be apended
 * @param c char to be appended
 * @return void
 */

int expand_string(char **string, char c);
void init_token(Token *token);
void alphabet(Token *token, char c);
int dash_minus(Token *token);
void number(Token *token, char c);
void string(Token *token);

#endif 