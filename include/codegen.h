#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

typedef enum {
    TAC_LOAD,
    TAC_JOIN,
    TAC_FILTER,
    TAC_GROUP,
    TAC_HAVING,
    TAC_PROJECT,
    TAC_SORT,
    TAC_LIMIT,
    TAC_RESULT
} TACOpcode;

typedef struct {
    TACOpcode op;
    char *target;
    char *arg1;
    char *arg2;
} TACInstruction;

typedef struct {
    TACInstruction *items;
    int count;
    int capacity;
    SelectNode *select;
} TACPlan;

TACPlan *codegen_create(SelectNode *select);
int codegen_execute(TACPlan *plan);
void codegen_print(TACPlan *plan);
void codegen_free(TACPlan *plan);

#endif