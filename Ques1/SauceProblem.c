#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

pthread_t philosophers[5];
sem_t sema[5];
int global[5]={0,0,0,0,0};
int forks[5]={0,1,2,3,4};
int phil[5]={1,2,3,4,5};
sem_t sauce;
int Useofsauce=0;
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

void take_sauce(int a){
    sem_wait(&sauce);
    Useofsauce++;
    printf("%d philosopher takes sauce\n",a);
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

int keep_sauce(int a){
    printf("%d philosopher keeps sauce\n",a);
    sem_post(&sauce);

}
int keep_fork(int a){
    printf("%d philosopher keeps forks %d %d\n",a,left(a),right(a));
    sem_post(&sema[left(a)]);
    sem_post(&sema[right(a)]);
    

}
int eat(int a ){
    global[left(a)]++;
    global[right(a)]++;
    sleep(0);
    printf("philosopher %d is eating with forks %d and %d with sauce\n",a,left(a),right(a));
}

void *Philosopher(void *arg){
    int *num=arg;
    int a=*num;
    int i=0;
    while(i<5){
        think();
        take_fork(a);
        take_sauce(a);
        eat(a);
        keep_sauce(a);
        keep_fork(a);
        i++;
    }
}
int main(int argc,char **argv){
    sem_init(&sauce,0,2);
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
    printf("Use of sauce %d\n",Useofsauce);
    

}