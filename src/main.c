
#include "main.h"

void StartServer(){
	SOCKET con_sock; 
	FILE *fp;

	fp = fopen(PID_FILE,"r");
	if(fp != NULL){
		printf("alerady running\n");
		exit(EXIT_FAILURE);
	}

	LoadConfig();

	Daemonize();
	con_sock = ListenTcp(config->listen_port);
	InfoLog("start server");
	if(strcmp(config->server_type,"prefork")==0){
		PreforkServer(con_sock);
	}else if(strcmp(config->server_type,"fork")==0){
		ForkServer(con_sock);
	}
}

void StopServer(){
	FILE *fp;
	int pid;

	fp = fopen(PID_FILE,"r");
	if(fp == NULL){
		printf("not run process\n");
		exit(EXIT_FAILURE);
	}else{
		fscanf(fp,"%d",&pid);
		InfoLog("stop server");
		//killだと接続中の子プロセスが残るためkillpgで一括に殺す
	//	kill(pid,SIGINT);
		killpg(pid,SIGINT);
		unlink(PID_FILE);
		free(config);
	}
}

int main(int argc, char *argv[]){
	char *mode;

	chdir(SERVER_ROOT);

	if(argc == 2){
		mode = argv[1];
		if(strcmp(mode,"start")==0){
			StartServer();
		}else if(strcmp(mode,"stop")==0){
			StopServer();
		}else if(strcmp(mode,"restart")==0){
			StopServer();
			StartServer();
		}else{
			printf("usage: start stop restart\n");
			exit(EXIT_FAILURE);
		}
	}else{
		printf("usage: start stop restart\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
