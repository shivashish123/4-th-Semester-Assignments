class Main inherits IO{

	ans:Int;	--output sum 
	y:Int;		--input number
	sumofcubes(num : Int) : Int {  -- returns the sum of cubes of natural numbers till num
		(
		  let result:Int <- 0 in
		  {
		      (let x : Int <- num in
		            while(not(x<=0)) loop
		            {
		            	result<-result+x*x*x;
		            	x<-x-1;
		            }
		            pool
		       );
	      result;
	  	  }
	  	)
	};
	main():Object {	{ out_string("Enter a positive number:\n"); 
	y <-(new IO).in_int(); ans<-sumofcubes(y); --taking input
	if y<0 then out_string("Error: You have entered a negative number.\n") else --checking if number is positive or not
	out_string("Sum of cubes of natural numbers upto ").out_int(y).out_string(" is : ").out_int(ans).out_string(".\n") fi;} };
};