#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/wait.h>

#define	DEFAULT_PORT		80
#define	DEFAULT_SERVERS		6
#define	DEFAULT_DOCROOT		"./htdocs"
#define	DEFAULT_SERVTYPE	"prefork"

#define	SERVER_ROOT			"./httpd"

#define	PID_FILE		"./httpd.pid"
#define	LOG_FILE		"./log/httpd.log"

typedef	int SOCKET;

typedef struct {
	int listen_port;
	int prefork_num;
	char doc_root[128];
	char server_type[16];
} Config;

Config *config;

void Reaper();
void ErrorLog(char *msg);
void InfoLog(char *fmt, ...);
SOCKET ListenTcp(int port);
SOCKET Accept(SOCKET con_sock);
void Receive(SOCKET data_sock);
int Response(SOCKET data_sock, char *msg);
void Analyze(char *recv_buf, SOCKET data_sock);
int FileRead(char *fpath, char *buf, int bufmax);
void ForkServer(SOCKET con_sock);
void PreforkServer(SOCKET con_sock);
void LoadConfig();
void StartServer();
void StopServer();
void Daemonize();
void MakePidFile();

