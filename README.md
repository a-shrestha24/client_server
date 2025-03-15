Aaron Shrestha
p4

Server: ./p4 server
This will start the server to start recieving commands and messages from the user.
It will handle all the file system calls 

Check: p4 check <ip> <port> <path>
This will check if the path at the given ip and port exists or not by running a 
stat sys call on the file system. It will returna 0 if there is a file and 1 otherwise

Load: p4 load <ip> <port> <path>
This will go to the given file if it exsists and take all the values from the file. Then return
the user everything in the file. 

Store: p4 store <ip> <port> <path>
This will take in user input on the client side until ctrl + D is pressed. Then take that string and 
place it within the given file or create a new file and place the string in there

Delete: p4 delete <ip> <port> <path>
This will take  a path from the user and go to that file if it exists and delete it. 0 returns if file is sucessfully deleted and returns 1 otherwise. 