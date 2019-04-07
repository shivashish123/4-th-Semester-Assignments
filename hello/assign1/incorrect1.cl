--Error from section 10.1 breaking rule for identifiers

class Main inherits IO{

	Ans:Int;				--object identifiers begin with small so 'A'is not allowed
	y@:Int;					-- special symbol like '@' are not allowed in name of identifier
	sumofsquares(num : Int) : Int {  -- sum of squares 
		(  								
		  let result:Int <- 0 in
		  {
		      (let x : Int <- num in
		            while(not(x<=0)) loop
		            {
		            	result<-result+x*x;
		            	x<-x-1;
		            }
		            pool
		       );
	      result;
	  	  }
	  	)
	};
	main():Object {	{ out_string("Enter a positive number:\n"); y@<-(new IO).in_int(); Ans<-sumofsquares(y@); 
	if y@<0 then out_string("Error: You have entered a negative number.\n") else 
	out_string("Sum of cubes of natural numbers upto ").out_int(y@).out_string(" is : ").out_int(Ans).out_string(".\n") fi;} };
};