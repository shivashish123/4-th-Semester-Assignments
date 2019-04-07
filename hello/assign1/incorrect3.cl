--Error from section 10.3 breaking rule for comments

(
	This program breaks rule of comment by not beginning with '*'
	in multi-line comment
*)
- This program breaks one more rule by having '-' instead of '--' before comment
class Main inherits IO{

	ans:String;
	y:String;
	modifystring(str : String) : String { 
		(
		  let result:String <- "Hi " in 
		  {
		    result <- result.concat(str);
	      	result;
	  	  }
	  	)
	};
	main():Object {	{ out_string("Enter your name:\n"); y <-(new IO).in_string(); ans<-modifystring(y); 
	out_string(ans).out_string("\n"); } };
};