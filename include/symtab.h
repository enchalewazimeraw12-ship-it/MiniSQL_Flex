#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

typedef enum {
    TYPE_UNKNOWN,
    TYPE_INT,
    TYPE_TEXT,
    TYPE_FLOAT,
    TYPE_BOOL
} MiniSQLType;

typedef struct {
    char *name;
    MiniSQLType type;
} ColumnDef;

typedef struct {
    char *name;
    ColumnDef *columns;
    int column_count;
} TableDef;

void symtab_init(void);
int symtab_load_csv(const char *table_name, const char *csv_path);
int symtab_validate_select(SelectNode *select);
MiniSQLType symtab_resolve_column(const char *qualifier, const char *name);
const char *symtab_type_name(MiniSQLType type);
void symtab_print(void);
void symtab_free(void);

#endif