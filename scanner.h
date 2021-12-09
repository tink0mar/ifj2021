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

    S_START,
    S_EOF,
    
    //operators
    S_PLUS,
    S_MINUS_COMMENT,
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

    char *string;
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
 * @brief Load data from stdin, set token attributes/type if data is valid
 * 
 * if data invalid, set error.c and return. Ignors whitespaces if it's irrelevant
 * 
 * @return void
 */
void get_token(Token *token);

/**
 * @brief Appends char to string
 *  
 * @param string string where the c will be apended
 * @param c - char to be appended
 * 
 * @return 0 - ok
 * @return 99 - realloc failed
 */

int expand_string(char **string, char c);

/**
 * @brief Init token to next use 
 * @param token - token->attribs.string set NULL
 *
 * @return void
 */
void init_token(Token *token);

/**
 * @brief Read identifier, check if there is any keyword
 * 
 * check if identifier is valid
 * 
 * @param token - load identifier to token->attribs.string and set token->type
 * @return void
 */
void alphabet(Token *token, char c);

/**
 * @brief Finds out whether it's a "-"sign or comment
 * 
 * if data invalid (eof inside multiple lines comment) set error.c and return
 * 
 * @param token - if its "-"sign, sets token->type to TT_MINUS
 * 
 * @return 1 - "-"sign
 * @return 0 - comment, get_token continue 
*/
int dash_minus(Token *token);

/**
 * @brief Read number to token, check valid format of number
 * 
 * if number invalid, set error.c and return
 * 
 * @param token - set token->type and load to token->attribs number 
 * @param c - first number loaded by get_token 
 *
 *  @return void
*/
void number(Token *token, char c);

/**
 * @brief Read string, if valid, set to token->attribs.string
 * 
 * if load "\", then expect number 1-255 or invalid 
 * if string invalid, set error.c and return
 * 
 * @param token - set token->type and load to token->attribs number 
 * @param c - first number loaded by get_token 
 *
 * @return void
*/
void string(Token *token);

#endif 