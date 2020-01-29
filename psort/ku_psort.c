#include "ku_psort.h"

int main(int argc, char *argv[])
{
    char *inputname = argv[3];
    char *outputname = argv[4];
    m = atoi(argv[1]);
    n = atoi(argv[2]);

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE;
    mqdes = mq_open(NAME_POSIX, O_CREAT | O_RDWR, 0666, &attr);
   
    file_open(inputname);

    make_process();

    main_quick_sort(0, m-1);

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

void make_process()
{
    int i;

    for(i = 0; i < n; i++)
    {
	p_num++;
	pid = fork();

	if(pid == 0)
	    break;
    }

    if(pid == 0)
	process_sort();

    else
    {
	while(wait(&state) > 0);
	mq_receiver();
    }
}

void process_sort()
{
    int div_num = m / n;
    int rest_num = m % n;
    int left = 0;
    int right;
    int temp;
    int i;
    
    if(p_num == n)
    {
	p_arr = (int *)malloc(sizeof(int)*div_num+rest_num);
	right = div_num + rest_num - 1;
    }

    else
    {
	p_arr = (int *)malloc(sizeof(int)*div_num);
	right = div_num - 1;
    }

    temp = right + 1;

    arr_copy(p_arr, num);

    quick_sort(0, right);

    for(i = 0; i < temp; i++)
    {
	if(mq_send(mqdes, (char *)&p_arr[i], MSG_SIZE, p_num) == -1)
	    perror("mq_send()");
    }

    exit(0);

}

void quick_sort(int left, int right)
{
    int l_hold = left;
    int r_hold = right;
    int pivot = p_arr[left];

    while(left < right)
    {
	while((p_arr[right] >= pivot) && (left < right))
	    right--;

	if(left != right)
	    p_arr[left] = p_arr[right];

	while((p_arr[left] <= pivot) && (left < right))
	    left++;

	if(left != right)
	{
	    p_arr[right] = p_arr[left];
	    right--;
	}
    }

    p_arr[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;

    if(left < pivot)
	quick_sort(left, pivot-1);

    if(right > pivot)
	quick_sort(pivot+1, right);
}

void main_quick_sort(int left, int right)
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
	main_quick_sort(left, pivot-1);

    if(right > pivot)
	main_quick_sort(pivot+1, right);
}

void arr_copy(int p_arr[], int num[])
{
    int div_num = m / n;
    int rest_num = m % n;
    int size;
    int first = (p_num-1) * div_num;
    int i;

    if(p_num == n)
	size = div_num + rest_num;

    else
	size = div_num;

    for(i = 0; i < size; i++)
	p_arr[i] = num[first + i];
}

void mq_receiver()
{
    int div_num = m / n;
    int rest_num = m % n;
    int i;

    for(i = 0; i < m; i++)
    {
	 mq_receive(mqdes, (char *)&data, MSG_SIZE, &prio);
	 num[i] = data;
    }

    mq_close(mqdes);
    mq_unlink(NAME_POSIX);
}
