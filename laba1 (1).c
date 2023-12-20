#include<stdio.h>//printf,scanf
#include<signal.h>//sigprogmask,sigaction
#include<fcntl.h>//flags
#include<unistd.h>//write,read,execl,pipe,fork,dup2,close
#include<string.h>//strlen,memset
#include<stdlib.h>//system
#include <sys/wait.h>

int ll = 0; //inter counter
char file[256];//file
int fd[2]; // дескрипторы межпроцессного канала 
int flag; //1 if name typed
char buff[600];

void inter()//Подпрограмма обработки прерывания
{
	char comand1[] = { "du -a" };
	char comand2[] = { "du -s" };
	if (ll > 0)
	{
		printf("\n");
		system(comand2);//больше 1 прерывания
	}
	else
	{
		printf("\n");
		pid_t pid;
		int pipefd[2], status;

		pipe(pipefd);

		pid = fork();
		if (!pid) {
			dup2(pipefd[1], 1);
			execl("/usr/bin/du", "du", "-a", NULL);
		} else if (pid != -1) {
			wait(&status);
		}

		close(pipefd[1]);

		pid = fork();
		if (!pid) {
			dup2(pipefd[0], 0);
			execl("/usr/bin/head", "head", "-n", "-1", NULL);
		} else if (pid != -1) {
			wait(&status);
		}
	
	}
	if (flag==0)
	{
		printf("File name: \n");
	}
	ll++;
} 

void main()
{
	int ex=0;
	struct sigaction act;

	memset(&act, 0, sizeof(act));//установка обработчика прерываний
	act.sa_handler = inter;
	act.sa_flags = 0;
	sigprocmask(0, NULL, &act.sa_mask);

	if (sigaction(SIGINT, &act, NULL) < 0)
	{ //проверка установки обработчика прерываний
		printf("FALURE");
		return;
	}
	pipe(fd);
	if (fork() == 0)
	{ 	//СЫН
		dup2(fd[1],1);//дублирует дескриптор межпроцессного канала на стандартный вывод
		close(fd[0]);//закрытие межпроцессного канала на чтение у сына
		close(fd[1]);//удаляем копию
		system("pwd");
		execl("/bin/ls", "ls", "-a", NULL);//отправляем результаты system и execl в межпроцессный канал
		
	}
	else
	{	//ОТЕЦ
		int res;
		printf("File name: \n"); 
		do
		{
			scanf("%s", file);//ждем ввода имени файла
		}
		while(file[0]==0);
		flag = 1;//имя введено
		int a = creat(file, 0777);//создаем файл с правами доступа 0777
		res = read(fd[0], buff , 600);//считываем из межпроцессного канала в буфер
		write(a,buff,res);//записываем в файл
		printf("Enter 1 to exit: \n"); //ожидние завершения прграммы (сделано чтобы программа не завершалась сразу и я мог протестировать прерывания после ввода имени)
		do
		{
			scanf("%d", &ex);
		}
		while(ex != 1);
	}
}
