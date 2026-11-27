#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"

#define MAX_EXEC_COLS 64
#define MAX_EXEC_ROWS 256

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

static void format_expr_str(const ASTNode *node, char *buf, size_t sz) {
    if (!node || sz == 0) return;
    if (node->type == AST_BINARY) {
        char lbuf[256] = "", rbuf[256] = "";
        format_expr_str(node->left, lbuf, sizeof(lbuf));
        format_expr_str(node->right, rbuf, sizeof(rbuf));
        snprintf(buf, sz, "(%s %s %s)", lbuf, node->text, rbuf);
    } else if (node->type == AST_UNARY) {
        char lbuf[256] = "";
        format_expr_str(node->left, lbuf, sizeof(lbuf));
        snprintf(buf, sz, "%s %s", node->text, lbuf);
    } else {
        snprintf(buf, sz, "%s", node->text ? node->text : "");
    }
}

static void add_tac(TACPlan *plan, TACOpcode op, const char *target, const char *arg1, const char *arg2) {
    if (plan->count == plan->capacity) {
        plan->capacity = plan->capacity ? plan->capacity * 2 : 12;
        plan->items = realloc(plan->items, plan->capacity * sizeof(*plan->items));
    }
    plan->items[plan->count].op = op;
    plan->items[plan->count].target = target ? copy_string(target) : NULL;
    plan->items[plan->count].arg1 = arg1 ? copy_string(arg1) : NULL;
    plan->items[plan->count].arg2 = arg2 ? copy_string(arg2) : NULL;
    plan->count++;
}

TACPlan *codegen_create(SelectNode *select) {
    TACPlan *plan = calloc(1, sizeof(*plan));
    plan->select = select;

    int reg = 1;
    char curr_t[32] = "t0";

    if (select->table_count > 0) {
        char tname[32], desc[128];
        snprintf(tname, sizeof(tname), "t%d", reg++);
        if (select->table_aliases && select->table_aliases[0])
            snprintf(desc, sizeof(desc), "%s AS %s", select->tables[0], select->table_aliases[0]);
        else
            snprintf(desc, sizeof(desc), "%s", select->tables[0]);
        add_tac(plan, TAC_LOAD, tname, desc, NULL);
        strcpy(curr_t, tname);
    }

    for (JoinNode *j = select->joins; j; j = j->next) {
        char rt[32], jt[32], rdesc[128], cond[256], jop[256];
        snprintf(rt, sizeof(rt), "t%d", reg++);
        if (j->alias)
            snprintf(rdesc, sizeof(rdesc), "%s AS %s", j->table, j->alias);
        else
            snprintf(rdesc, sizeof(rdesc), "%s", j->table);
        add_tac(plan, TAC_LOAD, rt, rdesc, NULL);

        snprintf(jt, sizeof(jt), "t%d", reg++);
        format_expr_str(j->on_clause, cond, sizeof(cond));
        const char *jkind = (j->type == JOIN_LEFT) ? "LEFT_JOIN" :
                            (j->type == JOIN_RIGHT) ? "RIGHT_JOIN" : "INNER_JOIN";
        snprintf(jop, sizeof(jop), "%s(%s, %s)", jkind, curr_t, rt);
        add_tac(plan, TAC_JOIN, jt, jop, cond);
        strcpy(curr_t, jt);
    }

    if (select->where_clause) {
        char wt[32], cond[256];
        snprintf(wt, sizeof(wt), "t%d", reg++);
        format_expr_str(select->where_clause, cond, sizeof(cond));
        add_tac(plan, TAC_FILTER, wt, curr_t, cond);
        strcpy(curr_t, wt);
    }

    if (select->group_count > 0) {
        char gt[32], gcols[256] = "";
        snprintf(gt, sizeof(gt), "t%d", reg++);
        for (int i = 0; i < select->group_count; i++) {
            if (i > 0) strncat(gcols, ", ", sizeof(gcols) - strlen(gcols) - 1);
            strncat(gcols, select->group_columns[i], sizeof(gcols) - strlen(gcols) - 1);
        }
        add_tac(plan, TAC_GROUP, gt, curr_t, gcols);
        strcpy(curr_t, gt);
    }

    if (select->having_clause) {
        char ht[32], cond[256];
        snprintf(ht, sizeof(ht), "t%d", reg++);
        format_expr_str(select->having_clause, cond, sizeof(cond));
        add_tac(plan, TAC_HAVING, ht, curr_t, cond);
        strcpy(curr_t, ht);
    }

    {
        char pt[32], cols[512] = "";
        snprintf(pt, sizeof(pt), "t%d", reg++);
        for (int i = 0; i < select->columns.count; i++) {
            if (i > 0) strncat(cols, ", ", sizeof(cols) - strlen(cols) - 1);
            strncat(cols, select->columns.items[i]->text, sizeof(cols) - strlen(cols) - 1);
        }
        add_tac(plan, TAC_PROJECT, pt, curr_t, cols);
        strcpy(curr_t, pt);
    }

    if (select->order_count > 0) {
        char ot[32], ocols[256] = "";
        snprintf(ot, sizeof(ot), "t%d", reg++);
        for (int i = 0; i < select->order_count; i++) {
            if (i > 0) strncat(ocols, ", ", sizeof(ocols) - strlen(ocols) - 1);
            strncat(ocols, select->order_columns[i], sizeof(ocols) - strlen(ocols) - 1);
            strncat(ocols, (select->order_desc && select->order_desc[i]) ? " DESC" : " ASC",
                    sizeof(ocols) - strlen(ocols) - 1);
        }
        add_tac(plan, TAC_SORT, ot, curr_t, ocols);
        strcpy(curr_t, ot);
    }

    if (select->limit >= 0) {
        char lt[32], lval[32];
        snprintf(lt, sizeof(lt), "t%d", reg++);
        snprintf(lval, sizeof(lval), "%d", select->limit);
        add_tac(plan, TAC_LIMIT, lt, curr_t, lval);
        strcpy(curr_t, lt);
    }

    add_tac(plan, TAC_RESULT, NULL, curr_t, NULL);
    return plan;
}

