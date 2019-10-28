makeExampleServer: serverCopy.c
	gcc -o server serverCopy.c 

makeExampleClient: 	clientCopy.c
	gcc -o client clientCopy.c

makeMyCLS:
	gcc -o client myclient.c
	gcc -o server myserver.c

makeTigerS:
	gcc -o tigerS tigerS.c

makeTigerC:
	gcc -o tigerC tigerC.c

threads:
	gcc -pthread -o bin pthreadExample.c


