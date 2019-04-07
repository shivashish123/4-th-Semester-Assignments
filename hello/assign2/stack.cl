class Stack{
      isNil() : Bool { true };				--Returns true if stack is empty, false otherwise.
   	top()  : String { { abort(); ""; } }; 		--Returns the string at the top of stack.
   	tail()  : Stack { { abort(); self; } };--Returns the remainder of the stack i.e. stack without the first element
   	push(i : String) : Stack {				--Returns a new stack containing i as the first element
      (new Cons).init(i, self)
   };
};

class Cons inherits Stack {
   element : String;	-- The element in this stack cell
   l : Stack;		-- The rest of the stack
   isNil() : Bool { false }; 
   top()  : String { element };
   tail()  : Stack { l };
   init(i : String, rest : Stack) : Stack { -- initializing the cell
      {
	 	element <- i;				--changing the top
	 	l <- rest;					--tail becomes the previous stack
	 	self;
      }
   };

};
class Main inherits A2I{

   myStack : Stack;  --stack
   x : String;       --input string
   temp1 : String;   --temporary variables to store the popped elements
   temp2 : String;
   add1 : Int;       --variables used to perform addition operation
   add2 : Int;

   print_Stack(l : Stack) : Object { -- printing the stack 
      {
         if l.isNil() then l<-l else 			--if stack is empty then do nothing
         {
         out_string(l.top());out_string("\n");print_Stack(l.tail()); --else print the top and recurse for rest of stack
         }
          fi;
      } 
   };



   main() : Object {
      {
         myStack <- new Stack;
         (
            let condition:Bool <- true in
               while(condition) loop
               {
               	  out_string(">");
                  x<-in_string();
                  if( x = "e") then
                  {
                     if(myStack.top()="s") then
                     {
                        myStack <- myStack.tail(); --pop operation
                        temp1   <- myStack.top();  --assigning temp1 to be top of stack
                        myStack <- myStack.tail(); --Again popping
                        temp2   <- myStack.top();  
                        myStack <- myStack.tail();
                        myStack <- myStack.push(temp1);
                        myStack <- myStack.push(temp2);
                     }
                     else if(myStack.top()="+") then
                     {
                        myStack <- myStack.tail(); 
                        temp1   <- myStack.top();       --popping two elements temp1 and temp2 
                        add1    <- (new A2I).a2i(temp1);--converting them into integers   
                        myStack <- myStack.tail();       
                        temp2   <- myStack.top();
                        myStack <- myStack.tail();
                        add2    <- (new A2I).a2i(temp2);
                        myStack <- myStack.push(i2a(add1+add2)); --adding the integers and then pushing back the result as string
                     }
                     else
                     {
                        myStack <-myStack;   --do not change the stack
                     }
                     fi fi;
                  }
                  else if (x = "d") then
                     print_Stack(myStack) --print the stack
                  else if (x = "x") then  
                     condition<-false     --break the loop
                  else
                      myStack <- myStack.push(x) --pushing the symbol
                  fi fi fi;
               }
               pool
         );
      } 
   };
};