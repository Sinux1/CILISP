/*
// Samad Mazarei
// Lab: CiLisp
// Date: 4/21/2020
*/
#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#include "ciLispParser.h"

#define DEFAULT_RET_VAL (RET_VAL){INT_TYPE, NAN}
#define DEFAULT_ADD_OP_NOARGS (RET_VAL){INT_TYPE, 0}
#define DEFAULT_MULT_OP_NOARGS (RET_VAL){INT_TYPE, 1}
#define DEFAULT_HYPOT_OP_NOARGS (RET_VAL){DOUBLE_TYPE, 0}


#define bufferSize 20
#define bufferSizestr "20"

FILE *flex_bison_log_file;

int yyparse(void);

int yylex(void);

void yyerror(char *);

// Enum of all operators.
// must be in sync with funcs in resolveFunc()
typedef enum oper {
    NEG_OPER,
    ABS_OPER,
    ADD_OPER,
    SUB_OPER,
    MULT_OPER,
    DIV_OPER,
    REMAINDER_OPER,
    EXP_OPER,
    EXP2_OPER,
    POW_OPER,
    LOG_OPER,
    SQRT_OPER,
    CBRT_OPER,
    HYPOT_OPER,
    MAX_OPER,
    MIN_OPER,
    PRINT_OPER,
    READ_OPER,
    RAND_OPER,
    EQUAL_OPER,
    LESS_OPER,
    GREATER_OPER,
    CUSTOM_OPER
} OPER_TYPE;


OPER_TYPE resolveFunc(char *);

// Types of numeric values
typedef enum {
    INT_TYPE,
    DOUBLE_TYPE,
    NO_TYPE
} NUM_TYPE;

// Node to store a number.
typedef struct {
    NUM_TYPE type;
    double value;
} NUM_AST_NODE;

// Values returned by eval function will be numbers with a type.
// They have the same structure as a NUM_AST_NODE.
// The line below allows us to give this struct another name for readability.
typedef NUM_AST_NODE RET_VAL;

typedef struct {
    char *id;
} SYM_AST_NODE;


// Node to store a function call with its inputs
typedef struct {
    OPER_TYPE oper;
    char *ident; // only needed for custom functions
    struct ast_node *opList;
} FUNC_AST_NODE;

// Types of Abstract Syntax Tree nodes.
// Initially, there are only numbers and functions.
// You will expand this enum as you build the project.
typedef enum {
    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYM_NODE_TYPE
} AST_NODE_TYPE;


// Generic Abstract Syntax Tree node. Stores the type of node,
// and reference to the corresponding specific node (initially a number or function call).
typedef struct ast_node {
    AST_NODE_TYPE type;
    struct ast_node *parent;
    struct symbol_table_node *symbolTable;
    struct symbol_table_node *arglist;
    union {
        NUM_AST_NODE number;
        FUNC_AST_NODE function;
        SYM_AST_NODE symbol;
    } data;
    struct ast_node *next;
} AST_NODE;

typedef enum {
    VAR_TYPE,
    LAMBDA_TYPE,
    ARG_TYPE
} SYMBOL_TYPE;

typedef struct symbol_table_node {
    NUM_TYPE type;
    char *id;
    AST_NODE *value;
    SYMBOL_TYPE symbolType;
    struct stack_node *stack;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

typedef struct stack_node {
    RET_VAL value;
    struct stack_node *next;
} STACK_NODE;

SYMBOL_TABLE_NODE *createArgTableNode(char *id);

AST_NODE *createNumberNode(double value, NUM_TYPE type);

AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList);

AST_NODE *createSymbolNode(char *id);

SYMBOL_TABLE_NODE *createSymbolTableNode(char *type, char *id, AST_NODE *node);

SYMBOL_TABLE_NODE *createLambdaTableNode(char *type, char *id, SYMBOL_TABLE_NODE *args, AST_NODE *node);

AST_NODE *assignSymbolTable(SYMBOL_TABLE_NODE *record, AST_NODE *node);

SYMBOL_TABLE_NODE *addRecordToList(SYMBOL_TABLE_NODE *newNode, SYMBOL_TABLE_NODE *head);

AST_NODE *addOperandToList(AST_NODE *newHead, AST_NODE *list);

SYMBOL_TABLE_NODE *addArgToList(char *id, SYMBOL_TABLE_NODE *list);

AST_NODE *decideConditional(AST_NODE *condition, AST_NODE *sExpr1, AST_NODE *sExpr2);

RET_VAL eval(AST_NODE *node);

void printRetVal(RET_VAL val);

void freeNode(AST_NODE *node);

RET_VAL neg_op(AST_NODE *oplist);

RET_VAL abs_op(AST_NODE *oplist);

RET_VAL add_op(AST_NODE *oplist);

RET_VAL sub_op(AST_NODE *oplist);

RET_VAL mult_op(AST_NODE *oplist);

RET_VAL div_op(AST_NODE *oplist);

RET_VAL remaind_op(AST_NODE *oplist);

RET_VAL exp_op(AST_NODE *oplist);

RET_VAL exp2_op(AST_NODE *oplist);

RET_VAL pow_op(AST_NODE *oplist);

RET_VAL log_op(AST_NODE *oplist);

RET_VAL sqrt_op(AST_NODE *oplist);

RET_VAL cbrt_op(AST_NODE *oplist);

RET_VAL hypot_op(AST_NODE *oplist);

RET_VAL max_op(AST_NODE *oplist);

RET_VAL min_op(AST_NODE *oplist);

RET_VAL rand_op();

RET_VAL equal_op(AST_NODE *oplist);

RET_VAL greater_op(AST_NODE *oplist);

RET_VAL less_op(AST_NODE *oplist);

RET_VAL print_op(AST_NODE *oplist);

RET_VAL read_op();

RET_VAL addTailRecursion(double st, NUM_TYPE sty, AST_NODE *oplist);

RET_VAL multTailRecursion(double st, NUM_TYPE sty, AST_NODE *oplist);

RET_VAL maxTailRecursion(double st, NUM_TYPE sty, AST_NODE *oplist);

RET_VAL minTailRecursion(double st, NUM_TYPE sty, AST_NODE *oplist);

RET_VAL hypotTailRecursion(double st, AST_NODE *oplist);

void freeSymbolTableRecursive(SYMBOL_TABLE_NODE *tnode);

RET_VAL customFunction_op(AST_NODE *node);

// Validates input token, returning type and value for valid token, otherwise nan
RET_VAL validateToken(char *token);


#endif
