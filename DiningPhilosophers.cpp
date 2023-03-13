
#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (n + 4) % N
#define RIGHT (n + 1) % N

int phil[N];
int times = 200;

class monitor {

	int state[N];
	pthread_cond_t cond[N];
	pthread_mutex_t mut;

public:

	void test(int n)
	{
		if (state[(n + 1) % 5] != EATING && state[(n + 4) % 5] != EATING && state[n] == HUNGRY) {
			state[n] = EATING;
			pthread_cond_signal(&cond[n]);
		}
	}

	void take_fork(int n)
	{
		pthread_mutex_lock(&mut);
		state[n] = HUNGRY;
		test(n);
		if (state[n] != EATING) {
			pthread_cond_wait(&cond[n], &mut);
		}
		cout << "Philosopher " << (n + 1) << " is eating."<< endl;
		pthread_mutex_unlock(&mut);
	}

	void put_fork(int n)
	{
		pthread_mutex_lock(&mut);
		state[n] = THINKING;
		test(RIGHT);
		test(LEFT);
		pthread_mutex_unlock(&mut);
	}

	monitor()
	{
		for (int i = 0; i < N; i++) {
			state[i] = THINKING;
		}
		for (int i = 0; i < N; i++) {
			pthread_cond_init(&cond[i], NULL);
		}
		pthread_mutex_init(&mut, NULL);
	}

	~monitor()
	{
		for (int i = 0; i < N; i++) {
			pthread_cond_destroy(&cond[i]);
		}
		pthread_mutex_destroy(&mut);
	}
}

phil_object;

void* philosopher(void* arg)
{
	int c = 0;
	while (c < times) {
		int i = *(int*)arg;
		sleep(1);
		phil_object.take_fork(i);
		sleep(0.5);
		phil_object.put_fork(i);
		c++;
	}
}

int main()
{

	pthread_t thread_id[N];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (int i = 0; i < N; i++) {
		phil[i] = i;
	}

	for (int i = 0; i < N; i++) {
		pthread_create(&thread_id[i], &attr, philosopher, &phil[i]);
		cout << "Philosopher " << (i + 1) << " is thinking" << endl;
	}

	for (int i = 0; i < N; i++) {
		pthread_join(thread_id[i], NULL);
	}

	pthread_attr_destroy(&attr);
	pthread_exit(NULL);

	return 0;
}
