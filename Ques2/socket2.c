#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/arn"

int main(int argc,char **argv){
    struct sockaddr_un addr;
    int sf,cc;
    ssize_t numRead;
    sf=socket(AF_UNIX,SOCK_STREAM,0);
    memset(&addr,0,sizeof(struct sockaddr_un));
    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path,SV_SOCK_PATH,sizeof(addr.sun_path)-1);
    if(connect(sf,(struct sockaddr *)&addr,sizeof(struct sockaddr_un))==-1){
        perror("Connection error");
    }
    char buffer[21];
    int n;
    for(int i=0;i<50;i++){
        int j=0;
        while(j<5){
            recv(sf,buffer,sizeof(char)*21,0);
            recv(sf,&n,sizeof(int),0);
            printf("%s %d\n",buffer,n);
            j++;
        }
        int d=n;
        printf("%d\n",d);
        send(sf,&d,sizeof(int),0);
        i=d;
        
    }
    

    close(sf);


}