void codegen_print(TACPlan *plan) {
    static const char *names[] = {
        "SCAN", "JOIN", "FILTER", "GROUP_BY", "HAVING", "PROJECT", "SORT", "LIMIT", "RETURN"
    };
    printf("\n===== GENERATED EXECUTION PLAN (TAC) =====\n");
    for (int i = 0; i < plan->count; i++) {
        TACInstruction *ins = &plan->items[i];
        if (ins->op == TAC_RESULT) {
            printf("%d: %s %s\n", i + 1, names[ins->op], ins->arg1);
        } else if (ins->op == TAC_JOIN) {
            printf("%d: %s = %s ON %s\n", i + 1, ins->target, ins->arg1, ins->arg2);
        } else if (ins->arg2) {
            printf("%d: %s = %s %s [%s]\n", i + 1, ins->target, names[ins->op], ins->arg1, ins->arg2);
        } else {
            printf("%d: %s = %s %s\n", i + 1, ins->target, names[ins->op], ins->arg1);
        }
    }
}

/* ---------- Execution Engine ---------- */

typedef struct {
    char col_names[MAX_EXEC_COLS][64];
    char table_names[MAX_EXEC_COLS][64];
    char aliases[MAX_EXEC_COLS][64];
    int col_count;
    char rows[MAX_EXEC_ROWS][MAX_EXEC_COLS][128];
    int row_count;
} ResultTable;

static char *trim(char *text) {
    while (*text == ' ' || *text == '\t') text++;
    char *end = text + strlen(text);
    while (end > text && (end[-1] == ' ' || end[-1] == '\r' || end[-1] == '\n'))
        *--end = '\0';
    return text;
}

static int split_csv(char *line, char **fields, int capacity) {
    int count = 0;
    char *start = line;
    for (char *p = line; ; p++) {
        if (*p == ',' || *p == '\0') {
            char saved = *p;
            *p = '\0';
            if (count < capacity) fields[count++] = trim(start);
            if (saved == '\0') break;
            start = p + 1;
        }
    }
    return count;
}

