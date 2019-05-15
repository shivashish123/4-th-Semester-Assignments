class Main inherits IO{

	ans:Int;
	y:Int;
	fibonacci(num : Int) : Int {  -- returns (num)th fibonacci number
	      (let x : Int <- num in
	            if x <= 0 then 0 else
	            if x = 1 then 1 else 
	            fibonacci (x-1) + fibonacci (x-2) fi fi
	      )
	};

	main():Object {	{ out_string("Enter a positive number:\n");
	y <-(new IO).in_int(); ans<-fibonacci(y); --taking input
	if y<0 then out_string("Error: You have entered a negative number.\n") else --checking if number is positive or not
	out_int(y).out_string("-th Fibonacci number is : ").out_int(ans).out_string(".\n") fi;} };
};