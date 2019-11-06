#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>

#define STR_MAX_LEN 200
#define MAX 200
#define RESPONSE "recvd"
#define SADDR_STRUCT struct sockaddr 
#define MAX_BACK_LOG 100
#define STAT_SERVER_ADDRESS "127.0.0.1"
#define TIMEOUT_TICKS 1000000000
#define TEST_PORT 8000
#define PASSWORD_TXT "password.txt"
#define BASE_SERVER_PORT 7000

char messageToSend[STR_MAX_LEN]="";
char messageToRecv[STR_MAX_LEN]="";

int messageToRecvBin[STR_MAX_LEN];

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

int sendAck(int socket)
{
	strcpy(messageToSend, RESPONSE);
    int res = write(socket, messageToSend, STR_MAX_LEN); 
    bzero(messageToSend, STR_MAX_LEN);
    return res;

}

int waitAck(int socket)
{
    bzero(messageToRecv, STR_MAX_LEN);
    for(int x =0; x<TIMEOUT_TICKS; x++)
    {
        int result = read(socket, messageToRecv, STR_MAX_LEN);
        if(strcmp(messageToRecv, RESPONSE)==0)
        {            
            return 0;
        }
    }
    return -1;
}

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
            return 0;
            break;
        }
    }
    return -1;
}

