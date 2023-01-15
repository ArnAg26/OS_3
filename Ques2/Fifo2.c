#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc,char **argv){
    
    for(int i=0;i<10;i++){
        int fd=open("testfifo",O_RDONLY);
        if(fd==-1){
            perror("Error opening");
        }
        int d;
        for(int j=0;j<5;j++){
            char *c=malloc(sizeof(char)*21);
            int e;
            if(read(fd,c,sizeof(char) *21)==-1){
                perror("Some error reading");
            }

            if(read(fd,&e,sizeof(int))==-1){
                perror("Some error reading");
            }
            printf("%s %d\n",c,e);
            if(j==4){
                d=e;
            }
        }
        close(fd);
        fd=open("testfifo",O_WRONLY);
        if(fd==-1){
            perror("Some error opening");
        }
        if(write(fd,&d,sizeof(int))==-1){
                perror("Some error writing");
        }
        close(fd);


        
    }
    
}