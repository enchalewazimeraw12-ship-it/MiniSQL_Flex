
%{
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap case-insensitive

%%

[ \t\r\n]+                 ;

"SELECT"                   { printf("KEYWORD: SELECT\n"); }
"FROM"                     { printf("KEYWORD: FROM\n"); }
"WHERE"                    { printf("KEYWORD: WHERE\n"); }
"JOIN"                     { printf("KEYWORD: JOIN\n"); }
"INNER"                    { printf("KEYWORD: INNER\n"); }
"LEFT"                     { printf("KEYWORD: LEFT\n"); }
"RIGHT"                    { printf("KEYWORD: RIGHT\n"); }
"ON"                       { printf("KEYWORD: ON\n"); }
"AND"                      { printf("KEYWORD: AND\n"); }
"OR"                       { printf("KEYWORD: OR\n"); }
"NOT"                      { printf("KEYWORD: NOT\n"); }
"AS"                       { printf("KEYWORD: AS\n"); }
"ORDER"                    { printf("KEYWORD: ORDER\n"); }
"BY"                       { printf("KEYWORD: BY\n"); }
"GROUP"                    { printf("KEYWORD: GROUP\n"); }
"HAVING"                   { printf("KEYWORD: HAVING\n"); }
"LIMIT"                    { printf("KEYWORD: LIMIT\n"); }
"ASC"                      { printf("KEYWORD: ASC\n"); }
"DESC"                     { printf("KEYWORD: DESC\n"); }

">="                       { printf("OPERATOR: >=\n"); }
"<="                       { printf("OPERATOR: <=\n"); }
"!="|"<>"                  { printf("OPERATOR: !=\n"); }
"="                        { printf("OPERATOR: =\n"); }
">"                        { printf("OPERATOR: >\n"); }
"<"                        { printf("OPERATOR: <\n"); }

[0-9]+"."[0-9]+            { printf("DECIMAL: %s\n", yytext); }
[0-9]+                     { printf("NUMBER: %s\n", yytext); }

\'[^'\n]*\'                { printf("STRING: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*     { printf("IDENTIFIER: %s\n", yytext); }

","                        { printf("COMMA\n"); }
";"                        { printf("SEMICOLON\n"); }
"*"                        { printf("STAR\n"); }
"("                        { printf("LEFT PARENTHESIS\n"); }
")"                        { printf("RIGHT PARENTHESIS\n"); }
"."                        { printf("DOT\n"); }

.                          { printf("UNKNOWN: %s\n", yytext); }

%%

int main(void)
{
    printf("===== MiniSQL Advanced Lexer =====\n");
    yylex();
    return 0;
}
