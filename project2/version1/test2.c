#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 

struct pool {
   int nr_reader, nr_writer;
   unsigned long long value;
   pthread_mutex_t may_read, may_write, wr_count_mutex, rd_count_mutex;
};
 
static void* writer(void* arg)
{
   struct pool* p = arg;
 
   //while (1) {
  /**/
  pthread_mutex_lock(&p->wr_count_mutex);
  ++p->nr_writer;
  if (p->nr_writer == 1)
     pthread_mutex_lock(&p->may_read); //只要有一个写者就不允许读
  pthread_mutex_unlock(&p->wr_count_mutex);

  /**/
  pthread_mutex_lock(&p->may_write); //写者使用资源
  ++p->value;
  printf("writer: value = %llu\n", p->value);
  sleep(1);
  pthread_mutex_unlock(&p->may_write);

  /**/ 
  pthread_mutex_lock(&p->wr_count_mutex);
  --p->nr_writer;
  if (p->nr_writer == 0)
     pthread_mutex_unlock(&p->may_read); //没有写者的时候才允许读
  pthread_mutex_unlock(&p->wr_count_mutex);


   //}
 
   return NULL;
}
 
static void* reader(void* arg)
{
   struct pool* p = arg;
 
   //while (1) {
      pthread_mutex_lock(&p->may_read);
 
      pthread_mutex_lock(&p->rd_count_mutex);
      ++p->nr_reader;
      if (p->nr_reader == 1)
         pthread_mutex_lock(&p->may_write); //只要有读者在读，就不能写
      pthread_mutex_unlock(&p->rd_count_mutex);
 
      pthread_mutex_unlock(&p->may_read);
 
      printf("%d reader(s), value = %llu\n", p->nr_reader, p->value);
      sleep(1);

      pthread_mutex_lock(&p->rd_count_mutex);
      --p->nr_reader;
      if (p->nr_reader == 0)//没有读者在读，就允许写者写
         pthread_mutex_unlock(&p->may_write);
      pthread_mutex_unlock(&p->rd_count_mutex);
   //}
 
   return NULL;
}
 
static inline void pool_init(struct pool* p)
{
   p->nr_reader = 0;
   p->value = 0;
   pthread_mutex_init(&p->may_read, NULL);
   pthread_mutex_init(&p->may_write, NULL);
   pthread_mutex_init(&p->rd_count_mutex, NULL);
   pthread_mutex_init(&p->wr_count_mutex, NULL);
}
 
#define NR_READER 5
#define NR_WRITER 5
 
int main(void)
{
   int i, status;
   struct pool pool;
   pthread_t reader_pid[NR_READER], writer_pid[NR_WRITER], writer_pid1[NR_WRITER];
 
   pool_init(&pool);
 
   for (i = 0; i < NR_WRITER; ++i) {
      status = pthread_create(&writer_pid[i], NULL, writer, &pool);
      if (status != 0)
         fprintf(stderr, "create writer %d failed.\n", i);
   }
 
   for (i = 0; i < NR_READER; ++i) {
      status = pthread_create(&reader_pid[i], NULL, reader, &pool);
      if (status != 0)
         fprintf(stderr, "create reader %d failed.\n", i);
   }

   for (i = 0; i < NR_WRITER; ++i) {
      status = pthread_create(&writer_pid1[i], NULL, writer, &pool);
      if (status != 0)
         fprintf(stderr, "create writer %d failed.\n", i);
   }
 
   for (i = 0; i < NR_WRITER; ++i)
      pthread_join(writer_pid[i], NULL);
 
   for (i = 0; i < NR_READER; ++i)
      pthread_join(reader_pid[i], NULL);


   for (i = 0; i < NR_WRITER; ++i){
      pthread_join(writer_pid1[i], NULL);
   }
 
   return 0;
}