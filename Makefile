makeExampleServer: serverCopy.c
	gcc -o server serverCopy.c 

makeExampleClient: 	clientCopy.c
	gcc -o client clientCopy.c

makeTigerS:
	gcc -o tigerS tigerS.c


make makeTigerC:
	gcc -o tigerC tigerC.c

