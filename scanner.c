/**
 * @file scanner.c
 * @brief Scanner for ifj compiler
 * @author Adam Juliš
 * 
 * Project: IFJ compiler
 * Date: 24.11.2021
 */ 

#include "scanner.h" 
#include "stdio.h"
#include "string.h"
#include "error.h"

int expand_string(char **string, char c){
    int str_len;

    if (*string == NULL) {
        str_len = 1;
    } else {    
        str_len = (int)strlen(*string);
    }

    char *ptr = (char *)realloc( *string , sizeof(char) * (str_len+1) );
    
    if (ptr == NULL){

        return 1;
    } else {
        
        ptr[str_len] = c;
        ptr[str_len+1] = '\0'; 
        
        *string = ptr;
        return 0;
    }
}
/**
 *@brief Init token to next use 
 * @param JE TO DOBRE MATKO?
 */
void init_token(Token *token){
    free(token->attribs.string);
    token->attribs.string = NULL;
    token->attribs.key_word = NULL;
    return;
}
/**
 * @brief Reading comments, return when comment ends
 * @param 
*/
void comment(Token *token){
    int tmp = 1;
    char symbol;

    symbol = getchar();
    if ( symbol == '[' ){
        symbol = getchar();
        if ( symbol == '[' ){
            tmp=0;
        }
        else
            ungetc(symbol, stdin);
    }
    /*detekovan --[[, ceka na EOF nebo ]] */
    /*otestuj velky soubor s --[[ ]] a vyjeb like xyz]EOF */
    while(tmp==0){
        symbol = getchar();
        if (symbol==']'){
            symbol=getchar();
            if (symbol == ']'){
                get_token(token);   //NA CO VLASTNE CEKAS, NA KONEC GET_TOKEN, PROMENNOU OR...?
                return; /*mohu takto ressetovat?*/
            }
            else if (symbol == 'EOF'){

              return;   /*mam vratit neuzavreny komentar? bych musel nacitat cely, staci jen token? asio vlastne*/  
            }
        if (symbol == 'EOF')
            return;

        }
    }
    /*radkovy komment*/
    while (tmp==1){
       if (symbol == 'EOL')
       /**rekurze?*/
            return;
       if (symbol == 'EOF'){

          /*KONEC, posli ze je chybicka*/
       } 
       symbol = getchar();
    }
    return;
}
/**
 *  @brief Load the char/string to token
 *  @param NEPODPORUJE JINY NEZ PISMENKOVY STRING
 * 
 */
void alphabet(Token *token, char symbol){

    expand_string(&token->attribs.string, symbol); 
    
    while (1){
        symbol = getchar();
        if (symbol < '0' || '9' < symbol && symbol < 'A' || 'Z' < symbol && symbol != '_' && 'symbol' < 'a' || 'z' < symbol ){
            ungetc(symbol, stdin);
            /**CHECK IT MORE*//*VYPRAZDNIT ATTRIBS OR NOT MORE*/
            if (strcmp(token->attribs.string, 'do')){
                token->type = TT_KW_DO;
                return;
                }
            if (strcmp(token->attribs.string, 'else')){
                token->type = TT_KW_ELSE;
                return;
                }
            if (strcmp(token->attribs.string, 'end')){
                token->type = TT_KW_END;
                return;
                }
            if (strcmp(token->attribs.string, 'function')){
                token->type = TT_KW_FUNCTION;
                return;
                }
            if (strcmp(token->attribs.string, 'global')){
                token->type = TT_KW_GLOBAL;
                return;
                }
            if (strcmp(token->attribs.string, 'if')){
                token->type = TT_KW_IF;
                return;
                }
            if (strcmp(token->attribs.string, 'integer')){
                token->type = TT_KW_INTEGER;
                return;
                }
            if (strcmp(token->attribs.string, 'local')){
                token->type = TT_KW_LOCAL;
                return;
                }
            if (strcmp(token->attribs.string, 'nil')){
                token->type = TT_KW_NIL;
                return;
                }
            if (strcmp(token->attribs.string, 'number')){
                token->type = TT_KW_NUMBER;
                return;
                }
            if (strcmp(token->attribs.string, 'require')){
                token->type = TT_KW_REQUIRE;
                return;
                }
            if (strcmp(token->attribs.string, 'return')){
                token->type = TT_KW_RETURN;
                return;
                }
            if (strcmp(token->attribs.string, 'string')){
                token->type = TT_KW_STRING;
                return;
                }
            if (strcmp(token->attribs.string, 'then')){
                token->type = TT_KW_THEN;
                return;
                }
            if (strcmp(token->attribs.string, 'while')){
                token->type = TT_KW_WHILE;
                return; 
                }
             /**CO KDYZ NACTU abcd: ??*//**musi byt operator oddelen mezerou? jaky typ chyby kdyz je za tim picovina */
            token->type = TT_IDENTIFIER;       //CO TU ZLE, dopici, HAST THESE AMPERSANT ODER NICHT
            return;
            }
        else {
            expand_string(&token->attribs.string, symbol);
            } 
        }
}
/**
 * @brief Load to the string numbers
 * @param nicht done dopici
*/
void number(Token *token, char symbol){
    number = atoi(symbol);
    expand_string(&token->attribs.number, number);/**/ 

    while(1){

    }

}

void get_token(Token *token) {
    init_token(&token);     /*JO ANEBO NE AMPERSANT""""????**/
    int a=1;
    char symbol = getchar();
    /*loading token, condition isnt yet
    **/
    switch (symbol) 
    {
    case ' ':
    case '\t':
    case 'EOL':
        //get_token(*token);
        /*skipped*/
        break;
    case 'EOF':
        /*KONEC, posli ze je chybicka*/
        break;
    case '-':                   /**HEY A MUZE BYT NECO JINEHO -- JAKO TREBA a-- ???????????*/
        char next_symbol = getchar();
        if (next_symbol == '-'){
            comment(&token);    /**MA TAM BYT AMPERSANT??????????**/
            get_token(&token);    //JE TO GUT?
            break;
        }
        ungetchar(next_symbol, stdin);
        token->attribs.key_word = TT_MINUS; /**NEMAM*/
        break;
    case 'a':
    case 'A':
    case 'b':
    case 'B':
    case 'c':
    case 'C':
    case 'd':
    case 'D':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    case 'g':
    case 'G':
    case 'h':
    case 'H':
    case 'i':
    case 'I':
    case 'j':
    case 'J':
    case 'k':
    case 'K':
    case 'l':
    case 'L':
    case 'm':
    case 'M':
    case 'n':
    case 'N':
    case 'o':
    case 'O':
    case 'p':
    case 'P':
    case 'q':
    case 'Q':
    case 'r':
    case 'R':
    case 's':
    case 'S':
    case 't':
    case 'T':
    case 'u':
    case 'U':
    case 'v':
    case 'V':
    case 'w':
    case 'W':
    case 'x':
    case 'X':
    case 'y':
    case 'Y':
    case 'z':
    case 'Z':
    case '_':
        alphabet(&token, symbol);
        break;
    case '0':       
    case '1':       
    case '2':       
    case '3':       
    case '4':       
    case '5':       
    case '6':       
    case '7':       
    case '8':       
    case '9':    
        number(&token, symbol);  
/***POUZE NEZAPORNA CISLA****/
    case '[':       /**nesmi tam byt??/*/

        break;
    case ']':

    default:
        break;
    }


    set_error(LEXICAL_ERR);


}





