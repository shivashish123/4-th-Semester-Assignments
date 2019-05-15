-- Example for section 10.5 breaking the rule for whitespace
class Main inherits IO{
	
	ans:Bool;

	iszero(num : Int):Bool{
			if(num=0) then true else  
			falsefi				  	  -- there should be whitespace between 'false' and 'fi'	  
	};
	main():Object {	{ out_string("Enter a number:\n"); ans<-iszero(in_int()); 
	if ans then out_string("Input Number is zero.\n") else out_string("Input number is non-zero.\n") fi;} };
};