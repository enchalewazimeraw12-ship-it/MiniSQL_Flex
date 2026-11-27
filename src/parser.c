/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "src/parser.y"

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

#line 116 "src/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SELECT = 3,                     /* SELECT  */
  YYSYMBOL_FROM = 4,                       /* FROM  */
  YYSYMBOL_WHERE = 5,                      /* WHERE  */
  YYSYMBOL_JOIN = 6,                       /* JOIN  */
  YYSYMBOL_INNER = 7,                      /* INNER  */
  YYSYMBOL_LEFT = 8,                       /* LEFT  */
  YYSYMBOL_RIGHT = 9,                      /* RIGHT  */
  YYSYMBOL_ON = 10,                        /* ON  */
  YYSYMBOL_AND = 11,                       /* AND  */
  YYSYMBOL_OR = 12,                        /* OR  */
  YYSYMBOL_NOT = 13,                       /* NOT  */
  YYSYMBOL_AS = 14,                        /* AS  */
  YYSYMBOL_ORDER = 15,                     /* ORDER  */
  YYSYMBOL_BY = 16,                        /* BY  */
  YYSYMBOL_GROUP = 17,                     /* GROUP  */
  YYSYMBOL_HAVING = 18,                    /* HAVING  */
  YYSYMBOL_LIMIT = 19,                     /* LIMIT  */
  YYSYMBOL_ASC = 20,                       /* ASC  */
  YYSYMBOL_DESC = 21,                      /* DESC  */
  YYSYMBOL_EQ = 22,                        /* EQ  */
  YYSYMBOL_NE = 23,                        /* NE  */
  YYSYMBOL_LT = 24,                        /* LT  */
  YYSYMBOL_GT = 25,                        /* GT  */
  YYSYMBOL_LE = 26,                        /* LE  */
  YYSYMBOL_GE = 27,                        /* GE  */
  YYSYMBOL_INTEGER = 28,                   /* INTEGER  */
  YYSYMBOL_FLOAT = 29,                     /* FLOAT  */
  YYSYMBOL_STRING = 30,                    /* STRING  */
  YYSYMBOL_IDENTIFIER = 31,                /* IDENTIFIER  */
  YYSYMBOL_COMMA = 32,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 33,                 /* SEMICOLON  */
  YYSYMBOL_STAR = 34,                      /* STAR  */
  YYSYMBOL_LPAREN = 35,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 36,                    /* RPAREN  */
  YYSYMBOL_DOT = 37,                       /* DOT  */
  YYSYMBOL_YYACCEPT = 38,                  /* $accept  */
  YYSYMBOL_input = 39,                     /* input  */
  YYSYMBOL_optional_semicolon = 40,        /* optional_semicolon  */
  YYSYMBOL_select_statement = 41,          /* select_statement  */
  YYSYMBOL_column_list = 42,               /* column_list  */
  YYSYMBOL_select_item = 43,               /* select_item  */
  YYSYMBOL_opt_alias = 44,                 /* opt_alias  */
  YYSYMBOL_table_list = 45,                /* table_list  */
  YYSYMBOL_table_item = 46,                /* table_item  */
  YYSYMBOL_opt_joins = 47,                 /* opt_joins  */
  YYSYMBOL_join_clause = 48,               /* join_clause  */
  YYSYMBOL_join_type = 49,                 /* join_type  */
  YYSYMBOL_opt_where = 50,                 /* opt_where  */
  YYSYMBOL_opt_group_by = 51,              /* opt_group_by  */
  YYSYMBOL_group_list = 52,                /* group_list  */
  YYSYMBOL_opt_having = 53,                /* opt_having  */
  YYSYMBOL_opt_order_by = 54,              /* opt_order_by  */
  YYSYMBOL_order_list = 55,                /* order_list  */
  YYSYMBOL_order_item = 56,                /* order_item  */
  YYSYMBOL_opt_dir = 57,                   /* opt_dir  */
  YYSYMBOL_opt_limit = 58,                 /* opt_limit  */
  YYSYMBOL_predicate = 59,                 /* predicate  */
  YYSYMBOL_expression = 60,                /* expression  */
  YYSYMBOL_column = 61,                    /* column  */
  YYSYMBOL_value = 62                      /* value  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   86

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  95

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    81,    81,    87,    89,    93,   113,   118,   126,   128,
     133,   141,   142,   143,   147,   148,   152,   165,   166,   180,
     193,   194,   195,   196,   200,   201,   204,   206,   210,   215,
     223,   224,   227,   229,   233,   234,   238,   246,   247,   248,
     252,   253,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   271,   272,   276,   278,   283,   284,   285
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SELECT", "FROM",
  "WHERE", "JOIN", "INNER", "LEFT", "RIGHT", "ON", "AND", "OR", "NOT",
  "AS", "ORDER", "BY", "GROUP", "HAVING", "LIMIT", "ASC", "DESC", "EQ",
  "NE", "LT", "GT", "LE", "GE", "INTEGER", "FLOAT", "STRING", "IDENTIFIER",
  "COMMA", "SEMICOLON", "STAR", "LPAREN", "RPAREN", "DOT", "$accept",
  "input", "optional_semicolon", "select_statement", "column_list",
  "select_item", "opt_alias", "table_list", "table_item", "opt_joins",
  "join_clause", "join_type", "opt_where", "opt_group_by", "group_list",
  "opt_having", "opt_order_by", "order_list", "order_item", "opt_dir",
  "opt_limit", "predicate", "expression", "column", "value", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-58)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-21)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      16,   -14,     5,   -12,   -58,   -58,   -58,     9,   -58,     2,
     -58,    -5,   -58,   -58,   -58,   -58,   -58,     4,    21,   -14,
      34,   -58,   -58,   -58,   -58,    -5,    35,   -58,   -58,   -58,
     -58,    21,    22,   -58,    12,   -58,   -58,   -58,   -58,    30,
      49,    12,    31,    12,    33,    37,    38,    54,    53,   -58,
      41,    -4,    12,    12,    20,    20,    20,    20,    20,    20,
      -5,    42,    12,    59,   -58,   -58,    64,   -58,   -58,   -58,
     -58,   -58,   -58,    66,    45,   -58,    33,    62,    60,    12,
      42,    42,    52,   -58,    33,   -58,    50,   -58,   -10,   -58,
      42,   -58,   -58,   -58,   -58
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     3,    57,    58,    59,    55,     8,     0,
       6,    11,    53,    54,     1,     4,     2,     0,     0,     0,
       0,    13,    10,    56,     9,    11,    17,    14,     7,    12,
      16,     0,    24,    15,     0,    21,    22,    23,    18,     0,
      26,     0,    55,     0,    25,    42,     0,     0,    30,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,     0,     0,    32,    46,    43,    44,    47,    48,    49,
      50,    51,    52,     0,    27,    28,    31,     0,    40,     0,
       0,     0,     0,     5,    19,    29,    33,    34,    37,    41,
       0,    38,    39,    36,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,   -58,   -58,   -58,   -58,    65,   -23,   -58,    55,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,    -9,   -58,
     -58,   -40,    -1,   -57,   -58
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    16,     3,     9,    10,    22,    26,    27,    32,
      38,    39,    40,    48,    74,    63,    78,    86,    87,    93,
      83,    44,    45,    12,    13
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      11,    49,    30,    51,    75,    14,    18,    52,    53,    20,
      91,    92,    65,    66,     4,     5,     6,     7,    11,     1,
       8,    15,    76,    85,    88,    41,    21,    34,   -20,    35,
      36,    37,    64,    88,    19,    23,    46,    73,    24,    84,
       4,     5,     6,    42,    52,    53,    17,    43,     4,     5,
       6,    42,    25,    67,    68,    69,    70,    71,    72,    54,
      55,    56,    57,    58,    59,    29,    47,    31,    50,    60,
      61,    62,    23,    42,    77,    52,    79,    80,    81,    82,
      89,    94,    90,     0,    28,     0,    33
};

static const yytype_int8 yycheck[] =
{
       1,    41,    25,    43,    61,     0,     4,    11,    12,    14,
      20,    21,    52,    53,    28,    29,    30,    31,    19,     3,
      34,    33,    62,    80,    81,    13,    31,     5,     6,     7,
       8,     9,    36,    90,    32,    31,     6,    60,    34,    79,
      28,    29,    30,    31,    11,    12,    37,    35,    28,    29,
      30,    31,    31,    54,    55,    56,    57,    58,    59,    22,
      23,    24,    25,    26,    27,    31,    17,    32,    37,    31,
      16,    18,    31,    31,    15,    11,    10,    32,    16,    19,
      28,    90,    32,    -1,    19,    -1,    31
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    39,    41,    28,    29,    30,    31,    34,    42,
      43,    60,    61,    62,     0,    33,    40,    37,     4,    32,
      14,    31,    44,    31,    34,    31,    45,    46,    43,    31,
      44,    32,    47,    46,     5,     7,     8,     9,    48,    49,
      50,    13,    31,    35,    59,    60,     6,    17,    51,    59,
      37,    59,    11,    12,    22,    23,    24,    25,    26,    27,
      31,    16,    18,    53,    36,    59,    59,    60,    60,    60,
      60,    60,    60,    44,    52,    61,    59,    15,    54,    10,
      32,    16,    19,    58,    59,    61,    55,    56,    61,    28,
      32,    20,    21,    57,    56
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    40,    40,    41,    42,    42,    43,    43,
      43,    44,    44,    44,    45,    45,    46,    47,    47,    48,
      49,    49,    49,    49,    50,    50,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    55,    56,    57,    57,    57,
      58,    58,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    60,    61,    61,    62,    62,    62
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,    10,     1,     3,     1,     3,
       2,     0,     2,     1,     1,     3,     2,     0,     2,     6,
       0,     1,     1,     1,     0,     2,     0,     3,     1,     3,
       0,     2,     0,     3,     1,     3,     2,     0,     1,     1,
       0,     2,     1,     3,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     3,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* input: select_statement optional_semicolon  */
