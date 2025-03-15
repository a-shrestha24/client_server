#include <iostream>
#include <cstdint>

// Standard *NIX headers
#include <unistd.h>
// Socket functionality
#include <sys/socket.h>
// TCP/IP protocol functionaltiy
#include <arpa/inet.h>

#include <sys/stat.h>
#include<sstream>

#include <fstream>
#include <filesystem>
#include <vector>


// Clients need an ip address and a port number to connect to
void client(in_addr_t ip,in_port_t port);
// Servers pick an arbitrary port number and reports its port
// number to the user
void server();


int check(in_addr_t ip, in_port_t port, const std::string &path);
int load(in_addr_t ip, in_port_t port, const std::string &path);
int store(in_addr_t ip, in_port_t port, const std::string &path);
int delete_file(in_addr_t, in_port_t port, const std::string &path);

// Functions for parsing ip addresses and port numbers from
// c strings
in_addr_t parse_ip(char*   ip_str);
in_port_t parse_port(char* port_str);

// Returns a socket file descriptor that is connected to the
// given ip/port
int connect_to(in_addr_t ip, in_port_t port);
// Returns a socket bound to an arbitrary port
int arbitrary_socket();
// Returns the port of the socket referenced by the input file descriptor
in_port_t get_port(int socket_fd);



int main(int argc, char *argv[]) {

    // A first argument must be supplied to indicate mode (client/server)
    if(argc < 2){
        std::cout << "Usage: p4 [mode] [options ...]" << std::endl;
        exit(1);
    }

    // Switch arg handling and execution based upon the mode
    // TODO: Make the new modes avaliable here to hit. 
    std::string mode = argv[1];
    if (mode == "server"){
        // Servers need no arguments, and simply report the port and
        // ip address they end up having
        if(argc != 2){
            std::cout << "Usage: p4 server" << std::endl;
            exit(1);
        }
        server();
    } else if (mode == "client") {
        // Clients need an ip address and port to connect to
        if(argc < 4){
            std::cout << "Usage: exchange server <ip> <port>" << std::endl;
            exit(1);
        }
        // Parse functions will kill the program early if the
        // address or port arguments are invalid.
        client(parse_ip(argv[2]),parse_port(argv[3]));
    } 
    // TODO: check(ip, port, path).
    // TODO: Parse the command, and check if the path given exists in the server. Return 0 if it exists 1 if it dosent
    else if (mode == "check") {
        // Clients need an ip address and port to connect to
        if(argc != 5){
            std::cout << "Usage: check <ip> <port> <path>" << std::endl;
            exit(1);
        }
        // Parse functions will kill the program early if the
        // address or port arguments are invalid.
        // TODO: make the check function and call it here


        // check will return a 0 is good and 1 if not there. 
        int answer  = check(parse_ip(argv[2]), parse_port(argv[3]), std::string(argv[4]));
        std::cout << answer<< std::endl;  // print answer. 
        return answer;

    } 
    // TODO: load(ip, port, path).
    // TODO: Parse the command, and check if the path given exists in the server. Return 0 if it exists and print the contents 1 if it dosent
    else if (mode == "load") 
    {
        // Clients need an ip address and port to connect to
        if(argc != 5){
            std::cout << "Usage: load <ip> <port> <path>" << std::endl;
            exit(1);
        }
        // Parse functions will kill the program early if the
        // address or port arguments are invalid.
        // TODO: make the check function and call it here


        // check will return a 0 is good and 1 if not there. 
        int answer  = load(parse_ip(argv[2]), parse_port(argv[3]), std::string(argv[4]));
        std::cout << answer<< std::endl;  // print answer. 
        return answer;

    } 

    else if (mode == "store") 
    {
        // Clients need an ip address and port to connect to
        if(argc != 5){
            std::cout << "Usage: store <ip> <port> <path>" << std::endl;
            exit(1);
        }
        // Parse functions will kill the program early if the
        // address or port arguments are invalid.
        // TODO: make the check function and call it here


        // check will return a 0 is good and 1 if not there. 
        int answer  = store(parse_ip(argv[2]), parse_port(argv[3]), std::string(argv[4]));
        std::cout << answer<< std::endl;  // print answer. 
        return answer;

    } 
    else if(mode == "delete")
    {
        //TODO:: make a delete function that goes to the path and deletes the file and retrun a 1 on competion. 

        if(argc !=5){
            std::cout<< "Usage: delete <ip> <port> <path>";
            exit(1);
        }

        int answer = delete_file(parse_ip(argv[2]), parse_port(argv[3]), std::string(argv[4]));
        std::cout<< answer << std::endl;
        return answer;
    }

   
    
    
    else 
    {
        std::cout << "Mode '" << mode << "' not recognized" << std::endl;
    }
    
    return 0;
}

