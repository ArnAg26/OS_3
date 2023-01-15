#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

pthread_t philosophers[5];
sem_t sema[5];
int global[5]={0,0,0,0,0};
int forks[5]={0,1,2,3,4};
int phil[5]={1,2,3,4,5};
int left(int a){
    return forks[a-1];
}
int right(int a){
    if(a==5){
        return forks[0];
    }
    return forks[a];
}
void think(){
    sleep(0);
}

int take_fork(int a){
    if(a==5){
        sem_wait(&sema[right(a)]);
        sem_wait(&sema[left(a)]);
    }
    else{
        sem_wait(&sema[left(a)]);
        sem_wait(&sema[right(a)]);
    }
    printf("%d philosopher takes forks %d and %d\n",a,left(a),right(a));


}
int keep_fork(int a){
    
    sem_post(&sema[left(a)]);
    sem_post(&sema[right(a)]);
    printf("%d philosopher keeps forks %d and %d\n",a,left(a),right(a));

}
int eat(int a ){
    global[left(a)]++;
    global[right(a)]++;
    printf("philosopher %d is eating with forks %d and %d\n",a,left(a),right(a));
    sleep(0);

}

void *Philosopher(void *arg){
    int *num=arg;
    int a=*num;
    int i=0;
    while(i<5){
        think();
        take_fork(a);
        eat(a);
        keep_fork(a);
        i++;
    }
}
int main(int argc,char **argv){
    for(int i=0;i<5;i++){
        sem_init(&sema[i],0,1);
    }
    for(int i=0;i<5;i++){
        if(pthread_create(&philosophers[i],NULL,&Philosopher,&phil[i])!=0){
            perror("Error creating thread");
        }
    }
    for(int i=0;i<5;i++){
        if(pthread_join(philosophers[i],NULL)!=0){
            perror("Error joining thread");
        }
    }
    printf("Use of the forks ");

    for(int i=0;i<5;i++){
        printf("%d ",global[i]);
    }
    printf("\n");
    

}