#line 82 "src/parser.y"
    {
        parsed_select = (yyvsp[-1].select);
    }
#line 1227 "src/parser.c"
    break;

  case 5: /* select_statement: SELECT column_list FROM table_list opt_joins opt_where opt_group_by opt_having opt_order_by opt_limit  */
#line 101 "src/parser.y"
    {
        /* table_list & group/order were collected into parsed_select helper */
        parsed_select->columns = (yyvsp[-8].list);
        parsed_select->joins = (yyvsp[-5].join_node);
        parsed_select->where_clause = (yyvsp[-4].node);
        parsed_select->having_clause = (yyvsp[-2].node);
        parsed_select->limit = (yyvsp[0].flag);
        (yyval.select) = parsed_select;
    }
#line 1241 "src/parser.c"
    break;

  case 6: /* column_list: select_item  */
#line 114 "src/parser.y"
    {
        ast_list_init(&(yyval.list));
        ast_list_add(&(yyval.list), (yyvsp[0].node));
    }
#line 1250 "src/parser.c"
    break;

  case 7: /* column_list: column_list COMMA select_item  */
#line 119 "src/parser.y"
    {
        (yyval.list) = (yyvsp[-2].list);
        ast_list_add(&(yyval.list), (yyvsp[0].node));
    }
#line 1259 "src/parser.c"
    break;

  case 8: /* select_item: STAR  */
