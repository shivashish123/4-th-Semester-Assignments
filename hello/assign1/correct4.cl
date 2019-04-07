class Main inherits IO{

	ans:Int;	--output factorial
	y:Int;		--input number
	factorial(num : Int) : Int {  -- returns factorial of num
	      (let x : Int <- num in
	            if x <= 0 then 1 else 
	            x*factorial(x-1) fi
	      )
	};

	main():Object {	{ out_string("Enter a positive number:\n"); 
	y <-(new IO).in_int(); ans<-factorial(y); -- taking input and computing factorial 
	if y<0 then out_string("Error: You have entered a negative number.\n") else 
	out_string("Factorial of ").out_int(y).out_string(" is : ").out_int(ans).out_string("\n") fi;} };
};