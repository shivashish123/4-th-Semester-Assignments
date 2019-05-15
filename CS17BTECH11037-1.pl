loop(0).
loop(M):-                                         % loop till M iterations.
  M > 0,
  read(List),
  ( 
    gettail(List, Value),        % Value contains the last element of list
    solvepuzzle(List,Answer,Value) -> writef('%w=%w\n',[Answer,Value]);  % solves the puzzle for different test cases.
    write("IMPOSSIBLE\n")        % if no solution then print 'IMPOSSIBLE' 
  ),                         
  M1 is M-1, loop(M1).

solvepuzzle(List, Answer,Value) :-
  removetail(List , L),        % L is the list 'List' with last element removed
  construct(L, Answer),        % construct an expression Answer
  Answer =:= Value, ! .        % compare the value of expression 'Answer' and last element of list
                               % if both the values are equal then we perform the cut.

% split the list L into non-empty parts L1 and L2 such that their concatenation is L

split(L,L1,L2) :- append(L1,L2,L), L1 = [_|_], L2 = [_|_].

construct([X],X).                    % if a list contains single element
construct(L,Expression) :-                    
   split(L,List1,List2),             % split the list L into two lists List1 and List2.
   construct(List1,LeftExpr),        % construct the expression from List1.
   construct(List2,RightExpr),       % construct the expression from List2.
   getexpression(LeftExpr,RightExpr,Expression).      % construct combined expression from left and right expression.

% getexpression combines left and right expression using (+,-,*,/) operators.

getexpression(LeftExpr,RightExpr,LeftExpr+RightExpr).
getexpression(LeftExpr,RightExpr,LeftExpr-RightExpr).
getexpression(LeftExpr,RightExpr,LeftExpr*RightExpr).
getexpression(LeftExpr,RightExpr,LeftExpr/RightExpr) :- RightExpr =\= 0.   % avoid division by zero

% removetail(L , L1) removes the last element of list L and resultant list is L1. 

removetail([ _ ],[]).
removetail([ X | L], [ X | L1]) :- removetail(L,L1).

% gettail(L , X) will give the value of last element of list L in X.

gettail([X],X).
gettail([ _ | L] , R) :- gettail(L , R).

% 'main' goal of the program.

main :-
	see('input.txt'),
  read(X),
	loop(X),
  seen.