static ResultTable *load_table_rows(const char *table_name, const char *alias) {
    char path[512];
    snprintf(path, sizeof(path), "data/%s.csv", table_name);
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Runtime error: cannot open %s\n", path);
        return NULL;
    }

    ResultTable *rt = calloc(1, sizeof(ResultTable));
    char line[2048];
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        free(rt);
        return NULL;
    }

    char *headers[MAX_EXEC_COLS];
    rt->col_count = split_csv(line, headers, MAX_EXEC_COLS);
    for (int i = 0; i < rt->col_count; i++) {
        snprintf(rt->col_names[i], 64, "%s", headers[i]);
        snprintf(rt->table_names[i], 64, "%s", table_name);
        snprintf(rt->aliases[i], 64, "%s", alias ? alias : table_name);
    }

    while (fgets(line, sizeof(line), file) && rt->row_count < MAX_EXEC_ROWS) {
        if (trim(line)[0] == '\0') continue;
        char *fields[MAX_EXEC_COLS];
        int fc = split_csv(line, fields, MAX_EXEC_COLS);
        if (fc != rt->col_count) continue;
        for (int i = 0; i < rt->col_count; i++) {
            snprintf(rt->rows[rt->row_count][i], 128, "%s", fields[i]);
        }
        rt->row_count++;
    }

    fclose(file);
    return rt;
}

static int resolve_col_index(const ResultTable *rt, const char *col_text) {
    char qual[64] = "", name[64] = "";
    const char *dot = strchr(col_text, '.');
    if (dot) {
        size_t qlen = (size_t)(dot - col_text);
        if (qlen >= sizeof(qual)) qlen = sizeof(qual) - 1;
        memcpy(qual, col_text, qlen);
        qual[qlen] = '\0';
        snprintf(name, sizeof(name), "%s", dot + 1);
    } else {
        snprintf(name, sizeof(name), "%s", col_text);
    }

    for (int i = 0; i < rt->col_count; i++) {
        if (iequals(rt->col_names[i], name)) {
            if (!qual[0] || iequals(rt->table_names[i], qual) || iequals(rt->aliases[i], qual))
                return i;
        }
    }
    return -1;
}

static int parse_number(const char *s, double *out) {
    if (!s || !*s) return 0;
    char *end = NULL;
    double val = strtod(s, &end);
    if (end != s && *end == '\0') {
        if (out) *out = val;
        return 1;
    }
    return 0;
}

typedef struct {
    int is_null;
    int is_num;
    double num;
    char str[128];
} EvalVal;

static EvalVal eval_operand(const ASTNode *node, const ResultTable *rt, int row) {
    EvalVal v;
    memset(&v, 0, sizeof(v));
    if (!node || !node->text) {
        v.is_null = 1;
        return v;
    }

    if (node->type == AST_COLUMN) {
        int idx = resolve_col_index(rt, node->text);
        if (idx < 0 || strcmp(rt->rows[row][idx], "NULL") == 0) {
            v.is_null = 1;
            return v;
        }
        snprintf(v.str, sizeof(v.str), "%s", rt->rows[row][idx]);
        v.is_num = parse_number(v.str, &v.num);
        return v;
    }

    /* AST_LITERAL */
    const char *raw = node->text;
    size_t len = strlen(raw);
    if (len >= 2 && raw[0] == '\'' && raw[len - 1] == '\'') {
        snprintf(v.str, sizeof(v.str), "%.*s", (int)(len - 2), raw + 1);
        v.is_num = 0;
    } else {
        snprintf(v.str, sizeof(v.str), "%s", raw);
        v.is_num = parse_number(v.str, &v.num);
    }
    return v;
}

static int compare_eval_vals(EvalVal a, EvalVal b, const char *op) {
    if (a.is_null || b.is_null) return 0;
    double cmp = 0;
    if (a.is_num && b.is_num) {
        cmp = a.num - b.num;
    } else {
        cmp = strcmp(a.str, b.str);
    }
    if (strcmp(op, "=") == 0)  return cmp == 0;
    if (strcmp(op, "!=") == 0) return cmp != 0;
    if (strcmp(op, "<") == 0)  return cmp < 0;
    if (strcmp(op, "<=") == 0) return cmp <= 0;
    if (strcmp(op, ">") == 0)  return cmp > 0;
    if (strcmp(op, ">=") == 0) return cmp >= 0;
    return 0;
}