#line 127 "src/parser.y"
    { (yyval.node) = ast_column(NULL, "*"); }
#line 1265 "src/parser.c"
    break;

  case 9: /* select_item: IDENTIFIER DOT STAR  */
#line 129 "src/parser.y"
    {
        (yyval.node) = ast_column((yyvsp[-2].text), "*");
        free((yyvsp[-2].text));
    }
#line 1274 "src/parser.c"
    break;

  case 10: /* select_item: expression opt_alias  */
#line 134 "src/parser.y"
    {
        (yyval.node) = (yyvsp[-1].node);
        (yyval.node)->alias = (yyvsp[0].text);
    }
#line 1283 "src/parser.c"
    break;

  case 11: /* opt_alias: %empty  */
#line 141 "src/parser.y"
                      { (yyval.text) = NULL; }
#line 1289 "src/parser.c"
    break;

  case 12: /* opt_alias: AS IDENTIFIER  */
#line 142 "src/parser.y"
                      { (yyval.text) = (yyvsp[0].text); }
#line 1295 "src/parser.c"
    break;

  case 13: /* opt_alias: IDENTIFIER  */
#line 143 "src/parser.y"
                      { (yyval.text) = (yyvsp[0].text); }
#line 1301 "src/parser.c"
    break;

  case 16: /* table_item: IDENTIFIER opt_alias  */