int readMessage(int socket, char* buffer)
{

    bzero(buffer, STR_MAX_LEN);
    char* start[STR_MAX_LEN];
    bzero(start, STR_MAX_LEN);
    strcpy(start, buffer);
    
    for(int x =0; x<TIMEOUT_TICKS; x++)
    {
        
        int result = read(socket, buffer, STR_MAX_LEN);
        
		//result(stdout, "%d", result);
        if(strcmp(buffer, start)!=0)
        {
            //s(stdout, "read success: %s\n", buffer);
            
            return 0;
 			break;
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

//Logic for get file put file and close and stuff
int runRespondProc(pid_t procNum, int line)
{
	char buf[STR_MAX_LEN] = "";
	char name[STR_MAX_LEN] = "";
	int fgetCRes[STR_MAX_LEN];
	FILE* fileptr = NULL;
	int on =1;
	int c = -1;
	int fileLen = 0;
	int error = 0;
	int bufferSendCount = 0;
	int lastBuf=-1;
	while(on)
	{
		fprintf(stdout, "[Server]  Server is ready to respond\n");
		error =	readMessageBlock(line, buf);
		//Read messages
		if(error==-1)
		{
			fprintf(stdout, "[Server]  Error in process\n");
			on = 0;
		}
		else
		{
			if (strcmp( "tputStart", buf) ==0 )
	        {
	        	//record filename
	        	error =	readMessage(line, name);
	        	if(error==-1)
				{
				fprintf(stdout, "[Server]  Error in process\n");
					on = 0;
				}
				strcat(name, "copied.txt");
	        	fileptr = fopen(name,"wb");
	        	if(fileptr)
	        	{
	        		error =	readMessage(line, messageToRecv);
					char delims[] = ":\n";
					char* token = strtok(messageToRecv, delims); 
					lastBuf = atoi(token);
					token = strtok(NULL, delims); 
					bufferSendCount = atoi(token);
					fprintf(stdout, "[Server]  %d and %d\n", bufferSendCount, lastBuf);
					for(int x = 0; x<bufferSendCount; x++)
	                {
						error =	readMessage(line, messageToRecv);
	                	fwrite(messageToRecv, sizeof(char), STR_MAX_LEN, fileptr);
	                	sendAck(line);
		        		if(error==-1)
						{
							fprintf(stdout, "[Server]  Error in process\n");
							on = 0;
						}
	                }
	               	if(lastBuf!=0)
	               	{
		                error =	readMessage(line, messageToRecv);
		            }
	                fwrite(messageToRecv, sizeof(char), lastBuf, fileptr);
	                fclose(fileptr);
	                strcpy(messageToSend, "all_done"); 
                    sendMessage(line, messageToSend);
	        		fprintf(stdout, "[Server]  Tput finished for %s\n", name);

				}
			}
			else if (strcmp( "tgetStart", buf) ==0 )
	        {
	        	sendAck(line);
	        	readMessage(line, messageToRecv);
	        	strcpy(name,messageToRecv);
	        	fileptr = fopen(name, "rb");
	        	if(fileptr==NULL)
	        	{
	        		fprintf(stdout, "[Server]  Error file does not exist\n");	
	        		strcpy(messageToSend, "ERROR FDE");
	        		sendMessage(line, messageToSend);
	        		waitAck(line);
	        	}
	        	else
	        	{
	        		fprintf(stdout, "[Server]  File request is valid\n");
	        		strcpy(messageToSend, "VALID_FILE");
	        		sendMessage(line, messageToSend);
	        		fileLen = 0;   
                    while ((c = getc(fileptr)) != EOF) {fileLen++;}
        			fflush(fileptr);
                    rewind(fileptr);
                    fclose(fileptr); 
                    fileptr = fopen(name, "rb");
                    bufferSendCount =  fileLen/STR_MAX_LEN;
                    lastBuf = fileLen%STR_MAX_LEN;
                    sprintf(messageToSend, "%d:%d:", lastBuf, bufferSendCount);
	        		sendMessage(line, messageToSend);
	        		waitAck(line);
	        		//fprintf(stdout, "[Server]  Server sending good\n");
	        		for(int i= 0; i < bufferSendCount; i++)
                        {                           
                           error = fread(messageToSend,sizeof(char),STR_MAX_LEN,fileptr);
                           if(error==0)
                           {    fprintf(stdout, "[Server]  ERROR\n"); }
                           else
                           {
                                sendMessage(line, messageToSend);
                           }
                           error = waitAck(line);
                           if(error==-1)
                           {
                            fprintf(stdout, "[Server]  ERROR\n");
                            fprintf(stdout, "[Server]  timeout error occoured\n");
                            break;
                           }
                        } 
                            if(lastBuf !=0)
                            {
                               error = fread(messageToSend,sizeof(char),lastBuf,fileptr);
                               if(error==0)
                               {
                                fprintf(stdout, "[Server]  ERROR\n");
                               }
                               else
                               {
                                    sendMessage(line, messageToSend);
                               }
                           }
                           	readMessage(line, messageToRecv);
                      		sendAck(line);
  			        		fprintf(stdout, "[Server]  Tget finished for %s\n", name);

	        	}
			}
		}
	}
	fprintf(stdout, "[Server] ERROR: process Died due to connection error\n");
	fflush(stdout);
}


int main()
{
	int bindRetry=0;
	int reassignPort = 0;
 	int sockfileDesc = -1;
	
    int connectionFileDesc = -1;
    int mesLen=0;
    struct sockaddr_in Server_info;
    struct sockaddr_in client_info; 
    SADDR_STRUCT sadd_serv;

    struct serverActivity serverUsage;
    int portTry = 0;
    sockfileDesc = socket(AF_INET, SOCK_STREAM, 0); 
    while(1)
    {

    	
    	fprintf(stdout, "[Server] Started\n");
    	if(sockfileDesc ==-1)
    	{
    		sleep(15);
        	fprintf(stdout, "[Server] Try again, socket not created\n");
    	}
	    else
	    {
	    	memset(&Server_info, 0, sizeof(Server_info));
	    	Server_info.sin_family = AF_INET;
	        //Provide string with saddr
	        	//inaddr any means any incoming connection will show up, not just loop back or a specific ip
	        Server_info.sin_addr.s_addr= htons(INADDR_ANY); //inet_addr(STAT_SERVER_ADDRESS);
	        portTry = (numClients+bindRetry) % MAX_BACK_LOG;
	        Server_info.sin_port= htons(TEST_PORT+portTry);
	       	//bind
	        int bindret = bind(sockfileDesc, (SADDR_STRUCT*) &(Server_info), sizeof(Server_info));
	        if(bindret==0)
	        {
	        	fprintf(stdout, "[Server] Bound properly\n");
	        	//fprintf(stdout, "[Server] %d bind ret\n", bindret);
	       		//listen
	        	int listenret = listen(sockfileDesc, MAX_BACK_LOG);
		        if(listenret==0)
		        {
		        	//accept
		        	int lengthaddr = sizeof(client_info); 
		        	fprintf(stdout, "[Server] Waiting for connection on port: %d \n", TEST_PORT+portTry);
		        	

		        	while(1)
    				{

			        	connectionFileDesc = accept(sockfileDesc, (SADDR_STRUCT*)&client_info, &lengthaddr);

			        	pid_t procNum = fork();
				        if(procNum == 0)
	        			{


	        				waitAck(connectionFileDesc);

				        	fprintf(stdout, "[Server] Connected succesfully to client number: %d\n", numClients);
				        	strcpy(messageToSend, "auth");
				        	//fprintf(stdout, "[Server] %s\n", messageToSend);
							//strcpy(messageToSend, "auth");

				        	int resultnum =sendMessage(connectionFileDesc,messageToSend);
				        	readMessage(connectionFileDesc, messageToRecv);
				        	int allowedIn =authorizeUser(messageToRecv);
				        	if(allowedIn==1)
				        	{
				        		fprintf(stdout, "[Server] ALLOWED ACCESS\n");
				        		//reassignPort = BASE_SERVER_PORT + numClients; 
				        		sprintf(messageToSend, "Authorized");//, reassignPort);
				        		int resultnum =sendMessage(connectionFileDesc,messageToSend);
				        		
		        				while(1)
		        				{
	                            	fprintf(stdout, "[Server] Client number %d on port %d\n", numClients,TEST_PORT+portTry);
									runRespondProc(procNum, connectionFileDesc);									
		        				}
		    	    			//shutdown(sockfileDesc, SHUT_RDWR);
		        				//close(sockfileDesc);
					    
				        	}
				        	else
				        	{
				        		strcpy(messageToSend, "FAILED");
				        		sendMessage(connectionFileDesc,messageToSend);
					        	fprintf(stdout, "[Server] Client %d failed auth process\n", numClients);

				        	}
				        	fprintf(stdout, "[Server] Closing connection\n");
				        	shutdown(connectionFileDesc, SHUT_RDWR); 
			    			close(connectionFileDesc);
							//shutdown(sockfileDesc, SHUT_RDWR);
			    			//close(sockfileDesc);

		    			}
	        			else
	        			{
	        				fprintf(stdout, "[Server] Continuing in parent\n");
	        				numClients++;
	        			}
        			}

		        }
		        else
		        {
		        	fprintf(stdout, "[Server] Error Listen Failiure\n");
		        	sleep(1);
		        	bindRetry++;
		        }
	        }
	        else
	        {
	        	fprintf(stdout, "[Server] Binding did not happen for %d\n", TEST_PORT+portTry);
	        	bindRetry++;
	        	sleep(1);
	        }
	    }
	
	fflush(stdout);

	
    }
}
