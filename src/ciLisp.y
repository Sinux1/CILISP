%{
    #include "ciLisp.h"
    #define ylog(r, p) fprintf(flex_bison_log_file, "BISON: %s ::= %s \n", #r, #p)
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *stNode;
}

%token <sval> FUNC SYMBOL TYPE
%token <dval> INT DOUBLE
%token LPAREN RPAREN EOL QUIT EOFT LET COND

%type <stNode> let_section let_list let_elem
%type <astNode> s_expr f_expr number s_expr_list

%%

program:
    s_expr EOL {
        ylog(program, s_expr EOL);
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
            YYACCEPT;
        }
    }
    | s_expr EOFT {
        ylog(program, s_expr EOFT);
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
            YYACCEPT;
        }
        exit(EXIT_SUCCESS);
    }
    | EOFT {
        exit(EXIT_SUCCESS);
    }
    |
     EOL {
     YYACCEPT;
     }
    ;

s_expr:
    f_expr {
        ylog(s_expr, f_expr);
        $$ = $1;
    }
    |
    number {
    ylog(s_expr, number);
    $$ = $1;
    }
    |
    SYMBOL {
    ylog(s_expr, symbol);
    $$ = createSymbolNode($1);
    }
    |
    LPAREN let_section s_expr RPAREN {
    ylog(s_expr,LPAR let_section s_expr RPAR );
    $$ = assignSymbolTable($2, $3);
    }
    |
    LPAREN COND s_expr s_expr s_expr RPAREN {
    ylog(s_expr,LPAR COND s_expr s_expr s_expr RPAR );
    $$ = decideConditional($3, $4, $5); // eval a, if != 0 then b, else c
    }
    | QUIT {
        ylog(s_expr, QUIT);
        exit(EXIT_SUCCESS);
    }
    | error {
        ylog(s_expr, error);
        yyerror("unexpected token");
        $$ = NULL;
    };

f_expr:
    LPAREN FUNC s_expr_list RPAREN {
    	ylog(f_expr, FUNC);
	$$ = createFunctionNode($2, $3);
    };

s_expr_list:
    s_expr {
        ylog(s_expr_list, s_expr);
        $$ = $1;
    }
    |
    s_expr s_expr_list {
    	ylog(s_expr_list, s_expr_list);
    	$$ = addOperandToList($1, $2);
    }

    |{
    	ylog(s_expr, NULL);
    	$$ = NULL;
    };

let_section:
	LPAREN LET let_list RPAREN {
	ylog(let_section, LPAR LET let_list RPAR);
	$$ = $3;
	}
	| {
	ylog(let_section, NULL);
	$$ = NULL;
	};

let_list:
	let_list let_elem {
	ylog(let_list, let_elem let_list);
	$$ = addRecordToList($1, $2);
	}
	|
	let_elem {
	ylog(let_list, let_elem);
	$$ = $1;
	}

let_elem:
	LPAREN SYMBOL s_expr RPAREN {
	ylog(let_elem, LPAR SYMBOL s_expr RPAR);
	$$ = createSymbolTableNode(NULL, $2, $3);
	}
	|
	LPAREN TYPE SYMBOL s_expr RPAREN {
        ylog(let_elem, LPAR SYMBOL s_expr RPAR);
        $$ = createSymbolTableNode($2, $3, $4);
        }

number:
    INT {
	ylog(number, INT);
	$$ = createNumberNode($1, INT_TYPE);
    }
    |
    DOUBLE {
	ylog(number, DOUBLE);
        $$ = createNumberNode($1, DOUBLE_TYPE);
    };

%%

