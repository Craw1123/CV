#include <sys/msg.h>
#include <stdio.h>
#include <string.h> //for strlen
#include <time.h>
#include <unistd.h> //for gethostname and sleep

int main()
{
  unsigned int id = 101;
 
  struct 
  { 
      long type; 
      char text[1024]; 
  } message;

  printf("Welcome to Server! Lab1. Var 8.\n");
  //create message query
  int mesq_id = msgget(id, IPC_CREAT | 0666);
  if (mesq_id == -1) {
    fprintf(stderr, "Error when opening message query with id=%u\n",id);
    return 1;
  }

  //retrive date and time
  time_t rawtime = time(NULL);
 
  strcpy(&message.text, ctime(&rawtime));
  message.type = 1;
  msgsnd(mesq_id,&message, strlen(message.text)+1,IPC_NOWAIT);

  //retrive active users
  char *shell_active_users = "who | awk -F' ' '{print $1}' | sort -u | tr '\n' ' '";
  
  FILE *fp;
  if ((fp = popen(shell_active_users, "r")) == NULL) {
    fprintf(stderr, "Error opening pipe\n");
    return 1;
  }
  char active_users_str[1024];
  fgets(active_users_str, 1024, fp);

  if(pclose(fp))  {
    fprintf(stderr, "Error retriving users\n");
    return 1;
  }

  strcpy(&message.text, active_users_str);
  message.type = 2;
  
  printf("Sending active users...\n");
  msgsnd(mesq_id,&message, strlen(message.text)+1,IPC_NOWAIT);
  
  //get hostname
  gethostname(active_users_str,1024);
  strcpy(&message.text, active_users_str);
  message.type = 3;
  printf("Sending hostname...\n");
  msgsnd(mesq_id,&message, strlen(message.text)+1,IPC_NOWAIT);
  
  char text[256];

  printf("\nWaiting client messages. Send 'exit' to end programm\n");
  
  while (strcmp(text,"exit") && strcmp(message.text, "exit")){
    sleep(1);
    if(msgrcv(mesq_id, &message, 1024, 21, IPC_NOWAIT) > 0) {
      printf("Client 1: %s\n",message.text);
      strcpy(&text, message.text);
    }
    
    if(msgrcv(mesq_id, &message, 1024, 22, IPC_NOWAIT) > 0) {
      printf("Client 2: %s\n",message.text);
    }
  }

  //delete message query
  if (msgctl(mesq_id, IPC_RMID,0) == -1) 
    fprintf(stderr, "Error when deleting message query with id=%u\n",id);
}
