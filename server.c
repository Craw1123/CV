#include<stdio.h> //popen() fread() pclose() exit()
//#include<sys/ipc.h> //ftok() msgget()+
//#include<sys/types.h> //ftok()+
#include <sys/stat.h> //struct stat stat()
#include <stdlib.h> // system()
#include<sys/msg.h> // msgget() msgsnd()+
#include <string.h> //strlen()+
#include <errno.h> //debug
#define PROJECT_ID_CLIENT 900
#define PROJECT_ID_SERVER 901

struct msqid_ds neznaukaknazvat;
struct stat buffer;
struct 
{
    long type; 
    char text[1024]; 
    } bufferfiles,bufferfilesbiigerthan10;
    
key_t ipckeyclient;
key_t ipckeyserver;
//FILE* openedfile;
//FILE* ls;
int idocherediclient;
int idocherediserver;
char * forsplit;
char filesbiigerthan10[1024];
int main(){
    
    int existserver = stat("/tmp/lab4client",&buffer);
    if (existserver!=0) {
        printf("No client file\nBye!!!\n");
        exit(0);
        }
    int existclient = stat("/tmp/lab4server",&buffer);
    if (existclient!=0) {
        system("touch /tmp/lab4server");
        printf("created /tmp/lab4server\n");
        }
    ipckeyclient = ftok("/tmp/lab4client", PROJECT_ID_CLIENT);
    ipckeyserver = ftok("/tmp/lab4server", PROJECT_ID_SERVER);
    printf("ipckeyclient: %d\n",ipckeyclient);
    printf("ipckeyserver: %d\n",ipckeyserver);
    idocherediclient = msgget(ipckeyclient, 0);
    if(idocherediclient>=0 && msgctl(idocherediclient,IPC_STAT,&neznaukaknazvat)==0) // idocherediclient>=0 - проверяем, msgget получил очередь или нет 
    {
        if(neznaukaknazvat.msg_qnum==0) // это мб лишнее
        {
            printf("Ochered' pustya\nBye!!!\n");
            exit(0);
            }
        printf("Poslednee soobshenie: %d\n",neznaukaknazvat.msg_stime);
        printf("Ochered' ot clienta: %d\n",neznaukaknazvat.msg_qnum);
        msgrcv(idocherediclient,&bufferfiles,1024,1,0);
        //printf(bufferfiles.text);
        forsplit = strtok(bufferfiles.text, "\n");
        while( forsplit != NULL ) {
            //printf( "%d\n", strlen(forsplit)); //printing each token
            if(strlen(forsplit)>10)
            {
                strncat(filesbiigerthan10,forsplit,strlen(forsplit));
                filesbiigerthan10[strlen(filesbiigerthan10)] = '\n';
                }
            forsplit = strtok(NULL, "\n");
            }
            
        //printf(filesbiigerthan10);
        
        if(strlen(filesbiigerthan10)>0)
        {
            idocherediserver = msgget(ipckeyserver,(IPC_CREAT|0666));
            strncpy(bufferfilesbiigerthan10.text,filesbiigerthan10,strlen(filesbiigerthan10));
            bufferfilesbiigerthan10.type = 1;
            //printf(bufferfilesbiigerthan10.text);
            msgsnd(idocherediserver,&bufferfilesbiigerthan10, strlen(bufferfilesbiigerthan10.text)+1,0);
            }

        
        }
    else
    {
        printf("Problema s Ochered'u. Pomolites'\n"); // очередь не существует или с ней возникли какие-то проблемы
        }
    }
