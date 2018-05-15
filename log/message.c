#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// 쓰레드 함수
void *t_function(void *data)
{
    FILE *f;

    f=fopen("log.txt","w");

    int id;
    int i = 0;
    id = *((int *)data);
    static int status = 1000;
    clock_t before, after;
    int  result;
    before  = clock();

    while(1)
    {
	printf("id %d : %d\n", id, i);
	i++;
	sleep(1);
	after = (clock() - before);
	result = after / (CLOCKS_PER_SEC/1000);
	printf("before : %d result : %d after: %d clock_per_sec : %d\n", before, result, after, CLOCKS_PER_SEC);
	fprintf(f,"before: %d, result: %d\n",before, result);
	if (result>=10)
	{
	    // 반환값 설정
	    printf("thread exit\n");
	    pthread_exit((void*)&status);
	}


    }
    fclose(f);
}

int main()
{
    pthread_t p_thread[2];
    int thr_id;
    int status;
    int a = 1;
    int b = 2;

    // 쓰레드 생성 아규먼트로 1 을 넘긴다.  
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
	perror("thread create error : ");
	exit(0);
    }

    // 쓰레드 생성 아규먼트로 2 를 넘긴다. 
    //    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&b);
    if (thr_id < 0)
    {
	perror("thread create error : ");
	exit(0);
    }

    // 쓰레드 종료를 기다린다. 
    pthread_join(p_thread[0], (void **)&status);
    //    pthread_join(p_thread[1], (void **)&status);

    return 0;
}
