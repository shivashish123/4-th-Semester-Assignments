class Main inherits IO{

	ans:Bool;		--stores true if number is divisible by 5 otherwise false
	
	--Following function checks whether a number is divisible by 5 or not
	divby5(num : Int) : Bool {  -- divisible by 5
	      (let x : Int <- num in
	            if x < 0 then divby5(~x) else 
	            if x = 1 then false else
	            if x = 2 then false else
	            if x = 3 then false else
	            if x = 4 then false else
		       	if x = 0 then true else 
		       	divby5(x-5) fi fi fi fi fi fi
	      )
	};
	main():Object {	{ out_string("Enter a number:\n"); ans<-divby5(in_int()); 
	if ans then out_string("Input Number is divisible by 5.\n") else 
	out_string("Input number is not divisible by 5.\n") fi;} };
};