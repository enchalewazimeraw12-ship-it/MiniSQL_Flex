#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtab.h"

static TableDef *tables;
static int table_count;
static int table_capacity;

static char *copy_string(const char *text) {
    char *copy = malloc(text ? strlen(text) + 1 : 1);
    if (copy) strcpy(copy, text ? text : "");
    return copy;
}

static int iequals(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static void trim(char *text) {
    char *p = text + strlen(text);
    while (p > text && isspace((unsigned char)p[-1])) *--p = '\0';
    p = text;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != text) memmove(text, p, strlen(p) + 1);
}

static MiniSQLType infer_type(const char *value) {
    if (!value || !*value) return TYPE_TEXT;
    char *end;
    strtol(value, &end, 10);
    if (*value && *end == '\0') return TYPE_INT;
    strtod(value, &end);
    if (*value && *end == '\0') return TYPE_FLOAT;
    return TYPE_TEXT;
}

const char *symtab_type_name(MiniSQLType type) {
    switch (type) {
        case TYPE_INT:   return "INT";
        case TYPE_FLOAT: return "FLOAT";
        case TYPE_TEXT:  return "TEXT";
        case TYPE_BOOL:  return "BOOL";
        default:         return "UNKNOWN";
    }
}

void symtab_init(void) {
    tables = NULL;
    table_count = 0;
    table_capacity = 0;
}

int symtab_load_csv(const char *table_name, const char *csv_path) {
    FILE *file = fopen(csv_path, "r");
    if (!file) {
        fprintf(stderr, "Cannot open table '%s': %s\n", table_name, csv_path);
        return 0;
    }
    char line[1024];
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }
    char *headers[256];
    int column_count = 0;
    char *token = strtok(line, ",\r\n");
    while (token && column_count < 256) {
        trim(token);
        headers[column_count++] = copy_string(token);
        token = strtok(NULL, ",\r\n");
    }

    char first_row[1024] = "";
    if (!fgets(first_row, sizeof(first_row), file)) {
        first_row[0] = '\0';
    }
    fclose(file);

    if (table_count == table_capacity) {
        table_capacity = table_capacity ? table_capacity * 2 : 4;
        tables = realloc(tables, table_capacity * sizeof(*tables));
    }
    TableDef *table = &tables[table_count++];
    table->name = copy_string(table_name);
    table->column_count = column_count;
    table->columns = calloc(column_count, sizeof(ColumnDef));

    char *values = first_row[0] ? strtok(first_row, ",\r\n") : NULL;
    int i = 0;
    while (values && i < column_count) {
        trim(values);
        table->columns[i].name = headers[i];
        table->columns[i].type = infer_type(values);
        i++;
        values = strtok(NULL, ",\r\n");
    }
    for (; i < column_count; i++) {
        table->columns[i].name = headers[i];
        table->columns[i].type = TYPE_TEXT;
    }
    return 1;
}

static TableDef *find_table_def(const char *name) {
    for (int i = 0; i < table_count; i++) {
        if (iequals(tables[i].name, name)) return &tables[i];
    }
    return NULL;
}

MiniSQLType symtab_resolve_column(const char *qualifier, const char *name) {
    for (int i = 0; i < table_count; i++) {
        if (qualifier && qualifier[0] && !iequals(tables[i].name, qualifier))
            continue;
        for (int j = 0; j < tables[i].column_count; j++) {
            if (iequals(tables[i].columns[j].name, name))
                return tables[i].columns[j].type;
        }
    }
    return TYPE_UNKNOWN;
}

/* ---------- Scope & Type Validation Helpers ---------- */

typedef struct {
    const char *alias;
    TableDef *def;
} ScopeEntry;

static void split_col_name(const char *text, char *qualifier, size_t qsz, char *name, size_t nsz) {
    const char *dot = strchr(text, '.');
    if (dot) {
        size_t len = (size_t)(dot - text);
        if (len >= qsz) len = qsz - 1;
        memcpy(qualifier, text, len);
        qualifier[len] = '\0';
        snprintf(name, nsz, "%s", dot + 1);
    } else {
        qualifier[0] = '\0';
        snprintf(name, nsz, "%s", text);
    }
}

