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

#define SERV_PORT 9877
#define MAXLINE 4096

#define SA struct sockaddr  //shortens the typcasts of pointer arguments


int     Accept(int, SA *, socklen_t *);
void	Bind(int, const SA *, socklen_t);
void    Connect(int, const SA *, socklen_t);
void    err_sys(const char* msg);
int     Socket(int family, int type, int protocol);
void    Writen(int fd, void *ptr, size_t nbytes);


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


/* Fatal error related to system call
 * Print message and terminate */

void err_sys(const char *msg)
{
    perror(msg);
    exit(1);
}

#endif  //UNP_H
