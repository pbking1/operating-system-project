/***
read the README.md before run the file

***/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>

struct thread_info{
	int id;
	int reader_or_writer;
	int resource_id;
	int arrive_t;
	int burst_t;
};

struct thread_pool{
	int value;
	pthread_mutex_t wl;
	pthread_mutex_t rl;
	pthread_mutex_t can_read;
	pthread_mutex_t can_write;
	int w_number;
	int r_number;
};

int number;
int writer_number = 0, reader_number = 0;

int compare(const void *a, const void *b){
	struct thread_info *aa = (struct thread_info*)a;
	struct thread_info *bb = (struct thread_info*)b;
	return aa->arrive_t - bb->arrive_t;
}

static void *reader_thread(void *a){
	struct thread_pool *p = a;

	pthread_mutex_lock(&p->can_read);
	pthread_mutex_lock(&p->rl);
	p->r_number = p->r_number + 1;
	if(p->r_number == 1){
		pthread_mutex_lock(&p->can_write);
	}
	pthread_mutex_unlock(&p->rl);
	pthread_mutex_unlock(&p->can_read);

	printf("reader read %d seconds and ", p -> value);
	sleep(p -> value);	
	time_t t = time(0);
	printf("current time %ld\n", t);

	pthread_mutex_lock(&p->rl);
	p->r_number = p->r_number - 1;
	if(p->r_number == 0){
		pthread_mutex_unlock(&p->can_write);
	}
	pthread_mutex_unlock(&p->rl);

	return NULL;
}

static void *writer_thread(void *a){
	struct thread_pool *p = a;

	pthread_mutex_lock(&p->wl);
	p->w_number = p->w_number + 1;
	if(p->w_number == 1){
		pthread_mutex_lock(&p->can_read);
	}
	pthread_mutex_unlock(&p->wl);

	pthread_mutex_lock(&p->can_write);
	printf("writerer write %d seconds and ", p -> value);
	sleep(p -> value);
	time_t t1 = time(0);
	printf("current time %ld\n", t1);
	pthread_mutex_unlock(&p->can_write);

	pthread_mutex_lock(&p->wl);
	p->w_number = p->w_number - 1;
	if(p->w_number == 0){
		pthread_mutex_unlock(&p->can_read);
	}
	pthread_mutex_unlock(&p->wl);
	return NULL;
}

static inline void init_thread_pool(struct thread_pool *p){
	p -> value = 0;
	p -> r_number = 0;
	p -> w_number = 0;
	pthread_mutex_init(&p->wl, NULL);
	pthread_mutex_init(&p->rl, NULL);
	pthread_mutex_init(&p->can_read, NULL);
	pthread_mutex_init(&p->can_write, NULL);
}

static inline void update_thread_pool(struct thread_pool *p, int time){
	p -> value = time;
}

void empty_array_row(int i, struct thread_info a[]){
	a[i].id = a[i].resource_id = a[i].reader_or_writer = a[i].arrive_t = a[i].burst_t = 0;
}

int sum_array_row(int i, struct thread_info a[]){
	if(a[i].id+a[i].resource_id+a[i].reader_or_writer+a[i].arrive_t+a[i].burst_t == 0)
		return 0;
	else
		return 1;
}

