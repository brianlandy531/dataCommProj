makeExampleServer: serverCopy.c
	gcc -o server serverCopy.c 

makeExampleClient: 	clientCopy.c
	gcc -o client clientCopy.c

exampleCLS:
	gcc -o client myclient.c
	gcc -o server myserver.c

TigerS: tigerS.c
	gcc -g -o TigerS tigerS.c

TigerC: tigerC.c
	gcc -g -o TigerC tigerC.c

threads:
	gcc -pthread -o bin pthreadExample.c


