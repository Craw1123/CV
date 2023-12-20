#include<stdio.h> //popen() fread() pclose()
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
struct stat buffer1,buffer2;
struct 
{
    long type; 
    char text[1024]; 
    } bufferfiles,bufferfilesbiigerthan10;

key_t ipckeyclient;
key_t ipckeyserver;
FILE* openedfile;
FILE* ls;
int idocherediclient;
int idocherediserver;
int main(){
    
    int existserver = stat("/tmp/lab4server",&buffer1);
    if (existserver!=0) {
        printf("No server file\n");
        }
    int existclient = stat("/tmp/lab4client",&buffer2);
    if (existclient!=0) {
        system("touch /tmp/lab4client");
        printf("created /tmp/lab4client\n");
        }
        
    
    ipckeyclient = ftok("/tmp/lab4client", PROJECT_ID_CLIENT);
    ipckeyserver = ftok("/tmp/lab4server", PROJECT_ID_SERVER);
    printf("ipckeyclient: %d\n",ipckeyclient);
    printf("ipckeyserver: %d\n",ipckeyserver);
    idocherediclient = msgget(ipckeyclient,(IPC_CREAT|0666));
    

    ls = popen("ls","r");
    fread(bufferfiles.text,1,1024,ls);
    pclose(ls);
	bufferfiles.type = 1;
	//printf(bufferfiles.text);
	msgsnd(idocherediclient,&bufferfiles, strlen(bufferfiles.text)+1,0);


    idocherediserver = msgget(ipckeyserver, 0);
    if(idocherediserver>=0 && msgctl(idocherediserver,IPC_STAT,&neznaukaknazvat)==0) // idocherediclient>=0 - проверяем, msgget получил очередь или нет 
    {
        //msgctl(idocherediserver,IPC_RMID,0); // очистить очередь сервера
        //msgctl(idocherediclient,IPC_RMID,0); // очистить очередь клиента
        if(neznaukaknazvat.msg_qnum==0) // это мб лишнее
        {
            printf("Otveta ot servera net\n");
            exit(0);
            }
        printf("Ochered' ot servera: %d\n",neznaukaknazvat.msg_qnum);
        //printf("here");
        msgrcv(idocherediserver,&bufferfilesbiigerthan10,1024,1,0);
        printf(bufferfilesbiigerthan10.text);
    
        }
    else
    {
        printf("Problema s Ochered'u. Pomolites'\n"); // очередь не существует или с ней возникли какие-то проблемы
        }

    }
