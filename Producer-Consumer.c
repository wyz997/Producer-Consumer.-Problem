#include <stdio.h>
#include <pthread.h>
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;  //记录当前缓存
int sale = 0;  //记录已经销售了多少个产品
int one = 1;
int two = 2;
int three = 3;

void *producer(void* p)
{
	int *ptr = (int*)p;  //ptr传入并记录了当前是几号进程
	while(sale <= 50)  //当销售量大于50的时候停止生产
	{
		pthread_mutex_lock(&the_mutex);
		while(buffer >= 10)  //当缓存大于等于10的时候暂缓生产
		{
			printf("producer%d wait...Now buffer is %d!\n", *ptr, buffer);
			pthread_cond_wait(&condp, &the_mutex);
		}
		buffer += 1;
		printf("producer%d produce!Now buffer is %d!\n", *ptr, buffer);
		pthread_cond_signal(&condc);
		pthread_mutex_unlock(&the_mutex);
	}
	pthread_exit(0);
}

void *consumer(void* p)
{
	int *ptr = (int*)p;
	while(sale <= 50)
	{
		pthread_mutex_lock(&the_mutex);
		while(buffer == 0)  //当缓存为0的时候暂缓消费
		{
			printf("consumer%d wait...Now buffer is %d!\n", *ptr, buffer);
			pthread_cond_wait(&condc, &the_mutex);
		}
		buffer -= 1;
		sale += 1;  //销售记录增加
		printf("consumer%d consume!Now sells %d!\n", *ptr, sale);
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&the_mutex);
	}
	pthread_exit(0);
}

int main(int argc, char **argv)
{
	pthread_t pro[3];
	pthread_t con[2];
	int i;
	void *index;
	pthread_mutex_init(&the_mutex, 0);
	pthread_cond_init(&condc, 0);
	pthread_cond_init(&condp, 0);
	for(i = 0;i < 2;i++)
	{
		if(i == 0)
		{
			index = &one;
		}
		else
		{
			index = &two;
		}
		pthread_create(&con[i], 0, consumer, index);
	}
	for(i = 0;i < 3;i++)
	{
		if(i == 0)
		{
			index = &one;
		}
		else if(i == 1)
		{
			index = &two;
		}
		else
		{
			index = &three;
		}
		pthread_create(&pro[i], 0, producer, index);
	}
	for(i = 0;i < 3;i++)
	{
		pthread_join(pro[i], 0);
	}
	for(i = 0;i < 2;i++)
	{
		pthread_join(con[i], 0);
	}
	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&the_mutex);
	return 0;
}
