#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <time.h> 

#define STR_MAX_LEN 200
#define MAX 200
#define MAX_ARGS 5
#define SADDR_STRUCT struct sockaddr 
#define TEST_PORT 8000
#define TIMEOUT_TICKS 1000000000
#define RESPONSE "recvd"
#define INPUT_RANGE 100

char messageToSend[STR_MAX_LEN];
char messageToRecv[STR_MAX_LEN];

struct tokenInputs
{
    int argCount;
    char inArg[MAX_ARGS][STR_MAX_LEN];
};

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


int findIndex(const char *fullstring, const char *lookfor) 
{
    int i =0;
    int j = 0;
    while(1) 
    {
        while(1) 
        {
            if (lookfor[j] == '\0') 
            {
                return i;
            }
            if (fullstring[i + j] != lookfor[j]) {
                break;
            }
            j++;
        }
        i++;
        if (fullstring[i] == '\0') 
        {
            return -1;
        }
    }
}


int getPortFromAuthMsg(const char *fullstring)
{
    char port[256]="";
    char newPort[10] = "";
    int k = findIndex(fullstring, ":");
    strncpy(port, fullstring, k);
    strcpy(port, &(fullstring[k+1]));
    return atoi(port);
}

int clearToks(struct tokenInputs *input)
{
    strncpy(input->inArg[0], "\0", STR_MAX_LEN);
    strncpy(input->inArg[1], "\0", STR_MAX_LEN);
    strncpy(input->inArg[2], "\0", STR_MAX_LEN);
    strncpy(input->inArg[3], "\0", STR_MAX_LEN);
    strncpy(input->inArg[4], "\0", STR_MAX_LEN);
    return 0;
}

int getTokens( char *inbuff, struct tokenInputs *outToks)
{
    char toks[STR_MAX_LEN] = "";
    int argCount =0; 
    char delims[] = " \n";
    struct tokenInputs* tokStruct = NULL;
    if(tokStruct ==NULL)
    {
        tokStruct = malloc(sizeof( struct tokenInputs));
    }    
    strncpy(toks, inbuff, STR_MAX_LEN);
    char* token = strtok(toks, delims); 
    while (token != NULL) { 
        strncpy(tokStruct->inArg[argCount], token, STR_MAX_LEN);
        argCount++;
        token = strtok(NULL, delims); 
        if(argCount==5)
        {
            break;
        }
    } 
    memcpy(outToks, tokStruct, sizeof(struct tokenInputs));
    clearToks(tokStruct);
    free(tokStruct);
    return 0; 
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
        if(strcmp(buffer, start)!=0)
        {
            return 0;
        }
    }
    return -1;
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
        }
    }
    return -1;
}

int sendMessage(int socket, char* buffer)
{
    int res = write(socket, buffer, STR_MAX_LEN); 
    bzero(buffer, STR_MAX_LEN);
    return res;

}

int sendMessageBin(int socket, int* buffer)
{
    int res = write(socket, buffer, sizeof(int)*STR_MAX_LEN); 
    bzero(buffer, sizeof(int)*STR_MAX_LEN);
    return res;

}


