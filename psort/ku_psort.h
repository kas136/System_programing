#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/wait.h>

#define MSG_SIZE 4
#define NAME_POSIX "/my_mq"

int num[10];
int *p_arr;
int m;
int n;
int p_num = 0;
int state;
int data = 0;
unsigned int prio = 0;
struct mq_attr attr;
pid_t pid;
mqd_t mqdes;

void file_open(char *inputname);
void file_write(char *outputname);
void make_process();
void process_sort();
void quick_sort(int left, int right);
void main_quick_sort(int left, int right);
void arr_copy(int p_arr[], int num[]);
void mq_receiver();
