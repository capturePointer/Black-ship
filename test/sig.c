#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
void handler(int sig)
{
  pid_t pid;

  pid = wait(NULL);

  if(pid <0) {printf("error");}

  printf("Pid %d exited.\n", pid);
}

int main(void)
{
  signal(SIGCHLD, handler);

  int rfork = fork();
  if(rfork <0){printf("Error fork");}
  else if(rfork == 0)
  {
    printf("Child pid is %d\n", getpid());
    exit(0);
  }
  else if(rfork >0)
    printf("Parent pid is %d\n", getpid());

}