int main(int argc, char *argv[])
{

    int sockfileDesc = -1;
   
    int mesLen=0;
    struct sockaddr_in Server_info;
    struct sockaddr_in client_info; 
    SADDR_STRUCT sadd_serv;
    FILE* fileptr;
    char name[STR_MAX_LEN] = "";
    int error = 0;

    int bufferSendCount = 0;
    int lastBuf=-1;

    int c = 0;
                
    int fileLen = 0;
    char buffer[STR_MAX_LEN];
    int cur=0;


	char inbuff[STR_MAX_LEN] = "";
    fprintf(stdout, "Program name %s\n", argv[0]);
    char delims[] = " \n";
    struct  tokenInputs* currTok;
    currTok = malloc(sizeof( struct tokenInputs));
    int tconnect = 0;



    while(fgets(inbuff, STR_MAX_LEN , stdin) != NULL)
    {
        getTokens(inbuff, currTok);
        if (strcmp( "tconnect", currTok->inArg[0]) ==0 )
        {
                // do something
                
                if(tconnect==1)
                {
                    fprintf(stdout, "Already connected\n");
                }
                {
                    fprintf(stdout, "tconnecting\n");
                    //Server connect logic
                    
                    tconnect =1;


                    sockfileDesc = socket(AF_INET, SOCK_STREAM, 0); 
                    sockfileDesc = socket(AF_INET, SOCK_STREAM, 0); 

                    if(sockfileDesc ==-1 || sockfileDesc ==-1)
                    {
                        fprintf(stdout, "Try again, socket not created\n");

                    }
                    else
                    {
                        memset(&Server_info, 0, sizeof(Server_info));
                        Server_info.sin_family = AF_INET;
                        //Provide string with saddr
                        Server_info.sin_addr.s_addr=inet_addr(currTok->inArg[1]);
                        
                        //sadd_serv = (SADDR_STRUCT) Server_info;
                        int retval = -1;
                        int ctr = 0;
                        int portToUSe = 0;
                        int MAX_RETRY = 15;
                        int retryRange = 0;
                        int found =0;
                        int MAXdel = 2500000;
                        int MINdel = 500000;
                        int randAdd;
                      
                      //////////////////////

                        while(retryRange < MAX_RETRY)
                        {
                            ctr = 0;

                        while(ctr< INPUT_RANGE)
                        {

                            
                            randAdd =0; // (rand()%(MAXdel-MINdel+1)) + MINdel; 
                            usleep(45000+randAdd);

                            portToUSe = TEST_PORT+ctr;
                            Server_info.sin_port= htons(portToUSe);
                            retval = connect(sockfileDesc, (SADDR_STRUCT*)&Server_info, sizeof(Server_info));
                           
                            if(retval==0)
                            {
                                fprintf(stdout, "Socket assigned to %d\n", ctr);
                                found =1;
                                break;

                            }
                            fprintf(stdout, "Failed to connect to: %d\n",portToUSe);
                            ctr = ctr+1;
                        }



                            if(found==1)
                            {
                                break;
                            }
                            retryRange++;

                        }

                            //////////////


                        if(found==0)
                        {
                            fprintf(stdout, "MAX RETRIES ATTEMPTED, Can't Connect\n");
                        }


                        if(retval==0)
                        {
                            //do client stuff
                            fprintf(stdout, "Connected to Server!\n");
                            //Pass connection off to sub client through threads
                                //sleep(1);
                                sendAck(sockfileDesc);

                                int resultnum = readMessage(sockfileDesc, messageToRecv);
                               
                                if(strcmp(messageToRecv, "auth")==0)
                                {
                                    fprintf(stdout, "Authingorizing client...\n");

                                    //send user and pass
                                    //put user in here
                                    strcpy(messageToSend, currTok->inArg[2]);
                                    strcat(messageToSend, ":");
                                    strcat(messageToSend, currTok->inArg[3]);
                                    sendMessage(sockfileDesc, messageToSend);

                                    int resultnum = readMessage(sockfileDesc, messageToRecv);
                                    
                                    //If did not fail
                                    if (strcmp("FAILED", messageToRecv) !=0 )
                                    {   

                                                            tconnect =1;
                                    }
                                    else
                                    {
                                        fprintf(stdout, "Authorization Failed, try again\n");
                                                            tconnect =0;


                                    }

                                }  

                                                 
                        }
                        else
                        {

                            fprintf(stdout, "Failed to connect\n");
                        }

                    }     

                }

        }
        else if (strcmp( "tget", currTok->inArg[0]) ==0)
        {     
                
                if(tconnect)
                {

                    error = 0;

                    bufferSendCount = 0;
                    lastBuf=-1;
                    // do something
                    fprintf(stdout, "tgetting\n");
                    //file request logic
                    

                        strcpy(messageToSend, "tgetStart");
                        sendMessage(sockfileDesc, messageToSend);
                        waitAck(sockfileDesc);
                            strcpy(name,currTok->inArg[1]);
                            strcat(name, "download.txt");
                            fileptr = fopen(name, "wb");

                            if(fileptr==NULL)
                            {
                                fprintf(stdout, "Error file not opened\n\n"); 

                            }
                            else
                            {

                                    //send file name lol
                                    strcpy(messageToSend, currTok->inArg[1]);
                                    sendMessage(sockfileDesc, messageToSend);

                                readMessage(sockfileDesc, messageToRecv);
                                if(strcmp(messageToRecv, "VALID_FILE")==0)
                                {

                                    fprintf(stdout, "got to file open\n");

                                    //file exists on other end
                                    error = readMessage(sockfileDesc, messageToRecv);

                                    char delims[] = ":\n";

                                    char* token = strtok(messageToRecv, delims); 

                                    lastBuf = atoi(token);

                                    token = strtok(NULL, delims); 

                                    bufferSendCount = atoi(token);

                                    fprintf(stdout, "File valid\n");

                                    fprintf(stdout, "Size of it: Buffers->%d, last buffer->%d\n", bufferSendCount, lastBuf);

                                    sendAck(sockfileDesc);

                                                for(int x = 0; x<bufferSendCount; x++)
                                                {
                                                    //read and do a buff write
                                                    error = readMessage(sockfileDesc, messageToRecv);
                                                    
                                                    fwrite(messageToRecv, sizeof(char), STR_MAX_LEN, fileptr);
                                                    sendAck(sockfileDesc);
                                                        //fprintf(stdout, "Line recieved\n");

                                                    if(error==-1)
                                                    {
                                                        fprintf(stdout, "Error in process file contents incorrect\n");
                                                        break;
                                                    }
                                                }

                                                

                                               if(lastBuf!=0)
                                               {

                                                error = readMessage(sockfileDesc, messageToRecv);

                                                }

                                                fwrite(messageToRecv, sizeof(char), lastBuf, fileptr);




                                                fclose(fileptr);
                                                //send done ack

                                                strcpy(messageToSend, "all_done");




                                                sendMessage(sockfileDesc, messageToSend);

                                                error = waitAck(sockfileDesc);

                                                if(error==0)
                                                {
                                                    fprintf(stdout, "Transferred succesfully please input a new command\n");

                                                }
                                                else
                                                {
                                                    fprintf(stdout, "Error occoured during transmission, no acknowledgment recieved.\n");
                                                }

                                }
                                else
                                {
                                    //not valid
                                    sendAck(sockfileDesc);
                                     fprintf(stdout, "Error occoured during transmission, no file to get on server.\n");
                                              

                                }

                            }


                    


                }
                else
                {
                    fprintf(stdout, "connect first\n");
                } 
        }
        else if (strcmp( "tput", currTok->inArg[0]) ==0)
        {

                
                error=-1;
                c = 0;
                
                fileLen = 0;
                buffer[STR_MAX_LEN];
                bufferSendCount = 0;
                cur=0;
                //file transfer logic
                if(tconnect)
                {
                    // do something
                    fprintf(stdout, "tputting\n");
                    fileptr = fopen(currTok->inArg[1], "rb");
                    if(fileptr==NULL)
                    {
                        fprintf(stdout, "Error file not sent\n\n"); 

                    }
                    else
                    {
                        strcpy(messageToSend, "tputStart");
                        sendMessage(sockfileDesc, messageToSend);
                        strcpy(messageToSend, currTok->inArg[1]);
                        sendMessage(sockfileDesc, messageToSend);
                        fileptr = fopen(currTok->inArg[1], "rb");
                        rewind(fileptr);
                        fileLen = 0;   
                        while ((c = getc(fileptr)) != EOF) {fileLen++;}
            
                        bufferSendCount =  fileLen/STR_MAX_LEN;
                        cur = fileLen%STR_MAX_LEN;
                        sprintf(messageToSend, "%d:%d:", cur, bufferSendCount);
                        fprintf(stdout,messageToSend);
                        sendMessage(sockfileDesc, messageToSend);
                        fflush(fileptr);
                        rewind(fileptr);
                        fclose(fileptr); 
                        fileptr = fopen(currTok->inArg[1], "rb");

                        for(int i= 0; i < bufferSendCount; i++)
                        {                           
                           error = fread(messageToSend,sizeof(char),STR_MAX_LEN,fileptr);
                           if(error==0)
                           {    fprintf(stdout, "ERROR\n"); }
                           else
                           {
                                sendMessage(sockfileDesc, messageToSend);
                           }
                           error = waitAck(sockfileDesc);
                           if(error==-1)
                           {
                            fprintf(stdout, "ERROR\n");
                            fprintf(stdout, "timeout error occoured\n");
                            break;
                            
                           }
                        } 

                            if(cur !=0)
                            {
                               error = fread(messageToSend,sizeof(char),cur,fileptr);
                               if(error==0)
                               {
                                fprintf(stdout, "ERROR\n");
                               }
                               else
                               {
                                    sendMessage(sockfileDesc, messageToSend);
                               }
                           }

                           readMessage(sockfileDesc, messageToRecv);
                        //check closing
                        if (strcmp( "all_done", messageToRecv) ==0 )
                        {
                            fprintf(stdout, "Transferred succesfully please input a new command\n");

                        }
                        else
                        {
                            fprintf(stdout, "Error occoured during transmission, no acknowledgment recieved.\n");
                        }
                    }

                }
                else
                {
                    fprintf(stdout, "Err: Must connect to server first\n");
                }
                
        }
        else if (strcmp( "exit", currTok->inArg[0]) ==0)
        {

                // do something

                //file transfer logic
                if(tconnect)
                {
                    tconnect = 0;
                    
                    
                    shutdown(sockfileDesc,SHUT_RDWR);
                    close(sockfileDesc);
                    fprintf(stdout, "Client closing closing connection\n");


                }
                else
                {
                    fprintf(stdout, "Err nothing to close\n");

                }
                
        }
        else
        {
            fprintf(stdout, "Err: INPUT NOT RECOGNIZED, try again\n");
        }

        memset(inbuff, 0, STR_MAX_LEN);

        fprintf(stdout, "Please input a command:\n");

    }


    if(tconnect)
    {

        fprintf(stdout,"Buggy closing! shutdown oocured while connected to server still! Did not hit exit point\n\n");


    }
    else
    {

        fprintf(stdout,"Exiting safely. Everything shut down\n\n");


    }

    fflush(stdout);


    exit(0);

}

