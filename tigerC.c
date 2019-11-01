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

#define INPUT_RANGE 100

char messageToSend[STR_MAX_LEN];
    char messageToRecv[STR_MAX_LEN];

int messageToSendBin[STR_MAX_LEN];
    int messageToRecvBin[STR_MAX_LEN];

struct tokenInputs
{
    int argCount;
    char inArg[MAX_ARGS][STR_MAX_LEN];
};

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
    //char inbuff[STR_MAX_LEN] = "";
    char toks[STR_MAX_LEN] = "";
    int argCount =0; 
    char delims[] = " \n";
    struct tokenInputs* tokStruct = NULL;
    
    //fprintf(stdout, "Start here for confusion");

    if(tokStruct ==NULL)
    {
        tokStruct = malloc(sizeof( struct tokenInputs));

    }    
    //if correct num args
    //if (argc == 4 || argc == 2)  
    //{ 
    strncpy(toks, inbuff, STR_MAX_LEN);
    //fprintf(stdout, "%s", inbuff);
    char* token = strtok(toks, delims); 
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    while (token != NULL) { 
        //fprintf(stdout, "%s\n", token); 
        strncpy(tokStruct->inArg[argCount], token, STR_MAX_LEN);
        argCount++;
        token = strtok(NULL, delims); 
        if(argCount==5)
        {
            break;
        }
        //strncpy([argCount], token, STR_MAX_LEN);
    } 
    memcpy(outToks, tokStruct, sizeof(struct tokenInputs));
    clearToks(tokStruct);
    free(tokStruct);
    return 0; 
}