// TODO: Implement the new functions that are called upon from the different modes. 

// desc : Parses a string to an ip address
// pre  : ip_str points to a valid c_string
// post : Returns the parsed ip or throws a runtime error
in_addr_t parse_ip(char*   ip_str) {
    // The 'in_addr_t' type represents an ip address, and can be
    // parsed from a string using 'inet_addr'
    in_addr_t ip_addr = inet_addr(ip_str);
    // If the parsing failed, the INADDR_NONE value will be produced
    if (ip_addr == INADDR_NONE) {
        throw std::runtime_error("Failed to convert input ip address.");     
    }
    return ip_addr;
}

// desc : Parses a string to a port number
// pre  : port_str points to a valid c_string
// post : Returns the parsed port number or throws a runtime exception
in_port_t parse_port(char* port_str) {
    // Parse port number from argument
    in_port_t port = atoi(port_str);
    // 'atoi' returns zero on error. Port zero is not a 'real' port.
    if(port == 0) {
        throw std::runtime_error("Invalid port argument.");     
    }
    return port;
}


// desc : Returns a tcp/ip socket
// pre  : None
// post : Returns a tcp/ip socket or throws a runtime exception
int make_tcp_ip_socket() {
    // Make a socket, which is a special type of file that acts as a
    // holding area for sent/recieved data.
    //
    //  - PF_INET means the Port Family is for InterNET
    //  - SOCK_STREAM indicates it should use the TCP protocol
    int socket_fd = socket(PF_INET,SOCK_STREAM,0);
    // If the fd is negative, socket allocation failed
    if(socket_fd < 0){
        throw std::runtime_error("Could not allocate socket.");
    }
    return socket_fd;
}


// desc : Returns a socket connected to the given ip address and
//        port number
// pre  : ip is a valid ip address. port is a valid port number
// post : If an error is encountered, a runtime exception is thrown
int connect_to(in_addr_t ip, in_port_t port) {
    // Set up socket address data structure, which we will use
    // to tell the OS what we want to do with our socket
    sockaddr_in socket_addr;
    // AF_INET means the Address Family is for InterNET
    socket_addr.sin_family = AF_INET;
    // Set the ip address to connect to
    socket_addr.sin_addr.s_addr = ip;
    // Set the port to connect to
    // htons converts endianness from host to network
    socket_addr.sin_port = htons(port);

    // Make socket to connect through
    int socket_fd = make_tcp_ip_socket();

    // Tell the OS we want to connect to the ip/port indicated by
    // socket_addr through the socket represented by the file
    // descriptor 'socket_fd'
    int status = connect(socket_fd,(sockaddr*)&socket_addr,sizeof(socket_addr));
    // If output is negative, the connection was not successful.
    if(status < 0) {
        // Make sure socket get cleaned up
        close(socket_fd);
        throw std::runtime_error("Connection failed.");
    }
    return socket_fd;
}

// desc : Returns a socket bound to an arbitrary port
// pre  : None
// post : If an error is returned, a runtime exception is thrown
int arbitrary_socket() {
    // Set up socket address data structure, which we will use
    // to tell the OS what we want to do with our socket
    sockaddr_in socket_addr;
    // AF_INET means the Address Family is for InterNET
    socket_addr.sin_family = AF_INET;
    // Indicate we are willing to connect with any ip address
    socket_addr.sin_addr.s_addr = INADDR_ANY;
    // Use zero-valued port to tell OS to pick any available
    // port number
    socket_addr.sin_port = 0;

    // Make a socket to listen through
    int socket_fd = make_tcp_ip_socket();

    // Bind socket to an arbitrary available port
    int status = bind(
        socket_fd,
        (struct sockaddr *) &socket_addr,
        sizeof(sockaddr_in)
    );
    if(status < 0) {
        throw std::runtime_error("Binding failed.");
    }
    return socket_fd;
}