static MiniSQLType resolve_in_scope(const char *col_text, ScopeEntry *scopes, int scope_count, int *valid) {
    char qualifier[128], name[128];
    split_col_name(col_text, qualifier, sizeof(qualifier), name, sizeof(name));

    if (qualifier[0]) {
        for (int i = 0; i < scope_count; i++) {
            if (iequals(scopes[i].alias, qualifier) || iequals(scopes[i].def->name, qualifier)) {
                if (strcmp(name, "*") == 0) return TYPE_UNKNOWN;
                for (int j = 0; j < scopes[i].def->column_count; j++) {
                    if (iequals(scopes[i].def->columns[j].name, name))
                        return scopes[i].def->columns[j].type;
                }
                fprintf(stderr, "Semantic Error: Unknown column '%s' in table '%s'\n", name, qualifier);
                *valid = 0;
                return TYPE_UNKNOWN;
            }
        }
        fprintf(stderr, "Semantic Error: Table/alias '%s' is not in scope\n", qualifier);
        *valid = 0;
        return TYPE_UNKNOWN;
    }

    if (strcmp(name, "*") == 0) return TYPE_UNKNOWN;

    int matches = 0;
    MiniSQLType found_type = TYPE_UNKNOWN;
    for (int i = 0; i < scope_count; i++) {
        for (int j = 0; j < scopes[i].def->column_count; j++) {
            if (iequals(scopes[i].def->columns[j].name, name)) {
                matches++;
                found_type = scopes[i].def->columns[j].type;
            }
        }
    }
    if (matches == 0) {
        fprintf(stderr, "Semantic Error: Unknown column '%s'\n", col_text);
        *valid = 0;
        return TYPE_UNKNOWN;
    }
    if (matches > 1) {
        fprintf(stderr, "Semantic Error: Column '%s' is ambiguous; specify table.column\n", col_text);
        *valid = 0;
        return TYPE_UNKNOWN;
    }
    return found_type;
}

static int is_numeric_type(MiniSQLType t) {
    return t == TYPE_INT || t == TYPE_FLOAT;
}

static MiniSQLType validate_expr(ASTNode *node, ScopeEntry *scopes, int scope_count, int *valid) {
    if (!node) return TYPE_UNKNOWN;

    switch (node->type) {
        case AST_COLUMN:
            return resolve_in_scope(node->text, scopes, scope_count, valid);

        case AST_LITERAL:
            if (node->text && node->text[0] == '\'') return TYPE_TEXT;
            if (node->text && strchr(node->text, '.')) return TYPE_FLOAT;
            return TYPE_INT;

        case AST_UNARY: {
            MiniSQLType t = validate_expr(node->left, scopes, scope_count, valid);
            if (t != TYPE_BOOL && t != TYPE_UNKNOWN) {
                fprintf(stderr, "Semantic Error: NOT operator requires a boolean condition\n");
                *valid = 0;
            }
            return TYPE_BOOL;
        }

        case AST_BINARY: {
            MiniSQLType lt = validate_expr(node->left, scopes, scope_count, valid);
            MiniSQLType rt = validate_expr(node->right, scopes, scope_count, valid);
            if (lt == TYPE_UNKNOWN || rt == TYPE_UNKNOWN) return TYPE_BOOL;

            if (iequals(node->text, "AND") || iequals(node->text, "OR")) {
                if (lt != TYPE_BOOL || rt != TYPE_BOOL) {
                    fprintf(stderr, "Semantic Error: Logical operator '%s' requires boolean operands\n", node->text);
                    *valid = 0;
                }
                return TYPE_BOOL;
            }

            if (is_numeric_type(lt) && is_numeric_type(rt)) return TYPE_BOOL;
            if (lt == rt) return TYPE_BOOL;

            fprintf(stderr, "Semantic Error: Type mismatch in '%s' (%s vs %s)\n",
                    node->text, symtab_type_name(lt), symtab_type_name(rt));
            *valid = 0;
            return TYPE_BOOL;
        }

        default:
            return TYPE_UNKNOWN;
    }
}

