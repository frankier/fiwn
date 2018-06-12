
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */



#include "wngrind.h"

static char *Id = "$Id: wnparse.y,v 1.48 2005/01/31 20:04:03 wn Rel $";

Pointer ptrlist = NULL;		/* keep track of ptrs in syngroup */
Framelist frlist = NULL;	/* keep track of frames in syngroup */
G_Synset lastss = NULL;		/* last synset in list */
Synonym lastsyn = NULL;		/* last synonym in this synset */
Symbol satellite = NULL;	/* satellite pointed to */
int headptr = FALSE;		/* indicate whether ptr is to head word */
int headword = FALSE;		/* indicate whether word is head word */
int foundhyper = FALSE;		/* indicate whether hypernym found in synset */
short wdcnt = 0;		/* index of current synonym in synset */
int aclass = 0;			/* keep track of adjective clss in syngroup */
int wsense = 0;			/* keep track of word's sense number */
int sense = 0;			/* keep track of sense number for ptrs */
int satellite_sense = 0;	/* sense number for pointer to satellite */
int cluscnt = 1;		/* count clusters (needed to resolve ptrs) */
char *label;
char buf[100];			/* useful buffer for building messages */
char fnbuf[100];		/* save cross-file pointer for warning msg */

/* FiWN: Moved declarations here to make the file compile with Bison */
static void AddToSynList();
static G_Synset MakeSynset(int, Synonym, Pointer, Framelist, char *, int);
static Synonym AddSynonym(Synonym, Synonym);
static Pointer AddPointer(Pointer, Pointer);
static Synonym InsertSynonym(Synonym, Synonym);
static void AddSynset(G_Synset);
static void AddToSynList(G_Synset);
static void MakeSyngroup(Pointer, Framelist);
static Pointer DoPointer(Symbol, int);
static int UniqueSynonym(Synonym, Synonym);
static int UniquePointer(Pointer, Pointer);
static int UniqueSynSense(Synonym, int);
static int UniqueFanSynonym(Synonym, int, int);
static char *UseLabel(char *);
static char *normalize_word(char *);
static char *tolower_non_xml(char *);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NOUNSYNSET = 258,
     VERBSYNSET = 259,
     ADJSYNSET = 260,
     ADVSYNSET = 261,
     OPENCLUS = 262,
     CLOSECLUS = 263,
     DEFN = 264,
     WYRD = 265,
     HEADWYRD = 266,
     VERBFRAMES = 267,
     FILENAME = 268,
     NUM = 269,
     PTRSYM = 270,
     ADJCLASS = 271,
     SYNSET = 272,
     SYNSETKEY = 273
   };
