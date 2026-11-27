#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static char *copy_text(const char *text) {
    char *copy = malloc(text ? strlen(text) + 1 : 1);
    if (copy) strcpy(copy, text ? text : "");
    return copy;
}

ASTNode *ast_new(ASTNodeType type, const char *text) {
    ASTNode *node = calloc(1, sizeof(*node));
    if (!node) return NULL;
    node->type = type;
    node->text = copy_text(text);
    return node;
}

ASTNode *ast_binary(ASTNode *left, const char *op, ASTNode *right) {
    ASTNode *node = ast_new(AST_BINARY, op);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *ast_unary(const char *op, ASTNode *operand) {
    ASTNode *node = ast_new(AST_UNARY, op);
    node->left = operand;
    return node;
}

ASTNode *ast_column(const char *qualifier, const char *name) {
    char combined[256];
    if (qualifier && qualifier[0]) {
        snprintf(combined, sizeof(combined), "%s.%s", qualifier, name);
    } else {
        snprintf(combined, sizeof(combined), "%s", name);
    }
    return ast_new(AST_COLUMN, combined);
}

void ast_list_init(ASTList *list) {
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void ast_list_add(ASTList *list, ASTNode *node) {
    if (list->count == list->capacity) {
        list->capacity = list->capacity ? list->capacity * 2 : 8;
        list->items = realloc(list->items, list->capacity * sizeof(*list->items));
    }
    list->items[list->count++] = node;
}

void ast_print(ASTNode *node) {
    if (!node) return;
    if (node->type == AST_BINARY) {
        printf("(");
        ast_print(node->left);
        printf(" %s ", node->text);
        ast_print(node->right);
        printf(")");
    } else if (node->type == AST_UNARY) {
        printf("%s ", node->text);
        ast_print(node->left);
    } else {
        printf("%s", node->text ? node->text : "");
    }
}

void ast_print_select(const SelectNode *select) {
    if (!select) return;
    printf("\n===== ABSTRACT SYNTAX TREE (AST) =====\n");
    printf("SelectNode\n");
    printf("  SELECT: ");
    for (int i = 0; i < select->columns.count; i++) {
        ast_print(select->columns.items[i]);
        if (select->columns.items[i]->alias) {
            printf(" AS %s", select->columns.items[i]->alias);
        }
        if (i + 1 < select->columns.count) printf(", ");
    }
    printf("\n");

    printf("  FROM: ");
    for (int i = 0; i < select->table_count; i++) {
        printf("%s", select->tables[i]);
        if (select->table_aliases && select->table_aliases[i]) {
            printf(" AS %s", select->table_aliases[i]);
        }
        if (i + 1 < select->table_count) printf(", ");
    }
    printf("\n");

    for (JoinNode *j = select->joins; j; j = j->next) {
        const char *jtype = (j->type == JOIN_LEFT) ? "LEFT JOIN" :
                            (j->type == JOIN_RIGHT) ? "RIGHT JOIN" : "INNER JOIN";
        printf("  %s: %s", jtype, j->table);
        if (j->alias) printf(" AS %s", j->alias);
        printf(" ON ");
        ast_print(j->on_clause);
        printf("\n");
    }

    if (select->where_clause) {
        printf("  WHERE: ");
        ast_print(select->where_clause);
        printf("\n");
    }
    if (select->group_count > 0) {
        printf("  GROUP BY: ");
        for (int i = 0; i < select->group_count; i++) {
            printf("%s%s", select->group_columns[i], (i + 1 < select->group_count) ? ", " : "");
        }
        printf("\n");
    }
    if (select->having_clause) {
        printf("  HAVING: ");
        ast_print(select->having_clause);
        printf("\n");
    }
    if (select->order_count > 0) {
        printf("  ORDER BY: ");
        for (int i = 0; i < select->order_count; i++) {
            printf("%s %s%s",
                   select->order_columns[i],
                   (select->order_desc && select->order_desc[i]) ? "DESC" : "ASC",
                   (i + 1 < select->order_count) ? ", " : "");
        }
        printf("\n");
    }
    if (select->limit >= 0) {
        printf("  LIMIT: %d\n", select->limit);
    }
}

void ast_free(ASTNode *node) {
    if (!node) return;
    ast_free(node->left);
    ast_free(node->right);
    free(node->text);
    free(node->alias);
    free(node);
}

void ast_select_free(SelectNode *select) {
    if (!select) return;
    for (int i = 0; i < select->columns.count; i++) {
        ast_free(select->columns.items[i]);
    }
    free(select->columns.items);
    for (int i = 0; i < select->table_count; i++) {
        free(select->tables[i]);
        if (select->table_aliases) free(select->table_aliases[i]);
    }
    free(select->tables);
    free(select->table_aliases);
    JoinNode *j = select->joins;
    while (j) {
        JoinNode *next = j->next;
        free(j->table);
        free(j->alias);
        ast_free(j->on_clause);
        free(j);
        j = next;
    }
    ast_free(select->where_clause);
    ast_free(select->having_clause);
    for (int i = 0; i < select->group_count; i++) free(select->group_columns[i]);
    free(select->group_columns);
    for (int i = 0; i < select->order_count; i++) free(select->order_columns[i]);
    free(select->order_columns);
    free(select->order_desc);
    free(select);
}