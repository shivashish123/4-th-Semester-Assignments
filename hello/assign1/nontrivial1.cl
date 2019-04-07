class List {
    isNil() : Bool { true };				--Returns true if list is empty, false otherwise.
   	head()  : Int { { abort(); 0; } }; 		--Returns the integer at the head of list.
   	tail()  : List { { abort(); self; } };--Returns the remainder of the list i.e. list without the first element
   	insert(i : Int) : List {				--Returns a new list containing i as the first element
      (new Cons).init(i, self)
   };
};

class Cons inherits List {
   element : Int;	-- The element in this list cell
   l : List;		-- The rest of the list
   isNil() : Bool { false }; 
   head()  : Int { element };
   tail()  : List { l };
   init(i : Int, rest : List) : List { -- initializing the cell
      {
	 	element <- i;				--changing the head
	 	l <- rest;					--tail becomes the previous list
	 	self;
      }
   };

};
class Main inherits IO {

   mylist : List;
   siz : Int;
   num : Int;

   linear_search(l : List, x: Int) : Bool { -- search an element in the list 
      if l.isNil() then false else 			--if list is empty then return false
      if(l.head()=x) then true else 		-- if head element is searched element then true
      linear_search(l.tail(),x)				-- we search in rest of the element
      fi fi 
   };



   main() : Object {
      {
         out_string("Enter the size of list: \n");	--taking input
            siz<-in_int();
         out_string("Enter the elements of list : \n");
	        mylist <- new List;
           (let x:Int <- siz in 
	           {
               while (not(x=0)) loop
	   	        {
	   	           mylist <- mylist.insert(in_int());
	                 x <- x-1;
	   	        }
	           pool;
            }
            );
         out_string("Enter the number to be searched : \n");
         num <- in_int();
         if linear_search(mylist,num) then out_string("Success : Number found in the list.\n") else
         out_string("Failure : Number not found!!!\n") fi;
      } 
   };

};