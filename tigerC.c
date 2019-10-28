#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>


#define STR_MAX_LEN 256
#define MAX_ARGS 5
#define SADDR_STRUCT struct sockaddr 
#define TEST_PORT 8080

struct tokenInputs
{
    int argCount;
    char inArg[MAX_ARGS][STR_MAX_LEN];
    

};

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


    struct tokenInputs* tokStruct;


    if(tokStruct)
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



int main(int argc, char *argv[])
{

    int sockfileDesc = -1;
    int connectionFileDesc = -1;
    int mesLen=0;
    struct sockaddr_in Server_info;
    struct sockaddr_in client_info; 
    SADDR_STRUCT sadd_serv;

    char messageToSend[STR_MAX_LEN];
    char messageToRecv[STR_MAX_LEN];



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
                printf("tconnecting\n");
                //Server connect logic
                tconnect =1;

                sockfileDesc = socket(AF_INET, SOCK_STREAM, 0); 

                if(sockfileDesc ==-1)
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

                    }
                    else
                    {
                        printf("Failed to connect\n");
                    }

                }


                
        }
        else if (strcmp( "tget", currTok->inArg[0]) ==0)
        {
              
                // do something
                printf("tgetting\n");

                //file request logic
                if(tconnect)
                {
                    
                }
                
        }
        else if (strcmp( "tput", currTok->inArg[0]) ==0)
        {

                // do something
                printf("tputting\n");

                //file transfer logic
                if(tconnect)
                {

                }
                
        }
        else if (strcmp( "testwrite", currTok->inArg[0]) ==0)
        {

                // do something
                printf("testwrite\n");

                //file transfer logic
                if(tconnect)
                {
//                    write();
                }
                
        }
        else if (strcmp( "testread", currTok->inArg[0]) ==0)
        {

                // do something
                printf("testread\n");

                //file transfer logic
                if(tconnect)
                {
//                    read();
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
                    printf("closing\n");

//                    read();
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

    }

}

