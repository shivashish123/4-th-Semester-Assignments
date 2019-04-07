class Main inherits IO{

	y1:String;y2:String; -- Input Strings

	--Following function checks whether two strings are same or not given they are of equal length
	same_string(string1 : String ,string2 :String ,len : Int) : Bool{  --equality of string
		(
			let equality : Bool <- true in
			{
				(let x : Int <- len-1 in
					while(not(x=0)) loop 
					{
						if(not(string1.substr(x,1)=string2.substr(x,1))) then equality <- false else x<-x fi;
						x<- x-1;
					}
					pool
				);
				equality;
			}
		)
	};

	--this function checks whether string str2 is a substring of string st1 or not
	pattern_search(str : String,str2 : String, len1 : Int, len2 : Int) : Bool {  -- pattern matching
	      (
	      	let match : Bool <- false in
	            {
	            	(let x : Int <- (len1-len2) in
		            	while(not(x<0)) loop
		            	{
		            		if ( same_string (str.substr(x,len2),str2,str2.length())) then match <-true else x<-x fi;
		            		x<-x-1;
		            	}
		            	pool
		            );
		            match;
	            }
	      )
	};

	main():Object {	{ out_string("Enter a string : \n"); y1 <-(new IO).in_string(); --taking input strings
	out_string("Enter a pattern to be matched :\n");y2 <-(new IO).in_string(); 
	if(y1.length()< y2.length()) 
		then out_string("Pattern cannot be matched!!!\n") 
		else
			if(pattern_search(y1,y2,y1.length(),y2.length())) 
				then out_string("Pattern found in the string!!!\n") 
			else 
				out_string("Pattern cannot be matched!!!\n") fi fi; } };
};