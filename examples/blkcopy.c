#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int main()
{
	int c;

	int buff[MAX];

	char s[25];

	FILE* input=NULL;
	FILE* output=NULL;;

	printf("FuckingWorkBitch\n");

	printf("In\n");
	gets(s);

	input = fopen(s, "rb");

	if(input==NULL)
	{
		exit(0);
	}


	printf("Out\n");
	gets(s);

	output = fopen(s, "wb");

	if(output==NULL)
	{
		exit(0);
	}

	int cur =0 ;



	while((c=fgetc(input))!=EOF)
	{

		//fputc(c,buff[cur]);
		
		buff[cur] = c;
		cur = cur +1;

		if(cur == MAX)
		{
			for(int j=0; j<MAX; j++)
			{
				fputc(buff[j],output);
				//send message
			}
			cur=0;

		}
	}

	for(int j=0; j<cur; j++)
			{
				fputc(buff[j],output);
				//send message
			}

	close(output);
	close(input);

}