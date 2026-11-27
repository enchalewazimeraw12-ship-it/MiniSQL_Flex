#ifndef AST_H
#define AST_H

typedef enum {
    AST_SELECT,
    AST_COLUMN,
    AST_LITERAL,
    AST_BINARY,
    AST_UNARY,
    AST_TABLE
} ASTNodeType;

typedef enum {
    JOIN_INNER,
    JOIN_LEFT,
    JOIN_RIGHT
} JoinType;

typedef struct ASTNode {
    ASTNodeType type;
    char *text;
    char *alias;
    double number;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

typedef struct {
    ASTNode **items;
    int count;
    int capacity;
} ASTList;

typedef struct JoinNode {
    JoinType type;
    char *table;
    char *alias;
    ASTNode *on_clause;
    struct JoinNode *next;
} JoinNode;

typedef struct {
    ASTList columns;
    char **tables;
    char **table_aliases;
    int table_count;
    JoinNode *joins;
    ASTNode *where_clause;
    ASTNode *having_clause;
    char **group_columns;
    int group_count;
    char **order_columns;
    int *order_desc;
    int order_count;
    int limit;
} SelectNode;

ASTNode *ast_new(ASTNodeType type, const char *text);
ASTNode *ast_binary(ASTNode *left, const char *op, ASTNode *right);
ASTNode *ast_unary(const char *op, ASTNode *operand);
ASTNode *ast_column(const char *qualifier, const char *name);
void ast_list_init(ASTList *list);
void ast_list_add(ASTList *list, ASTNode *node);
void ast_print(ASTNode *node);
void ast_print_select(const SelectNode *select);
void ast_free(ASTNode *node);
void ast_select_free(SelectNode *select);

#endif