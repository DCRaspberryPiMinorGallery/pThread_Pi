/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  ******************************************************************************
  *
  * <h2><center>&copy; Copyright (c) 2020 DCRaspberryPiMinorGallery.
  * All rights reserved.</center></h2>
  *
  * 랒파이 마이너 갤러리 https://gall.dcinside.com/mgallery/board/lists?id=pi
  *
  ******************************************************************************
  */

/*
 *	include
 */
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

/*
 *	define
 */
#define LOOP 20000
#define THREAD	4
#define MAXLINE 256

/*
 *	Global variables
 */
unsigned int iGlobalTick = 0;
int iThreadFinshCount = 0;

// function prototypes 
/*
 *	
 */
double fDoubleSquare(double src, int cnt);
int createTimer(timer_t* timerID, int sec, int msec);
void fPtheradStart(void);
int popen_init(const char *cmd, const char *mod);
void popen_readonly(void);

/*
 *	Main Function
 */
int main(int argc, char* argv[]) 
{
	//Timer 시작
    timer_t timerID;
    createTimer(&timerID, 0, 1);
	
	// pThread 시작
	fPtheradStart();
	
	while(1)
	{
		// Main loop
	}	
}

//---------------------------------------------------------------------------------------------
/*
 *	Double Type Square Function
 */
double fDoubleSquare(double src, int cnt){
    double res = src;
    if (cnt > 1) {
        for (int i = 2; i <= cnt; i++) {
        res = res * src;
        }
    }
    else if (cnt == 1) {
        res = src;
    }
    else if (cnt == 0) {
        res = 1.0f;
    }
    return res;
}

//---------------------------------------------------------------------------------------------
/*
 *	Linux Timer
 */
 
// 타이머 주기에 따라 호출될 타이머
void timer(void){
    iGlobalTick++;
}

//타이머 생성
int createTimer(timer_t* timerID, int sec, int msec){
    struct sigevent         te;
    struct itimerspec       its;
    struct sigaction        sa;
    int                     sigNo = SIGRTMIN;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sigNo, &sa, NULL) == -1){
        printf("sigaction error\n");
        return -1;
    }

    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_REALTIME, &te, timerID);

    its.it_interval.tv_sec = sec;
    its.it_interval.tv_nsec = msec * 1000000;
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = msec * 1000000;

    timer_settime(*timerID, 0, &its, NULL);

    return 0;
}

//---------------------------------------------------------------------------------------------
/*
 *	pThread Fubction
 */
 
// 주 연산용 pThread. 원주율 구할때 사용한 함수는 마틴급수
void* pThread_Function(void* data){
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    char* thread_name = (char*)data;
	double res = 0.0f;
		
    printf("Start [pThread_Function]\r\n");
	
	while(1){
		res = 0.0f;
		for (int i = 0; i <= LOOP; i++){
			res = res + (((4.0f * fDoubleSquare(1.0f / 5.0f, (2 * i) + 1)) - fDoubleSquare(1.0f / 239.0f, (2 * i) + 1)) * ((4.0f * fDoubleSquare(-1.0f, i)) / ((2.0f * (double)i) + 1.0f)));
		}
		iThreadFinshCount++;
	}
}

void* pThread_temp(void* data){
	pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    char* thread_name = (char*)data;

	FILE *fpPopen;
	int state;
	char readbuff[MAXLINE];
			
    printf("Start [pThread_temp]\r\n");
	
	while(1){
		if(iThreadFinshCount == THREAD){
			iThreadFinshCount = 0;
			
			fpPopen = popen("vcgencmd measure_temp", "r");
			if (fpPopen == NULL){
				perror("erro : ");
				exit(0);
			}
			while(fgets(readbuff, MAXLINE, fpPopen) != NULL){
				printf("%s", readbuff);
			}
			state = pclose(fpPopen);
			if(state != 0){
				printf("state is %d\n", state);
			}
			printf("time=%11.4f\r\n", ((double)iGlobalTick / 1000.0f));
		}
	}
}

//pThread Start
void fPtheradStart(void)
{
    int thr_id;
    int status;
	int i;
	
    pthread_t p_thread[THREAD];
	pthread_t p_poenPthreadd;
	
    char p1[] = "pThread_Function";
	char p2[] = "p_poenPthreadd";   
	
	for( i = 0 ; i < THREAD ; i++){
		thr_id = pthread_create(&p_thread[i], NULL, pThread_Function, (void*)p1);
		if (thr_id < 0){
			perror("thread create error : ");
			exit(0);
		}
		pthread_detach(p_thread[i]);
		pthread_join(p_thread[i], NULL);	
	}	
	
	thr_id = pthread_create(&p_poenPthreadd, NULL, pThread_temp, (void*)p1);
	if (thr_id < 0){
		perror("thread create error : ");
		exit(0);
	}
	pthread_detach(p_poenPthreadd);
	pthread_join(p_poenPthreadd, NULL);	
}



