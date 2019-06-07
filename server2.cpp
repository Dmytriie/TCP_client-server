/*
	C socket server example
*/

#include<stdio.h>	
#include <stdlib.h> //itoa
#include <string> //strlen
#include <cstring> 
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

using namespace std;

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size; 
	struct sockaddr_in server , client; //to connect to the socket. Shitty place in this code
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0); //in this way all sockets are initialized. AF_INET = family of addresses for tcp
	//more is here https://stackoverflow.com/questions/1593946/what-is-af-inet-and-why-do-i-need-it
	if (socket_desc == -1) //socket method will return -1 if there were mistakes
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure. After the point you will have special fileds of the server structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );  //explanation here https://linux.die.net/man/3/htons
	
	//Bind to establish the connection
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen everything we will have on the port 8888
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");
	
	//Receive a message from client
	while(1) 
	{
	    char client_message[2000]; //size of the message. 
		recv(client_sock , client_message , 2000 , 0); //command to get data from port if they are there. Returns int which is siz of the message, but client_message is the shit you need 
		strcat(client_message,"\n"); //add newline symbol to your char array. You can not do it so simple, only with a special function. DO NOT CHANGE DOUBLE QUOTES TO UNARY!!! const *char != char DON`T DO IT, BLYAT!!!  
		strcat(client_message, to_string(strlen(client_message) ).c_str() ); //add length of your message to the existing message AND newline symbol/
		write(client_sock , client_message , strlen(client_message ) ); //Send the message back to client
		for (int i = 0; i < 2000; i++) //clean the array to be able to read new message and forget the old one. Without loop it doesn`t work, without '/0' you will only add stuff and size of the message will just grow
		{
			client_message[0] = '\0'; //DO NOT CHANGE DOUBLE QUOTES TO UNARY!!! char != const *char DON`T DO IT, BLYAT!!!  Here you really need unary.  
		}
	}
	
	if(read_size == 0) //0 means "goodbye, I am leaving now"
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
	
	return 0;
}
