#include <stdio.h>
#include <unistd.h>
#include <lib.h>
#include <stdlib.h>

int main( int argc, char ** argv )
{
    int i;
    int value;
    message m;
    int retPath, retPid;
    int children = atoi(argv[1]);
    int pid = atoi(argv[2]);
    for(i = 0; i < children; ++i)
    {
        if(fork() > 0)
        {
          sleep( 5 );
          return 0;
        }
    }
    sleep ( 1 );
    m.m1_i1 = pid;
    retPath = _syscall( MM, FINDMAXPATH, & m);
    retPid = _syscall( MM, WHOMAXPATH, & m);
    printf("syscall return: Max path: %d, Pid: %d\n", 
    retPath, retPid);
    return 0;
}
