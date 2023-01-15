#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>


int global[5]={0,0,0,0,0};
int philosophers[5]={1,2,3,4,5};
int count[5]={0,0,0,0,0};
int forks[5]={0,1,2,3,4};

int left(int a){
    return forks[a-1];
}
int right(int a){
    if(a==5){
        return forks[0];
    }
    return forks[a];
}

int eat(int a ){
    global[left(a)]++;
    global[right(a)]++;
    printf("philosopher %d is eating with forks %d and %d\n",a,left(a),right(a));
    count[a-1]++;
}

void think(){
    sleep(1);
}
bool calcCount(int a){
    for(int i=0;i<a-1;i++){
        if(count[i]<=count[a-1]){
            return false;
        }
    }
    return true;
}

void *Philosopher(void *arg){
    int *a=arg;
    int num=*a;
    int i=0;
    while(i<5){
        think();
        printf("philosopher %d requests forks %d and %d\n",num,left(num),right(num));
        while(!calcCount(num)){
            sleep(0);
        }
        printf("philosopher %d takes forks %d and %d\n",num,left(num),right(num));
        eat(num);
        i++;
        printf("%d philosopher keeps forks %d and %d\n",num,left(num),right(num));
    }
}

int main(int argc,char **argv){
    pthread_t thr[5];
    for(int i=0;i<5;i++){
        pthread_create(&thr[i],NULL,&Philosopher,&philosophers[i]);
    }
    for(int i=0;i<5;i++){
        pthread_join(thr[i],NULL);
    }
    printf("Use of the forks ");

    for(int i=0;i<5;i++){
        printf("%d ",global[i]);
    }
    printf("\n");

}
