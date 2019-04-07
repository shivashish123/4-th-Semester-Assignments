class Main inherits IO{

	ans:String;		--output reversed string
	y:String;		--input string
	reversestring(str : String,len : Int) : String {  -- reverse a string
	      (
	      	let result : String in
	            {
	            	(let x : Int <- len in
		            	while(not(x=0)) loop
		            	{
		            		result <- result.concat(str.substr(x-1,1));
		            		x<-x-1;
		            	}
		            	pool
		            );
		            result;
	            }
	      )
	};

	main():Object {	{ out_string("Enter a string:\n"); 
	y <-(new IO).in_string(); ans<-reversestring(y,y.length()); --takes a input string and reverse it
	out_string("Reverse of ").out_string(y).out_string(" is : ").out_string(ans).out_string("\n"); } };
};