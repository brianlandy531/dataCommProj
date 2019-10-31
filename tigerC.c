#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>


#define STR_MAX_LEN 200
#define MAX 200
#define MAX_ARGS 5
#define SADDR_STRUCT struct sockaddr 
#define TEST_PORT 8000
#define TIMEOUT_TICKS 1000000000

char messageToSend[STR_MAX_LEN];
    char messageToRecv[STR_MAX_LEN];

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
    
    //printf("Start here for confusion");

    if(tokStruct ==NULL)
    {
        tokStruct = malloc(sizeof( struct tokenInputs));

    }    
    //if correct num args
    //if (argc == 4 || argc == 2)  
    //{ 
    strncpy(toks, inbuff, STR_MAX_LEN);
    //printf("%s", inbuff);
    char* token = strtok(toks, delims); 
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    while (token != NULL) { 
        //printf("%s\n", token); 
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

int sendMessage(int socket, char* buffer)
{
    int res = write(socket, buffer, STR_MAX_LEN); 
    bzero(buffer, STR_MAX_LEN);
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
    printf("Program name %s\n", argv[0]);
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
                    printf("Already connected\n");
                }
                {
                    printf("tconnecting\n");
                    //Server connect logic
                    
                    tconnect =1;

                    sockfileDesc = socket(AF_INET, SOCK_STREAM, 0); 
                    sockSeperateFileDesc = socket(AF_INET, SOCK_STREAM, 0); 

                    if(sockfileDesc ==-1 || sockSeperateFileDesc ==-1)
                    {
                        printf("Try again, socket not created\n");

                    }
                    else
                    {
                        memset(&Server_info, 0, sizeof(Server_info));
                        Server_info.sin_family = AF_INET;
                        //Provide string with saddr
                        Server_info.sin_addr.s_addr=inet_addr(currTok->inArg[1]);
                        Server_info.sin_port= htons(TEST_PORT);
                        //sadd_serv = (SADDR_STRUCT) Server_info;
                        int retval = connect(sockfileDesc, (SADDR_STRUCT*)&Server_info, sizeof(Server_info));
                        if(retval==0)
                        {
                            //do client stuff
                            printf("Ready to do server lcient stuff\n");
                            //Pass connection off to sub client through threads
                                //sleep(1);
                                int resultnum = readMessage(sockfileDesc, messageToRecv);
                                //printf("%d", resultnum);
                                printf("%s\n", messageToRecv);
                                //respond to message
                                if(strcmp(messageToRecv, "auth")==0)
                                {
                                    printf("authing\n");
                                    printf("\n");

                                    //send user and pass
                                    //put user in here
                                    strcpy(messageToSend, currTok->inArg[2]);
                                    strcat(messageToSend, ":");
                                    strcat(messageToSend, currTok->inArg[3]);
                                    sendMessage(sockfileDesc, messageToSend);

                                    int resultnum = readMessage(sockfileDesc, messageToRecv);
                                    printf("%s\n", messageToRecv);

                                    int newPort = getPortFromAuthMsg(messageToRecv);
                                    printf("--> %d", newPort);

                                    Server_info.sin_port= htons(newPort);
                                    
                                    sleep(2);
                                    //make new connection
                                    retval = connect(sockSeperateFileDesc, (SADDR_STRUCT*)&Server_info, sizeof(Server_info));
                                    printf("\n%d\n", retval);

                                    strcpy(messageToSend,"I'm ALive!!!\n");
                                    sendMessage(sockSeperateFileDesc, messageToSend);

                                    close(sockfileDesc);
                                    shutdown(sockfileDesc,SHUT_RDWR);

                                }  

                                                 
                        }
                        else
                        {
                            printf("Failed to connect\n");
                        }

                    }     

                }

        }
        else if (strcmp( "tget", currTok->inArg[0]) ==0)
        {     
                
                if(tconnect)
                {
                    // do something
                printf("tgetting\n");
                //file request logic
                }
                else
                {
                    printf("connect first\n");
                }
                
        }
        else if (strcmp( "tput", currTok->inArg[0]) ==0)
        {

               
                //file transfer logic
                if(tconnect)
                {
                    // do something
                    printf("tputting\n");

                }
                else
                {
                    printf("connect first\n");
                }
                
        }
        else if (strcmp( "exit", currTok->inArg[0]) ==0)
        {

                // do something

                //file transfer logic
                if(tconnect)
                {
                    tconnect = 0;
                    close(sockfileDesc);
                    shutdown(sockfileDesc,SHUT_RDWR);
                    printf("closing\n");


                }
                else
                {
                    printf("Nothing to close\n");

                }
                
        }
        else
        {
            printf("INPUT NOT RECOGNIZED\n");
        }

        memset(inbuff, 0, STR_MAX_LEN);
    }

}

