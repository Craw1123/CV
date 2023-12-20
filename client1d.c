#include <sys/msg.h>
#include <stdio.h>
#include <string.h> //for strlen
#include <time.h>

int main()
{
  unsigned int id = 101;
  //printf("Client 1 started.\n");
  
  //create message query
  int mesq_id = msgget(id,0);
  if (mesq_id == -1) {
    fprintf(stderr, "Error when opening message query with id=%u\n",id);
    return 1;
  }
  printf("Connected to message query. ID is %d\n", mesq_id);

  struct msqid_ds ds;
  msgctl(mesq_id, IPC_STAT, &ds);
  printf("Messages in query: %d\n", ds.msg_qnum);

  struct { long type; char text[ds.msg_qbytes]; } message;

  //int msgrcv(int fd, const void *obbuf, int len, int mtype, int flag);

  message.type = 2;
  int status = msgrcv(mesq_id, &message, 1024, message.type, IPC_NOWAIT);
  if (status > 0){
    printf("Message: %s", message.text);
  } else {
    fprintf(stderr,"No Message with this type: %d\n", message.type);
  }
  char text[256];

  //2 - send, 1 - first client
  message.type = 21;
  while (strcmp(text,"exit")){
    memset(text, 0, 256);   // null filename
    printf("\nEnter message text: ");
    scanf("%s", text);
    strcpy(&message.text, text);
    msgsnd(mesq_id,&message, strlen(message.text)+1,IPC_NOWAIT);
  }
}
