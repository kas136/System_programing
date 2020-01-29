#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int num[10];
int m;
int n;
pthread_mutex_t mutex;

void file_open(char *inputname);
void file_write(char *outputname);
void make_thread(pthread_t *p_thread);
void thread_sort(void *temp);
void quick_sort(int left, int right);
