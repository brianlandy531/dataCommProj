#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <time.h> 


#define MAX 256

int main()
{

	char buffStart[MAX] = "1234567890123456789012345678901234567890123456789012345678901234567890\n\0";
	char readin[10]="";

    for (;;) {
        const int size = 10;
        char str[size];

        printf("> ");

        fgets(readin, size, stdin);

        
        printf("Result: %s\n", readin);
    }
}