
#include "main.h"

void Reaper(){
	int status;
	int chld_pid;
	chld_pid = wait(&status);
	InfoLog("reaper:chldpid=%d,status=%d",chld_pid,status);
}

void MakePidFile(){
	FILE *fp;

	fp = fopen(PID_FILE,"w");
	fprintf(fp, "%d", getpid());
	fclose(fp);
}

void Daemonize(){
	int pid;

	pid = fork();
	if(pid == -1){
		exit(1);
	}else if(pid > 0){
		//親プロセスの終了
		_exit(0);
	}

	//セッションリーダとなり制御端末を持たない状態にする
	if(setsid() < 0){
		exit(1);
	}

/*
	//ダブルフォーク（必要ないぽいのでしない）
	// HUPシグナルを無視.親が死んだ時にHUPシグナルが子にも送られる可能性があるため.
	signal(SIGHUP, SIG_IGN);
	pid = fork();
	if(pid == -1){
		exit(1);
	}else if(pid > 0){
		//最初の子プロセス（セッションリーダー）の終了
		_exit(0);
	}
*/

//	chdir("/");
	close(0); //STDIN
	close(1); //STDOUT
	close(2); //STDERR

	MakePidFile();
	signal(SIGCHLD, (void *)Reaper);
}

