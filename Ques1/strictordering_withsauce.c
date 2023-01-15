#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int global[5]={0,0,0,0,0};
int philosophers[5]={1,2,3,4,5};
int count[5]={0,0,0,0,0};
int forks[5]={0,1,2,3,4};
int sauce1=0;
int sauce2=0;


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
void eat(int arg){
    if(count[left(arg)]==0 && count[right(arg)]==0){
        count[left(arg)]=1;
        count[right(arg)]=1;
        printf("philospher %d eats with forks %d and %d\n",arg,left(arg),right(arg));
        count[left(arg)]=0;
        count[right(arg)]=0;
    }
    else{
        sleep(1);
        eat(arg);
    }
}
void take_sauce_bowl(int arg){
    if(sauce1==0){
        sauce1=1;
        global[arg-1]=1;
        printf("philosopher %d has acquired sauce 1\n",arg);
    }
    else if(sauce2==0){
        sauce2=2;
        global[arg-1]=2;
        printf("philsopher %d has acquired sauce 2\n",arg);
    }
    else{
        sleep(0);
        take_sauce_bowl(arg);
    }
}
void keeps_sauce_bowl(int arg){
    if(global[arg-1]==1){
        printf("philosopher %d has kept sauce 1\n",arg);
        global[arg-1]=0;
        sauce1=0;
    }
    else if(global[arg-1]==2){
        printf("philosopher %d has kept sauce 2\n",arg);
        global[arg-1]=0;
        sauce2=0;
    }

}
void *philosopher(void *a){
    int *num=a;
    int arg=*num;
    int i=0;
    while(i<5){
        think();
        take_sauce_bowl(arg);
        eat(arg);
        keeps_sauce_bowl(arg);
        sleep(1);
        i++;
    }


}
int main(int argc,char **argv){
    pthread_t thr[5];
    for(int i=0;i<5;i++){
        
        pthread_create(&thr[i],NULL,&philosopher,&philosophers[i]);
    }
    for(int i=0;i<5;i++){
        pthread_join(thr[i],NULL);
    }

}