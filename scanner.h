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
    TT_NOT_OR_EQ,           //  ~=
    TT_EQ,                  //  ==
    
    TT_ASSIGN,              //  =
    TT_LEFT_PAR,            //  ()
    TT_RIGHT_PAR,           //  )
    TT_COLON,               //  :
    TT_CONCAT,              //  ..
    TT_COMMA,               //  ,

    TT_NUMBER,   // uvidime ci to je potrebne zadanie 8 strana        
    TT_INTEGER,
    TT_STRING,
    TT_NIL,

    TT_COMMENT,             //  comment
    TT_MULT_COMMENT,        //  multiline comment 
    TT_IDENTIFIER

} TypeToken;

/**
 * @enum Enumerate for states 
 */

typedef enum {

    S_EOF,
    
    //operators
    S_PLUS,
    S_MINUS,
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

    S_DASH,
    S_LINE_COMMENT,
    S_START_MULT_COMMENT,
    S_END_MULT_COMMENT,

    S_IDENTIFIER

} State;


typedef enum {
    KW_END,
    KW_ELSE,
    KW_END,
    KW_FUNCTION

} KeyWords;


/**
 * @struct Structure for Token
 */

typedef union {

    char *string; // TODO dynamicky string 
    int integer;
    double number; 

} TokenAttributes;

typedef struct {

    TypeToken token_type;
    TokenAttributes token_attrib;

} TToken;

