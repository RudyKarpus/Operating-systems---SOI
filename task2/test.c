#include <lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int main(int argc, char ** argv)
{
	message m;
	int groupTwoSize, groupThreeSize, i;
 
	groupTwoSize = atoi(argv[1]);
	groupThreeSize = atoi(argv[2]);
 
	for(i = 0; i < groupTwoSize; ++i)
	{
		if (fork() == 0)
		{
			m.m1_i1 = getpid();
			m.m1_i2 = 1;
 
			if(_syscall(MM, SETGROUP, &m) == -1)
			{
				printf("Error\n");
				return 1;
			}
			while(1);
		}
	}
 
	for (i = 0; i < groupThreeSize; ++i)
	{
		if(fork() == 0)
		{
			m.m1_i1 = getpid();
			m.m1_i2 = 2;
 
			if(_syscall(MM, SETGROUP, &m) == -1)
			{
				printf("Error\n");
				return 1;
			}
			while(1);
		}
	}
}