int symtab_validate_select(SelectNode *select) {
    if (!select) return 0;
    int valid = 1;
    ScopeEntry scopes[64];
    int scope_count = 0;

    /* 1. Validate FROM tables & register scope */
    for (int t = 0; t < select->table_count; t++) {
        TableDef *def = find_table_def(select->tables[t]);
        if (!def) {
            fprintf(stderr, "Semantic Error: Unknown table '%s'\n", select->tables[t]);
            valid = 0;
        } else if (scope_count < 64) {
            const char *al = (select->table_aliases && select->table_aliases[t])
                             ? select->table_aliases[t] : select->tables[t];
            scopes[scope_count].alias = al;
            scopes[scope_count].def = def;
            scope_count++;
        }
    }

    /* 2. Validate JOIN tables & ON conditions */
    for (JoinNode *j = select->joins; j; j = j->next) {
        TableDef *def = find_table_def(j->table);
        if (!def) {
            fprintf(stderr, "Semantic Error: Unknown joined table '%s'\n", j->table);
            valid = 0;
        } else if (scope_count < 64) {
            const char *al = j->alias ? j->alias : j->table;
            for (int i = 0; i < scope_count; i++) {
                if (iequals(scopes[i].alias, al)) {
                    fprintf(stderr, "Semantic Error: Duplicate table/alias '%s'\n", al);
                    valid = 0;
                }
            }
            scopes[scope_count].alias = al;
            scopes[scope_count].def = def;
            scope_count++;
        }
        if (j->on_clause) {
            MiniSQLType ont = validate_expr(j->on_clause, scopes, scope_count, &valid);
            if (ont != TYPE_BOOL && ont != TYPE_UNKNOWN) {
                fprintf(stderr, "Semantic Error: JOIN ON clause must be a boolean condition\n");
                valid = 0;
            }
        }
    }

    /* 3. Validate SELECT columns */
    for (int i = 0; i < select->columns.count; i++) {
        validate_expr(select->columns.items[i], scopes, scope_count, &valid);
    }

    /* 4. Validate WHERE & HAVING */
    if (select->where_clause) {
        MiniSQLType wt = validate_expr(select->where_clause, scopes, scope_count, &valid);
        if (wt != TYPE_BOOL && wt != TYPE_UNKNOWN) {
            fprintf(stderr, "Semantic Error: WHERE clause must evaluate to boolean\n");
            valid = 0;
        }
    }
    if (select->having_clause) {
        MiniSQLType ht = validate_expr(select->having_clause, scopes, scope_count, &valid);
        if (ht != TYPE_BOOL && ht != TYPE_UNKNOWN) {
            fprintf(stderr, "Semantic Error: HAVING clause must evaluate to boolean\n");
            valid = 0;
        }
    }

    /* 5. Validate GROUP BY & ORDER BY columns */
    for (int i = 0; i < select->group_count; i++) {
        resolve_in_scope(select->group_columns[i], scopes, scope_count, &valid);
    }
    for (int i = 0; i < select->order_count; i++) {
        resolve_in_scope(select->order_columns[i], scopes, scope_count, &valid);
    }

    if (valid) {
        printf("\n[Semantic Analysis] PASSED (0 errors)\n");
    }
    return valid;
}

void symtab_print(void) {
    printf("\n===== SYMBOL TABLE =====\n");
    for (int i = 0; i < table_count; i++) {
        printf("TABLE %s:\n", tables[i].name);
        for (int j = 0; j < tables[i].column_count; j++) {
            printf("  - %-15s : %s\n",
                   tables[i].columns[j].name,
                   symtab_type_name(tables[i].columns[j].type));
        }
    }
}

void symtab_free(void) {
    for (int i = 0; i < table_count; i++) {
        free(tables[i].name);
        for (int j = 0; j < tables[i].column_count; j++)
            free(tables[i].columns[j].name);
        free(tables[i].columns);
    }
    free(tables);
    tables = NULL;
    table_count = 0;
    table_capacity = 0;
}