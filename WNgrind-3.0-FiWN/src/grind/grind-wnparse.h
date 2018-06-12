
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

extern YYSTYPE yylval;


