#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include <pthread.h> 
  

#define NUMBER_OF_CLIENTS 5


struct NETWORK_INFO
{
    int value;
  
};


// A normal C function that is executed as a thread  
// when its name is specified in pthread_create() 
void *serverChildBehavior(void *vargp) 
{ 
    
    struct NETWORK_INFO *myid = (struct NETWORK_INFO *)vargp; 



    printf("I'm doing child behavior now for connection number: %d\n", myid->value);   //: %d \n"); 
    sleep(1); 
    
    pthread_exit((void*)(0));

    return NULL; 


} 

  
int main(int argc, char *argv[]) 
{ 
    //int i; 
    pthread_t tid[NUMBER_OF_CLIENTS]; 


    struct NETWORK_INFO args[NUMBER_OF_CLIENTS];
  
    //Create child processes
    for (int i = 0; i <NUMBER_OF_CLIENTS; i++) 
    { 
        printf("making %d\n", i);



        args[i].value = i+1;
        pthread_create(&tid[i], NULL, serverChildBehavior, &args[i]); 
    }

    sleep(10);
    printf("Done\n");
    return 0; 
} 


