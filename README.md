# CiLisp
## Summary  
#### Task 1 ::= CPN Calculator 
This is the first task. It required completing both the lex and yacc files, as well as eval and math op functions for 
the grammar of a lisp like language for evaluating expressions in CNP form, that is as an n-tuple, first element is an 
operation, subsequent elements are operands. Further functionality will be added in subsequent tasks.    
---
#### Task 2 ::= CPN Calculator featuring Symbols  
This task required adding to both the flex and bison files. Symbols must be composed of one or more alphabetical characters. 
No other characters are allowed. Assignment is dane by the let expression. The key word `let` signifies the following ordered 
pairs `(Symbol s_expr)` is semantically equivelant to `Symbol = s_expr`.    
---
#### Task 3 ::= CPN Calculator with Symbols featuring Symbol Casting  
This task added the ability to cast a symbol value to int or double, regardless of what the symbol node evaluates to. 
This required an additional production to `let_section` in yacc file, and the addition of key words `int` and `double` in 
the lex file. 
---
#### Task 4 ::= CPN Calculator with Symbol Casting featuring Conditionals and Additional Functions  
This requires the definition of additional functions to CPN calculator, and additional productions and key words 
for conditionals in the lex and yacc files.

---

   
##_Noteable_:  
* The expression `(123)` was invalid for task 1, but is valid from task 2 forward.  
* All input tested is collected in two files, `allBadInput.ciLisp` and `allValidInput.ciLisp` found in `inputs/`  

#### Task 1 
- I had to add a production for _s_expr_list_ to handle the instance when no operand is provided. 
- A production was added to _program_ for the case when there is no instruction. In an input file, this is represented by 
a blank line.    
- Added \v and \r to the lex file for ignoring whitespace.  
- Originally I had written the math op functions to use recursion, but I could not figure out how to accuratly track 
return type through recursive calls, but I could easily do it iterating through the linked oplist.    
- Called freeNode($1) after execution is done on base of tree, to free memory and satisfy the Valgrind monster.  
- I make frequent use of the ternary operator. When unclear, I comment.  
- For division by 0, I have chosen to return nan without throwing an error or warning. nan is enough of a warning. There 
was no explicit instruction for divide by zero error in function implementation description, so I took my own liberty.  
- The file `/inputs/allValidTestInputs.txt` is a running collection of valid inputs that will continue to be added to 
and tested through all tasks in this lab.  

---
#### Task 2
* The underlying data structure of an AST node was added to. In task 1, each AST node had a `type`, `*data`, and `*next` 
values. This allowed them to be used as linked lists, and when evaluating an AST node, all of the information for that 
evaluation could be found in the data pointed to by the `data` pointer and the value of `type`. Now in this task, the AST 
node structure needed the addition of parent and symbol table pointers. `symbolTable` points to a linked list of symbol 
table nodes (`let_list`), and `parent` points to its parent AST node on the parse tree. Now, when evaluating an AST node, 
data found in that node's symbol table or the symbol table of any of its parent nodes may be used.  
* The following functionality was added, function definitions can be found in `ciLisp.c`  
    * To create a symbol node from the string value of a symbol token.  
    `AST_NODE *createSymbolNode(char *id);`  
    * To point the `symbolTable` pointer to the linked list of symbol table nodes representing a `let_section`.  
     `AST_NODE *assignSymbolTable(SYMBOL_TABLE_NODE *record, AST_NODE *node);`  
    * To add a symbol table node to the linked list of symbol table nodes, the rightmost `let_elem` in a `let_list` is 
    the head.  
    `SYMBOL_TABLE_NODE *addRecordToList(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *newHead);`  
    * To create a symbol table node whose `id` points the string value of the symbol token and `value` points to 
    the AST node representing the s_expr token of a `let_elem` production.  
    `SYMBOL_TABLE_NODE *createSymbolTableNode(char *id, AST_NODE *node);`  
    * For evaluation of a symbol node.  
    `RET_VAL evalSymbolNode(AST_NODE *node)`  
* `RET_VAL eval(AST_NODE *node)` was modified to call `evalSymbolNode(AST_NODE *node)` when passed a symbol node.  
* In `evalSymbolNode(AST_NODE *node)`, the character string pointed to by `node->id` is compared against the `id` of every 
symbol table node in that node's `symbolTable`, and if a match is not found, or the table is `NULL`, it checks the symbol 
table of it's parent node. This is continued until a match is found or `parent` is `NULL`. If no match is found, an 
`Undefined Symbol` error is thrown and `nan` is returned. If a match is found, the node `value` pointer of that symbol 
table node is evaluated and returned.  

