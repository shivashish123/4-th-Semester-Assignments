--Error from section 10.2 breaking rule for strings

class Main inherits IO{

	ans:String;
	y:String;
	modifystring(str : String) : String {  -- concat with string enclosed in single quotes
		(
		  let result:String <- 'Hi ' in 	-- strings must be enclosed in double quotes
		  {
		    result <- result.concat(str);
	      	result;
	  	  }
	  	)
	};
	main():Object {	{ out_string("Enter your name:\n"); y <-(new IO).in_string(); ans<-modifystring(y); 
	out_string(ans).out_string("\n"); } };
};