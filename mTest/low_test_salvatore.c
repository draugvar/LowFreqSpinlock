//
// Created by Salvatore Rivieccio on 01/10/16.
//
#include "../LowFreqSpinlock/low_freq_spinlock.h"
#include <pthread.h>
#include "test.h"
#include "../rapl-power/rapl.h"

low_freq_spinlock_t try;
volatile pthread_spinlock_t lock;
pthread_barrier_t thread_barrier;
int executed_critical_sections[1024], global_thread_id;
int volatile run;
int critical_section_length;

int cpu_id[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

static inline void set_affinity(int thread_id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    thread_id=thread_id%32;
    CPU_SET(cpu_id[thread_id], &cpuset);
    sched_setaffinity(0, sizeof(cpuset),&cpuset);
}

void *threadFunc(void *arg)
{
    pthread_spin_lock(&lock);
    unsigned int i = global_thread_id++;
    pthread_spin_unlock(&lock);
    set_affinity(i);
    pthread_barrier_wait(&thread_barrier);
    while(run) {

	low_freq_op_lock(&try);
//	pthread_spin_lock(&lock);

	int k =0, m=0;
	for(k = 0; k < critical_section_length; k++) m++;

	executed_critical_sections[i]++;

        low_freq_op_unlock(&try);
//	pthread_spin_unlock(&lock);

	for(k = 0; k < critical_section_length; k++) m++;

    }
}

int main(int argc, char *argv[])
{
    int i;
    global_thread_id=0;
    try = LOW_FREQ_UNLOCKED;
    if (argc!= 4) {
	printf("Usage: executable_name number_of_threads critical_section length duration\n");
	exit(1);
    }
    int n_threads = atoi(argv[1]);
    critical_section_length = atoi(argv[2]);
    int duration = atoi(argv[3]);

    pthread_spin_init(&lock, 0);
    pthread_barrier_init(&thread_barrier,NULL,n_threads+1);

    //printf("Benckmark started\nCreating threads... ");

    run =1;
    pthread_t pthread[1024];

    for(i = 0; i < n_threads; i++)
    {
        pthread_create(&pthread[i], NULL, threadFunc, (void*) sleep);
	executed_critical_sections[i]=0;
    }

    printf("ok.\nRunning ...\n");
    
    struct timeval t0, t1;    
    sleep(1);
    pthread_barrier_wait(&thread_barrier);
    
    startEnergy();
    
    gettimeofday(&t0, 0);
    sleep(duration);
    
    float joule=endEnergy();

    gettimeofday(&t1, 0);
    run=0;
    pthread_spin_destroy(&lock);   
    double time = (double)(t1.tv_sec-t0.tv_sec) + (double)(t1.tv_usec-t0.tv_usec)/(double)1000000;
    int total_executed_critical_sections=0;
    for (i=0; i < n_threads; i++)
    {
	//printf("Executed critical sections by thread %i: %i\n", i, executed_critical_sections[i]);
	total_executed_critical_sections+=executed_critical_sections[i];
    }
    double throughput = (double)total_executed_critical_sections/(double) time;
    printf("Benckmark completed in %f sec\t critical sections per sec: %f\t energy: %f\n", time, throughput, joule);    

    return 0;
}
