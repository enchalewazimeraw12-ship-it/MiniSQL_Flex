#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "ast.h"
#include "parser.h"
#include "symtab.h"
#include "codegen.h"

extern FILE *yyin;
extern int yyparse(void);
extern SelectNode *parsed_select;

static void load_all_csvs(const char *dir_path) {
    DIR *d = opendir(dir_path);
    if (!d) return;
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        size_t len = strlen(entry->d_name);
        if (len > 4 && strcmp(entry->d_name + len - 4, ".csv") == 0) {
            char tname[128], path[512];
            size_t copy_len = (len - 4 < sizeof(tname) - 1) ? (len - 4) : (sizeof(tname) - 1);
            memcpy(tname, entry->d_name, copy_len);
            tname[copy_len] = '\0';
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            symtab_load_csv(tname, path);
        }
    }
    closedir(d);
}

int main(int argc, char **argv) {
    printf("===============================================\n");
    printf("  MiniSQL Relational Query Compiler (Group 8)  \n");
    printf("===============================================\n");

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Cannot open SQL file: %s\n", argv[1]);
            return 1;
        }
        printf("SQL File: %s\n", argv[1]);
    } else {
        printf("Enter SQL query (finish with Ctrl+D / Ctrl+Z + Enter):\n");
        yyin = stdin;
    }

    symtab_init();
    load_all_csvs("data");

    if (yyparse() != 0 || !parsed_select) {
        fprintf(stderr, "Compilation failed during Syntax Analysis.\n");
        if (argc > 1) fclose(yyin);
        symtab_free();
        return 1;
    }
    if (argc > 1) fclose(yyin);

    /* Step 2: Print AST */
    ast_print_select(parsed_select);

    /* Step 3: Symbol Table & Semantic Validation */
    symtab_print();
    if (!symtab_validate_select(parsed_select)) {
        fprintf(stderr, "Compilation aborted due to Semantic Errors.\n");
        ast_select_free(parsed_select);
        symtab_free();
        return 1;
    }

    /* Step 4: Execution Plan (TAC) & Query Execution */
    TACPlan *plan = codegen_create(parsed_select);
    codegen_print(plan);
    codegen_execute(plan);

    codegen_free(plan);
    ast_select_free(parsed_select);
    symtab_free();
    return 0;
}