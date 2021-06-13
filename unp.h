#define SERV_PORT 9877
#define MAXLINE 4096

#define SA struct sockaddr  //shortens the typcasts of pointer arguments


int     Accept(int, SA *, socklen_t *);


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
