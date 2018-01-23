
#include "main.h"

SOCKET ListenTcp(int port){
	SOCKET con_sock;
	struct sockaddr_in server;

	//TCPでソケットオープン
	con_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(con_sock < 0){
		ErrorLog("socket");
		exit(EXIT_FAILURE);
	}

	//プロセス停止後すぐにbindができるようにする
	int on=1;
	setsockopt(con_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	//サーバのアドレスを設定
	memset(&server, 0, sizeof(server));
	server.sin_family		= AF_INET;
	server.sin_addr.s_addr	= htonl(INADDR_ANY);
	server.sin_port			= htons(port);
	if(bind(con_sock, (struct sockaddr *)&server, sizeof(server)) < 0){
		close(con_sock);
		ErrorLog("bind");
		exit(EXIT_FAILURE);
	}

	//コネクション確立要求受付開始
	if(listen(con_sock, 5) < 0){
		close(con_sock);
		ErrorLog("listen");
		exit(EXIT_FAILURE);
	}

	return con_sock;
}

SOCKET Accept(SOCKET con_sock){
	SOCKET data_sock;
	struct sockaddr_in client;
	u_int len;

	//コネクション処理
	len = sizeof(client);
	data_sock = accept(con_sock, (struct sockaddr *)&client, &len);
	if(data_sock < 0){
		close(con_sock);
		ErrorLog("accept");
		exit(EXIT_FAILURE);
	}
	//接続したクライアントのIPアドレス表示
	InfoLog("connected from '%s' con_sock=%d,data_sock=%d", inet_ntoa(client.sin_addr), con_sock, data_sock);

	return data_sock;
}