#endif
/* Tokens.  */
#define NOUNSYNSET 258
#define VERBSYNSET 259
#define ADJSYNSET 260
#define ADVSYNSET 261
#define OPENCLUS 262
#define CLOSECLUS 263
#define DEFN 264
#define WYRD 265
#define HEADWYRD 266
#define VERBFRAMES 267
#define FILENAME 268
#define NUM 269
#define PTRSYM 270
#define ADJCLASS 271
#define SYNSET 272
#define SYNSETKEY 273




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


    char *tx;			/* for definitions, WYRD print names */
    int number;			/* file #, sense # */
    G_Synset ss;			/* for pointers to sense space */
    Pointer p;			/* for pointers */
    Synonym s;			/* for synonyms */
    Symbol sym;			/* for pointer into symbol table */
    Framelist fr;		/* for frame lists */



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   170

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  25
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNRULES -- Number of states.  */
#define YYNSTATES  138

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   273

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    23,    24,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,    19,    22,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    20,     2,     2,     2,     2,
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
      15,    16,    17,    18
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    13,    15,    17,
      19,    26,    32,    40,    47,    54,    60,    67,    73,    74,
      77,    81,    84,    90,    97,   103,   105,   108,   110,   113,
     115,   118,   120,   123,   126,   130,   135,   141,   148,   154,
     157,   160,   162,   165,   166,   168,   169,   171,   173,   176,
     179,   183,   186,   190,   193,   195,   199,   202,   205,   206,
     209,   216,   222,   225,   233,   240,   242,   247,   250
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      26,     0,    -1,    26,    27,    -1,    -1,    28,    -1,    29,
      -1,    30,    -1,    41,    -1,    45,    -1,    19,    -1,     3,
      31,    36,    37,    20,    38,    -1,     3,    31,    37,    20,
      38,    -1,     4,    31,    36,    40,    37,    20,    38,    -1,
       4,    31,    40,    37,    20,    38,    -1,     4,    31,    36,
      37,    20,    38,    -1,     4,    31,    37,    20,    38,    -1,
       6,    31,    36,    37,    20,    38,    -1,     6,    31,    37,
      20,    38,    -1,    -1,    31,    32,    -1,    31,    33,    39,
      -1,    31,     1,    -1,    21,    33,    39,    36,    19,    -1,
      21,    33,    39,    36,    40,    19,    -1,    21,    33,    39,
      40,    19,    -1,    10,    -1,    10,    14,    -1,    11,    -1,
      11,    14,    -1,    10,    -1,    10,    14,    -1,    11,    -1,
      11,    14,    -1,    13,    10,    -1,    13,    10,    14,    -1,
      13,    10,    22,    10,    -1,    13,    10,    14,    22,    10,
      -1,    13,    10,    14,    22,    10,    14,    -1,    13,    10,
      22,    10,    14,    -1,    34,    15,    -1,    36,    35,    -1,
      35,    -1,    36,     1,    -1,    -1,     9,    -1,    -1,    18,
      -1,    23,    -1,    16,    23,    -1,    12,    14,    -1,    40,
      23,    14,    -1,    40,     1,    -1,     7,    42,     8,    -1,
       1,    19,    -1,    43,    -1,    42,    24,    43,    -1,    46,
      44,    -1,     1,    44,    -1,    -1,    45,    44,    -1,     5,
      31,    36,    37,    20,    38,    -1,     5,    31,    37,    20,
      38,    -1,    21,    45,    -1,     5,    47,    31,    36,    37,
      20,    38,    -1,     5,    47,    31,    37,    20,    38,    -1,
      48,    -1,    21,    48,    36,    19,    -1,    11,    39,    -1,
      11,    14,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    69,    69,    70,    73,    75,    77,    79,    82,    92,
      96,   100,   106,   110,   114,   118,   124,   128,   134,   136,
     147,   158,   173,   179,   185,   193,   201,   215,   241,   274,
     284,   294,   304,   314,   325,   337,   349,   361,   373,   387,
     389,   397,   399,   416,   418,   422,   424,   427,   429,   433,
     442,   453,   460,   462,   470,   472,   476,   485,   492,   494,
     498,   506,   514,   521,   525,   532,   549,   569,   578
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NOUNSYNSET", "VERBSYNSET", "ADJSYNSET",
  "ADVSYNSET", "OPENCLUS", "CLOSECLUS", "DEFN", "WYRD", "HEADWYRD",
  "VERBFRAMES", "FILENAME", "NUM", "PTRSYM", "ADJCLASS", "SYNSET",
  "SYNSETKEY", "']'", "'}'", "'['", "'^'", "','", "'-'", "$accept",
  "wordnet", "wordelem", "nss", "vss", "rss", "syns", "syngroup", "syn",
  "psyn", "ptr", "ptrs", "defn", "key", "adjclass", "framelist",
  "acluster", "asslist", "ass", "fanlist", "fan", "ahead", "headsyn",
  "headwyrd", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,    93,
     125,    91,    94,    44,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    25,    26,    26,    27,    27,    27,    27,    27,    27,
      28,    28,    29,    29,    29,    29,    30,    30,    31,    31,
      31,    31,    32,    32,    32,    33,    33,    33,    33,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    35,
      36,    36,    36,    37,    37,    38,    38,    39,    39,    40,
      40,    40,    41,    41,    42,    42,    43,    43,    44,    44,
      45,    45,    45,    46,    46,    47,    47,    48,    48
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       6,     5,     7,     6,     6,     5,     6,     5,     0,     2,
       3,     2,     5,     6,     5,     1,     2,     1,     2,     1,
       2,     1,     2,     2,     3,     4,     5,     6,     5,     2,
       2,     1,     2,     0,     1,     0,     1,     1,     2,     2,
       3,     2,     3,     2,     1,     3,     2,     2,     0,     2,
       6,     5,     2,     7,     6,     1,     4,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,     0,    18,    18,    18,    18,     0,     9,
       0,     2,     4,     5,     6,     7,     8,    53,     0,     0,
       0,     0,    58,     0,     0,    54,    58,    62,    21,    44,
      25,    27,     0,     0,    19,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,    18,    65,    52,     0,    56,    26,    28,    33,
      25,    27,     0,     0,    47,    20,    39,    42,    29,    31,
      40,     0,    45,    49,     0,     0,    45,    51,     0,     0,
       0,    45,     0,    45,    59,     0,    67,     0,     0,    55,
      34,     0,    26,    28,     0,    48,    30,    32,    45,    46,
      11,    45,     0,    15,    50,    45,    45,    61,    45,    17,
      68,     0,     0,     0,     0,    35,     0,     0,    10,    14,
      45,    13,    60,    16,    66,     0,    45,    36,    38,    22,
       0,    24,    12,    45,    64,    37,    23,    63
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    11,    12,    13,    14,    18,    34,    35,    36,
      37,    38,    39,   100,    65,    43,    15,    24,    25,    48,
      49,    26,    52,    53
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -56
static const yytype_int16 yypact[] =
{
     -56,    91,   -56,   -12,   -56,   -56,   -56,   -56,   102,   -56,
      31,   -56,   -56,   -56,   -56,   -56,   -56,   -56,    53,   104,
      53,    53,    31,    26,    60,   -56,    31,   -56,   -56,   -56,
      -2,   113,    34,   136,   -56,    32,    55,   -56,   125,    52,
      61,   110,    84,     9,   125,    86,   125,    98,   -56,    31,
     117,   118,   -56,   -56,   -56,   102,   -56,   122,   124,    87,
     129,   134,    32,   109,   -56,   -56,   -56,   -56,   135,   137,
     -56,   130,   138,   -56,   132,     9,   138,   -56,   139,   140,
     141,   138,   142,   138,   -56,    32,   -56,   131,    53,   -56,
     133,   144,   -56,   -56,    75,   -56,   -56,   -56,   138,   -56,
     -56,   138,   143,   -56,   -56,   138,   138,   -56,   138,   -56,
     -56,    89,   125,   145,   147,   150,    48,    19,   -56,   -56,
     138,   -56,   -56,   -56,   -56,   146,   138,   153,   -56,   -56,
      22,   -56,   -56,   138,   -56,   -56,   -56,   -56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -56,   -56,   -56,   -56,   -56,   -56,    28,   -56,   126,   -56,
     -35,    -5,   -19,   -55,   -45,   -37,   -56,   -56,   103,   -18,
      29,   -56,   -56,   119
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -44
static const yytype_int16 yytable[] =
{
      42,    45,    47,    70,    75,    86,    70,    17,    56,    70,
      77,    70,    57,   -29,    41,    44,    46,    94,    29,    71,
      77,   103,    74,    77,    79,    80,   107,    82,   109,   -43,
      16,    84,    78,    19,    20,    21,     6,    50,   131,    27,
     110,   136,    78,   118,    59,    78,   119,    51,    63,    67,
     121,   122,    10,   123,    28,    64,   102,   117,    68,    69,
      40,    32,    29,    30,    31,   132,    32,   129,    54,   113,
      66,   134,    72,   -43,    33,    73,    70,    70,   137,   130,
      88,    70,   111,   112,    55,    68,    69,    40,    32,   116,
      67,     2,     3,   125,     4,     5,     6,     7,     8,    68,
      69,    90,    32,    22,    76,    28,    81,    23,   124,    91,
       9,    67,    10,    29,    30,    31,    40,    32,    83,    29,
      68,    69,    40,    32,   -43,    33,    67,    58,   -31,    50,
     -43,    85,    95,    63,    29,    68,    69,   -30,    32,   -32,
      64,    68,    69,    92,    32,   -43,    60,    61,    93,    96,
      98,    97,   101,   104,   115,   114,    99,   127,    89,    62,
     105,   106,   108,   120,   128,   126,   133,   135,     0,     0,
      87
};

static const yytype_int16 yycheck[] =
{
      19,    20,    21,    38,    41,    50,    41,    19,    26,    44,
       1,    46,    14,    15,    19,    20,    21,    62,     9,    38,
       1,    76,    41,     1,    43,    44,    81,    46,    83,    20,
       1,    49,    23,     5,     6,     7,     5,    11,    19,    10,
      85,    19,    23,    98,    10,    23,   101,    21,    16,     1,
     105,   106,    21,   108,     1,    23,    75,    94,    10,    11,
      12,    13,     9,    10,    11,   120,    13,    19,     8,    88,
      15,   126,    20,    20,    21,    14,   111,   112,   133,   116,
      52,   116,    87,    88,    24,    10,    11,    12,    13,    94,
       1,     0,     1,   112,     3,     4,     5,     6,     7,    10,
      11,    14,    13,     1,    20,     1,    20,     5,    19,    22,
      19,     1,    21,     9,    10,    11,    12,    13,    20,     9,
      10,    11,    12,    13,    20,    21,     1,    14,    15,    11,
      20,    14,    23,    16,     9,    10,    11,    15,    13,    15,
      23,    10,    11,    14,    13,    20,    10,    11,    14,    14,
      20,    14,    20,    14,    10,    22,    18,    10,    55,    33,
      20,    20,    20,    20,    14,    20,    20,    14,    -1,    -1,
      51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    26,     0,     1,     3,     4,     5,     6,     7,    19,
      21,    27,    28,    29,    30,    41,    45,    19,    31,    31,
      31,    31,     1,     5,    42,    43,    46,    45,     1,     9,
      10,    11,    13,    21,    32,    33,    34,    35,    36,    37,
      12,    36,    37,    40,    36,    37,    36,    37,    44,    45,
      11,    21,    47,    48,     8,    24,    44,    14,    14,    10,
      10,    11,    33,    16,    23,    39,    15,     1,    10,    11,
      35,    37,    20,    14,    37,    40,    20,     1,    23,    37,
      37,    20,    37,    20,    44,    14,    39,    48,    31,    43,
      14,    22,    14,    14,    39,    23,    14,    14,    20,    18,
      38,    20,    37,    38,    14,    20,    20,    38,    20,    38,
      39,    36,    36,    37,    22,    10,    36,    40,    38,    38,
      20,    38,    38,    38,    19,    37,    20,    10,    14,    19,
      40,    19,    38,    20,    38,    14,    19,    38
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

    { if ((yyvsp[(1) - (1)].ss)) AddSynset((yyvsp[(1) - (1)].ss)); }
    break;

  case 5:

    { if ((yyvsp[(1) - (1)].ss)) AddSynset((yyvsp[(1) - (1)].ss)); }
    break;

  case 6:

    { if ((yyvsp[(1) - (1)].ss)) AddSynset((yyvsp[(1) - (1)].ss)); }
    break;

  case 7:

    { }
    break;

  case 8:

    {
	    if ((yyvsp[(1) - (1)].ss)) {
		(yyvsp[(1) - (1)].ss)->isfanss = FALSE;
		(yyvsp[(1) - (1)].ss)->part = ADJ;
		(yyvsp[(1) - (1)].ss)->clusnum = 0;
		AddSynset((yyvsp[(1) - (1)].ss));
	    }
	}
    break;

  case 9:

    {	yyerror("']' without matching '['\n"); }
    break;

  case 10:

    { 
            (yyval.ss) = MakeSynset(NOUN, (yyvsp[(2) - (6)].s), (yyvsp[(3) - (6)].p), NULL, (yyvsp[(4) - (6)].tx), (yyvsp[(6) - (6)].number));
	}
    break;

  case 11:

    {
            (yyval.ss) = MakeSynset(NOUN, (yyvsp[(2) - (5)].s), NULL, NULL, (yyvsp[(3) - (5)].tx), (yyvsp[(5) - (5)].number));
	}
    break;

  case 12:

    {
            (yyval.ss) = MakeSynset(VERB, (yyvsp[(2) - (7)].s), (yyvsp[(3) - (7)].p), (yyvsp[(4) - (7)].fr), (yyvsp[(5) - (7)].tx), (yyvsp[(7) - (7)].number));
	}
    break;

  case 13:

    {
	    (yyval.ss) = MakeSynset(VERB, (yyvsp[(2) - (6)].s), NULL, (yyvsp[(3) - (6)].fr), (yyvsp[(4) - (6)].tx), (yyvsp[(6) - (6)].number));
	}
    break;

  case 14:

    {
	    (yyval.ss) = MakeSynset(VERB, (yyvsp[(2) - (6)].s), (yyvsp[(3) - (6)].p), NULL, (yyvsp[(4) - (6)].tx), (yyvsp[(6) - (6)].number));
	}
    break;

  case 15:

    { 
	    (yyval.ss) = MakeSynset(VERB, (yyvsp[(2) - (5)].s), NULL, NULL, (yyvsp[(3) - (5)].tx), (yyvsp[(5) - (5)].number));
	}
    break;

  case 16:

    { 
            (yyval.ss) = MakeSynset(ADV, (yyvsp[(2) - (6)].s), (yyvsp[(3) - (6)].p), NULL, (yyvsp[(4) - (6)].tx), (yyvsp[(6) - (6)].number));
	}
    break;

  case 17:

    {
            (yyval.ss) = MakeSynset(ADV, (yyvsp[(2) - (5)].s), NULL, NULL, (yyvsp[(3) - (5)].tx), (yyvsp[(5) - (5)].number));
	}
    break;

  case 18:

    { (yyval.s)=NULL; }
    break;

  case 19:

    {
	    wdcnt++;
	    (yyval.s) = AddSynonym((yyvsp[(1) - (2)].s), CreateSynonym((yyvsp[(2) - (2)].sym),
					      (unsigned char)wsense,
					      (headword ? -wdcnt : wdcnt),
					      (unsigned char)aclass,
					      label));
	    headword = FALSE;
	    aclass = 0;
	}
    break;

  case 20:

    {
	    wdcnt++;
	    (yyval.s) = AddSynonym((yyvsp[(1) - (3)].s), CreateSynonym((yyvsp[(2) - (3)].sym),
					      (unsigned char)wsense,
					      (headword ? -wdcnt : wdcnt),
					      (unsigned char)(yyvsp[(3) - (3)].number),
					      label));
	    headword = FALSE;
	    aclass = 0;
	}
    break;

  case 21:

    { 
	    yyerror("Error in synonym list, "); 
	    if ((yyvsp[(1) - (2)].s)) {
		fprintf(logfile, "last good synonym: \"%s\"\n",
			PrintSynonym(lastsyn));
	    } else if (!wdcnt) {
		fprintf(logfile, "invalid string found\n");
	    } else {
		fprintf(logfile,"first synonym group is bad\n");
	    }
	    (yyval.s) = (yyvsp[(1) - (2)].s);
	}
    break;

  case 22:

    {
            aclass = (yyvsp[(3) - (5)].number);
	    MakeSyngroup((yyvsp[(4) - (5)].p), NULL);
	    (yyval.sym) = (yyvsp[(2) - (5)].sym);
	}
    break;

  case 23:

    {
            aclass = (yyvsp[(3) - (6)].number);
	    MakeSyngroup((yyvsp[(4) - (6)].p), (yyvsp[(5) - (6)].fr));
	    (yyval.sym) = (yyvsp[(2) - (6)].sym);
	}
    break;

  case 24:

    {
            aclass = (yyvsp[(3) - (5)].number);
	    MakeSyngroup(NULL, (yyvsp[(4) - (5)].fr));
	    (yyval.sym) = (yyvsp[(2) - (5)].sym);
	}
    break;

  case 25:

    {
	    /* FiWN: Use normalize_word instead of strlower */
            (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (1)].tx)));
	    label = UseLabel((yyvsp[(1) - (1)].tx)); /* decide whether to save original string */
	    headword = FALSE;
	    wsense = 0;
	}
    break;

  case 26:

    { 
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (2)].tx)));
	    label = UseLabel((yyvsp[(1) - (2)].tx)); /* decide whether to save original string */
	    headword = FALSE;
	    wsense = (yyvsp[(2) - (2)].number);
	    if (wsense > MAXID) {
		/* FiWN: Use MAXID */
		sprintf(buf, "ID must be less than %d: %s%d\n",
			MAXID + 1, (yyval.sym)->label, wsense);
		yyerror(buf);
	    }
	}
    break;

  case 27:

    {
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (1)].tx)));

