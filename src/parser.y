%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex(void);
extern int yylineno;
void yyerror(const char *message);

SelectNode *parsed_select = NULL;

static char *string_copy(const char *text) {
    if (!text) return NULL;
    char *copy = malloc(strlen(text) + 1);
    if (copy) strcpy(copy, text);
    return copy;
}

static void add_table_entry(SelectNode *sel, const char *name, const char *alias) {
    int n = sel->table_count;
    sel->tables = realloc(sel->tables, (n + 1) * sizeof(char *));
    sel->table_aliases = realloc(sel->table_aliases, (n + 1) * sizeof(char *));
    sel->tables[n] = string_copy(name);
    sel->table_aliases[n] = alias ? string_copy(alias) : NULL;
    sel->table_count++;
}

static void add_group_entry(SelectNode *sel, const char *col) {
    int n = sel->group_count;
    sel->group_columns = realloc(sel->group_columns, (n + 1) * sizeof(char *));
    sel->group_columns[n] = string_copy(col);
    sel->group_count++;
}

static void add_order_entry(SelectNode *sel, const char *col, int is_desc) {
    int n = sel->order_count;
    sel->order_columns = realloc(sel->order_columns, (n + 1) * sizeof(char *));
    sel->order_desc = realloc(sel->order_desc, (n + 1) * sizeof(int));
    sel->order_columns[n] = string_copy(col);
    sel->order_desc[n] = is_desc;
    sel->order_count++;
}
%}

%union {
    char *text;
    int flag;
    ASTNode *node;
    ASTList list;
    JoinType join_type;
    JoinNode *join_node;
    SelectNode *select;
}

%token SELECT FROM WHERE JOIN INNER LEFT RIGHT ON AND OR NOT AS
%token ORDER BY GROUP HAVING LIMIT ASC DESC
%token EQ NE LT GT LE GE
%token INTEGER FLOAT STRING IDENTIFIER
%token COMMA SEMICOLON STAR LPAREN RPAREN DOT

%type <text> IDENTIFIER INTEGER FLOAT STRING EQ NE LT GT LE GE opt_alias
%type <flag> opt_dir opt_limit
%type <node> expression predicate value column select_item opt_where opt_having
%type <list> column_list
%type <join_type> join_type
%type <join_node> join_clause opt_joins
%type <select> select_statement

%left OR
%left AND
%right NOT

%%

input:
    select_statement optional_semicolon
    {
        parsed_select = $1;
    }
;

optional_semicolon:
    /* empty */
  | SEMICOLON
;

select_statement:
    SELECT column_list
    FROM table_list
    opt_joins
    opt_where
    opt_group_by
    opt_having
    opt_order_by
    opt_limit
    {
        /* table_list & group/order were collected into parsed_select helper */
        parsed_select->columns = $2;
        parsed_select->joins = $5;
        parsed_select->where_clause = $6;
        parsed_select->having_clause = $8;
        parsed_select->limit = $10;
        $$ = parsed_select;
    }
;

column_list:
    select_item
    {
        ast_list_init(&$$);
        ast_list_add(&$$, $1);
    }
  | column_list COMMA select_item
    {
        $$ = $1;
        ast_list_add(&$$, $3);
    }
;

select_item:
    STAR
    { $$ = ast_column(NULL, "*"); }
  | IDENTIFIER DOT STAR
    {
        $$ = ast_column($1, "*");
        free($1);
    }
  | expression opt_alias
    {
        $$ = $1;
        $$->alias = $2;
    }
;

opt_alias:
    /* empty */       { $$ = NULL; }
  | AS IDENTIFIER     { $$ = $2; }
  | IDENTIFIER        { $$ = $1; }
;

table_list:
    table_item
  | table_list COMMA table_item
;

table_item:
    IDENTIFIER opt_alias
    {
        if (!parsed_select) {
            parsed_select = calloc(1, sizeof(SelectNode));
            parsed_select->limit = -1;
        }
        add_table_entry(parsed_select, $1, $2);
        free($1);
        free($2);
    }
;

opt_joins:
    /* empty */               { $$ = NULL; }
  | opt_joins join_clause
    {
        if (!$1) {
            $$ = $2;
        } else {
            JoinNode *p = $1;
            while (p->next) p = p->next;
            p->next = $2;
            $$ = $1;
        }
    }
;

join_clause:
    join_type JOIN IDENTIFIER opt_alias ON predicate
    {
        JoinNode *j = calloc(1, sizeof(JoinNode));
        j->type = $1;
        j->table = $3;
        j->alias = $4;
        j->on_clause = $6;
        j->next = NULL;
        $$ = j;
    }
;

join_type:
    /* empty */   { $$ = JOIN_INNER; }
  | INNER         { $$ = JOIN_INNER; }
  | LEFT          { $$ = JOIN_LEFT; }
  | RIGHT         { $$ = JOIN_RIGHT; }
;

opt_where:
    /* empty */       { $$ = NULL; }
  | WHERE predicate   { $$ = $2; }
;

opt_group_by:
    /* empty */
  | GROUP BY group_list
;

group_list:
    column
    {
        add_group_entry(parsed_select, $1->text);
        ast_free($1);
    }
  | group_list COMMA column
    {
        add_group_entry(parsed_select, $3->text);
        ast_free($3);
    }
;

opt_having:
    /* empty */       { $$ = NULL; }
  | HAVING predicate  { $$ = $2; }
;

opt_order_by:
    /* empty */
  | ORDER BY order_list
;

order_list:
    order_item
  | order_list COMMA order_item
;

order_item:
    column opt_dir
    {
        add_order_entry(parsed_select, $1->text, $2);
        ast_free($1);
    }
;

opt_dir:
    /* empty */   { $$ = 0; }
  | ASC           { $$ = 0; }
  | DESC          { $$ = 1; }
;

opt_limit:
    /* empty */       { $$ = -1; }
  | LIMIT INTEGER     { $$ = atoi($2); free($2); }
;

predicate:
    expression                    { $$ = $1; }
  | predicate AND predicate       { $$ = ast_binary($1, "AND", $3); }
  | predicate OR predicate        { $$ = ast_binary($1, "OR", $3); }
  | NOT predicate                 { $$ = ast_unary("NOT", $2); }
  | LPAREN predicate RPAREN       { $$ = $2; }
  | expression EQ expression      { $$ = ast_binary($1, "=", $3); }
  | expression NE expression      { $$ = ast_binary($1, "!=", $3); }
  | expression LT expression      { $$ = ast_binary($1, "<", $3); }
  | expression GT expression      { $$ = ast_binary($1, ">", $3); }
  | expression LE expression      { $$ = ast_binary($1, "<=", $3); }
  | expression GE expression      { $$ = ast_binary($1, ">=", $3); }
;

expression:
    column    { $$ = $1; }
  | value     { $$ = $1; }
;

column:
    IDENTIFIER
    { $$ = ast_column(NULL, $1); free($1); }
  | IDENTIFIER DOT IDENTIFIER
    { $$ = ast_column($1, $3); free($1); free($3); }
;

value:
    INTEGER   { $$ = ast_new(AST_LITERAL, $1); free($1); }
  | FLOAT     { $$ = ast_new(AST_LITERAL, $1); free($1); }
  | STRING    { $$ = ast_new(AST_LITERAL, $1); free($1); }
;

%%

void yyerror(const char *message) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, message);
}