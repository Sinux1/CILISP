#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "ciLispParser.h"

#define DEFAULT_RET_VAL (RET_VAL){INT_TYPE, NAN}

FILE* flex_bison_log_file;

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

// My function eval function signatures
double neg(double);
double
OPER_TYPE resolveFunc(char *);

// Types of numeric values
typedef enum {
    INT_TYPE,
    DOUBLE_TYPE
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
    FUNC_NODE_TYPE
} AST_NODE_TYPE;

// Generic Abstract Syntax Tree node. Stores the type of node,
// and reference to the corresponding specific node (initially a number or function call).
typedef struct ast_node {
    AST_NODE_TYPE type;
    union {
        NUM_AST_NODE number;
        FUNC_AST_NODE function;
    } data;
    struct ast_node *next; // for linked-list style operand inputs
} AST_NODE;


AST_NODE *createNumberNode(double value, NUM_TYPE type);
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList);

AST_NODE *addOperandToList(AST_NODE *newHead, AST_NODE *list);

RET_VAL eval(AST_NODE *node);

void printRetVal(RET_VAL val);

void freeNode(AST_NODE *node);

#endif