/* If file is adj.all, then label must be NULL.  Otherwise, allow upper
   case in synset since there's no head. */

	    if ( ! strcmp(filelist[curfilenum()].fname, "adj.all" ) ) {
#ifndef NO_XML
		/* FiWN: Convert head adjectives to lowercase for the database
		   if XML is to be ignored when indexing */
		if (ignore_xmlwords)
		    label = UseLabel(tolower_non_xml((yyvsp[(1) - (1)].tx)));
		else
		    label = NULL;
#else
		label = NULL;
#endif
		headword = TRUE;
	    } else {
		label = UseLabel((yyvsp[(1) - (1)].tx));
		headword = FALSE;
	    }
	    wsense = 0;
	}
    break;

  case 28:

    { 
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (2)].tx)));

/* If file is adj.all, then label must be NULL.  Otherwise, allow upper
   case in synset since there's no head. */

	    if ( ! strcmp(filelist[curfilenum()].fname, "adj.all" ) ) {
#ifndef NO_XML
		/* FiWN: Convert head adjectives to lowercase for the database
		   if XML is to be ignored when indexing */
		if (ignore_xmlwords)
		    label = UseLabel(tolower_non_xml((yyvsp[(1) - (2)].tx)));
		else
		    label = NULL;
#else
		label = NULL;
#endif
		headword = TRUE;
	    } else {
		label = UseLabel((yyvsp[(1) - (2)].tx));
		headword = FALSE;
	    }
	    wsense = (yyvsp[(2) - (2)].number);
	    if (wsense > MAXID) {
		/* FiWN: Use MAXID */
		sprintf(buf, "ID must be less than %d: %s%d\n",
			MAXID + 1, (yyval.sym)->label, wsense);
		yyerror(buf);
	    }
	}
    break;

  case 29:

    {
	    /* FiWN: Use normalize_word instead of strlower */
            (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (1)].tx)));
	    sense = 0;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    fnbuf[0] = '\0';
	}
    break;

  case 30:

    { 
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (2)].tx)));
	    sense = (yyvsp[(2) - (2)].number);
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    fnbuf[0] = '\0';
	}
    break;

  case 31:

    {
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (1)].tx)));
	    sense = 0;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = TRUE;
	    fnbuf[0] = '\0';
	}
    break;

  case 32:

    { 
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (2)].tx)));
	    sense = (yyvsp[(2) - (2)].number);
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = TRUE;
	    fnbuf[0] = '\0';
	}
    break;

  case 33:

    {
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(2) - (2)].tx)));
	    sense = 0;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, (yyvsp[(1) - (2)].tx));	/* save filename for warning msg */
	    free((yyvsp[(1) - (2)].tx));		/* free space malloc'd for string */
	}
    break;

  case 34:

    { 
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(2) - (3)].tx)));
	    sense = (yyvsp[(3) - (3)].number);
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, (yyvsp[(1) - (3)].tx));	/* save filename for warning msg */
	    free((yyvsp[(1) - (3)].tx));		/* free space malloc'd for string */
	}
    break;

  case 35:

    {/* new ones start here */
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(2) - (4)].tx)));
	    sense = 0;
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word((yyvsp[(4) - (4)].tx)));
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, (yyvsp[(1) - (4)].tx));	/* save filename for warning msg */
	    free((yyvsp[(1) - (4)].tx));		/* free space malloc'd for string */
	}
    break;

  case 36:

    {
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(2) - (5)].tx)));
	    sense = (yyvsp[(3) - (5)].number);
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word((yyvsp[(5) - (5)].tx)));
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, (yyvsp[(1) - (5)].tx));	/* save filename for warning msg */
	    free((yyvsp[(1) - (5)].tx));		/* free space malloc'd for string */
	}
    break;

  case 37:

    {
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(2) - (6)].tx)));
	    sense = (yyvsp[(3) - (6)].number);
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word((yyvsp[(5) - (6)].tx)));
	    satellite_sense = (yyvsp[(6) - (6)].number);
	    headptr = FALSE;
	    strcpy(fnbuf, (yyvsp[(1) - (6)].tx));	/* save filename for warning msg */
	    free((yyvsp[(1) - (6)].tx));		/* free space malloc'd for string */
	}
    break;

  case 38:

    {
	    /* FiWN: Use normalize_word instead of strlower */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(2) - (5)].tx)));
	    sense = 0;
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word((yyvsp[(4) - (5)].tx)));
	    satellite_sense = (yyvsp[(5) - (5)].number);
	    headptr = FALSE;
	    strcpy(fnbuf, (yyvsp[(1) - (5)].tx));	/* save filename for warning msg */
	    free((yyvsp[(1) - (5)].tx));		/* free space malloc'd for string */
	}
    break;

  case 39:

    { (yyval.p) = DoPointer((yyvsp[(1) - (2)].sym), (yyvsp[(2) - (2)].number)); }
    break;

  case 40:

    {
            if ((yyvsp[(2) - (2)].p)) {
		(yyval.p) = AddPointer((yyvsp[(1) - (2)].p), (yyvsp[(2) - (2)].p));
	    } else { 
		(yyval.p)=(yyvsp[(1) - (2)].p);
	    }
	}
    break;

  case 41:

    { (yyval.p)=(yyvsp[(1) - (1)].p); }
    break;

  case 42:

    {
	    Pointer p;

	    yyerror("Error in pointer list, ");
	    p = (yyvsp[(1) - (2)].p);
	    if (p) {
		for (; p->pnext; p = p->pnext);	/* find end of list */
		fprintf(logfile,
			"last valid pointer: \"%s\"\n",
			PrintPointer(p));
	    } else
		fprintf(logfile,"first pointer in list is bad\n");
	    (yyval.p)=(yyvsp[(1) - (2)].p);
	}
    break;

  case 43:

    { (yyval.tx) = NULL; }
    break;

  case 44:

    { (yyval.tx) = (yyvsp[(1) - (1)].tx); }
    break;

  case 45:

    { (yyval.number) = NULL; }
    break;

  case 46:

    { (yyval.number) = (yyvsp[(1) - (1)].number); }
    break;

  case 47:

    { (yyval.number)=0; }
    break;

  case 48:

    { (yyval.number)=(yyvsp[(1) - (2)].number); }
    break;

  case 49:

    {
            if ((yyvsp[(2) - (2)].number) < 1 || (yyvsp[(2) - (2)].number) > NUMFRAMES) {
		sprintf(buf, "Bad frame number: %d\n", (yyvsp[(2) - (2)].number));
		yyerror(buf);
		(yyval.fr) = CreateFramelist(0);
	    } else
		(yyval.fr) = CreateFramelist((yyvsp[(2) - (2)].number));
	}
    break;

  case 50:

    {
	    if ((yyvsp[(3) - (3)].number) < 1 || (yyvsp[(3) - (3)].number) > NUMFRAMES) {
		sprintf(buf, "Bad frame number: %d\n", (yyvsp[(3) - (3)].number));
		yyerror(buf);
		(yyval.fr) = (yyvsp[(1) - (3)].fr);
	    } else {
		(yyvsp[(1) - (3)].fr)->frames[((yyvsp[(3) - (3)].number)-1)/32] |= 1<<(((yyvsp[(3) - (3)].number) - 1) % 32);
		(yyval.fr) = (yyvsp[(1) - (3)].fr);
	    }
	}
    break;

  case 51:

    { 
	    yyerror("Error in frame list.\n"); 
	    (yyval.fr) = (yyvsp[(1) - (2)].fr);
	}
    break;

  case 52:

    { cluscnt++; (yyval.ss) = (yyvsp[(2) - (3)].ss); }
    break;

  case 53:

    {
	    (yyval.ss)=NULL;
	    yyerror("Had trouble finding the beginning of this cluster\n");
	    yyerrok;
	}
    break;

  case 54:

    { (yyval.ss) = (yyvsp[(1) - (1)].ss); }
    break;

  case 55:

    { (yyval.ss) = (yyvsp[(1) - (3)].ss); }
    break;

  case 56:

    {
            if ((yyvsp[(1) - (2)].ss)) {
		(yyvsp[(1) - (2)].ss)->fans = (yyvsp[(2) - (2)].ss);
		(yyvsp[(1) - (2)].ss)->clusnum = cluscnt;
		AddSynset((yyvsp[(1) - (2)].ss));
	    }
	    (yyval.ss) = (yyvsp[(1) - (2)].ss);
	}
    break;

  case 57:

    {
	    (yyval.ss)=NULL;
	    yyerror("Can't find a head synset for this part of the cluster\n");
	}
    break;

  case 58:

    { (yyval.ss)=NULL; }
    break;

  case 59:

    { if ((yyvsp[(1) - (2)].ss)) (yyvsp[(1) - (2)].ss)->ssnext = (yyvsp[(2) - (2)].ss); (yyval.ss) = (yyvsp[(1) - (2)].ss); }
    break;

  case 60:

    {
	    if ((yyval.ss) = MakeSynset(SATELLITE, (yyvsp[(2) - (6)].s), (yyvsp[(3) - (6)].p), NULL, (yyvsp[(4) - (6)].tx), (yyvsp[(6) - (6)].number))) {
		(yyval.ss)->isfanss = TRUE;
		(yyval.ss)->clusnum = cluscnt;
		AddToSynList((yyval.ss)); /* need since AddSynset() not done */
	    }
	}
    break;

  case 61:

    {
	    if ((yyval.ss) = MakeSynset(SATELLITE, (yyvsp[(2) - (5)].s), NULL, NULL, (yyvsp[(3) - (5)].tx), (yyvsp[(5) - (5)].number))) {
		(yyval.ss)->isfanss = TRUE;
		(yyval.ss)->clusnum = cluscnt;
		AddToSynList((yyval.ss)); /* need since AddSynset() not done */
	    }
	}
    break;

  case 62:

    {
	    yyerror("Opened a '[' in the middle of a cluster somewhere above here\n");
	    (yyval.ss)=(yyvsp[(2) - (2)].ss);
	}
    break;

  case 63:

    {
	    (yyval.ss) = MakeSynset(ADJ, InsertSynonym((yyvsp[(3) - (7)].s), (yyvsp[(2) - (7)].s)), (yyvsp[(4) - (7)].p), NULL, (yyvsp[(5) - (7)].tx), (yyvsp[(7) - (7)].number));
	}
    break;

  case 64:

    {
	    (yyval.ss) = MakeSynset(ADJ, InsertSynonym((yyvsp[(3) - (6)].s), (yyvsp[(2) - (6)].s)), NULL, NULL, (yyvsp[(4) - (6)].tx), (yyvsp[(6) - (6)].number));
	}
    break;

  case 65:

    {
           wdcnt = 1;
           (yyval.s) = CreateSynonym((yyvsp[(1) - (1)].sym),
			      (unsigned char)wsense,
			      -wdcnt,
			      (unsigned char)aclass,
#ifndef NO_XML
			      /* FiWN: Use label if XML in words is ignored */
			      (ignore_xmlwords ? label : NULL)
#else 
			      NULL
#endif
	       );
	   headword = FALSE;
	   aclass = 0;
       }
    break;

  case 66:

    {
	   MakeSyngroup((yyvsp[(3) - (4)].p), NULL);
	   wdcnt = 1;
	   (yyval.s) = CreateSynonym((yyvsp[(2) - (4)].sym),
			      (unsigned char)wsense,
			      -wdcnt,
			      (unsigned char)aclass,
#ifndef NO_XML
			      /* FiWN: Use label if XML in words is ignored */
			      (ignore_xmlwords ? label : NULL)
#else 
			      NULL
#endif
	       );
	   headword = FALSE;
	   aclass = 0;
       }
    break;

  case 67:

    {
	    /* FiWN: Use normalize_word instead of strlower and use label */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (2)].tx)));
	    label = UseLabel(tolower_non_xml((yyvsp[(1) - (2)].tx)));
	    wsense = 0;
    	    aclass = (yyvsp[(2) - (2)].number);
	    headword = TRUE;
	}
    break;

  case 68:

    {
	    /* FiWN: Use normalize_word instead of strlower and use label */
	    (yyval.sym) = CreateSymbol(normalize_word((yyvsp[(1) - (3)].tx)));
	    label = UseLabel(tolower_non_xml((yyvsp[(1) - (3)].tx)));
	    wsense = (yyvsp[(2) - (3)].number);
	    aclass = (yyvsp[(3) - (3)].number);
	    headword = TRUE;
	}
    break;



      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}





