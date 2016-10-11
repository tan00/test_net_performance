#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
//#include "unp.h"

/*
size_t readn(int fd , void *buf , size_t nbytes);
size_t writen(int fd , const void *buf , size_t nbytes);
size_t readline(int fd , void *buf , size_t bytes);
*/

size_t readn(int fd , void *vptr , size_t n){
    size_t nleft = n;
    size_t nread = 0;
    char *ptr = vptr;

    while(nleft>0){
        if ( (nread = read(fd,ptr,nleft))<0){
            if (errno == EINTR)
                nread = 0;
            else
                return -1;            
        }
        else if (nread==0)
            break;    //EOF

        nleft -= nread;
        ptr += nread;
    }

    return (n-nleft);
}

size_t writen(int fd , void *vptr , size_t n){
	char *ptr = vptr;
	size_t nleft = n;
	size_t nwrite = 0 ;
	while(nleft>0){
		if(( nwrite = write(fd,ptr,nleft))<0){
			  if (errno == EINTR)
                		nwrite = 0;
            		else
              		 	 return -1;
		}
		else if (nwrite == 0)
			break;		
		nleft -= nwrite;
		ptr += nwrite;
	}
	return (n-nleft);
}

int socket_init( int port){
	

}

void freeBuf(void *ptr)
{
    if(ptr!=NULL)
        free(ptr) , ptr = NULL;        
}



unsigned long GetTickCount()  
{  
	struct timespec ts;  

	clock_gettime(CLOCK_MONOTONIC, &ts);  

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  
}