#line 153 "src/parser.y"
    {
        if (!parsed_select) {
            parsed_select = calloc(1, sizeof(SelectNode));
            parsed_select->limit = -1;
        }
        add_table_entry(parsed_select, (yyvsp[-1].text), (yyvsp[0].text));
        free((yyvsp[-1].text));
        free((yyvsp[0].text));
    }
#line 1315 "src/parser.c"
    break;

  case 17: /* opt_joins: %empty  */
#line 165 "src/parser.y"
                              { (yyval.join_node) = NULL; }
#line 1321 "src/parser.c"
    break;

  case 18: /* opt_joins: opt_joins join_clause  */
#line 167 "src/parser.y"
    {
        if (!(yyvsp[-1].join_node)) {
            (yyval.join_node) = (yyvsp[0].join_node);
        } else {
            JoinNode *p = (yyvsp[-1].join_node);
            while (p->next) p = p->next;
            p->next = (yyvsp[0].join_node);
            (yyval.join_node) = (yyvsp[-1].join_node);
        }
    }
#line 1336 "src/parser.c"
    break;

  case 19: /* join_clause: join_type JOIN IDENTIFIER opt_alias ON predicate  */
#line 181 "src/parser.y"
    {
        JoinNode *j = calloc(1, sizeof(JoinNode));
        j->type = (yyvsp[-5].join_type);
        j->table = (yyvsp[-3].text);
        j->alias = (yyvsp[-2].text);
        j->on_clause = (yyvsp[0].node);
        j->next = NULL;
        (yyval.join_node) = j;
    }
#line 1350 "src/parser.c"
    break;

  case 20: /* join_type: %empty  */
#line 193 "src/parser.y"
                  { (yyval.join_type) = JOIN_INNER; }
#line 1356 "src/parser.c"
    break;

  case 21: /* join_type: INNER  */
#line 194 "src/parser.y"
                  { (yyval.join_type) = JOIN_INNER; }
#line 1362 "src/parser.c"
    break;

  case 22: /* join_type: LEFT  */
#line 195 "src/parser.y"
                  { (yyval.join_type) = JOIN_LEFT; }
#line 1368 "src/parser.c"
    break;

  case 23: /* join_type: RIGHT  */
#line 196 "src/parser.y"
                  { (yyval.join_type) = JOIN_RIGHT; }
#line 1374 "src/parser.c"
    break;

  case 24: /* opt_where: %empty  */
#line 200 "src/parser.y"
                      { (yyval.node) = NULL; }
#line 1380 "src/parser.c"
    break;

  case 25: /* opt_where: WHERE predicate  */
#line 201 "src/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1386 "src/parser.c"
    break;

  case 28: /* group_list: column  */
#line 211 "src/parser.y"
    {
        add_group_entry(parsed_select, (yyvsp[0].node)->text);
        ast_free((yyvsp[0].node));
    }
#line 1395 "src/parser.c"
    break;

  case 29: /* group_list: group_list COMMA column  */
#line 216 "src/parser.y"
    {
        add_group_entry(parsed_select, (yyvsp[0].node)->text);
        ast_free((yyvsp[0].node));
    }
#line 1404 "src/parser.c"
    break;

  case 30: /* opt_having: %empty  */
#line 223 "src/parser.y"
                      { (yyval.node) = NULL; }
#line 1410 "src/parser.c"
    break;

  case 31: /* opt_having: HAVING predicate  */
#line 224 "src/parser.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1416 "src/parser.c"
    break;

  case 36: /* order_item: column opt_dir  */
#line 239 "src/parser.y"
    {
        add_order_entry(parsed_select, (yyvsp[-1].node)->text, (yyvsp[0].flag));
        ast_free((yyvsp[-1].node));
    }
#line 1425 "src/parser.c"
    break;

  case 37: /* opt_dir: %empty  */
#line 246 "src/parser.y"
                  { (yyval.flag) = 0; }
#line 1431 "src/parser.c"
    break;

  case 38: /* opt_dir: ASC  */
#line 247 "src/parser.y"
                  { (yyval.flag) = 0; }
#line 1437 "src/parser.c"
    break;

  case 39: /* opt_dir: DESC  */
