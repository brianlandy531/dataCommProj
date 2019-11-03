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
            //fprintf(stdout, "read success: %s\n", buffer);
            
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
		fprintf(stdout, "Server is ready to respond\n");
		error =	readMessageBlock(line, buf);
		//Read messages
		if(error==-1)
		{
			fprintf(stdout, "Error in process\n");
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
				fprintf(stdout, "Error in process\n");
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
					fprintf(stdout, "%d and %d\n", bufferSendCount, lastBuf);
					for(int x = 0; x<bufferSendCount; x++)
	                {
						error =	readMessage(line, messageToRecv);
	                	fwrite(messageToRecv, sizeof(char), STR_MAX_LEN, fileptr);
	                	sendAck(line);
		        		if(error==-1)
						{
							fprintf(stdout, "Error in process\n");
							on = 0;
						}
	                }
	               	if(lastBuf!=0)
	               	{
		                error =	readMessage(line, messageToRecv);
		            }
	                fwrite(messageToRecv, sizeof(char), lastBuf, fileptr);
                    fprintf(stdout,"Hereserver1\n");
	                fclose(fileptr);
	                strcpy(messageToSend, "all_done"); 
	                fprintf(stdout,"Hereserver2\n");
                    sendMessage(line, messageToSend);
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
	        		fprintf(stdout, "Error file does not exist\n");	
	        	}
	        	else
	        	{
	        		fprintf(stdout, "hellooooo");
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
	        		fprintf(stdout, "Server sending good\n");
	        		for(int i= 0; i < bufferSendCount; i++)
                        {                           
                           error = fread(messageToSend,sizeof(char),STR_MAX_LEN,fileptr);
                           if(error==0)
                           {    printf("ERROR\n"); }
                           else
                           {
                                sendMessage(line, messageToSend);
                           }
                           error = waitAck(line);
                           if(error==-1)
                           {
                            printf("ERROR\n");
                            printf("timeout error occoured\n");
                            break;
                           }
                        } 
                            if(lastBuf !=0)
                            {
                               error = fread(messageToSend,sizeof(char),lastBuf,fileptr);
                               if(error==0)
                               {
                                printf("ERROR\n");
                               }
                               else
                               {
                                    sendMessage(line, messageToSend);
                               }
                           }
                           	readMessage(line, messageToRecv);
                      		sendAck(line);
	        	}
			}
		}
	}
	fprintf(stdout, "ERROR: process Died due to connection error\n");
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
    while(1)
    {
    	sockfileDesc = socket(AF_INET, SOCK_STREAM, 0); 
    	fprintf(stdout, "Started\n");
    	if(sockfileDesc ==-1)
    	{
    		sleep(15);
        	fprintf(stdout, "Try again, socket not created\n");
    	}
	    else
	    {
	    	memset(&Server_info, 0, sizeof(Server_info));
	    	Server_info.sin_family = AF_INET;
	        //Provide string with saddr
	        Server_info.sin_addr.s_addr=inet_addr(STAT_SERVER_ADDRESS);
	        portTry = (numClients+bindRetry) % MAX_BACK_LOG;
	        Server_info.sin_port= htons(TEST_PORT+portTry);
	       	//bind
	        int bindret = bind(sockfileDesc, (SADDR_STRUCT*) &(Server_info), sizeof(Server_info));
	        if(bindret==0)
	        {
	        	fprintf(stdout, "bound properly\n");
	        	//fprintf(stdout, "%d bind ret\n", bindret);
	       		//listen
	        	int listenret = listen(sockfileDesc, MAX_BACK_LOG);
		        if(listenret==0)
		        {
		        	//accept
		        	int lengthaddr = sizeof(client_info); 
		        	printf("Waiting for connection on: %d \n", TEST_PORT+numClients);
		        	connectionFileDesc = accept(sockfileDesc, (SADDR_STRUCT*)&client_info, &lengthaddr);

		        	pid_t procNum = fork();
			        if(procNum == 0)
        			{

		        	fprintf(stdout, "connect succesfully to client number%d\n", numClients);
		        	strcpy(messageToSend, "auth");
		        	//fprintf(stdout, "%s\n", messageToSend);
					strcpy(messageToSend, "auth");
		        	int resultnum =sendMessage(connectionFileDesc,messageToSend);
		        	readMessage(connectionFileDesc, messageToRecv);
		        	int allowedIn =authorizeUser(messageToRecv);
		        	if(allowedIn==1)
		        	{
		        		fprintf(stdout, "ALLOWED ACCESS\n");
		        		reassignPort = BASE_SERVER_PORT + numClients; 
		        		sprintf(messageToSend, "Authorized:%d", reassignPort);
		        		int resultnum =sendMessage(connectionFileDesc,messageToSend);
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
			        			fprintf(stdout, "\nGot past listening\n\n");
			        			int lengthaddr = sizeof(client_info); 
			        			connectionFileDescNew = accept(sockfileDescNew, (SADDR_STRUCT*)&client_info, &lengthaddr);
			        			//pid_t procNum = fork();
			        			//if(procNum == 0)
			        			//{
			        				//Child 
	                	        	shutdown(connectionFileDesc, SHUT_RDWR); 
	        						close(connectionFileDesc);
	    							//shutdown(sockfileDesc, SHUT_RDWR);
	   	    	 					//close(sockfileDesc);		        
			        				while(1)
			        				{
	                                	fprintf(stdout, "Client number %d on port%d\n", numClients,reassignPort);
										runRespondProc(procNum, connectionFileDescNew);									
			        				}
			        			// }
			        			// else
			        			// {
			        			// 	fprintf(stdout, "Continuing in parent\n");
			        			// 	numClients++;
			        			// }
		    	    			shutdown(connectionFileDesc, SHUT_RDWR); 
			        			close(connectionFileDesc);
		        				shutdown(sockfileDesc, SHUT_RDWR);
			        			close(sockfileDesc);
			        		}	
			        	}
		        	}
		        	else
		        	{
		        		strcpy(messageToSend, "FAILED, user not in file\n");
		        		sendMessage(connectionFileDesc,messageToSend);
		        	}
		        	fprintf(stdout, "closing\n");
		        	shutdown(connectionFileDesc, SHUT_RDWR); 
	    			close(connectionFileDesc);
					shutdown(sockfileDesc, SHUT_RDWR);
	    			close(sockfileDesc);

	    			}
        			else
        			{
        				fprintf(stdout, "Continuing in parent\n");
        				numClients++;
        			}

		        }
		        else
		        {
		        	fprintf(stdout, "Error Listen Failiure\n");
		        	sleep(15);
		        }
	        }
	        else
	        {
	        	fprintf(stdout, "Binding did not happen for %d\n", TEST_PORT+portTry);
	        	bindRetry++;
	        	sleep(3);
	        }
	    }
	}
	fflush(stdout);

	
}