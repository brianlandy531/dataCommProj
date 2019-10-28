#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 

#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>

#define STR_MAX_LEN 256
#define SADDR_STRUCT struct sockaddr 
#define MAX_BACK_LOG 101
#define STAT_SERVER_ADDRESS "127.0.0.1"

#define TEST_PORT 8080
//Turn this into an array of ports to use

//connect to input port
	//assign actual port by sending a port message
	//recconect

int numClients = 0;

int main()
{
 	int sockfileDesc = -1;
    int connectionFileDesc = -1;
    int mesLen=0;
    struct sockaddr_in Server_info;
    struct sockaddr_in client_info; 
    SADDR_STRUCT sadd_serv;

    char messageToSend[STR_MAX_LEN];
    char messageToRecv[STR_MAX_LEN];

    sockfileDesc = socket(AF_INET, SOCK_STREAM, 0); 

    printf("Started\n");

    if(sockfileDesc ==-1)
    {
        printf("Try again, socket not created\n");

    }
    else
    {

    	memset(&Server_info, 0, sizeof(Server_info));
    	Server_info.sin_family = AF_INET;
        //Provide string with saddr
        Server_info.sin_addr.s_addr=inet_addr(STAT_SERVER_ADDRESS);
        Server_info.sin_port= htons(TEST_PORT);

       	//bind
        int bindret = bind(sockfileDesc, (SADDR_STRUCT*) &(Server_info), sizeof(Server_info));

        if(bindret==0)
        {
        	printf("bound properly\n");
        	printf("%d bind ret\n", bindret);


       		//listen

        	int listenret = listen(sockfileDesc, MAX_BACK_LOG);

       		

	        if(listenret==0)
	        {
	        	//accept
	        	connectionFileDesc = accept(sockfileDesc, (SADDR_STRUCT*)&client_info, sizeof(client_info));

	        	numClients++;

       			//do
	        	//Branch to a child thread passing in this FD and child information about the connect
	        	printf("connect succesfully to client number", numClients);

	        	//so stuff here

	        	

	        	
	        	printf("closing\n");

	        	close(sockfileDesc); 


	        }


        }







    }


	
}