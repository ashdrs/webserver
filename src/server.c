
#include "main.h"

void ForkServer(SOCKET con_sock){
	SOCKET data_sock;

	while(1){
		InfoLog("accept loop");
		data_sock = Accept(con_sock);
		
		//コネクションの度（acceptの度）forkして子プロセスに処理させる
		int pid = fork();
		if(pid == -1){
			close(con_sock);
			close(data_sock);
			ErrorLog("fork");
			exit(EXIT_FAILURE);
		}else if(pid == 0){
			close(con_sock); //子はcon_sock必要ない
			Receive(data_sock);
			InfoLog("connection closed. data_sock=%d",data_sock);
			close(data_sock);
			exit(EXIT_SUCCESS);
		}
		close(data_sock); //親はdata_sock必要ない
	}
}
void PreforkServer(SOCKET con_sock){
	SOCKET data_sock;

	int i;
	for(i=0; i<config->prefork_num; i++){
		int pid = fork();
		if(pid == -1){
			close(con_sock);
			ErrorLog("fork");
			exit(EXIT_FAILURE);
		}else if(pid == 0){
			while(1){
				InfoLog("accept loop");
				data_sock = Accept(con_sock);
				Receive(data_sock);
				InfoLog("connection closed. data_sock=%d",data_sock);
				close(data_sock);
			}
		}
	}
	while(1){
		pause();
	}
}

void LoadConfig(){
	FILE *fp;
	char config_file[] = "./config/server.conf";
	char line[256];

	config = (Config *)malloc(sizeof(Config));
	config->listen_port	= DEFAULT_PORT;
	config->prefork_num	= DEFAULT_SERVERS;
	strcpy(config->doc_root,DEFAULT_DOCROOT);
	strcpy(config->server_type,DEFAULT_SERVTYPE);

	fp = fopen(config_file,"r");
	if(fp == NULL){
		printf("config file open error\n");
		exit(EXIT_FAILURE);
	}else{
		while(fgets(line, 256, fp) != NULL){
			line[strlen(line)-1] = '\0';
			char *name = strtok(line,"=");
			char *value = strtok(NULL,"=");

			//コメントアウト行を無視
			char first_char[2];
			sprintf(first_char,"%c",*name);
			if(strcmp(first_char,"#") == 0){
				continue;
			}

			if(strcmp(name,"Listen") == 0){
				int listen_port = atoi(value);
				if(listen_port > 0){
					config->listen_port = listen_port;
				}
			}else if(strcmp(name,"Prefork") == 0){
				int prefork_num = atoi(value);
				if(0 < prefork_num && prefork_num < 13){
					config->prefork_num = prefork_num;
				}else{
					printf("Prefork between 1 and 12\n");
					exit(EXIT_FAILURE);
				}
			}else if(strcmp(name,"DocumentRoot") == 0){
				if(strlen(value) < 128){
					strcpy(config->doc_root,value);
				}else{
					printf("DocumentRoot max string 128\n");
					exit(EXIT_FAILURE);
				}
			}else if(strcmp(name, "ServerType") == 0){
				if(strcmp(value,"prefork") == 0 || strcmp(value,"fork") == 0){
					strcpy(config->server_type,value);
				}else{
					printf("ServerType is not supported\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	fclose(fp);
}