#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

int main()
{
    char boof[256] = "Authorized:5000";
    int newPort = getPortFromAuthMsg(boof);
    printf("%d", newPort);

}