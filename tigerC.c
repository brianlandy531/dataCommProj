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

	char inbuff[STR_MAX_LEN] = "";

    printf("Program name %s\n", argv[0]);

    char delims[] = " \n";

    struct  tokenInputs* currTok;

    currTok = malloc(sizeof( struct tokenInputs));
    
    tconnect = 0;

    while(fgets(inbuff, STR_MAX_LEN , stdin) != NULL)
    {
        getTokens(inbuff, currTok);
     
        if (strcmp( "tconnect", currTok->inArg[0]) ==0 )
        {

                // do something
                printf("tconnecting\n");
                //Server connect logic
                tconnect =1;
                
                
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
        else
        {
            printf("INPUT NOT RECOGNIZED\n");
        }

    }

}

