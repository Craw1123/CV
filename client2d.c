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

  message.type = 1;
  int status = msgrcv(mesq_id, &message, 1024, message.type, IPC_NOWAIT);
  
  //reconfigure to send, 2 - send, 1 - first client
  message.type = 22;
  if (status > 0){
    char tmp[1024]; 
    strcpy(tmp, &message.text);
    printf("Message: %s", tmp);
    
    //send message with day of week in query
	  size_t init_size = strlen(tmp);
	  size_t i = 0;
    char day[1024];
    while((tmp[i]!=' ') && i<init_size){
      strncat(day,&tmp[i],1);
      ++i;
    }
    strcpy(&message.text, &day);
    msgsnd(mesq_id,&message, strlen(message.text)+1,IPC_NOWAIT);
  } else {
    fprintf(stderr,"No Message with this type: %d\n", message.type);
  }

  //interactive part
  char text[256];

  while (strcmp(text,"exit")){
    memset(text, 0, 256);   // null filename
    printf("\nEnter message text: ");
    scanf("%s", text);
    strcpy(&message.text, text);
    msgsnd(mesq_id,&message, strlen(message.text)+1,IPC_NOWAIT);
  }
}
