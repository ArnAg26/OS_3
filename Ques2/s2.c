#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>

struct aa{
    char *str;
    int a;
};

int main(int argc,char **argv){
    size_t SIZE=512;
    int sh2=shm_open("p",O_RDWR|O_CREAT,0777);
    if(sh2==-1){
        perror("Error creating shm");
    }
    int sh=shm_open("pp",O_RDWR|O_CREAT,0777);
    if(sh==-1){
        perror("Error creating shm");
    }
    ftruncate(sh,SIZE);
    int *p=(int *)mmap(NULL,SIZE,PROT_WRITE,MAP_SHARED,sh,0);
    if(p<0){
        perror("Mapping error");
    }
    struct stat rr;

    sem_t *semdes=sem_open("read",O_CREAT,0644,1);
    
    if(semdes == (void*)-1){
         perror("sem_open failure");
    }
    sem_t *sem_des=sem_open("write",0,0644,0);
    sleep(2);
    
    
    for(int i=0;i<10;i++){
        for(int j=0;j<5;j++){
            sem_wait(sem_des);
            if(fstat(sh,&rr)==-1){
                perror("Read error");
            }
            char *p2=mmap(NULL,256,PROT_READ,MAP_SHARED,sh2,0);
            if(p2==MAP_FAILED){
                perror("Mapping error");
            }
            write(STDOUT_FILENO,p2,rr.st_size);
            //printf(" %d",(i*5+j));
            printf("\n");
            sem_post(sem_des);
            sleep(0.25);
            int x=i*5+j;
            p[0]=x;
            sem_post(semdes);
        }
    }
    munmap(p,SIZE);
    close(sh);
    shm_unlink("p");
    close(sh2);
    shm_unlink("pp");

}