---    
#### Task 3  
- There is now a warning for invalid characters per sample run example.    
- Very little was changed from task 2 to task 3, so bad inputs for testing task 3 functionality are minimal.  
---
#### Task 4
- I tested the precision of the equal operation by comparing 0 to 0.000...1 with increasing number of decimal digits 
until the comparison returned true, and found that this implementation, on my development PC, will return true when the 
number of zeroes that follow the decimal, preceding the last 1, is 322. That is, 322 zeroes and a 1 after the decimal is 
considered 0 and when compared to 0 will evaluate to true.
- Any instruction containing a read function is always executed from stdin, and is never redirected from a file.  
---

# Sample Runs 
## Task 1   
### Run 1 : valid input 
Input in `/inputs/input1.txt` was collected from Task 1 example sample runs and includes other inputs that I wanted to 
test to make sure things like whitespace were correctly ignored, and that when nested functions threw an error, the rest 
of the function completed correctly. Such as when a call to sub with no operands was nested deep in other functions, the 
output should only be the error warning followed by NAN, or whatever the correct output is supposed to be paired with the 
specific warning.  
```
(neg 5)

(neg 5.5)
(neg -5.0)
(neg -5)
(neg)
(neg 1 2)
(abs 1)
(abs 1.2)
(abs -3)
(abs 0)
(abs 0.0)
(abs -1.4)
(abs)
(abs -1 2)
(add)
(add 1)
(add 1.0)
(add 1 2 3 4 5)
(add 1 -2 3 -4 5 -6)
(add 0.0 1 -2 3 -4 5 -6)
(add 1 -1.0)
(sub)
(sub 1)
(sub 1.0)
(sub 1 2)
(sub 2 1)
(sub 2 -1)
(sub 2.0 1)
(sub 2.0 -1)
(sub 1 1.0)
(sub 2.0 1.0)
(sub 1 2 3)
(mult)
(mult 1)
(mult 1.0)
(mult -1)
(mult -1 -1.0)
(mult 1 -2 3 -4 5)
(mult -1.0 2 -3.0 4 -5)
(div)
(div 1)
(div 1.0)
(div 1 2)
(div 1.0 2)
(div 2 1)
(div 2.0 1)
(div 5 2.0)
(div -20.0 4)
(div 1 2 3 4)
(div 1 2 3)
(div 5.0 2 3)
(div 20 0)
(remainder)
(remainder 1)
(remainder 0)
(remainder -1.0)
(remainder 1 2)
(remainder 2 1)
(remainder 2.5 1)
(remainder 2 3)
(remainder -6 10)
(remainder -6.0 10.0)
(remainder -6.0 -10.0)
(remainder 1 2 3)
(remainder 23 7 10)
(exp)
(exp 1)
(exp (log 1))
(exp -1)
(exp 5.0)
(exp -2.0)
(exp 1 2)
(exp2)
(exp2 1)
(exp2 1.0)
(exp2 0)
(exp2 0.0)
(exp2 0.5)
(exp2 (div 1 2.0))
(exp2 -2)
(exp2 -2.0)
(exp2 1 2)
(pow)
(pow 1)
(pow 1.0)
(pow 1 1)
(pow 1 1.0)
(pow 2 1)
(pow 2.1 1)
(pow -2 0.5)
(pow -2 0)
(pow -2.0 0.0)
(pow -2.0 0)
(pow 3 3)
(pow 3.0 3)
(pow 27 (div 1 3.0))
(pow 1 2 3)
(log)
(log 1)
(log 0)
(log -1)
(log 0.0)
(log -1.0)
(log (exp 1))
(div (log 27) (log 3))
(div (log 27.0) (log 3))
(log 1 2)
(sqrt)
(sqrt 1)
(sqrt 1.0)
(sqrt 0)
(sqrt 0.0)
(sqrt -1)
(sqrt -1.0)
(sqrt 4)
(sqrt 4.0)
(sqrt 2)
(sqrt 1 2)
(cbrt)
(cbrt 0)
(cbrt 0.0)
(cbrt -1)
(cbrt -1.0)
(cbrt 1)
(cbrt 1.0)
(cbrt 27)
(cbrt           27.0)
(cbrt 4)
(cbrt (pow 3 3))
(cbrt (pow 3 6))
(cbrt 1 2)
(hypot)
(hypot 1)
(hypot 1.0)
(hypot 3 4)
(hypot -3 4)
(hypot -30 -40.0)
(hypot 4 4 7)
(hypot 7.0 4 4.0)
(hypot 12 13 14)
(hypot 5 5 5)
(hypot -5 -5.0 (sqrt 25))
(hypot 0 0 0.0 -3 0 0 0 0 4 0.0 -0.0 12)
(max)
(max 1)
(max -1)
(max 1.0)
(max 	232311.121)
(max 1 2 3 4 5 6 7 8.0 9)
(max 1 2 25.0 -26.0 12)
(min)
(min 1)
(min 0.0)
(min 0)

(min -1 2 -3 4 -5 6)
(min                -1.0    -12.0   12)
(log (exp (log (exp 1))))
(sub (mult 1 2 3 4) (add 1 2 3 4))
(sub (mult 1 2 3 -4.0) (add -1 -2 -3 -4))
(hypot (sqrt (div 100 7.0)) (mult 6 (sqrt (div 100.0 7))))
(       hypot (sqrt (div 100 7.0)) (sqrt (mult 6 (div           100.0 7))))
(add 1      (add              2 (add 3 (add 4 (add 5 (add 6 (add 7)))))))
(add 1 (add     2 (add 3 (add 4 (add 5 (add 6 (sub 0 -7.0)))))))
(mult 1 (sub))
(sub 2 1 (add 1))
```
### Run 1 : output
```
INTEGER: -5
DOUBLE: -5.500000
DOUBLE: 5.000000
INTEGER: 5
ERROR: neg called with no operands!
INTEGER: nan
WARNING: neg called with extra (ignored) operands!
INTEGER: -1
INTEGER: 1
DOUBLE: 1.200000
INTEGER: 3
INTEGER: 0
DOUBLE: 0.000000
DOUBLE: 1.400000
ERROR: abs called with no args.
INTEGER: nan
WARNING: abs call with extra operands. Only first operand used!
INTEGER: 1
WARNING: add call with no operands, 0 returned!
INTEGER: 0
INTEGER: 1
DOUBLE: 1.000000
INTEGER: 15
INTEGER: -3
DOUBLE: -3.000000
DOUBLE: 0.000000
ERROR: sub called with no operands!
INTEGER: nan
ERROR: sub called with only one arg!
INTEGER: nan
ERROR: sub called with only one arg!
INTEGER: nan
INTEGER: -1
INTEGER: 1
INTEGER: 3
DOUBLE: 1.000000
DOUBLE: 3.000000
DOUBLE: 0.000000
DOUBLE: 1.000000
WARNING: sub called with extra (ignored) operands!
INTEGER: -1
WARNING: mult call with no operands, 1 returned!
INTEGER: 1
INTEGER: 1
DOUBLE: 1.000000
INTEGER: -1
DOUBLE: 1.000000
INTEGER: 120
DOUBLE: -120.000000
ERROR: div called with no operands!
INTEGER: nan
ERROR: div called with only one arg!
INTEGER: nan
ERROR: div called with only one arg!
INTEGER: nan
INTEGER: 0
DOUBLE: 0.500000
INTEGER: 2
DOUBLE: 2.000000
DOUBLE: 2.500000
DOUBLE: -5.000000
WARNING: div called with extra (ignored) operands!
INTEGER: 0
WARNING: div called with extra (ignored) operands!
INTEGER: 0
WARNING: div called with extra (ignored) operands!
DOUBLE: 2.500000
Division by zero error
INTEGER: nan
ERROR: remainder called with no operands!
INTEGER: nan
ERROR: remainder called with only one arg!
INTEGER: nan
ERROR: remainder called with only one arg!
INTEGER: nan
ERROR: remainder called with only one arg!
INTEGER: nan
INTEGER: 1
INTEGER: 0
DOUBLE: 0.500000
INTEGER: 2
INTEGER: 4
DOUBLE: 4.000000
DOUBLE: 4.000000
WARNING: remainder called with extra (ignored) operands!
INTEGER: 1
WARNING: remainder called with extra (ignored) operands!
INTEGER: 2
ERROR: exp called with no operands!
INTEGER: nan
DOUBLE: 2.718282
DOUBLE: 1.000000
DOUBLE: 0.367879
DOUBLE: 148.413159
DOUBLE: 0.135335
WARNING: exp called with extra (ignored) operands!
DOUBLE: 2.718282
ERROR: exp2 called with no operands!
INTEGER: nan
INTEGER: 2
DOUBLE: 2.000000
INTEGER: 1
DOUBLE: 1.000000
DOUBLE: 1.414214
DOUBLE: 1.414214
DOUBLE: 0.250000
DOUBLE: 0.250000
WARNING: exp2 called with extra (ignored) operands!
INTEGER: 2
ERROR: pow called with no operands!
INTEGER: nan
ERROR: pow called with only one arg!
INTEGER: nan
ERROR: pow called with only one arg!
INTEGER: nan
INTEGER: 1
DOUBLE: 1.000000
INTEGER: 2
DOUBLE: 2.100000
INTEGER: nan
INTEGER: 1
DOUBLE: 1.000000
DOUBLE: 1.000000
INTEGER: 27
DOUBLE: 27.000000
DOUBLE: 3.000000
WARNING: pow called with extra (ignored) operands!
INTEGER: 1
ERROR: log called with no operands!
INTEGER: nan
DOUBLE: 0.000000
DOUBLE: -inf
INTEGER: nan
DOUBLE: -inf
INTEGER: nan
DOUBLE: 1.000000
DOUBLE: 3.000000
DOUBLE: 3.000000
WARNING: log called with extra (ignored) operands!
DOUBLE: 0.000000
ERROR: sqrt called with no operands!
INTEGER: nan
DOUBLE: 1.000000
DOUBLE: 1.000000
DOUBLE: 0.000000
DOUBLE: 0.000000
INTEGER: nan
INTEGER: nan
DOUBLE: 2.000000
DOUBLE: 2.000000
DOUBLE: 1.414214
WARNING: sqrt called with extra (ignored) operands!
DOUBLE: 1.000000
ERROR: cbrt called with no operands!
INTEGER: nan
DOUBLE: 0.000000
DOUBLE: 0.000000
DOUBLE: -1.000000
DOUBLE: -1.000000
DOUBLE: 1.000000
DOUBLE: 1.000000
DOUBLE: 3.000000
DOUBLE: 3.000000
DOUBLE: 1.587401
DOUBLE: 3.000000
DOUBLE: 9.000000
WARNING: cbrt called with extra (ignored) operands!
DOUBLE: 1.000000
ERROR: hypot called with no operands, 0.0 returned!
DOUBLE: 0.000000
DOUBLE: 1.000000
DOUBLE: 1.000000
DOUBLE: 5.000000
DOUBLE: 5.000000
DOUBLE: 50.000000
DOUBLE: 9.000000
DOUBLE: 9.000000
DOUBLE: 22.561028
DOUBLE: 8.660254
DOUBLE: 8.660254
DOUBLE: 13.000000
ERROR: max called with no operands!
INTEGER: nan
INTEGER: 1
INTEGER: -1
DOUBLE: 1.000000
DOUBLE: 232311.121000
INTEGER: 9
DOUBLE: 25.000000
ERROR: min called with no operands!
INTEGER: nan
INTEGER: 1
DOUBLE: 0.000000
INTEGER: 0
INTEGER: -5
DOUBLE: -12.000000
DOUBLE: 1.000000
INTEGER: 14
DOUBLE: -14.000000
DOUBLE: 22.990681
DOUBLE: 10.000000
INTEGER: 28
DOUBLE: 28.000000
ERROR: sub called with no operands!
INTEGER: nan
WARNING: sub called with extra (ignored) operands!
INTEGER: 1

Process finished with exit code 0
```
---
### Run 2 : invalid inputs
Input in `/inputs/badinput1.txt` are instructions I tested that I knew to be invalid, and were all correctly rejected.  
```
()
(a dd 1 2)
(add 1 1(
add 12 12
(1 2 add)
(func 2 3)
(123)
```
### Run 2 : output
Each invalid input correctly threw this error.  
```

ERROR: syntax error

Process finished with exit code 1

```
---  