#line 248 "src/parser.y"
                  { (yyval.flag) = 1; }
#line 1443 "src/parser.c"
    break;

  case 40: /* opt_limit: %empty  */
#line 252 "src/parser.y"
                      { (yyval.flag) = -1; }
#line 1449 "src/parser.c"
    break;

  case 41: /* opt_limit: LIMIT INTEGER  */
#line 253 "src/parser.y"
                      { (yyval.flag) = atoi((yyvsp[0].text)); free((yyvsp[0].text)); }
#line 1455 "src/parser.c"
    break;

  case 42: /* predicate: expression  */
#line 257 "src/parser.y"
                                  { (yyval.node) = (yyvsp[0].node); }
#line 1461 "src/parser.c"
    break;

  case 43: /* predicate: predicate AND predicate  */
#line 258 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), "AND", (yyvsp[0].node)); }
#line 1467 "src/parser.c"
    break;

  case 44: /* predicate: predicate OR predicate  */
#line 259 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), "OR", (yyvsp[0].node)); }
#line 1473 "src/parser.c"
    break;

  case 45: /* predicate: NOT predicate  */
#line 260 "src/parser.y"
                                  { (yyval.node) = ast_unary("NOT", (yyvsp[0].node)); }
#line 1479 "src/parser.c"
    break;

  case 46: /* predicate: LPAREN predicate RPAREN  */
#line 261 "src/parser.y"
                                  { (yyval.node) = (yyvsp[-1].node); }
#line 1485 "src/parser.c"
    break;

  case 47: /* predicate: expression EQ expression  */
#line 262 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), "=", (yyvsp[0].node)); }
#line 1491 "src/parser.c"
    break;

  case 48: /* predicate: expression NE expression  */
#line 263 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), "!=", (yyvsp[0].node)); }
#line 1497 "src/parser.c"
    break;

  case 49: /* predicate: expression LT expression  */
#line 264 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), "<", (yyvsp[0].node)); }
#line 1503 "src/parser.c"
    break;

  case 50: /* predicate: expression GT expression  */
#line 265 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), ">", (yyvsp[0].node)); }
#line 1509 "src/parser.c"
    break;

  case 51: /* predicate: expression LE expression  */
#line 266 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), "<=", (yyvsp[0].node)); }
#line 1515 "src/parser.c"
    break;

  case 52: /* predicate: expression GE expression  */
#line 267 "src/parser.y"
                                  { (yyval.node) = ast_binary((yyvsp[-2].node), ">=", (yyvsp[0].node)); }
#line 1521 "src/parser.c"
    break;

  case 53: /* expression: column  */
#line 271 "src/parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1527 "src/parser.c"
    break;

  case 54: /* expression: value  */
#line 272 "src/parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1533 "src/parser.c"
    break;

  case 55: /* column: IDENTIFIER  */
#line 277 "src/parser.y"
    { (yyval.node) = ast_column(NULL, (yyvsp[0].text)); free((yyvsp[0].text)); }
#line 1539 "src/parser.c"
    break;

  case 56: /* column: IDENTIFIER DOT IDENTIFIER  */
#line 279 "src/parser.y"
    { (yyval.node) = ast_column((yyvsp[-2].text), (yyvsp[0].text)); free((yyvsp[-2].text)); free((yyvsp[0].text)); }
#line 1545 "src/parser.c"
    break;

  case 57: /* value: INTEGER  */
#line 283 "src/parser.y"
              { (yyval.node) = ast_new(AST_LITERAL, (yyvsp[0].text)); free((yyvsp[0].text)); }
#line 1551 "src/parser.c"
    break;

  case 58: /* value: FLOAT  */
#line 284 "src/parser.y"
              { (yyval.node) = ast_new(AST_LITERAL, (yyvsp[0].text)); free((yyvsp[0].text)); }
#line 1557 "src/parser.c"
    break;

  case 59: /* value: STRING  */
#line 285 "src/parser.y"
              { (yyval.node) = ast_new(AST_LITERAL, (yyvsp[0].text)); free((yyvsp[0].text)); }
#line 1563 "src/parser.c"
    break;


#line 1567 "src/parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 288 "src/parser.y"


void yyerror(const char *message) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, message);
}
