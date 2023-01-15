#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
char *randstring(size_t length,char *String,int len) {

    char *result=malloc(sizeof(char)*(length+1));
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
    char *String="abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$&*()_+=-?/<>:;',.";
    size_t len=strlen(String);
    char *array[50];
    for(int i=0;i<50;i++){
        array[i]=randstring(20,String,len);
        
    }

    struct timespec start,finish;
    struct timespec *td=(struct timespec *)malloc(sizeof(struct timespec*));
    clock_gettime(CLOCK_REALTIME,&start);
    mkfifo("testfifo",0777);
    for(int i=0;i<10;i++){
        int fd=open("testfifo",O_WRONLY);
        if(fd==-1){
            perror("Some error opening to write");
        }
        char *x=malloc(sizeof(char)*21);
        for(int j=0;j<5;j++){
            x=array[i];
            if(write(fd,array[i*5+j],sizeof(char)*21)==-1){
                perror("Some error writing");
            }
            int x=i*5+j;
            if(write(fd,&x,sizeof(int))==-1){
                perror("Some error writing");
            }
        }  
        close(fd); 
        fd=open("testfifo",O_RDONLY);
        if(fd==-1){
            perror("Some error");
        }
        int c;
        if(read(fd,&c,sizeof(int))==-1){
            perror("Some error reading");
        }
        printf("%d\n",c);
        close(fd);
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
    printf("\n Total time taken");
    printf("%f seconds\n",sec);
    
}
