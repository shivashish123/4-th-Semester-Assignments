-- Example for section 10.4 breaking the rule for keyword
class Main inherits IO{
	
	ans:Bool;

	iszero(num : Int):Bool{
			if(num=0) then True else  -- keyword true and false must begin with small letter
			fAlse fi				  -- and the trailing letters may be upper or lower case	  
	};
	main():Object {	{ out_string("Enter a number:\n"); ans<-iszero(in_int()); 
	if ans then out_string("Input Number is zero.\n") else out_string("Input number is non-zero.\n") fi;} };
};