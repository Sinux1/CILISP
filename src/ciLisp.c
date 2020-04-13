#include "ciLisp.h"

void yyerror(char *s) {
    fprintf(stdout, "\nERROR: %s\n", s);
    exit(1);
}

void printWarning(char *s) {
    fprintf(stdout, "\nWARNING: %s\n", s);
}

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

OPER_TYPE resolveFunc(char *funcName)
{
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
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
AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

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
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList)
{
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
    // Assign function operation, using assignment return value to test if custom op,
    // assigning ident if it is and returning node.
    if((node->data.function.oper = resolveFunc(funcName)) == CUSTOM_OPER)
    {
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
AST_NODE *addOperandToList(AST_NODE *newHead, AST_NODE *list)
{
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
RET_VAL evalNumNode(AST_NODE *node)
{
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
RET_VAL evalFuncNode(AST_NODE *node)
{
    if (!node)
        return DEFAULT_RET_VAL;

    RET_VAL result = DEFAULT_RET_VAL;

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE
    switch(node->data.function.oper){
        case NEG_OPER:
            result = neg_op(*node->data.function.opList);
            break;
        case ABS_OPER:
            result = abs_op(*node->data.function.opList);
            break;
        case ADD_OPER:
            result = add_op(*node->data.function.opList);
            break;
        case SUB_OPER:
            result = sub_op(*node->data.function.opList);
            break;
        case MULT_OPER:
            result = mult_op(*node->data.function.opList);
            break;
        case DIV_OPER:
            result = div_op(*node->data.function.opList);
        case REMAINDER_OPER:
            result = remaind_op(*node->data.function.opList);
            break;
        case EXP_OPER:
            result = exp_op(*node->data.function.opList);
            break;
        case EXP2_OPER:
            result = exp2_op(*node->data.function.opList);
            break;
        case POW_OPER:
            result = pow_op(*node->data.function.opList);
            break;
        case LOG_OPER:
            result = log_op(*node->data.function.opList);
            break;
        case SQRT_OPER:
            result = sqrt_op(*node->data.function.opList);
            break;
        case CBRT_OPER:
            result = cbrt_op(*node->data.function.opList);
            break;
        case HYPOT_OPER:
            result = hypot_op(*node->data.function.opList);
            break;
        case MAX_OPER:
            result = max_op(*node->data.function.opList);
            break;
        case MIN_OPER:
            result = min_op(*node->data.function.opList);
            break;
        default:
            puts("This error is in evalFuncNode()");
    }

    return result;
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node)
{
    if (!node)
        return DEFAULT_RET_VAL;

    RET_VAL result = DEFAULT_RET_VAL; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type)
    {
        case NUM_NODE_TYPE:
            evalNumNode(node);
            break;
        case FUNC_NODE_TYPE:
            evalFuncNode(node);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}


// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
    // TODO print the type and value of the value passed in.
    printf("TYPE: %s VALUE: %lf\n", (val.type)? "INTEGER": "DOUBLE", val.value);

}


// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees an abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
// The TODOs below are for task 1, you will need to expand freeNode with
// most of the subsequent tasks.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;

    // TODO if the node's next pointer isn't NULL,
    if(node->next != NULL)
    {
        freeNode(node->next);
    }

    // TODO:
    // if the node is a function node:
    if(node->type == FUNC_NODE_TYPE)
    {
        // make a recursive call to free its opList
        freeNode(node->data.function.opList);
    }

    free(node);
}

RET_VAL neg_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL abs_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL add_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL sub_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL mult_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL div_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL remaind_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL exp_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL exp2_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL pow_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL log_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL sqrt_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL cbrt_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL hypot_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL max_op(AST_NODE) {
    RET_VAL result;
    return result;
}

RET_VAL min_op(AST_NODE) {
    RET_VAL result;
    return result;
}