static G_Synset MakeSynset(int type, Synonym syns, Pointer ptrs,
			 Framelist frames, char *defn, int key)
{
    register Synonym syn;
    register Pointer p;
    G_Synset ss;
    Framelist f;
    
    wdcnt = 0;			/* reset word counter for next synset */
    lastsyn = NULL;		/* reset pointer to last synonym */

    if (!syns) {
	yywarn("No valid synonyms: synset discarded\n");
	return(NULL);
    } else if (type == VERB && !frames) {
	/* FiWN: Only warn if no verb frame list */
        yywarn("No frame list in verb synset\n");
	/* yyerror("No frame list in verb synset: synset discarded\n"); */
	/* return(NULL); */
    } else if (type == NOUN && foundhyper == FALSE) {
	yywarn("No hypernyms in synset\n");
	if (verifyflag)
	    errcount--;		/* don't count as real error */
    }
    foundhyper = FALSE;

    /* Link in any pointers and frame lists found in syngroups. */
    
    if (ptrlist) {
	if (ptrs) {
	    for (p = ptrs; p->pnext; p = p->pnext);
	    p->pnext = ptrlist;
	} else {
	    ptrs = ptrlist;
	}
	ptrlist = NULL;
    }
    
    if (frlist) {
	if (frames)
	    frames->fnext = frlist;
	else
	    frames = frlist;
	frlist = NULL;
    }

    ss = CreateSynset((unsigned char)type, syns, ptrs, frames, defn, key,
		      yylineno, curfilenum());

    /* Add pointer to the synset to each synonym in it.  Make sure
       that the synonym is unique (using sense number) within the file. */

    for (syn = syns; syn; syn = syn->synnext) {
	if ((type != SATELLITE && !UniqueSynSense(syn, FALSE)) ||
	    (type == SATELLITE && (curfilenum() != 0) &&
	     !UniqueSynSense(syn, TRUE))) {
	    if (syn->sensenum)
		sprintf(buf,
			"Synonym \"%s%d\" is not unique in file\n",
			syn->word->label, syn->sensenum);
	    else
		sprintf(buf,
			"Synonym \"%s\" is not unique in file\n",
			syn->word->label);
	    yyerror(buf);
	}
	syn->ss = ss;
	syn->infanss = (type == SATELLITE) ? TRUE : FALSE;
    }

    return(ss);
}