// desc : Returns the port that the provided file descriptor's
//        socket is bound to
// pre  : The provided socket file descriptor is valid
// post : If an error is encountered, a runtime exception is thrown
in_port_t get_port(int socket_fd) {
    // A receptacle for the syscall to write the port number
    sockaddr_in socket_addr;
    // You need to supply the size of the receptacle through
    // a pointer. This seems rather silly, but is onetheless necessary.
    socklen_t socklen = sizeof(sockaddr_in);
    // Get the "name" (aka port number) of socket
    int status = getsockname(
        socket_fd,
        (struct sockaddr *) &socket_addr,
        &socklen
    );
    if (status < 0) {
        throw std::runtime_error("Failed to find socket's port number.");
    }
    // Flip endianness from network to host
    return ntohs(socket_addr.sin_port);
}

// desc : Connects to server and sends a one-line message
// pre  : ip is a vaid ip address and port is a valid port number
// post : If an error is encountered, a runtime exception is thrown
void client(in_addr_t ip, in_port_t port) {

    // Attempt to connect to server through a new socket.
    // Return early if this fails.
    int socket_fd = connect_to(ip,port);
    if(socket_fd < 0) {
        return;
    }

    // Get a one-line message from the user
    std::string message;
    std::getline(std::cin,message);

    // Transmit message size ahead of time, so server can
    // pre-allocate message storage
    uint32_t message_size = message.size();
    send(socket_fd,&message_size,sizeof(message_size),0);

    // Transmit message
    send(socket_fd,message.c_str(),message.size(),0);

    // close connection
    close(socket_fd);
}

