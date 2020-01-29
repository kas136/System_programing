#include "ku_tsort.h"

int main(int argc, char *argv[])
{
    char *inputname = argv[3];
    char *outputname = argv[4];
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    file_open(inputname);               // file input
    
    pthread_t *p_thread[n];
    
    make_thread(p_thread);            //thread play

    quick_sort(0, m-1);

    file_write(outputname);
}

void file_open(char *inputname)
{
    FILE *fp;
    int i;
    int line;
   
    fp = fopen(inputname, "rt");

    if(fp == NULL)
    {
	printf("error\n");
    }

    else
    {
	i = 0;
	fseek(fp, 0, SEEK_SET);
	while(fscanf(fp, "%d", &line) != EOF)
	    num[i++] = line;

	fclose(fp);
    }
}

void file_write(char *outputname)
{
    FILE *fp;
    int i;

    fp = fopen(outputname, "wt");


    for(i = 0; i < m; i++)
    {
	fprintf(fp, "%d\n", num[i]);
    }

    fclose(fp);
}

void make_thread(pthread_t *p_thread)
{
    int i;
    int temp[10] = {0};
    void *t_return = NULL;

    pthread_mutex_init(&mutex, NULL);

    for(i = 0; i < n; i++)
    {
	temp[i] = i + 1;
	pthread_create(&p_thread[i], NULL, thread_sort, (void *)&temp[i]);
    }

    for(i = 0; i < n; i++)
    {
	pthread_join(p_thread[i], &t_return);
    }
    pthread_mutex_destroy(&mutex);
}

void thread_sort(void *temp)
{
    pthread_mutex_lock(&mutex);
    
    int thread_num = *((int *)temp);
    int div_num = m / n;
    int rest_num = m % n;
    int left = (thread_num-1) * div_num;
    int right;

    if(thread_num == n)
	right = m-1;
    else
	right = left + div_num - 1;

    quick_sort(left, right);
    
    pthread_mutex_unlock(&mutex);
}



void quick_sort(int left, int right)
{
    int l_hold = left;
    int r_hold = right;
    int pivot = num[left];
    
    while(left < right)
    {
	while((num[right] >= pivot) && (left < right))
	    right--;

	if(left != right)
	    num[left] = num[right];

	while((num[left] <= pivot) && (left < right))
	    left++;

	if(left != right)
	{
	    num[right] = num[left];
	    right--;
	}
    }

    num[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;

    if(left < pivot)
	quick_sort(left, pivot-1);

    if(right > pivot)
	quick_sort(pivot+1, right);
}
