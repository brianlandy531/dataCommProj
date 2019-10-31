#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 

#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>

#define STR_MAX_LEN 200
#define MAX 200

#define SADDR_STRUCT struct sockaddr 
#define MAX_BACK_LOG 101
#define STAT_SERVER_ADDRESS "127.0.0.1"
#define TIMEOUT_TICKS 1000000000

#define TEST_PORT 8000

#define PASSWORD_TXT "password.txt"

#define BASE_SERVER_PORT 7000

char messageToSend[STR_MAX_LEN]="";
    char messageToRecv[STR_MAX_LEN]="";

//Turn this into an array of ports to use

//connect to input port
	//assign actual port by sending a port message
	//recconect


struct serverActivity
{
	int portsInUse[MAX_BACK_LOG];
    int startPort;

};

int numClients = 0;



int sendMessage(int socket, char* buffer)
{
	int res=0;
    
    
    res = write(socket, buffer, STR_MAX_LEN); 
    
    bzero(buffer, STR_MAX_LEN);
    return res;

}

int readMessageBlock(int socket, char* buffer)
{

    bzero(buffer, STR_MAX_LEN);
    char* start[STR_MAX_LEN];
    bzero(start, STR_MAX_LEN);
    strcpy(start, buffer);
    
    
    while(1)
    {    
        read(socket, buffer, STR_MAX_LEN);
        if(strcmp(buffer, start)!=0)
        {
            printf("%s\n", buffer);
            break;
            return 0;
        }

    }
    return -1;
}

// int recieveMessage(int socket, char* buffer)
// {



//     bzero(buffer, STR_MAX_LEN);
    


//     for(int x=0; x<TIMEOUT_TICKS; x++)
//     {


//     int res = read(socket, buffer, STR_MAX_LEN);

// 	if(strcmp()    

//     }

// }
int readMessage(int socket, char* buffer)
{

    bzero(buffer, STR_MAX_LEN);
    char* start[STR_MAX_LEN];
    bzero(start, STR_MAX_LEN);
    strcpy(start, buffer);
    
    for(int x =0; x<TIMEOUT_TICKS; x++)
    {
        
        int result = read(socket, buffer, STR_MAX_LEN);
        
		printf("%d", result);
        if(strcmp(buffer, start)!=0)
        {
            printf("read success: %s\n", buffer);
            break;
            return 0;
        }

    }
    return -1;
}


int authorizeUser(char* user)
{

	FILE* fileptr;

   	char buffer[255];

	fileptr = fopen(PASSWORD_TXT, "r");

	if(fileptr==NULL)
	{
		return -1;
	}

	while(fgets(buffer, 255, (FILE*) fileptr)) 
	{
    printf("%s\n", buffer);
    
    buffer[strcspn(buffer, "\n")] = 0;
    if(strcmp(buffer, user)==0)
	{
		fclose(fileptr);
		return 1;
	}

	}

	fclose(fileptr);

	return -1;

}


int main()
{
	int reassignPort = 0;
 	int sockfileDesc = -1;
	
    int connectionFileDesc = -1;
    int mesLen=0;
    struct sockaddr_in Server_info;
    struct sockaddr_in client_info; 
    SADDR_STRUCT sadd_serv;

    struct serverActivity serverUsage;
    

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
	        	int lengthaddr = sizeof(client_info); 
	        	connectionFileDesc = accept(sockfileDesc, (SADDR_STRUCT*)&client_info, &lengthaddr);



	        	numClients++;


	        	//need to authenticate or close connection

       			//do
	        	//Branch to a child thread passing in this FD and child information about the connect
	        	printf("connect succesfully to client number%d\n", numClients);

	        	//so stuff here


	        	

	        	//int valid = authenticate_user()

	        	sleep(1);
	        	strcpy(messageToSend, "auth");
	        	
	        	//printf("%s\n", messageToSend);
	        	
				strcpy(messageToSend, "auth");
	        	int resultnum =sendMessage(connectionFileDesc,messageToSend);
	        	//printf("%d", resultnum);



	        	readMessage(connectionFileDesc, messageToRecv);

	        	printf("%s\n", messageToRecv);

	        	int allowedIn =authorizeUser(messageToRecv);

	        	if(allowedIn)
	        	{

	        		//////////////////////////////////////////////////////////////////////
	        		//spawn child/swap to other port
	        		printf("ALLOWED ACCESS\n");

	        		//break socket out

	        		reassignPort = BASE_SERVER_PORT + numClients; 
	        		sprintf(messageToSend, "Authorized:%d", reassignPort);

	        		int resultnum =sendMessage(connectionFileDesc,messageToSend);
	        	
	        		//jump out here yo
	        		//fork();
	        		int sockfileDescNew = -1;
					int connectionFileDescNew = -1;
    
        			Server_info.sin_port= htons(reassignPort);

       				//bind
       				sockfileDescNew = socket(AF_INET, SOCK_STREAM, 0); 
        			bindret = bind(sockfileDescNew, (SADDR_STRUCT*) &(Server_info), sizeof(Server_info));
        			if(bindret==0)
        			{
        				listenret = listen(sockfileDescNew, MAX_BACK_LOG);
		        		if(listenret==0)
		       	 		{
		        			//accept

		       	 			printf("\nGot past listening\n\n");

		        			int lengthaddr = sizeof(client_info); 
		        			connectionFileDescNew = accept(sockfileDescNew, (SADDR_STRUCT*)&client_info, &lengthaddr);
		        		
//		        			sleep(1);
		        				resultnum = readMessage(connectionFileDescNew, messageToRecv);
                                printf("%s\n", messageToRecv);



				        	close(sockfileDesc);
	    	    			shutdown(connectionFileDesc, SHUT_RDWR); 
		        			close(connectionFileDesc);



		        		}	

		        	}

		        	//Close incoming traffic port

		        	///////////////////////////////////////////////////////////////////

	        	}
	        	else{

	        		printf("closing\n");

	        		shutdown(sockfileDesc, SHUT_RDWR); 

	        		close(sockfileDesc);
	        		shutdown(connectionFileDesc, SHUT_RDWR); 

	        		close(connectionFileDesc);
	        	}

	        	
	        	printf("closing\n");

	        	shutdown(sockfileDesc, SHUT_RDWR); 

	        	close(sockfileDesc);
	        	shutdown(connectionFileDesc, SHUT_RDWR); 

	        	close(connectionFileDesc);

	        }
	        else
	        {
	        	printf("error could not bind\n");
	        }


        }
        else
        {
        	printf("binding did not happen\n");
        }







    }


	
}