// desc : Listens on an arbitrary port (announced through stdout)
//        for connections, recieving messages as 32-bit string
//        lengths followed my a sequence of characters matching that
//        length.
// pre  : None
// post : If a listening socket cannot be set up, a runtime exception
//        is thrown. If a connection fails or disconnects early, the
//        error is announced but the server continues operation.
void server() {

    // Make an arbitrary socket to listen through
    int socket_fd = arbitrary_socket();
    int port = get_port(socket_fd);

    std::cout << "Setup server at port "<< port << std::endl;

    // Tell OS to start listening at port for its set protocol
    // (in this case, TCP IP), with a waiting queue of size 1.
    // Additional connection requests that cannot fit in the
    // queue will be refused.
    int status = listen(socket_fd,1);
    if( status < 0 ) {
        std::cout << "Listening failed." << std::endl;
        return;
    }

    // A receptacle to store information about sockets we will
    // accept connections through
    sockaddr_storage storage;
    socklen_t socket_len = sizeof(sockaddr_storage);
    while(true){
        
        // Wait until a connection request arrives at the port
        int connection_fd = accept(
            socket_fd,
            (sockaddr*)&storage,
            &socket_len
        );

        // Ignore failures. Clients can do weird things sometimes, and
        // it's not a good idea to simply crash whenever that happens.
        if(connection_fd < 0){
            std::cout << "Could not accept connection.\n";
            continue;
        }

        uint32_t message_size = 0;
        uint32_t iterator     = 0;
        // Attempt to recieve up to 'message_size' bytes from the connection,
        // recieving them in the variable 'message_size'.
        //
        // NOTE: recv may not recieve 'message_size' bytes. A program must
        // react to the returned byte count to know what to do next. If
        // fewer than the requested size is needed, you may need to re-call
        // recv to get the subsequent bytes.
        int n = recv(connection_fd,&message_size,sizeof(message_size),0);
        
        // If recieved char size is zero, the connection was closed on the
        // other side
        if(n == 0){
            std::cout << "Client disconnected early.\n";
            close(connection_fd);
            continue;
        }

        // Allocate a buffer to recieve the real message, with one
        // additional byte to hold a null character
        char* message = new char[message_size+1];

        // Attempt to recieve the real message.
        // NOTE: Again, this may not recieve all of the bytes that
        // are requested. In your submission, you may need to react to
        // n<message_size for long messages.
        n = recv(connection_fd,&message[iterator],message_size,0);
        if(n == 0){
            std::cout << "Client disconnected early.\n";
            continue;
        }
        message[message_size] = '\0';
        

        // TODO: Seperate the message. 
        std::string msg(message); // helps split apart message. 
        delete[] message; // clean up message. 

        std::istringstream iss(msg); // needed to get the file path. 

        std::string command; // the command that is given from the user. 
        iss>>command; // get the command 
        
        
        //TODO: Sever handles the CHECK command if given
        if(command == "CHECK"){ // if CHECK is the command then server needs to send back if file path exists. 
            
            // get the file path from iss. 
            std::string filepath;
            iss>>filepath;
           
            struct stat buffer;
            bool exists = (stat(filepath.c_str(), &buffer) == 0); // use the stat system call fill the buffer. 

            int answer = 0; // file is good

            if(exists == false){
                answer = 1;
            }

            int answer_size = sizeof(answer);

            send(connection_fd, &answer, answer_size, 0);
        }

        // TODO: Server handles the Load command == Open file and send the message back
        else if(command == "LOAD")
        {
            // get the file path 
            std::string filepath;
            iss>>filepath;

            // open the file
            std::ifstream infile(filepath, std::ios::binary);

            // file cant open so send back a 1
            if(!infile){

                int error = 1;
                int errorsize = sizeof(error);

                send(connection_fd, &error, errorsize,0);
                close(connection_fd);

                continue;
            }

            // get the file size, and set pointer back the begining
            infile.seekg(0, std::ios::end); // pointer to end
            std::streamsize file_size = infile.tellg(); // get the file size
            infile.seekg(0,std::ios::beg); // set back to the start
            
            // send back the file size so the first chars arent missing. 
            send(connection_fd, &file_size, sizeof(file_size), 0);
                
            
            // array to hold message to send. 
            char buffer[10000];

            // While there is data send to the client. 
            while(infile){

                infile.read(buffer, 10000);// send 1024 bytes to the buffer. 
                // get the actual count of the file. 
                int total = infile.gcount();

                // make sure there is something in current
                if (total>0){
                    // make sure we dont go above current
                    int count = 0;

                   
                    while(count <= total){

                        // send message to the client,
                        // buffer needs to be where count is pointing to, 
                        // get the length of the message by doing the total - count. 
                        int size = total - count;
                        int push = send(connection_fd, &buffer[count], size, 0);

                        // update the count with the amount of bytes pushed
                        count += push;
                    }
                }
            }
            infile.close();
        }
        else if(command == "STORE")
        {

            // get the file path
            std::string filepath;
            iss>>filepath;

            // open file or create a new one
            std::ofstream outFile(filepath);
            
            // check if the outFile works
            if(!outFile){

                int error = 1;
                int errorsize = sizeof(error);

                send(connection_fd, &error, errorsize,0);
                close(connection_fd);

                continue;
            }

            int data = 0;
            int recive = recv(connection_fd, &data, sizeof(data),0);

            std::string file_data;

            file_data.resize(data);

            int total = 0;

            while(total< data){
                int x = recv(connection_fd, &file_data[total], data - total, 0);
                total += x;
            }

            outFile.write(file_data.c_str(), total);
            outFile.close();
            close(connection_fd);
        }

        // TODO: Delete the file at the file path
        else if(command == "DELETE"){
            std::string filepath;
            iss>>filepath;

            // check should be 1 if true. 
            int check = std::filesystem::remove(filepath);

            // swap around the check

            if(check == 1){
                check = 0;
            }else{
                check = 1;
            }
        
            int checksize = sizeof(check);

            send(connection_fd, &check, checksize,0);
            close(connection_fd);

            
        }
        

        

        else{
            std::cout <<'('<<message<<')'<<std::endl; 
        }


        // Close connection once transaction is complete 
        close(connection_fd);
    }
}



int check(in_addr_t ip, in_port_t port, const std::string &path) {
    int socket_fd = connect_to(ip, port); // make the socket needed. 

    // Error checking for the socket
    if(socket_fd < 0) {
        std::cout<< "Failed to connect to server" << std::endl;
        return 1;  // return an error value
    }

    // build command to always be "check (path)"
    std::string command = "CHECK ";
    std::string command_string = command + path;
    int command_length = command_string.size();


    // SEND commands to the sever
    if (send(socket_fd, &command_length, sizeof(command_length), 0) != sizeof(command_length)) {

        std::cout<< "Failed to connect" << std::endl;
        close(socket_fd);
        return 1;
    }

    // Send the command string.
    if (send(socket_fd, command_string.c_str(), command_length, 0) != (ssize_t)command_length) {
        std::cout<< "Failed to connect" << std::endl;
        close(socket_fd);
        return 1;
    }



    // TODO: Get the answers from the server. 

    // Placeholders
    int answer = 0; 
    int size = sizeof(answer);

    // get the response from server
    int response_code = recv(socket_fd, &answer, size, 0);
    close(socket_fd); // close socket


    // handle no response
    if(response_code == 0) {
        std::cerr << "Server didn't respond." << std::endl;
        return 1;
    }

    // If the server returns 0, it means the file exists; otherwise, 1.
    
    if (answer == 1){
        return 1;
    }else{
        return answer;
    }
}

