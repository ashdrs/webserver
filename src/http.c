
#include "main.h"

void Receive(SOCKET data_sock){
	char recv_buf[2048];		//受信バッファ
	int size;
	char head_buf[2048]="";
	while(1){
		size=recv(data_sock, recv_buf, 1024, 0);
		if(size <= 0){
			break;
		}else{
			strcat(head_buf,recv_buf);
			int head_size = sizeof(head_buf);
			int i=0;
			int j=0;
			int last_point=0;
			char one_header[1024]="";
			for(i=0; i<head_size; i++){
				one_header[j]=head_buf[i];
				j++;
				if(head_buf[i]=='\n'){
					if(one_header[0]=='\r' && one_header[1]=='\n'){
						return; // \r\nのみなので終了
					}
				//	InfoLog("one_header: %s",one_header);
					Analyze(one_header, data_sock);
					memset(one_header,'\0',1024);
					j=0;
					last_point=i;
				}
			}
			char tmp_head[2048]="";
			j=0;
			for(i=last_point; i<size; i++){
				tmp_head[j] = head_buf[i];
				j++;
			}
			memset(head_buf,'\0',head_size);
			memcpy(head_buf,tmp_head,sizeof(tmp_head));
		}
	}
	return;
}

int Response(SOCKET data_sock, char *msg){
	int len;
	len = strlen(msg);
	if(send(data_sock, msg, len, 0) < 0){
		return -1;
	}
	return len;
}

void Analyze(char *recv_buf, SOCKET data_sock){
	int word_cnt;		//受信したバッファのワード数
	//HTTPヘッダすべてくるのでバッファ多めにとっておく
	char method[512]="";
	char uri[1024]="";
	char proto[1024]="";

	word_cnt = sscanf(recv_buf, "%s %s %s", method, uri, proto);
	if(word_cnt <= 0){
		return;
	}

	if(strcmp(method, "GET") != 0){
	//	Response(data_sock, "HTTP/1.1 501 Not Implemented\r\n");
	//	Response(data_sock, "Server: WebServer\r\n");
	//	Response(data_sock, "\r\n");
	}else{
		char openfile[256] = "";
		char file_buf[1024] = "";
		FILE *fp;
		int read_size;

		if (strcmp(uri,"/") == 0){
			strcat(uri, "index.html");
		}
		strcat(openfile, config->doc_root);
		strcat(openfile, uri);
		InfoLog("file: %s",openfile);
		fp = fopen(openfile,"r");
		if(fp == NULL){
			Response(data_sock, "HTTP/1.1 404 Not Found\r\n");
			Response(data_sock, "Server: WebServer\r\n");
			Response(data_sock, "\r\n");
		}else{
			Response(data_sock, "HTTP/1.1 200 OK\r\n");
			Response(data_sock, "Server: WebServer\r\n");
			Response(data_sock, "\r\n");
			while((read_size = fread(file_buf, 1, 1024, fp)) > 0){
				if(send(data_sock, file_buf, read_size, 0) < 0){
					break;
				}
			}
			fclose(fp);
		}
	}
	return;
}
