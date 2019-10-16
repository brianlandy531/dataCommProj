#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>

#define STR_MAX_LEN 256
#define SADDR_STRUCT struct sockaddr 
#define TEST_PORT 8080


int printTokens( char *inbuff)
{

    //char inbuff[STR_MAX_LEN] = "";
    char toks[STR_MAX_LEN] = "";
    int counter =0; 
    char delims[] = " \n";

    //if correct num args
    //if (argc == 4 || argc == 2)  
    //{ 
    
    strncpy(toks, inbuff, STR_MAX_LEN);
   
    //printf("%s", inbuff);
    char* token = strtok(toks, delims); 
  
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    while (token != NULL) { 
        printf("%s\n", token); 
        token = strtok(NULL, delims); 
    } 

        //Chose operating mode (connect, get put)

    //} 
    //else
    //{
    //  printf("enter 4 arguments for connect or 2 arguments for get or put\n"); 
    //    return 0; 
    //}

  
    return 0; 

}


int main(int argc, char *argv[])
{

	char inbuff[STR_MAX_LEN] = "";
	char boof[STR_MAX_LEN] = "yessir a b c";

    char currIn[STR_MAX_LEN] = "";

    printf("Program name %s\n", argv[0]);

    char delims[] = " \n";
    
    while(fgets(inbuff, STR_MAX_LEN , stdin) != NULL)
    {
        //printf("%s\n", inbuff);
    
        printTokens(inbuff);



    /*if (argc == 4 || argc == 2)  
    { 

        for(int j =0; j<argc; j++)
        {
            printf("[%d]: ", j);

            printf("%s\n", argv[j]);
        }
    }*/

    }



}
