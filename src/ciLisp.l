%option noyywrap
%option nounput
%option noinput

%{
    #include "ciLisp.h"
    #define FLEX_BISON_LOG_PATH "../logs/flex_bison.log"
    #define llog(token) fprintf(flex_bison_log_file, "LEX: %s \"%s\"\n", #token, yytext);
%}

digit [0-9]
int [+-]?{digit}+
dbl [+-]?{digit}+\.{digit}+
let "let"
type "int"|"double"
cond "cond"
func "neg"|"abs"|"add"|"sub"|"mult"|"div"|"remainder"|"exp"|"exp2"|"pow"|"log"|"sqrt"|"cbrt"|"hypot"|"max"|"min"|"print"|"read"|"equal"|"less"|"greater"|"rand"|"equal"|"less"|"greater"
sym [a-zA-Z]+


%%

"quit" {
    return QUIT;
    }

{int} {
    llog(INT);
    yylval.dval = strtod(yytext, NULL);
    return INT;
    }

{dbl} {
    llog(DOUBLE);
    yylval.dval = strtod(yytext, NULL);
    return DOUBLE;
    }

{func} {
    llog(FUNC);
    yylval.sval = (char *) calloc(sizeof(char), strlen(yytext) + 1);
    strcpy(yylval.sval, yytext);
    return FUNC;
    }

{let} {
    llog(LET);
    return LET;
}

{cond} {
    llog(COND);
    return COND;
    }

{type} {
    llog(TYPE);
    yylval.sval = (char *) calloc(sizeof(char), strlen(yytext) + 1);
    strcpy(yylval.sval, yytext);
    return TYPE;
}



{sym} {
    llog(SYMBOL);
    yylval.sval = (char *) calloc(sizeof(char), strlen(yytext) + 1);
    strcpy(yylval.sval, yytext);
    return SYMBOL;
}

\(  {
    llog(LPAREN)
    return LPAREN;
    }

\)  {
    llog(RPAREN)
    return RPAREN;
    }

[\n] {
    llog(EOL);
    return EOL;
    }

<<EOF>>  {
    llog(EOFT);
    return EOFT;
    }

[ \t\v\r] ; /* skip whitespace */


. { // anything else
    llog(INVALID);
    printf("ERROR: invalid character: >>%s<<\n", yytext);
    }

%%

/*
 * The code below should NOT be edited except to comment or uncomment
 * the "freopen" call in the main.
 */

#include <stdio.h>

int main(int argc, char **argv)
{
    int input_from_file = 0;
    flex_bison_log_file = fopen(FLEX_BISON_LOG_PATH, "w");
    time_t t;
    srand((unsigned) time(&t));
    // Comment the line below to type inputs directly into the console.
    // Otherwise, they will be read from the file specified in the first program argument.
    freopen(argv[1], "r", stdin); input_from_file = 1;

    while(1) {
        if (!input_from_file)
            printf("> ");
        if (yyparse()) {
            YY_FLUSH_BUFFER;
        }
    }
}
