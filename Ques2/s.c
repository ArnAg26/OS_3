#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <semaphore.h>

struct aa{
    char *str;
    int a;
};
char *randstring(size_t length,char *String,int len) {

    char *result=malloc(sizeof(char)*length+1);
    if(result!=NULL){
        for(int i=0;i<length;i++){
            int k=rand()%len;
            result[i]=String[k];
        }
        result[length]='\0';
    }
    return result;
}


int main(int argc,char **argv){
    int length=20;
    char *String="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t len=strlen(String);
    char *array[50];
    srand(time(0));
    for(int i=0;i<50;i++){
        array[i]=randstring(20,String,len);
    }
    for(int i=0;i<50;i++){
        printf("%s\n",array[i]);
    }
    int SIZE=512;
    int sh;
    if((sh=shm_open("p",O_RDWR|O_CREAT,0777))==-1){
        perror("Error creating shm");
    }
    ftruncate(sh,SIZE);
    void *p=mmap(NULL,SIZE,PROT_WRITE,MAP_SHARED,sh,0);
    if(p<0){
        perror("Mapping error");
    }
    int sh2;
    if((sh2=shm_open("pp",O_RDWR|O_CREAT,0777))==-1){
        perror("Error creating shm");
    }
    struct stat rr;

    sem_t *sem_des = sem_open("write", O_CREAT, 0644, 1);
    if(sem_des == (void*)-1){
         perror("sem_open failure");
    }
    sleep(3);
    int h;
    sem_t *semdes=sem_open("read",0,0644,0);
    struct timespec start,finish;
    struct timespec *td=(struct timespec *)malloc(sizeof(struct timespec*));
    clock_gettime(CLOCK_REALTIME,&start);
    for(int i=0;i<10;i++){
        for(int j=0;j<5;j++){
            char *k=array[i*5+j];
            char st[4];
            sprintf(st,"%d",(i*5+j));
            strcat(k," ");
            strcat(k,st);
            memcpy(p,k,sizeof(char)*24);
            sem_post(sem_des);
            sleep(1);
            sem_wait(semdes);
            if(fstat(sh2,&rr)==-1){
                perror("Read error");
            }
            int *p2=(int *)mmap(NULL,256,PROT_READ,MAP_SHARED,sh2,0);
            if(p2==MAP_FAILED){
                perror("Mapping error");
            }
            h=p2[0];
            if((h+1)%5==0){
                printf("%d",p2[0]);
                printf("\n");
            }
        }
    }
    
    clock_gettime(CLOCK_REALTIME,&finish);
    td->tv_nsec=finish.tv_nsec-start.tv_nsec;
    td->tv_sec=finish.tv_sec-start.tv_sec;
    if(td->tv_nsec<0){
        td->tv_sec--;
        td->tv_nsec+=1000000000;
    }
    double nsec=(double)(td->tv_nsec)/(double)(1000000000);
    double sec=(double)(td->tv_sec);
    sec=nsec+sec;
    printf("49\n Total time taken");
    printf("%f seconds\n",sec-50);
    munmap(p,SIZE);
    close(sh);
    shm_unlink("p");
    close(sh2);
    shm_unlink("pp");
    


//     int x=i*5+j;
        //     p=mmap(NULL,SIZE,PROT_WRITE,MAP_SHARED,sh,64);
        //     if(p==MAP_FAILED){
        //         perror("Mapping failed");
        //     }
        //     memcpy(p,&x,sizeof(int));
    


}