int main(){
	//read file
	FILE *fin;
	if((fin = fopen("input.2", "r")) == NULL){
	//read file fail
		printf("read input file failed, please modify you input as input.2 format and name\n");
		exit(0);
	}
	fscanf(fin, "%d", &number);
	struct thread_info a[number];
	for(int i = 0; i < number; i++){
		a[i].id = a[i].resource_id = a[i].reader_or_writer = a[i].arrive_t = a[i].burst_t = 0;
	}
	for(int i = 0; i < number; i++){
		fscanf(fin, "%d %d %d %d %d\n", &a[i].id, &a[i].reader_or_writer, &a[i].resource_id, &a[i].arrive_t, &a[i].burst_t);
		if(a[i].reader_or_writer == 2){
			++reader_number;
		}else if(a[i].reader_or_writer == 1){
			++writer_number;
		}
		
	}
	struct thread_info writer[writer_number];
	struct thread_info reader[reader_number];
	for(int i = 0; i < writer_number; i++){
		writer[i].id = writer[i].resource_id = writer[i].reader_or_writer = writer[i].arrive_t = writer[i].burst_t = 0;
	}
	for(int i = 0; i < reader_number; i++){
		reader[i].id = reader[i].resource_id = reader[i].reader_or_writer = reader[i].arrive_t = reader[i].burst_t = 0;
	}
	int ri = 0, wi = 0;
	for(int i = 0; i < number; i++){
		if(a[i].reader_or_writer == 2){
			reader[ri].id = a[i].id;
			reader[ri].resource_id = a[i].resource_id;
			reader[ri].reader_or_writer = a[i].reader_or_writer;
			reader[ri].arrive_t = a[i].arrive_t;
			reader[ri].burst_t = a[i].burst_t;
			ri++;
		}
		if(a[i].reader_or_writer == 1){
			writer[wi].id = a[i].id;
			writer[wi].resource_id = a[i].resource_id;
			writer[wi].reader_or_writer = a[i].reader_or_writer;
			writer[wi].arrive_t = a[i].arrive_t;
			writer[wi].burst_t = a[i].burst_t;
			wi++;
		}
		
	}
	fclose(fin);
	FILE *fout = fopen("processevent.log", "w");
	fprintf(fout, "processID Resource arrival-time start-time end-time\n");
	//sort the array by arrive time 
	qsort(reader, reader_number, sizeof(struct thread_info), compare);
	qsort(writer, writer_number, sizeof(struct thread_info), compare);

	pthread_t pid[number];

	struct thread_pool pool;
	init_thread_pool(&pool);

	//begin time
	time_t t1 = time(0);
	printf("current time %ld\n", t1);
	//algorithm begin
	int total_time = 0;
	int all_exec = 0;
	while(1){
		if(all_exec >= number){
			break;
		}
		int flag = 1;
		int start_t = 0;
		int temp_id, temp_resourceid, temp_arrivet;
		//judge whether there is writer waiting or not
		for(int i = 0; i < writer_number; i++){
			if(sum_array_row(i, writer) != 0){
				if(writer[i].arrive_t <= total_time){
					update_thread_pool(&pool, writer[i].burst_t);
					pthread_create(&pid[writer[i].id], NULL, writer_thread, &pool);
					all_exec++;
					pthread_join(pid[writer[i].id], NULL);
					temp_id = writer[i].id;
					temp_resourceid = writer[i].resource_id;
					temp_arrivet = writer[i].arrive_t;
					start_t = total_time;
					total_time = total_time + writer[i].burst_t;
					empty_array_row(i, writer);
					fprintf(fout, "%d %d %d %d %d\n", temp_id , temp_resourceid, temp_arrivet, start_t, total_time);
					flag = 0;
					break;
				}else{
					break;
				}
			}else{
				continue;
			}
		}

		//judge whether there is reader waiting if there is not writer
		if(flag == 1){
			for(int i = 0; i < reader_number; i++){
				if(sum_array_row(i, reader) != 0){
					if(reader[i].arrive_t <= total_time){
						update_thread_pool(&pool, reader[i].burst_t);
						pthread_create(&pid[reader[i].id], NULL, reader_thread, &pool);
						all_exec++;
						pthread_join(pid[reader[i].id], NULL);
						temp_id = reader[i].id;
						temp_resourceid = reader[i].resource_id;
						temp_arrivet = reader[i].arrive_t;
						start_t = total_time;
						total_time += reader[i].burst_t;
						empty_array_row(i, reader);
						fprintf(fout, "%d %d %d %d %d\n", temp_id , temp_resourceid, temp_arrivet, start_t, total_time);
						break;
					}else{
						break;
					}
				}else{
					continue;
				}
			}
		}
	}
	return 0;
}























