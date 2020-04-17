## CiLisp
### Summary
This is the first task. It required completing both the lex and yacc files, as well as eval functions for the grammar of a lisp like language for evaluating expressions in CNP form.
#### Task 1 ::= CPN Calculator  
**noteable**:  
For the NEG operation, I had to add a production for _s_expr_list_ to handle the instance when no operand is provided. I suspect it is necessary for all ops.  
Added \v and \r to the lex file for ignoring whitespace.  
For add_op, used recursion to sum the oplist.  
Called freeNode($1) after execution is done on base of tree, to free memory and satisfy the Valgring monster.  
I use ternary operators wherever (appropriately) possible and, for example, in conditionals when differentiating enumerated types, I take advantage of the fact that enumerated types resolve to integer values, and that integer values  are interpreted as true or false values. I do this when determining the RET_VAL type from NUM_AST_NODE types.  
For division by 0, I have chosen to return nan without throwing an error or warning. nan is enough of a warning.  
 
      
Summarize what has been done so far. Specify which tasks have been completed so far.
### Sample Runs
Sample runs should all come from the most recent implementation. Sample runs which were done to test previous tasks should be done again with the new implementation.
You'll save a lot of time here if you save all of your test inputs for each task in the inputs folder and keep them organized!
Sample runs should be organized by task; there should be sample runs showing that everything in task 1 still works in the up-to-date implementation, and so on up through the most recent task.
### Known Bugs
If anything is not functioning to specification, you are responsible enough for testing enough to recognize that this is the case and fix it. If you give up on fixing something, you are required to list it here.
Bugs which are discovered by the grader but not listed here will be graded harshly. Bugs which are listed here will be graded nicely, within reason.
Known bugs should also be organized by task, and should be well-described.
Lazy bug descriptions which don't go into detail about what is going wrong ("<functionality> doesn't work", "<feature> throws an error") will be graded as if the bug was not listed here.
This includes bugs that were created in functionality from prior tasks. If your task 5 breaks your task 3, we will check your task 3 submission if the bug is noted here, but if it is not noted here then we will assume that task 3 never worked and grade accordingly.