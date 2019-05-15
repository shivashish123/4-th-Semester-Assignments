import matplotlib.pyplot as plt 

# x axis values 
x = [20,40,60,80,100] 
# corresponding y axis values 
y = [54,107,160,212,257] 

y1 = [148,420,850,1230,1870] 

# plotting the points 
plt.plot(x, y, color='blue', linewidth = 2, 
		marker='o', markerfacecolor='blue',label='CME', markersize=7) 
plt.plot(x, y1, color='red', linewidth = 2, 
		marker='o', markerfacecolor='red',label='ME', markersize=7) 
# naming the x axis 
plt.xlabel('Number of threads') 
# naming the y axis 
plt.ylabel('Average waiting time (in milliseconds)') 

# giving a title to my graph 
plt.title('Comparison of CME and ME algorithms') 
plt.legend() 

# function to show the plot 
plt.show() 