## Task 2
### Run 1: valid input
Input in `/inputs/input2.txt` was collected from Task 1 example sample runs and includes other expressions to test various 
nestings and to be sure that the symbol tables and parent nodes were implemented correctly programmatically. For example, 
when evaluating a symbol node, the value of the most recent definition is used, or the successful evaluation of a 
parenthesized s_expr with a `NULL` let_section.  
```
( (let (x 1) ) x )
(add ((let (abc 1)) (sub 3 abc)) 4)
(mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))
(add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
((let (abc 1)) (sub ((let (abc 2) (de 3)) (add abc de)) abc))
((let (x 3) (x 30)) (add ((let (x 10) (x 100)) x) x))
((let (abc 1)) (sub ((let (abc 2)) (add abc de)) abc))
((let (x 1)) y)
(1)
((let (x 1)     (x 3) (x 5) (y 10)) (add x ((let (x 10)) (add x (neg y)))))
((let (x 1)     (x 3)  (y 10) (x 5)) (add x ((let (x 10)) (add x (neg y)))))
QUIT
quit
```
### Run 1: output
```
INTEGER: 1
INTEGER: 6
DOUBLE: 4.242641
DOUBLE: 5.398346
DOUBLE: 17.414214
INTEGER: 4
INTEGER: 130
Undefined Symbol!
INTEGER: nan
Undefined Symbol!
INTEGER: nan
INTEGER: 1
INTEGER: 5
Undefined Symbol!
INTEGER: nan

Process finished with exit code 0
```
### Run 2: invalid input
Input in `/inputs/badinput2.txt` are instructions I tested that I knew to be invalid, and were all correctly rejected.  
```
((let (x 1)) (add y ((let (y 1)) x)))
()
((let ()) 1)
((let ) 1)
((let (abc1d 1)) 1)
(((((((1)))))))
(2 3)
```
### Run 2: output
Each invalid input correctly threw this error.  
```

ERROR: syntax error

Process finished with exit code 1

```
---
## Task 3  
### Run 1 : valid input  
Input from `../inputs/validInput3.txt` is pulled from the sample runs in the task instructions, and from task 2 valid 
input, except modified by adding a cast to at least one let_elem in every instruction. All output is valid.  
``` 
((let (int a 1.25))(add a 1))
((let (double a 5))(add a .25))
((let (double a 5))(add a 0.25))

( (let (double x 1) ) x )
(add ((let (double abc 1)) (sub 3 abc)) 4)
(mult ((let (a 1) (int b 2.5)) (add a b)) (sqrt 2))
(add ((let (int a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
((let (double abc 1)) (sub ((let (abc 2) (de 3)) (add abc de)) abc))
((let (x 3) (x 30)) (add ((let (x 10) (double x 100)) x) x))
((let (int abc 1)) (sub ((let (double abc 2)) (add abc de)) abc))
((let (double x 1)) y)
(1)
((let (x 1)     (x 3) (x 5) (double y 10)) (add x ((let (x 10)) (add x (neg y)))))
((let (x 1)     (double x 3)  (y 10) (x 5)) (add x ((let (x 10)) (add x (neg y)))))
QUIT
(add ((let (double x 1)) x) ((let (y 10)) x))
quit
```
### Run 1: output  
```

WARNING: precision loss on int cast from 1.25 to 1 for variable a.
INTEGER: 2
ERROR: invalid character: >>.<<
DOUBLE: 30.000000
DOUBLE: 5.250000
DOUBLE: 1.000000
DOUBLE: 6.000000
WARNING: precision loss on int cast from 2.5 to 2 for variable b.
DOUBLE: 4.949747
WARNING: precision loss on int cast from 6.32456 to 6 for variable a.
DOUBLE: 5.398346
DOUBLE: 17.414214
DOUBLE: 4.000000
DOUBLE: 130.000000
Undefined Symbol!
INTEGER: nan
Undefined Symbol!
INTEGER: nan
INTEGER: 1
DOUBLE: 5.000000
INTEGER: 5
Undefined Symbol!
INTEGER: nan
Undefined Symbol!
INTEGER: nan

Process finished with exit code 0

``` 
## Task 4
### Run 1 : valid input
Input file `../inputs/input4.txt` begins with several calls to rand to test correct seeding of rand function, and it 
contains all sample runs as well as additional instructions crafted to test proper functionality of more complicated 
instructions, such as `(add ((let (x 1) (y (cond 1 (print 1 2 3 (rand)) (print (rand)) ) ) ) y ) 1)`, which is an 
instruction that caused a very confusing memory leak that has been now fixed, and `(add 1 (print 2) (cond 0 (print 
(rand) (rand) (rand) (rand) (rand)) 3 ) )`, which should not/ does not evaluate any of the calls to rand nested in the
print function, since the condition is false.  
```
(rand)
(rand)
(rand)
(rand)
(rand)
(rand)
(rand)
(rand)
(rand)
(add ((let (x 1) (y (cond 1 (print 1 2 3 (rand)) (print (rand)) ) ) ) y ) 1)
(print 1)
(equal 0 0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)

(add 1 (print 2) (cond 0 (print (rand) (rand) (rand) (rand) (rand)) 3 ) )
(add 1 (print 2 3 4 (rand)) )

( ( let (x (rand)) ) (add (print x) (print x) ) )
(equal 0 0)
(print 0 2.0 0.0)
((let (is 15)) (equal ((let (this 15) (is 150)) this) is))
(less 0 0)
(less -1 0)
(less 0 -1.0)
(greater -30000000 30000000)
(greater (print 1 2 3 0) 0)
(greater 0 0.0)
( ( let (x 0) (y 1) ) (less x y) )
```
### Run 1 : output
```
DOUBLE: 0.102750
DOUBLE: 0.513107
DOUBLE: 0.572894
DOUBLE: 0.646519
DOUBLE: 0.791061
DOUBLE: 0.479726
DOUBLE: 0.174375
DOUBLE: 0.142548
DOUBLE: 0.303393
INTEGER : 1
INTEGER : 2
INTEGER : 3
DOUBLE : 0.541883
DOUBLE: 1.541883
INTEGER : 1
INTEGER : 1
INTEGER : 0
INTEGER : 2
INTEGER : 6
INTEGER : 2
INTEGER : 3
INTEGER : 4
DOUBLE : 0.975509
DOUBLE: 1.975509
DOUBLE : 0.889982
DOUBLE : 0.889982
DOUBLE: 1.779964
INTEGER : 1
INTEGER : 0
DOUBLE : 2.000000
DOUBLE : 0.000000
DOUBLE: 0.000000
INTEGER : 1
INTEGER : 0
INTEGER : 1
INTEGER : 0
INTEGER : 0
INTEGER : 1
INTEGER : 2
INTEGER : 3
INTEGER : 0
INTEGER : 0
INTEGER : 0
INTEGER : 1

Process finished with exit code 0

```
### Run 2 : valid read instructions
The following instructions and outputs are the result of manually typing them into the terminal, and can be found in 
`/inputs/validReadInstructions.txt`.
```
> ( ( let (x (read)) ) (print x) )
( ( let (x (read)) ) (print x) )
read :: 1
1
INTEGER : 1
INTEGER : 1
> ( ( let (x (read)) (y (read)) ) (print x x y y) )
( ( let (x (read)) (y (read)) ) (print x x y y) )
read :: 1
1
read :: 2
2
INTEGER : 1
INTEGER : 1
INTEGER : 2
INTEGER : 2
INTEGER : 2
> (add ((let (double x 1) (y (cond 1 (print 1 2 3 (rand)) (print (rand)) ) ) ) y ) (read))
(add ((let (double x 1) (y (cond 1 (print 1 2 3 (rand)) (print (rand)) ) ) ) y ) (read))
INTEGER : 1
INTEGER : 2
INTEGER : 3
DOUBLE : 0.364077
read :: 666
666
DOUBLE: 666.364077
> quit
quit

Process finished with exit code 0

```


### Run 3: invalid inputs
```

```
### Run 3 : output  
Every bad input threw the following error correctly.
``` 
ERROR: syntax error

Process finished with exit code 1

```
--- 

# Known Bugs 
- Valgrind complains of UninitConditions and UninitValues when running read instructions from command line, but the 
location is deep inside files I did not edit. I made every attempt at tracking the cause, but to no avail.  
![](./figures/valgrindtask4.png) 
- Instructions from file (when stdin is redirected with freopen) are not echoed to console. 
- The read function can only be called from stdin without redirection with freopen. Any attempt to use read from input 
file will result in undefined behavior.

--- 
