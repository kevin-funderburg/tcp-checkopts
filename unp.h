#ifndef UNP_H
#define UNP_H

#include    <sys/types.h>   /* basic system data types */
#include    <sys/socket.h>  /* basic socket definitions */
#if TIME_WITH_SYS_TIME
#include    <sys/time.h>    /* timeval{} for select() */
#include    <time.h>        /* timespec{} for pselect() */
#else
#if HAVE_SYS_TIME_H
#include    <sys/time.h>    /* includes <time.h> unsafely */
#else
#include    <time.h>        /* old system? */
#endif
#endif
#include    <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include    <arpa/inet.h>   /* inet(3) functions */
#include    <errno.h>
#include    <fcntl.h>       /* for nonblocking */
#include    <netdb.h>
#include    <signal.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/stat.h>    /* for S_xxx file mode constants */
#include    <sys/uio.h>     /* for iovec{} and readv/writev */
#include    <unistd.h>
#include    <sys/wait.h>
#include    <sys/un.h>      /* for Unix domain sockets */
#include    <sys/select.h>
#include    <stdbool.h>

#define LISTENQ 5
#define SERV_PORT 9877
#define MAXLINE 4096

#define SA struct sockaddr  //shortens the typcasts of pointer arguments


int             Accept(int, SA *, socklen_t *);
void	        Bind(int, const SA *, socklen_t);
void            Connect(int, const SA *, socklen_t);
void            err_sys(const char* msg);
void            err_quit(const char* msg);
void            Shutdown(int fd, int how);
int             Socket(int family, int type, int protocol);
void            Writen(int fd, void *ptr, size_t nbytes);
ssize_t         writen(int fd, const void *vptr, size_t n);
const char*     Inet_ntop(int family, const void *addrptr, char *strptr, size_t len);
static          ssize_t my_read(int fd, char *ptr);
ssize_t         readline(int fd, void *vptr, size_t maxlen);
ssize_t         readlinebuf(void **vptrptr);

static int read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int n;

    again:
        if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef EPROTO
            if (errno == EPROTO || errno == ECONNABORTED)
#else
            if (errno == ECONNABORTED)
#endif
                goto again;
            else
                err_sys("accept error");
        }
        return(n);
}


void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind error");
}


int Socket(int family, int type, int protocol)
{
    int n;

    if ( (n = socket(family, type, protocol)) < 0)
        err_sys("socket error");
    return(n);
}


void Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        err_sys("writen error");
}


ssize_t writen(int fd, const void *vptr, size_t n)		/* Write "n" bytes to a descriptor. */
{
    size_t		nleft;
    ssize_t		nwritten;
    const char	*ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;		/* and call write() again */
            else
                return(-1);			/* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/* end writen */


static ssize_t my_read(int fd, char *ptr)
{
    if (read_cnt <= 0) {
        again:
            if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
                if (errno == EINTR)
                    goto again;
                return (-1);
            } else if (read_cnt == 0)
                return (0);
            read_ptr = read_buf;
    }
    
    read_cnt--;
    *ptr = *read_ptr++;
    return (1);
}


ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc; 
    char    c, *ptr;

    ptr=vptr;

    for (n = 1; n < maxlen; n++) {
        if ( (rc = my_read(fd, &c)) == 1) { 
            *ptr++ = c;
            if (c == '\n')
                break;          //newline is stored, like fgets()
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);     //EOF, n - 1 bytes were read
        } else
            return (-1);        //error, errno set by read()
    }
    
    *ptr = 0;
    return(n);
}


ssize_t readlinebuf(void **vptrptr)
{
    if(read_cnt) 
        *vptrptr = read_ptr;
    return (read_cnt);
}


const char * Inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
    const char	*ptr;

    if (strptr == NULL)		/* check for old code */
        err_quit("NULL 3rd argument to inet_ntop");
    if ( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
        err_sys("inet_ntop error");		/* sets errno */
    return(ptr);
}


/* Fatal error related to system call
 * Print message and terminate */

void err_sys(const char *msg)
{
    perror(msg);
    exit(1);
}


void err_quit(const char *msg)
{
    perror(msg);
    exit(1);
}


void Shutdown(int fd, int how)
{
    if (shutdown(fd, how) < 0)
        err_sys("shutdown error");
}

#endif  //UNP_H