// int sendMessage(int socket, char* buffer)
// {
//     write(socket, buffer, STR_MAX_LEN); 
//     bzero(buffer, STR_MAX_LEN);
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
        //fprintf(stdout, "%d", result);

        if(strcmp(buffer, start)!=0)
        {
            //fprintf(stdout, "read success: %s\n", buffer);
            break;
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
            //fprintf(stdout, "%s\n", buffer);
            break;
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
    int sockSeperateFileDesc = -1;
   
    int mesLen=0;
    struct sockaddr_in Server_info;
    struct sockaddr_in client_info; 
    SADDR_STRUCT sadd_serv;
    


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
                    sockSeperateFileDesc = socket(AF_INET, SOCK_STREAM, 0); 

                    if(sockfileDesc ==-1 || sockSeperateFileDesc ==-1)
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

                        sleep(1);

                        int MAX_RETRY = 15;

                        int retryRange = 0;
                        int found =0;

                        int MAXdel = 2500000;

                        int MINdel = 500000;

                        int randAdd;

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
                            

                            //Print retval for socket

                            if(retval==0)
                            {
                                printf("Socket assigned to %d", ctr);
                                found =1;
                                break;

                            }
                            printf("Failed to connect to: %d\n",portToUSe);
                            ctr = ctr+1;

                        }

                        if(found==1)
                        {
                            break;
                        }
                        retryRange++;

                        }

                        if(found==0)
                        {
                            fprintf(stdout, "MAX RETRIES ATTEMPTED, Can't Connect\n");
                        }


                        if(retval==0)
                        {
                            //do client stuff
                            fprintf(stdout, "Ready to do server lcient stuff\n");
                            //Pass connection off to sub client through threads
                                //sleep(1);
                                int resultnum = readMessage(sockfileDesc, messageToRecv);
                                //fprintf(stdout, "%d", resultnum);
                                //fprintf(stdout, "%s\n", messageToRecv);
                                //respond to message
                                if(strcmp(messageToRecv, "auth")==0)
                                {
                                    fprintf(stdout, "authing\n");
                                    fprintf(stdout, "\n");

                                    //send user and pass
                                    //put user in here
                                    strcpy(messageToSend, currTok->inArg[2]);
                                    strcat(messageToSend, ":");
                                    strcat(messageToSend, currTok->inArg[3]);
                                    sendMessage(sockfileDesc, messageToSend);

                                    int resultnum = readMessage(sockfileDesc, messageToRecv);
                                    
                                    if (strcmp( "FAILED\n", messageToRecv) !=0 )
                                    {   

                                        fprintf(stdout, "%s\n", messageToRecv);

                                        int newPort = getPortFromAuthMsg(messageToRecv);
                                        fprintf(stdout, "--> %d", newPort);

                                        Server_info.sin_port= htons(newPort);
                                        
                                        sleep(2);
                                        //make new connection
                                        retval = connect(sockSeperateFileDesc, (SADDR_STRUCT*)&Server_info, sizeof(Server_info));
                                        //fprintf(stdout, "\n%d\n", retval);

                                        strcpy(messageToSend,"I'm ALive!!!\n");
                                        sendMessage(sockSeperateFileDesc, messageToSend);

                                        // int first = 0;
                                        // while(1)
                                        // {

                                            strcpy(messageToSend,"Still awake?!!!\n");
                                            sendMessage(sockSeperateFileDesc, messageToSend);

                                        shutdown(sockfileDesc,SHUT_RDWR);
                                        close(sockfileDesc);
                                    }
                                    else
                                    {
                                        fprintf(stdout, "Auth Failed, try again\n");

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
                    // do something
                fprintf(stdout, "tgetting\n");
                //file request logic
                }
                else
                {
                    fprintf(stdout, "connect first\n");
                }
                
        }
        else if (strcmp( "tput", currTok->inArg[0]) ==0)
        {

               FILE* fileptr;

               int c = 0;
               int cur=0;

               int buff[STR_MAX_LEN];

                char buffer[STR_MAX_LEN];
                int bufferSendCount = 0;
                //file transfer logic
                if(tconnect)
                {
                    // do something
                    fprintf(stdout, "tputting\n");
                    fileptr = fopen(currTok->inArg[1], "rb");
                    if(fileptr==NULL)
                    {
                        printf("Error file not sent\n\n"); 

                    }
                    else
                    {
                        strcpy(messageToSend, "tputStart");
                        sendMessage(sockSeperateFileDesc, messageToSend);
                        
                        strcpy(messageToSend, currTok->inArg[1]);
                        sendMessage(sockSeperateFileDesc, messageToSend);

                        while((c=fgetc(fileptr))!=EOF)
                        {
                                buff[cur] = c;
                                cur = cur +1;
                                if(cur == STR_MAX_LEN)
                                {

                                    bufferSendCount++;
                                    cur=0;
                                    //printf("Limit reached, sending\n");
                                }
                        }

                        
                        
                        
                        //send last line size
                        sprintf(messageToSend, "%d:%d:", cur, bufferSendCount);
                        

                        fprintf(stdout,messageToSend);

                        sendMessage(sockSeperateFileDesc, messageToSend);

                        fflush(fileptr);
                        rewind(fileptr);
                        fclose(fileptr); 
                        fflush(fileptr);
                        fileptr = fopen(currTok->inArg[1], "rb");
                        rewind(fileptr);

                        while((c=fgetc(fileptr))!=EOF)
                        {
                                buff[cur] = c;
                                cur = cur +1;
                                if(cur == STR_MAX_LEN)
                                {
                                    for(int j=0; j<STR_MAX_LEN; j++)
                                    {
                                         messageToSendBin[j] = buff[j];
                                        //send message
                                    }
                                    printf("sending\n\n");
                                       sendMessageBin(sockSeperateFileDesc, messageToSendBin);
                                    cur=0;
                                    //printf("Limit reached, sending\n");
                                }
                        }

                        for(int j=0; j<cur; j++)
                        {
                             messageToSendBin[j] = buff[j];
                        }

                        //send last line
                        sendMessageBin(sockSeperateFileDesc, messageToSendBin);
                        
                        printf("here client\n\n");

                        //send close directive
                        //strcpy(messageToSend, "close_file");
                        //sendMessage(sockSeperateFileDesc, messageToSend);
                    
                        //acknowledge closing
                        printf("Here2\n");
                        readMessage(sockSeperateFileDesc, messageToRecv);


                        printf("Here3\n");

                        //check closing
                        if (strcmp( "all_done", messageToRecv) ==0 )
                        {
                            printf("Transferred succesfully please input a new command\n");

                        }
                        else
                        {

                            printf("Error occoured\n");

                        }


                    }

                    

                    //while(file)


                    //send close signature


                }
                else
                {
                    fprintf(stdout, "connect first\n");
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
                    fprintf(stdout, "closing\n");


                }
                else
                {
                    fprintf(stdout, "Nothing to close\n");

                }
                
        }
        else
        {
            fprintf(stdout, "INPUT NOT RECOGNIZED\n");
        }

        memset(inbuff, 0, STR_MAX_LEN);
    }

}