static Synonym AddSynonym(Synonym syns, Synonym newsyn)
{
    register Synonym s;

    /* Add synonym to end of list.  First make sure this synonym
       is unique within the synset. */
    
    if (UniqueSynonym(syns, newsyn)) {
	if (syns) {
	    for (s = syns; s->synnext; s = s->synnext);	/* find end of list */
	    s->synnext = newsyn;
	} else
	    syns = newsyn;
	
	lastsyn = newsyn;	/* save last synonym in synset for errors */
    } else {
	if (newsyn->sensenum)
	    sprintf(buf,
		    "Synonym \"%s%d\" is not unique in synset\n",
		    newsyn->word->label, newsyn->sensenum);
	else
	    sprintf(buf,
		    "Synonym \"%s\" is not unique in synset\n",
		    newsyn->word->label);
	yyerror(buf);
	free(newsyn);		/* free up space */
    }
    /* FiWN: Use MAXID */
    if (newsyn->sensenum > MAXID) {
	sprintf(buf,
		"Sense number must be less than %d: \"%s%d\" - changed to %d\n",
		MAXID + 1, newsyn->word->label, newsyn->sensenum,
		newsyn->sensenum % (MAXID + 1));
	yywarn(buf);
    }

    return(syns);
}

static Pointer AddPointer(Pointer ptrs, Pointer newptr)
{
    register Pointer p;
    extern int nominalizations;

    /* Discard nominalization pointer if we're skipping nominalizations */

    if (!nominalizations && (newptr->ptype == DERIVATION) ) {
	free(newptr);
	return (ptrs);
    }

    if (UniquePointer(ptrs, newptr)) {
	if (ptrs) {
	    for (p = ptrs; p->pnext; p = p->pnext);	/* find end of list */
	    p->pnext = newptr;
	} else
	    ptrs = newptr;
    } else {
	if (newptr->pfilenum != curfilenum()) {
	    sprintf(buf,
		    "Pointer \"%s:%s\" is not unique: pointer discarded\n",
		    PrintFileName(newptr->pfilenum), PrintPointer(newptr));
	} else {
	    sprintf(buf, "Pointer \"%s\" is not unique: pointer discarded\n",
		    PrintPointer(newptr));
	}
	yywarn(buf);
	free(newptr);		/* free up space */
    }

    return(ptrs);
}