int load(in_addr_t ip, in_port_t port, const std::string &path)
{
    // TODO: Check if file exists, then print file and return zero if exists otherwise return 1

    int socket_fd = connect_to(ip, port);
    if(socket_fd < 0) {
        return 1;
    }

    std::string command = "LOAD ";
    std::string command_string = command + path;
    uint32_t command_length = command_string.size();


    // SEND commands to the sever
    if (send(socket_fd, &command_length, sizeof(command_length), 0) != sizeof(command_length)) {

        std::cout<< "Failed to connect" << std::endl;
        close(socket_fd);
        return 1; // return error. 
    }

    // Send the command string.
    if (send(socket_fd, command_string.c_str(), command_length, 0) != (ssize_t)command_length) {
        std::cout<< "Failed to connect" << std::endl;
        close(socket_fd);
        return 1; // return error
    }
    

    // TODO: check for errors

    uint32_t check =0;

    int reality_check = recv(socket_fd, &check, sizeof(check),0);
    if(reality_check != sizeof(check)){
        close(socket_fd);
        return 1;
    }

    if(check == 1){
        close(socket_fd);
        return 1;
    }

    // TODO: Recieve the incoming bytes of the file and print them. 

    std::vector<char> buffer(check);
    size_t total_received = 0;
    while(total_received < check) {
        reality_check = recv(socket_fd, buffer.data() + total_received, check - total_received, 0);
        if(reality_check<= 0) {
            std::cerr << "Error receiving file data" << std::endl;
            close(socket_fd);
            return 1;
        }
        total_received += reality_check;
    }
    
    // Print the file contents.
    std::cout.write(buffer.data(), total_received);
    std::cout << std::endl;

    // close socket after reading
    close(socket_fd);

    std::cout << std::endl;
    return 0;// return all good. 
    
}
    



int store(in_addr_t ip, in_port_t port, const std::string &path){
    int socket_fd = connect_to(ip, port);

    if(socket_fd < 0) {
        std::cout<< "Failed to connect to server" << std::endl;
        return 1;  // return an error value
    }

    // build command to always be "check (path)"
    std::string command = "STORE ";
    std::string command_string = command + path;
    int command_length = command_string.size();

    // SEND commands to the sever
    if (send(socket_fd, &command_length, sizeof(command_length), 0) != sizeof(command_length)) {

        std::cout<< "Failed to connect" << std::endl;
        close(socket_fd);
        return 1;
    }

    // Send the command string.
    if (send(socket_fd, command_string.c_str(), command_length, 0) != (ssize_t)command_length) {
        std::cout<< "Failed to connect" << std::endl;
        close(socket_fd);
        return 1;
    }




    

    std::string line;
    std::string fullInput;

    while (std::getline(std::cin, line)) {
        fullInput += line + "\n"; // Append the line with a newline
    }

    std::cout << "You entered:" << std::endl;
    std::cout << fullInput;
    
    // Send the size of the input string first.
    int input_size = fullInput.size();
    send(socket_fd, &input_size, sizeof(input_size), 0);
       
    
    // Send the entire input string.
    send(socket_fd, fullInput.c_str(), input_size, 0);
       
    
    close(socket_fd);
    return 0;


    

}


int delete_file(in_addr_t ip, in_port_t port, const std::string &path){
    //Open socket
    int socket_fd = connect_to(ip,port);
    // error catcher
    if(socket_fd < 0) {
        return 1;
    }

    std::string command = "DELETE ";
    std::string command_string = command + path;
    
    int command_length = command_string.size();

    //send message size
    send(socket_fd, &command_length, sizeof(command_length),0);

    //send actual message
    send(socket_fd, command_string.c_str(), command_string.size(),0);



    // Placeholders
    int answer = 0; 
    int size = sizeof(answer);

    // get the response from server
    int response_code = recv(socket_fd, &answer, size, 0);
    


    // handle no response
    if(response_code == 1) {
        std::cerr << "Server didn't respond." << std::endl;
        close(socket_fd); // close socket
        return 1;
    }
    close(socket_fd); // close socket
    // If the server returns 0, it means the file exists; otherwise, 1.
    
    if (answer == 1){
        return 1;
    }else{
        return answer;
    }
 
    


}