static int evaluate_expr(const ASTNode *node, const ResultTable *rt, int row) {
    if (!node) return 1;
    if (node->type == AST_UNARY) {
        return !evaluate_expr(node->left, rt, row);
    }
    if (node->type == AST_BINARY) {
        if (iequals(node->text, "AND"))
            return evaluate_expr(node->left, rt, row) && evaluate_expr(node->right, rt, row);
        if (iequals(node->text, "OR"))
            return evaluate_expr(node->left, rt, row) || evaluate_expr(node->right, rt, row);
        EvalVal lv = eval_operand(node->left, rt, row);
        EvalVal rv = eval_operand(node->right, rt, row);
        return compare_eval_vals(lv, rv, node->text);
    }
    return 1;
}

int codegen_execute(TACPlan *plan) {
    SelectNode *select = plan->select;
    if (!select || select->table_count == 0) return 0;

    const char *base_alias = (select->table_aliases && select->table_aliases[0])
                             ? select->table_aliases[0] : select->tables[0];
    ResultTable *curr = load_table_rows(select->tables[0], base_alias);
    if (!curr) return 0;

    /* Execute JOINs */
    for (JoinNode *j = select->joins; j; j = j->next) {
        ResultTable *right = load_table_rows(j->table, j->alias ? j->alias : j->table);
        if (!right) { free(curr); return 0; }

        ResultTable *next = calloc(1, sizeof(ResultTable));
        for (int c = 0; c < curr->col_count; c++) {
            strcpy(next->col_names[c], curr->col_names[c]);
            strcpy(next->table_names[c], curr->table_names[c]);
            strcpy(next->aliases[c], curr->aliases[c]);
        }
        for (int c = 0; c < right->col_count; c++) {
            int idx = curr->col_count + c;
            if (idx >= MAX_EXEC_COLS) break;
            strcpy(next->col_names[idx], right->col_names[c]);
            strcpy(next->table_names[idx], right->table_names[c]);
            strcpy(next->aliases[idx], right->aliases[c]);
        }
        next->col_count = curr->col_count + right->col_count;
        if (next->col_count > MAX_EXEC_COLS) next->col_count = MAX_EXEC_COLS;

        int right_matched[MAX_EXEC_ROWS] = {0};

        for (int r1 = 0; r1 < curr->row_count; r1++) {
            int left_matched = 0;
            for (int r2 = 0; r2 < right->row_count && next->row_count < MAX_EXEC_ROWS; r2++) {
                int tmp = next->row_count;
                for (int c = 0; c < curr->col_count; c++)
                    strcpy(next->rows[tmp][c], curr->rows[r1][c]);
                for (int c = 0; c < right->col_count && curr->col_count + c < MAX_EXEC_COLS; c++)
                    strcpy(next->rows[tmp][curr->col_count + c], right->rows[r2][c]);

                if (evaluate_expr(j->on_clause, next, tmp)) {
                    next->row_count++;
                    left_matched = 1;
                    right_matched[r2] = 1;
                }
            }
            if (!left_matched && j->type == JOIN_LEFT && next->row_count < MAX_EXEC_ROWS) {
                int tmp = next->row_count;
                for (int c = 0; c < curr->col_count; c++)
                    strcpy(next->rows[tmp][c], curr->rows[r1][c]);
                for (int c = 0; c < right->col_count && curr->col_count + c < MAX_EXEC_COLS; c++)
                    strcpy(next->rows[tmp][curr->col_count + c], "NULL");
                next->row_count++;
            }
        }

        if (j->type == JOIN_RIGHT) {
            for (int r2 = 0; r2 < right->row_count && next->row_count < MAX_EXEC_ROWS; r2++) {
                if (!right_matched[r2]) {
                    int tmp = next->row_count;
                    for (int c = 0; c < curr->col_count; c++)
                        strcpy(next->rows[tmp][c], "NULL");
                    for (int c = 0; c < right->col_count && curr->col_count + c < MAX_EXEC_COLS; c++)
                        strcpy(next->rows[tmp][curr->col_count + c], right->rows[r2][c]);
                    next->row_count++;
                }
            }
        }

        free(right);
        free(curr);
        curr = next;
    }

    /* Filter WHERE */
    ResultTable *filtered = calloc(1, sizeof(ResultTable));
    *filtered = *curr;
    filtered->row_count = 0;
    for (int r = 0; r < curr->row_count; r++) {
        if (evaluate_expr(select->where_clause, curr, r)) {
            for (int c = 0; c < curr->col_count; c++)
                strcpy(filtered->rows[filtered->row_count][c], curr->rows[r][c]);
            filtered->row_count++;
        }
    }
    free(curr);
    curr = filtered;

    /* ORDER BY */
    if (select->order_count > 0) {
        int oidx = resolve_col_index(curr, select->order_columns[0]);
        int desc = (select->order_desc && select->order_desc[0]);
        if (oidx >= 0) {
            ASTNode tmp_col = { .type = AST_COLUMN, .text = select->order_columns[0] };
            for (int i = 0; i < curr->row_count - 1; i++) {
                for (int j = i + 1; j < curr->row_count; j++) {
                    EvalVal va = eval_operand(&tmp_col, curr, i);
                    EvalVal vb = eval_operand(&tmp_col, curr, j);
                    int swap = desc ? compare_eval_vals(va, vb, "<")
                                    : compare_eval_vals(va, vb, ">");
                    if (swap) {
                        char tmp[128];
                        for (int c = 0; c < curr->col_count; c++) {
                            strcpy(tmp, curr->rows[i][c]);
                            strcpy(curr->rows[i][c], curr->rows[j][c]);
                            strcpy(curr->rows[j][c], tmp);
                        }
                    }
                }
            }
        }
    }

    /* Collect Projection */
    int proj_idx[MAX_EXEC_COLS];
    char proj_hdr[MAX_EXEC_COLS][64];
    int proj_cnt = 0;

    for (int i = 0; i < select->columns.count; i++) {
        ASTNode *item = select->columns.items[i];
        const char *txt = item->text;
        if (strcmp(txt, "*") == 0) {
            for (int c = 0; c < curr->col_count && proj_cnt < MAX_EXEC_COLS; c++) {
                proj_idx[proj_cnt] = c;
                strcpy(proj_hdr[proj_cnt], curr->col_names[c]);
                proj_cnt++;
            }
        } else if (strlen(txt) > 2 && strcmp(txt + strlen(txt) - 2, ".*") == 0) {
            char tbl[64];
            size_t tlen = strlen(txt) - 2;
            if (tlen >= sizeof(tbl)) tlen = sizeof(tbl) - 1;
            memcpy(tbl, txt, tlen);
            tbl[tlen] = '\0';
            for (int c = 0; c < curr->col_count && proj_cnt < MAX_EXEC_COLS; c++) {
                if (iequals(curr->table_names[c], tbl) || iequals(curr->aliases[c], tbl)) {
                    proj_idx[proj_cnt] = c;
                    strcpy(proj_hdr[proj_cnt], curr->col_names[c]);
                    proj_cnt++;
                }
            }
        } else {
            int idx = resolve_col_index(curr, txt);
            if (idx >= 0 && proj_cnt < MAX_EXEC_COLS) {
                proj_idx[proj_cnt] = idx;
                snprintf(proj_hdr[proj_cnt], 64, "%s",
                         item->alias ? item->alias : curr->col_names[idx]);
                proj_cnt++;
            }
        }
    }

    int limit = (select->limit >= 0 && select->limit < curr->row_count)
                ? select->limit : curr->row_count;

    printf("\n===== QUERY EXECUTION RESULT (%d rows) =====\n", limit);
    for (int c = 0; c < proj_cnt; c++) printf("| %-17s ", proj_hdr[c]);
    printf("|\n");
    for (int c = 0; c < proj_cnt; c++) printf("+-------------------");
    printf("+\n");

    for (int r = 0; r < limit; r++) {
        for (int c = 0; c < proj_cnt; c++) {
            printf("| %-17s ", curr->rows[r][proj_idx[c]]);
        }
        printf("|\n");
    }

    free(curr);
    return 1;
}

void codegen_free(TACPlan *plan) {
    if (!plan) return;
    for (int i = 0; i < plan->count; i++) {
        free(plan->items[i].target);
        free(plan->items[i].arg1);
        free(plan->items[i].arg2);
    }
    free(plan->items);
    free(plan);
}