static Synonym InsertSynonym(Synonym syns, Synonym newsyn)
{
    /* Insert synonym onto front of list.  First make sure this synonym
       is unique within the synset. */
    
    if (UniqueSynonym(syns, newsyn)) {
	newsyn->synnext = syns;
	syns = newsyn;
    } else {
	if (newsyn->sensenum)
	    sprintf(buf,
		    "Synonym \"%s%d\" is not unique in synset\n",
		    newsyn->word->label, newsyn->sensenum);
	else
	    sprintf(buf,
		    "Synonym \"%s\" is not unique in synset\n",
		    newsyn->word->label);
	yyerror(buf);
	free(newsyn);		/* free up space */
    }
    return(syns);
}

static void AddSynset(G_Synset ss)
{
    register G_Synset fan;
    register Synonym fansyn;
    int svlineno;
    
    if (headss == NULL)
	headss = ss;
    else
	lastss->ssnext = ss;
    lastss = ss;		/* set current end of list to this synset  */

    AddToSynList(ss);		/* update synlists for words in synset */

    /* If a cluster, make fans point back to head synset and check for
       use of a HEADWORD in a fan. */

    for (fan = ss->fans; fan; fan = fan->ssnext) {
	for (fansyn = fan->syns; fansyn; fansyn = fansyn->synnext)
	    if (!UniqueFanSynonym(fansyn, ss->clusnum, fan->lineno)) {
		sprintf(buf,
			"\"%s\" is not unique in this cluster\n",
			fansyn->word->label);
		svlineno = yylineno;
		yylineno = fan->lineno;
		yyerror(buf);
		yylineno = svlineno;
	    }
	fan->fans = ss;
    }
}    

