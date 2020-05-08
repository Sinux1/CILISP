/*
// Samad Mazarei
// Lab: CiLisp
// Date: 4/21/2020
*/
#include "ciLisp.h"


void yyerror(char *s) {
    fprintf(stdout, "\nERROR: %s\n", s);
    exit(1);
}

void printWarning(char *s) {
    fprintf(stdout, "\nWARNING: %s\n", s);
}

char *typeNames[] = {
        "int",
        "double"
};

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "add",
        "sub",
        "mult",
        "div",
        "remainder",
        "exp",
        "exp2",
        "pow",
        "log",
        "sqrt",
        "cbrt",
        "hypot",
        "max",
        "min",
        "print",
        "read",
        "rand",
        "equal",
        "less",
        "greater",
        ""
};

OPER_TYPE resolveFunc(char *funcName) {
    int i = 0;
    while (funcNames[i][0] != '\0') {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode(double value, NUM_TYPE type) {
    AST_NODE *node;
    size_t nodeSize;
    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    node = (AST_NODE *)calloc(1, nodeSize);
    if ((node) == NULL)
    {
        yyerror("Memory allocation failed!");

    }

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE
    node->type = NUM_NODE_TYPE;
    node->data.number.type = type;
    node->data.number.value = value;

    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList) {
    // TODO allocate space for the node being created.
    AST_NODE *node;
    size_t nodeSize;
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");


    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER.
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here.
    // The funcName will be a string identifier for which space should be allocated in the tokenizer.
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you've assigned the OPER_TYPE.

    // Sets type
    node->type = FUNC_NODE_TYPE;

    // Assign reference to oplist
    node->data.function.opList = opList;
    // Make sure there is an oplist before trying to set parents
    if (opList != NULL) {
        AST_NODE *lnode = node->data.function.opList;

        // Assign every operand in oplist the same parent - the AST_NODE being returned
        lnode->parent = node;
        while (lnode->next != NULL) {
            lnode = lnode->next;
            lnode->parent = node;
        }

    }

    // Assign function operation, using assignment return value to test if custom op,
    // assigning ident if it is and returning node.
    if ((node->data.function.oper = resolveFunc(funcName)) == CUSTOM_OPER) {
        node->data.function.ident = funcName;
        return node;
    }

    // If not custom op, free funcName
    free(funcName);
    return node;
}

// Receives an AST_NODE *list (an s_expr_list) and an
// AST_NODE *newHead (the new element to add to the list as
// its head). Links newHead up to list, with newHead as the head,
// and returns the head. That is, prepends newHead to the list.
AST_NODE *addOperandToList(AST_NODE *newHead, AST_NODE *list) {
    // TODO
    // Point the new head at the old head (list)
    newHead->next = list;
    // return the newHead
    return newHead;
}

// Evaluates an AST_NODE whose type is NUM_NODE_TYPE.
// Called by the eval function, which evaluates any AST_NODE.
// Returns a RET_VAL with the data stored in the contained NUMBER_AST_NODE.
// SEE: AST_NODE, NUM_AST_NODE, RET_VAL
RET_VAL evalNumNode(AST_NODE *node) {
    if (!node)
        return DEFAULT_RET_VAL;

    RET_VAL result = DEFAULT_RET_VAL;

    // TODO populate result with the values stored in the node.
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE
    result.value = node->data.number.value;
    result.type = node->data.number.type;
    return result;
}

// Evaluates an AST_NODE whose type is FUNC_NODE_TYPE.
// Called by the eval function, which evaluates any AST_NODE.
// Returns the result of running the referenced function on
// the referenced operands (as a RET_VAL);
// SEE: AST_NODE, FUNC_AST_NODE, OPER_TYPE, RET_VAL
// You should create more functions for each OPER_TYPE member,
// which take as input the opList and which output the result
// of performing the specified operation on that opList.
// You should then call the appropriate function in evalFuncNode
// based on the contents of the argument.
RET_VAL evalFuncNode(AST_NODE *node) {
    if (!node)
        return DEFAULT_RET_VAL;

    RET_VAL result = DEFAULT_RET_VAL;

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE
    switch (node->data.function.oper) {
        case NEG_OPER:
            result = neg_op(node->data.function.opList);
            break;
        case ABS_OPER:
            result = abs_op(node->data.function.opList);
            break;
        case ADD_OPER:
            result = add_op(node->data.function.opList);
            break;
        case SUB_OPER:
            result = sub_op(node->data.function.opList);
            break;
        case MULT_OPER:
            result = mult_op(node->data.function.opList);
            break;
        case DIV_OPER:
            result = div_op(node->data.function.opList);
            break;
        case REMAINDER_OPER:
            result = remaind_op(node->data.function.opList);
            break;
        case EXP_OPER:
            result = exp_op(node->data.function.opList);
            break;
        case EXP2_OPER:
            result = exp2_op(node->data.function.opList);
            break;
        case POW_OPER:
            result = pow_op(node->data.function.opList);
            break;
        case LOG_OPER:
            result = log_op(node->data.function.opList);
            break;
        case SQRT_OPER:
            result = sqrt_op(node->data.function.opList);
            break;
        case CBRT_OPER:
            result = cbrt_op(node->data.function.opList);
            break;
        case HYPOT_OPER:
            result = hypot_op(node->data.function.opList);
            break;
        case MAX_OPER:
            result = max_op(node->data.function.opList);
            break;
        case MIN_OPER:
            result = min_op(node->data.function.opList);
            break;
        case PRINT_OPER:
            result = print_op(node->data.function.opList);
            break;
        case READ_OPER:
            result = read_op(node->data.function.opList);
            break;
        case RAND_OPER:
            result = rand_op();
            break;
        case EQUAL_OPER:
            result = equal_op(node->data.function.opList);
            break;
        case GREATER_OPER:
            result = greater_op(node->data.function.opList);
            break;
        case LESS_OPER:
            result = less_op(node->data.function.opList);
            break;
        default:
            puts("This error is in evalFuncNode()");
    }

    return result;
}

// Evaluate an AST_NODE of Symbol type.
// Called by eval function. To evaluate, the symbol nodes symbol table is iterated over
// for a sybol table node with an id that matches. If found, the AST_NODE pointed to by
// the table nodes value pointer is evaluated. If table is NULL, or a match is not found,
// symbol table of the parent node is searched for a match. This is repeated for each parent
// until a match is found or parent node is NULL.
RET_VAL evalSymbolNode(AST_NODE *node) {
    // Symbol we are looking for
    char *sid = node->data.symbol.id;
    // searching the most recent scope first
    AST_NODE *parent = node;
    // Declare table pointer
    SYMBOL_TABLE_NODE *table;
    // Declare record pointer
    SYMBOL_TABLE_NODE *record;
    // If symbol never resolves, initialize default value
    RET_VAL result = DEFAULT_RET_VAL;
    do {
        // Initialize table and record
        table = parent->symbolTable;
        record = table;
        // Iterate over table comparing record to sid
        while (record != NULL) {
            // Condition resolves to 0 (False) when strings are equal
            // so !(strcmp()) will resolve to true when equal
            if (!(strcmp(record->id, sid))) {
                // call to eval will resolve the value node
                result = eval(record->value);
                // If symbol type has been cast, set type accordingly
                if (record->type != NO_TYPE) {
                    if (record->type == INT_TYPE && result.type == DOUBLE_TYPE) {
                        // Precision loss warning
                        printf("WARNING: precision loss on int cast from %g to %d for variable %s.\n",
                               result.value, (int) result.value, sid);
                    }
                    // set appropriate type
                    result.type = record->type;
                }
                return result;
            }
            // Check next record
            record = record->next;
        }
        // No match in current table or current symbol table is NULL,
        // move up to parent node and try again
        parent = parent->parent;
    } while (parent != NULL);
    // If the next line executes it is because the symbol was not found in the table
    puts("Undefined Symbol!");

    return result;
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node) {
    if (!node)
        return DEFAULT_RET_VAL;

    RET_VAL result = DEFAULT_RET_VAL; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type) {
        case NUM_NODE_TYPE:
            result = evalNumNode(node);
            break;
        case FUNC_NODE_TYPE:
            result = evalFuncNode(node);
            break;
        case SYM_NODE_TYPE:
            result = evalSymbolNode(node);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}

// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val) {
    // TODO print the type and value of the value passed in.
    if (isnan(val.value)) {
        // Interesting that the example calls for integer when NAN type is a double!?
        // To match example output for task, where there were no -nan values, I
        // kept getting -nan when op1 was < 0 and op2 is finite non integer,
        // so I explicitly passed NAN to printf.
        printf("INTEGER : %lf\n", NAN);
    } else if (val.type == INT_TYPE) {
        // Casting to type int from double
        printf("INTEGER : %d\n", (int) val.value);
    } else if (val.type == DOUBLE_TYPE) {
        printf("DOUBLE: %lf\n", val.value);
    }


}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees an abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
// The TODOs below are for task 1, you will need to expand freeNode with
// most of the subsequent tasks.
void freeNode(AST_NODE *node) {
    if (!node)
        return;

    // TODO if the node's next pointer isn't NULL,
    if (node->next != NULL) {
        freeNode(node->next);
    }

    if(node->symbolTable != NULL)
    {
        freeSymbolTableRecursive(node->symbolTable);
    }

    // TODO:
    // if the node is a function node:
    if (node->type == FUNC_NODE_TYPE) {
        // make a recursive call to free its opList if not NULL
        if(node->data.function.opList != NULL){
            freeNode(node->data.function.opList);
        }

    }
    if(node->type ==SYM_NODE_TYPE)
    {
        free(node->data.symbol.id);
    }
    free(node);
}

// Negates the value of the first operand
RET_VAL neg_op(AST_NODE *oplist) {
    RET_VAL result;
    if (oplist == NULL) {
        puts("ERROR: neg called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    // evaluate the head of the oplist, number or function and assign to object.jkkjj
    RET_VAL op1 = eval(oplist);
    if (oplist->next != NULL) {
        puts("WARNING: neg called with extra (ignored) operands!");
    }
    result.type = op1.type;
    result.value = -op1.value;
    return result;
}

RET_VAL abs_op(AST_NODE *oplist) {
    RET_VAL result;
    if (oplist == NULL) {
        puts("ERROR: abs called with no args.");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    // evaluate the head of the oplist, number or function and assign to object.jkkjj
    RET_VAL op1 = eval(oplist);
    if (oplist->next != NULL) {
        puts("WARNING: abs call with extra operands. Only first operand used!");
    }
    result.type = op1.type;
    result.value = (op1.value < 0) ? -op1.value : op1.value;
    return result;
}

RET_VAL add_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op1;
    if (oplist == NULL) {
        puts("WARNING: add call with no operands, 0 returned!");
        result.type = INT_TYPE;
        result.value = 0;
        return result;
    }
    AST_NODE *nextop = oplist;
    op1 = eval(nextop);
    double sum = op1.value;
    result.type = op1.type;
    while (nextop->next != NULL) {
        nextop = nextop->next;
        op1 = eval(nextop);
        if (op1.type == DOUBLE_TYPE) {
            result.type = DOUBLE_TYPE;
        }
        sum += op1.value;
    }
    result.value = sum;
    return result;

}

RET_VAL sub_op(AST_NODE *oplist) {
    RET_VAL result;
    if (oplist == NULL) {
        puts("ERROR: sub called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    } else if (oplist->next == NULL) {
        puts("ERROR: sub called with only one arg!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    // evaluate the head of the oplist, number or function and assign to object.jkkjj
    RET_VAL op1 = eval(oplist);
    RET_VAL op2 = eval(oplist->next);
    if (oplist->next->next != NULL) {
        puts("WARNING: sub called with extra (ignored) operands!");
    }
    // Ternary op to assign type to result object - INT_TYPE resolves to 0, 0 resolves to false,
    // If the conditional below resolves to 0, resolves to false, and INT_TYPE is assigned. If either are not 0,
    // then it resolves to true, and DOUBLE_TYP is assigned. 0||0 is 0, 0||1 is 1.
    result.type = (op1.type || op2.type) ? DOUBLE_TYPE
                                         : INT_TYPE;
    result.value = op1.value - op2.value;

    return result;
}

RET_VAL mult_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op1;
    if (oplist == NULL) {
        puts("WARNING: mult call with no operands, 1 returned!");
        result.type = INT_TYPE;
        result.value = 1;
        return result;
    }
    AST_NODE *nextop = oplist;
    op1 = eval(nextop);
    double product = op1.value;
    result.type = op1.type;
    while (nextop->next != NULL) {
        nextop = nextop->next;
        op1 = eval(nextop);
        if (op1.type == DOUBLE_TYPE) {
            result.type = DOUBLE_TYPE;
        }
        product *= op1.value;
    }
    result.value = product;
    return result;
}

RET_VAL div_op(AST_NODE *oplist) {
    RET_VAL result;
    if (oplist == NULL) {
        puts("ERROR: div called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    } else if (oplist->next == NULL) {
        puts("ERROR: div called with only one arg!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    RET_VAL op1 = eval(oplist), op2 = eval(oplist->next);
    if (oplist->next->next != NULL) {
        puts("WARNING: div called with extra (ignored) operands!");
    }
    // Ternary op to assign type to result object - INT_TYPE resolves to 0, 0 resolves to false,
    // If the conditional below resolves to 0, resolves to false, and INT_TYPE is assigned. If either are not 0,
    // then it resolves to true, and DOUBLE_TYP is assigned. 0||0 is 0, 0||1 is 1.
    result.type = (op1.type || op2.type) ? DOUBLE_TYPE : INT_TYPE;
    if (isnan(result.value = (op2.value) ? op1.value / op2.value : NAN))
        puts("Division by zero error");
    return result;
}

RET_VAL remaind_op(AST_NODE *oplist) {
    RET_VAL result;
    if (oplist == NULL) {
        puts("ERROR: remainder called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    } else if (oplist->next == NULL) {
        puts("ERROR: remainder called with only one arg!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    RET_VAL op1 = eval(oplist), op2 = eval(oplist->next);
    if (oplist->next->next != NULL) {
        puts("WARNING: remainder called with extra (ignored) operands!");
    }
    // Ternary op to assign type to result object - INT_TYPE resolves to 0, 0 resolves to false,
    // If the conditional below resolves to 0, resolves to false, and INT_TYPE is assigned. If either are not 0,
    // then it resolves to true, and DOUBLE_TYP is assigned. 0||0 is 0, 0||1 is 1.
    result.type = (op1.type || op2.type) ? DOUBLE_TYPE : INT_TYPE;
    result.value = remainder(op1.value, op2.value);
    if (result.value < 0) {
        result.value += op2.value;
    }

    return result;
}

RET_VAL exp_op(AST_NODE *oplist) {
    RET_VAL result;
    if (oplist == NULL) {
        puts("ERROR: exp called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    RET_VAL op1 = eval(oplist);
    if (oplist->next != NULL) {
        puts("WARNING: exp called with extra (ignored) operands!");
    }
    result.type = DOUBLE_TYPE;
    result.value = exp(op1.value);
    return result;
}

RET_VAL exp2_op(AST_NODE *oplist) {
    RET_VAL result;
    if (oplist == NULL) {
        puts("ERROR: exp2 called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }

    RET_VAL op1 = eval(oplist);
    if (oplist->next != NULL) {
        puts("WARNING: exp2 called with extra (ignored) operands!");
    }
    // Ternary operator. If op1 is positive and INT, return INT type, otherwise returns DOUBLE type
    result.type = (op1.value >= 0 && op1.type == INT_TYPE) ? INT_TYPE : DOUBLE_TYPE;
    result.value = pow(2, op1.value);

    return result;
}

RET_VAL pow_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op1, op2;
    if (oplist == NULL) {
        puts("ERROR: pow called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    } else if (oplist->next == NULL) {
        puts("ERROR: pow called with only one arg!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    op1 = eval(oplist), op2 = eval(oplist->next);
    if (oplist->next->next != NULL) {
        puts("WARNING: pow called with extra (ignored) operands!");
    }
    result.type = (op1.type || op2.type) ? DOUBLE_TYPE
                                         : INT_TYPE;
    result.value = pow(op1.value, op2.value);
    return result;
}

RET_VAL log_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op1;
    if (oplist == NULL) {
        puts("ERROR: log called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    op1 = eval(oplist);
    if (oplist->next != NULL) {
        puts("WARNING: log called with extra (ignored) operands!");
    }
    result.type = DOUBLE_TYPE;
    result.value = log(op1.value);
    return result;
}

RET_VAL sqrt_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op1;
    if (oplist == NULL) {
        puts("ERROR: sqrt called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    op1 = eval(oplist);
    if (oplist->next != NULL) {
        puts("WARNING: sqrt called with extra (ignored) operands!");
    }
    result.type = DOUBLE_TYPE;
    result.value = sqrt(op1.value);
    return result;
}

RET_VAL cbrt_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op1;
    if (oplist == NULL) {
        puts("ERROR: cbrt called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    op1 = eval(oplist);
    if (oplist->next != NULL) {
        puts("WARNING: cbrt called with extra (ignored) operands!");
    }
    result.type = DOUBLE_TYPE;
    result.value = cbrt(op1.value);
    return result;
}

RET_VAL hypot_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op1;
    if (oplist == NULL) {
        puts("ERROR: hypot called with no operands, 0.0 returned!");
        result.type = DOUBLE_TYPE;
        result.value = 0;
        return result;
    }

    AST_NODE *nextop = oplist;
    op1 = eval(nextop);
    double soq = op1.value * op1.value;
    while (nextop->next != NULL) {
        nextop = nextop->next;
        op1 = eval(nextop);
        soq += op1.value * op1.value;
    }
    result.type = DOUBLE_TYPE;
    result.value = sqrt(soq);
    return result;
}

RET_VAL max_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op;
    double maxVal;
    if (oplist == NULL) {
        puts("ERROR: max called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    AST_NODE *nextop = oplist;
    op = eval(nextop);
    result.type = op.type;
    maxVal = op.value;
    while (nextop->next != NULL) {

        nextop = nextop->next;
        op = eval(nextop);
        if (maxVal < op.value) {
            maxVal = op.value;
            result.type = op.type;
        }
    }
    result.value = maxVal;
    return result;
}

RET_VAL min_op(AST_NODE *oplist) {
    RET_VAL result;
    RET_VAL op;
    double minVal;
    if (oplist == NULL) {
        puts("ERROR: min called with no operands!");
        result.type = INT_TYPE;
        result.value = NAN;
        return result;
    }
    AST_NODE *nextop = oplist;
    op = eval(nextop);
    result.type = op.type;
    minVal = op.value;
    while (nextop->next != NULL) {

        nextop = nextop->next;
        op = eval(nextop);
        if (minVal > op.value) {
            minVal = op.value;
            result.type = op.type;
        }
    }
    result.value = minVal;
    return result;
}

RET_VAL rand_op() {
    RET_VAL result;
    result.type = DOUBLE_TYPE;
    // This mapping allows for RAND_MAX + 1 possible return values
    // between 0 and 1 inclusive
    result.value = rand() / ((double) RAND_MAX);
    return result;
}

// Create Symbol Node- Receives a string from tokenizer
// Sets node type and symbol id
AST_NODE *createSymbolNode(char *id) {
    AST_NODE *node;
    size_t nodeSize;
    nodeSize = sizeof(AST_NODE);
    node = (AST_NODE *)calloc(nodeSize, 1);
    if ((node) == NULL) {
        yyerror("Memory allocation failed!");
    }
    node->type = SYM_NODE_TYPE;
    node->data.symbol.id = id;
    return node;
}

// Receives a limked list of records and and ast node,
// pointing the symbol table ptr at the list
// then iterating over the list to set all the ast node value's parents to node
AST_NODE *assignSymbolTable(SYMBOL_TABLE_NODE *record, AST_NODE *node) {

    if (record != NULL) {
        SYMBOL_TABLE_NODE *cRecord = record;
        while (cRecord != NULL) {
            if (cRecord->value != NULL)
                cRecord->value->parent = node;
            cRecord = cRecord->next;
        }
        node->symbolTable = record;
    }
    return node;

}


// Adds newHead to head list of sym table nodes
SYMBOL_TABLE_NODE *addRecordToList(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *newHead) {
    newHead->next = list;
    return newHead;
}

// Receives a string from SYMBOL token and creates an AST node
// pointing *value at the argument node
SYMBOL_TABLE_NODE *createSymbolTableNode(char *type, char *id, AST_NODE *node) {
    SYMBOL_TABLE_NODE *tnode;
    size_t nodeSize;
    AST_NODE *numberNode = (AST_NODE *) calloc(1, sizeof(AST_NODE));
    numberNode->type = NUM_NODE_TYPE;
    numberNode->data.number = eval(node);
    freeNode(node);
    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if ((tnode = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    if (type == NULL) {
        tnode->type = NO_TYPE;
    } else {
        tnode->type = (strcmp(type, typeNames[INT_TYPE]) == 0) ? INT_TYPE : DOUBLE_TYPE;
    }

    tnode->id = id;
    tnode->value = numberNode;

    return tnode;
}

AST_NODE *decideConditional(AST_NODE *condition, AST_NODE *sExpr1, AST_NODE *sExpr2) {

    if(eval(condition).value){
        freeNode(condition);
        freeNode(sExpr2);
        return sExpr1;
    }
    else{
        freeNode(condition);
        freeNode(sExpr1);
        return sExpr2;
    }
}

// Recieves two operands and compares them. If they are quantitatively equal,
// returns integer 1. Otherwise, 0
RET_VAL equal_op(AST_NODE *oplist) {
    RET_VAL result;
    result.type = INT_TYPE;
    // Make sure there are at least two operands
    if (oplist == NULL || oplist->next == NULL) {
        puts("Error: equal is a binary function and requires two operands\nReturning false");
        result.value = 0;
        return result;
    }else if(oplist->next->next != NULL)
    {
        puts("WARNING: equal called with extra (ignored) operands!.");
    }
    // Using ternary operator to check if operands are equal, returning 1 if they are, 0 otherwise.
    result.value = (eval(oplist).value == eval(oplist->next).value) ? 1 : 0;
    return result;
}

RET_VAL greater_op(AST_NODE *oplist) {
    RET_VAL result;
    result.type = INT_TYPE;
    // Make sure there are at least two operands
    if (oplist == NULL || oplist->next == NULL) {
        puts("Error: greater is a binary function and requires two operands\nReturning false");
        result.value = 0;
        return result;
    }else if(oplist->next->next != NULL)
    {
        puts("WARNING: greater called with extra (ignored) operands!.");
    }
    // Using ternary operator to check if operands are equal, returning 1 if they are, 0 otherwise.
    result.value = (eval(oplist).value > eval(oplist->next).value) ? 1 : 0;
    return result;
}

RET_VAL less_op(AST_NODE *oplist) {
    RET_VAL result;
    result.type = INT_TYPE;
    // Make sure there are at least two operands
    if (oplist == NULL || oplist->next == NULL) {
        puts("Error: equal is a binary function and requires two operands\nReturning false");
        result.value = 0;
        return result;
    }else if(oplist->next->next != NULL)
    {
        puts("WARNING: equal called with extra (ignored) operands!.");
    }
    // Using ternary operator to check if operands are equal, returning 1 if they are, 0 otherwise.
    result.value = (eval(oplist).value < eval(oplist->next).value) ? 1 : 0;
    return result;
}

RET_VAL print_op(AST_NODE *oplist)
{
    RET_VAL result;
    AST_NODE *operand = oplist;

    if (operand == NULL) {
        puts("ERROR: print called with no operands!");
        return DEFAULT_RET_VAL;
    }
    // If there are at least two operands, iterate through them,
    // evaluating and printing the result until the last operand is
    // reached
    while(operand->next != NULL){
        result = eval(operand);
        if(result.type == INT_TYPE)
        {
            printf("INTEGER : %d\n", (int)result.value);
        } else{
            printf("DOUBLE : %lf\n", result.value);
        }
        operand = operand->next;
    }
    // Print last operand type and value to stdout
    result = eval(operand);
    if(result.type == INT_TYPE)
    {
        printf("INTEGER : %d\n", (int)result.value);
    } else{
        printf("DOUBLE : %lf\n", result.value);
    }
    return result;
}

RET_VAL read_op()
{
    RET_VAL result = DEFAULT_RET_VAL;
    char *input =  (char*)malloc(bufferSize* sizeof(char));

    do{
        printf("read :: ");
        scanf("%s", input); getchar();
        if(input != NULL){
            result = validateToken(input);
        }
        if(isnan(result.value))
            printf("Please enter a valid int or double\n");
    }while(isnan(result.value));
    free(input);
    return result;
}

// If token is an int or double, return value and type, otherwise return nan
RET_VAL validateToken(char *token)
{
    if(token == NULL)
    {
        return DEFAULT_RET_VAL;
    }
    RET_VAL result;
    int index = 0;
    bool isDouble = false;
    int nNegSigns = 0;
    int nSigns = 0;
    bool prefix = true;

    while(prefix)
    {
        if(token[index] == '-')
        {
            nNegSigns++; nSigns++; index++;
        }else if(token[index] == '+')
        {
            nSigns++; index++;
            continue;
        } else{
            prefix = false;
        }
    }

    char *bString = (char *) malloc((bufferSize - nSigns) * sizeof(char) + 1 );


    while(index < strlen(token))
    {
        // Iterating over token, ensuring all characters are digits
        // and if they are not, they can only be a '.' char, and only if one has
        // not already appeared - d+\.d+
        if(isdigit(token[index]))
        {
            bString[index - nSigns] = token[index];
            index++;
            continue;
        }else if(token[index] == '.' && !isDouble)
        {
            bString[index - nSigns] = token[index];
            index++;
            isDouble = true;
            continue;
        } else{
            return DEFAULT_RET_VAL;
        }
    }
    // If while loop breaks with isDouble true, then it is a double, otherwise an int
    result.type = (isDouble) ? DOUBLE_TYPE: INT_TYPE;
    // strtod converts string to double rep
    result.value = (nNegSigns % 2 == 1)? -strtod(bString, NULL):strtod(bString, NULL);
    free(bString);
    return result;
}

void freeSymbolTableRecursive(SYMBOL_TABLE_NODE *tnode) {
    if(tnode->next != NULL)
    {
        freeSymbolTableRecursive(tnode->next);
    }
    freeNode(tnode->value);
    free(tnode->id);
    free(tnode);
}


// debug function
void showme(SYMBOL_TABLE_NODE *stnode) {
    printf("%s, %lf\n", stnode->id, stnode->value->data.number.value);
}



