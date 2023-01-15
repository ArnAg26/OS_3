#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>


#define SV_SOCK_PATH "/tmp/arn"

char *randstring(size_t length,char *String,int len) {

    char *result=malloc(sizeof(char)*(length+1));
    if(result!=NULL){
        for(int i=0;i<length;i++){
            int k=rand()%len;
            result[i]=String[k];
            sleep(0.1);
        }
        result[length]='\0';
    }
    return result;
}

int main(int argc,char **argv){
    int length=20;
    char *String="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t len=strlen(String);
    srand(time(0));
    char *array[51];
    for(int i=0;i<50;i++){
        array[i]=randstring(20,String,len);
    }

    // for(int j=0;j<50;j++){
    //     printf("%s\n",array[j]);
    // }
    struct sockaddr_un addr;
    int sd,cd;
    if(remove(SV_SOCK_PATH)==-1 && errno!=ENOENT){
        perror("Socket removed");
    }
    sd=socket(AF_UNIX,SOCK_STREAM,0);
    if(sd==-1){
        perror("Socket cant be created");
    }
    memset(&addr,0,sizeof(struct sockaddr_un));
    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path,SV_SOCK_PATH,sizeof(struct sockaddr_un)-1);
    bind(sd,(struct sockaddr *)&addr,sizeof(struct sockaddr_un));
    listen(sd,2);
    cd=accept(sd,NULL,NULL);
    struct timespec start,finish;
    struct timespec *td=(struct timespec *)malloc(sizeof(struct timespec*));
    clock_gettime(CLOCK_REALTIME,&start);
    for(int i=0;i<50;i++){
        int j=0;
        while(j<5){
            int x=i+j;
            send(cd,array[x],sizeof(char)*21,0);
            send(cd,&x,sizeof(int),0);
            j++;
        }
        int *k=malloc(sizeof(int));
        recv(cd,k,sizeof(int),0);
        printf("%d\n",*k);
        i=(*k);
    }
    close(cd);
    close(sd);
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
    printf("\n Total time taken ");
    printf("%f seconds\n",sec);
}