static void AddToSynList(G_Synset ss)
{
    register Synonym syn;
    register SynList sl;
    int unique;

    /* For all words in this synset, add a pointer to this synset to
       the word's list of pointers to synsets containing the word.
       (confusing, but if you read it slowly, it makes sense) */

    for (syn = ss->syns; syn; syn = syn->synnext) {

	/* First, make sure that a pointer to this word in this
	   synset isn't already on the list.  This can occur if there
	   are multiple spellings of a word in a synset.  If only
	   one occurrence in the synset is added to the list, only
	   one pointer to this synset will be output in the index
	   file, and pointer resolution will be easier since we
	   don't have to weed out multiples. */

	unique = TRUE;

	for (sl = syn->word->syns; sl; sl = sl->snext)
#ifndef NO_XML
	    /* FiWN: If XML is to be ignored in words, also check the sense
	       number when testing for the uniqueness of a synonym */
	    if (sl->psyn->ss == syn->ss
		&& (! ignore_xmlwords || sl->psyn->sensenum == syn->sensenum)) {
#else
	    if (sl->psyn->ss == syn->ss) {
#endif
		unique = FALSE;
		break;
	    }
	if (unique) {
	    if (sl = (SynList)malloc(sizeof(Synl))) {
		sl->psyn = syn;	/* add this synonym to list */
		sl->snext = syn->word->syns;
		syn->word->syns = sl;
	    } else {
		fprintf(logfile, "Cannot malloc space for sslist\n");
		exit(-1);
	    }
	}
    }
}

static void MakeSyngroup(Pointer ptrs, Framelist frame)
{
    register Pointer p, lp;
    register int wc;

    wc = wdcnt + 1;		/* count this word */
    
    /* Set the word count for all pointers specific to this word.
       Note that since the lists of pointers and synonyms are built
       with the last entry first, the word count counts from the
       wrong direction.  Once the number of words in the synset is
       known, the word counts are adjusted. */
    
    if (ptrs) {
	for (p = ptrs; p; p = p->pnext) {
	    p->fromwdnum = (headword ? -wc : wc);
	    lp = p;
	}
	if (!ptrlist) {
	    ptrlist = ptrs;
	} else {
	    lp->pnext = ptrlist;
	    ptrlist = ptrs;
	}
    }
    
    if (frame) {
	frame->frwdnum = wc;
	if (!frlist)
	    frlist = frame;
	else {
	    frame->fnext = frlist;
	    frlist = frame;
	}
    }
}

static Pointer DoPointer(Symbol sym, int ptrtype)
{
    Pointer p = NULL;
    int mode, fn;
    char ptrbuf[100];
    static int pmode[] = { 0, P_NOUN, P_VERB, P_ADJ, P_ADV };
    extern int nominalizations;

   /* Discard nominalization pointer if we're skipping nominalizations */

    if (!nominalizations && (ptrtype == DERIVATION) ) {
	return (NULL);
    }
    
    /* used later to make sure NOUN synset has HYPERNYM */
    if (ptrtype == HYPERPTR || ptrtype == INSTANCE)
	foundhyper = TRUE;

    fn = (fnbuf[0] ? filenum(fnbuf) : curfilenum());
    mode=filemode(fn);

    p = CreatePointer(sym,
		      satellite,
		      (unsigned char)fn,
		      (unsigned char)sense,
		      (unsigned char)satellite_sense,
		      (unsigned char)ptrtype,
		      ALLWORDS, ALLWORDS);

    p->phead = headptr;

    if (mode && (legalptrs[ptrtype] & pmode[mode])) {
	return(p);
    } else {			/* Construct pretty error message & print it */
	if (fnbuf[0])
	    sprintf(ptrbuf, "\"%s:", fnbuf);
	else
	    sprintf(ptrbuf, "\"");

	sprintf(ptrbuf + strlen(ptrbuf), "%s\"", PrintPointer(p));

	if (!mode) { /* Don't print this message if only verifyng files */
	    if (!verifyflag) {
		sprintf(buf,
			"Pointer to unknown file %s: pointer discarded\n",
			ptrbuf);
		yywarn(buf);
	    }
	} else if (!legalptrs[ptrtype]) {
	    sprintf(buf,
		    "Invalid pointer type %s in %s file: pointer discarded\n",
		    ptrbuf, partnames[curfilemode()]);
	    yywarn(buf);
	} else if (!(legalptrs[ptrtype] & pmode[mode])) {
	    sprintf(buf,
		    "%s can't point to a %s file: pointer discarded\n",
		    ptrbuf, partnames[mode]);
	    yywarn(buf);
	} else {
	    sprintf(buf, "Error in pointer %s: pointer discarded\n", ptrbuf);
	    yywarn(buf);
	}
	free(p);
	return(NULL);
    }
}

static int UniqueSynonym(Synonym syns, Synonym syn)
{
    register Synonym s;

    /* check for uniqueness within this synset */

    for (s = syns; s; s = s->synnext)
	if (s->word == syn->word) {
	    if (s->label || syn->label)
		continue;	/* alternate spelling, ok. */
	    else
		return(0);
	}
    return(1);
}

static int UniquePointer(Pointer ptrs, Pointer ptr)
{
    register Pointer p;

    for (p = ptrs; p; p = p->pnext)
	if (p->pword == ptr->pword &&
	    p->psensenum == ptr->psensenum &&
	    p->pfilenum == ptr->pfilenum &&
	    p->ptype == ptr->ptype &&
	    p->fromwdnum == ptr->fromwdnum &&
	    p->towdnum == ptr->towdnum &&
	    p->pslite == ptr->pslite &&
	    p->pslite_sense == ptr->pslite_sense &&
	    p->phead == ptr->phead)
	    return(0);


    return(1);
}

static int UniqueSynSense(Synonym syn, int test)
{
    register SynList sl;
    
    for (sl = syn->word->syns; sl; sl = sl->snext)
	if ((sl->psyn->sensenum == syn->sensenum) &&
	    (sl->psyn->ss->filenum == curfilenum()) &&
	    (sl->psyn->infanss == test))
	    return(0);

    return(1);
}

static int UniqueFanSynonym(Synonym syn, int clusnum, int lineno)
{
    register SynList sl;

    for (sl = syn->word->syns; sl; sl = sl->snext)
	if ((sl->psyn->ss->clusnum == clusnum) &&
	    (sl->psyn->sensenum == syn->sensenum) && 
	    (sl->psyn->ss->lineno != lineno))
#ifdef FOOP
	if ((sl->psyn->ss->part == ADJ) && /* word is an adjective */
	    (!sl->psyn->infanss) && /* word is in a cluster head */
	    (sl->psyn->ss->clusnum == clusnum)) /* word is in same cluster */
#endif
	    return(0);

    return(1);
}

static char *UseLabel(char *s)
{
    register int i = 0;
    register char *ls;

    /* Decide if string is lower case or not */

    /* FiWN: Use normalize_word instead of strlower */
    ls = normalize_word(s);
    while (*(s + i)) {
	if (*(s + i) != *(ls + i))
	    return(s);
	i++;
    }
    return(NULL);
}

/*
 * FiWN: Strip XML tags and element contents from a string.
 */

static char *strip_xml(char *s)
{
    register int i1 = 0;
    register int i2 = 0;
    int taglevel = 0;
    int intag = 0;
    int tagstart = 0;
    int endtag = 0;
    int slash = 0;
    int bs = 0;

    while (*(s + i1)) {
	if (intag) {
	    switch (*(s + i1)) {
	    case '/':
		endtag = (tagstart == i1 - 1 - bs);
		slash = i1;
		bs = 0;
		break;
	    case '>':
		if (endtag && taglevel > 0)
		    taglevel--;
		else if (! endtag && slash != i1 - 1 - bs)
		    taglevel++;
		intag = endtag = slash = bs = 0;
		break;
	    case '\\':
		bs = 1 - bs;
		break;
	    }
	} else {
	    switch (*(s + i1)) {
	    case '<':
		intag = 1;
		tagstart = i1;
		slash = bs = 0;
		break;
	    case '\\':
		if (! bs) {
		    bs = 1;
		    break;
		} else {
		    /* fall through */
		}
	    default:
		if (taglevel == 0) {
		    if (bs) {
			/* Copy backslash */
			*(s + i2) = *(s + i1 - 1);
			i2++;
			bs = 0;
		    }
		    *(s + i2) = *(s + i1);
		    i2++;
		}
	    }
	}
	i1++;
    }
    *(s + i2) = '\0';
    return (s);
}

/*
 * FiWN: Normalize a word by lowercasing it and removing XML if XML is to be
 * ignored.
 */

static char *normalize_word(char *s)
{
    register char *ls;

    ls = strlower(s);
#ifndef NO_XML
    if (ignore_xmlwords)
	strip_xml(ls);
#endif
    return (ls);
}

/*
 * FiWN: Lowercase a string (the parts that are not within XML tags).
 */

static char *tolower_non_xml(char *s)
{
    register char *ls;

    /* FIXME: This converts to lower case everything, not only non-XML parts;
       it works for the current Finnish data, though. */
    ls = strlower(s);
    return (ls);
}

/*
  Revision Log: (since version 1.5)

  $Log: wnparse.y,v $
  Revision 1.48  2005/01/31 20:04:03  wn
  cleaned up and consolidated include files

  Revision 1.47  2004/07/16 15:15:21  wn
  fixed problem with upper case in adj files that are not in clusters

  Revision 1.46  2004/07/14 16:14:46  wn
  only convert upper case adj to lower case if not in adj.all

  Revision 1.45  2004/01/16 18:13:36  wn
  added INSTANCE

  Revision 1.44  2003/10/31 15:33:56  wn
  added code to support DERIVATION link in adj satellites

  Revision 1.43  2003/03/27 15:27:12  wn
  changed NOMIN_START and NOMIN_END with DERIVATION

  Revision 1.42  2001/09/06 17:55:38  wn
  added code for synset keys

  Revision 1.41  2001/02/16 19:52:06  wn
  *** empty log message ***

  Revision 1.40  2000/10/27 16:53:29  wn
  fixed error message

  Revision 1.39  2000/07/11 15:23:59  wn
  added code to AddPointer to skip nominalization pointers if
  !nominalizations (-n not specified)

  Revision 1.38  1997/08/05 14:23:46  wn
  added function prototypes, other small cleanups

 * Revision 1.37  1996/05/30  20:56:48  wn
 * *** empty log message ***
 *
 * Revision 1.36  1995/07/28  21:29:19  wn
 * added check for wsense > MAXID (only 1 hex digit in databae allowed)
 *
 * Revision 1.35  1995/06/16  21:09:34  wn
 * fixed bug with adj head pointers
 *
 * Revision 1.34  1995/05/25  19:00:09  wn
 * added code to find senses taht are not unique in adj.pert files
 *
 * Revision 1.33  1995/02/14  18:50:40  wn
 * fixed bug in UniquePointer
 *
 * Revision 1.32  1995/02/14  18:24:49  wn
 * *** empty log message ***
 *
 * Revision 1.31  1995/02/07  21:40:17  wn
 * fixed bug in fan and changed NOUNSS to NOUN, etc.
 *
 * Revision 1.30  1994/09/27  19:50:46  wn
 * *** empty log message ***
 *
 * Revision 1.29  1994/04/27  09:49:02  wn
 * *** empty log message ***
 *
 * Revision 1.28  94/03/03  11:56:02  wn
 * no changes
 * 
 * Revision 1.27  93/07/08  16:23:35  wn
 * added test to find noun synsets which don't have hypernyms
 * 
 * Revision 1.26  93/04/21  12:40:33  wn
 * added new ATTRIBUTE pointer (=)
 * 
 * Revision 1.1  91/09/11  11:58:56  wn
 * Initial revision
 * 

*/

