%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.h"
%}

%option noyywrap case-insensitive
%option yylineno
%option nounput noinput
%%

[ \t\r\n]+                ;
"--".*                    ;
"/*"([^*]|"*"+[^*/])*"*"+"/" ;

"SELECT"                  return SELECT;
"FROM"                    return FROM;
"WHERE"                   return WHERE;
"JOIN"                    return JOIN;
"INNER"                   return INNER;
"LEFT"                    return LEFT;
"RIGHT"                   return RIGHT;
"ON"                      return ON;
"AND"                     return AND;
"OR"                      return OR;
"NOT"                     return NOT;
"AS"                      return AS;
"ORDER"                   return ORDER;
"BY"                      return BY;
"GROUP"                   return GROUP;
"HAVING"                  return HAVING;
"LIMIT"                   return LIMIT;
"ASC"                     return ASC;
"DESC"                    return DESC;

"="                       { yylval.text = strdup(yytext); return EQ; }
"!="|"<>"                 { yylval.text = strdup(yytext); return NE; }
"<="                      { yylval.text = strdup(yytext); return LE; }
">="                      { yylval.text = strdup(yytext); return GE; }
"<"                       { yylval.text = strdup(yytext); return LT; }
">"                       { yylval.text = strdup(yytext); return GT; }

[0-9]+"."[0-9]+           { yylval.text = strdup(yytext); return FLOAT; }
[0-9]+                    { yylval.text = strdup(yytext); return INTEGER; }
\'([^\\\']|\\.)*\'        { yylval.text = strdup(yytext); return STRING; }
[a-zA-Z_][a-zA-Z0-9_]*    { yylval.text = strdup(yytext); return IDENTIFIER; }

","                       return COMMA;
";"                       return SEMICOLON;
"*"                       return STAR;
"("                       return LPAREN;
")"                       return RPAREN;
"."                       return DOT;

.                         {
                              fprintf(stderr, "Lexical Error (line %d): Unexpected character '%s'\n", yylineno, yytext);
                              return yytext[0];
                          }

%%