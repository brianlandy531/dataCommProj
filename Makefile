makeExampleServer: serverCopy.c
	gcc -o server serverCopy.c 

makeExampleClient: 	clientCopy.c
	gcc -o client clientCopy.c

exampleCLS:
	gcc -o client myclient.c
	gcc -o server myserver.c

tigerS: tigerS.c
	gcc -g -o tigerS tigerS.c

tigerC: tigerC.c
	gcc -g -o tigerC tigerC.c

threads:
	gcc -pthread -o bin pthreadExample.c


