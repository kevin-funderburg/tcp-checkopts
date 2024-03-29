#include	"unp.h"
#include	<netinet/tcp.h>		/* for TCP_xxx defines */

#define ZEUS "147.26.231.156"
#define EROS "147.26.231.153"
#define LOCALHOST "127.0.0.1"

struct optData {
    char* optStr;
    int   val,
          lvl,
          name;
};

struct optData clientData[28];

union val {
  int				i_val;
  long				l_val;
  struct linger		linger_val;
  struct timeval	timeval_val;
} val;

static char	*sock_str_flag(union val *, int);
static char	*sock_str_int(union val *, int);
static char	*sock_str_linger(union val *, int);
static char	*sock_str_timeval(union val *, int);
void getSetOpts(int sockfd);

struct sock_opts {
  const char	   *opt_str;
  int		opt_level;
  int		opt_name;
  char   *(*opt_val_str)(union val *, int);
} sock_opts[] = {
	{ "SO_BROADCAST",		SOL_SOCKET,	SO_BROADCAST,	sock_str_flag },
	{ "SO_DEBUG",			SOL_SOCKET,	SO_DEBUG,		sock_str_flag },
	{ "SO_DONTROUTE",		SOL_SOCKET,	SO_DONTROUTE,	sock_str_flag },
	{ "SO_ERROR",			SOL_SOCKET,	SO_ERROR,		sock_str_int },
	{ "SO_KEEPALIVE",		SOL_SOCKET,	SO_KEEPALIVE,	sock_str_flag },
	{ "SO_LINGER",			SOL_SOCKET,	SO_LINGER,		sock_str_linger },
	{ "SO_OOBINLINE",		SOL_SOCKET,	SO_OOBINLINE,	sock_str_flag },
	{ "SO_RCVBUF",			SOL_SOCKET,	SO_RCVBUF,		sock_str_int },
	{ "SO_SNDBUF",			SOL_SOCKET,	SO_SNDBUF,		sock_str_int },
	{ "SO_RCVLOWAT",		SOL_SOCKET,	SO_RCVLOWAT,	sock_str_int },
	{ "SO_SNDLOWAT",		SOL_SOCKET,	SO_SNDLOWAT,	sock_str_int },
	{ "SO_RCVTIMEO",		SOL_SOCKET,	SO_RCVTIMEO,	sock_str_timeval },
	{ "SO_SNDTIMEO",		SOL_SOCKET,	SO_SNDTIMEO,	sock_str_timeval },
	{ "SO_REUSEADDR",		SOL_SOCKET,	SO_REUSEADDR,	sock_str_flag },
#ifdef	SO_REUSEPORT
	{ "SO_REUSEPORT",		SOL_SOCKET,	SO_REUSEPORT,	sock_str_flag },
#else
	{ "SO_REUSEPORT",		0,			0,				NULL },
#endif
	{ "SO_TYPE",			SOL_SOCKET,	SO_TYPE,		sock_str_int },
#ifdef	SO_USELOOPBACK
	{ "SO_USELOOPBACK",		SOL_SOCKET,	SO_USELOOPBACK,	sock_str_flag },
#else
	{ "IPV6_DONTFRAG",		0,			0,				NULL },
#endif
	{ "IP_TOS",				IPPROTO_IP,	IP_TOS,			sock_str_int },
	{ "IP_TTL",				IPPROTO_IP,	IP_TTL,			sock_str_int },
#ifdef	IPV6_DONTFRAG
	{ "IPV6_DONTFRAG",		IPPROTO_IPV6,IPV6_DONTFRAG,	sock_str_flag },
#else
	{ "IPV6_DONTFRAG",		0,			0,				NULL },
#endif
#ifdef	IPV6_UNICAST_HOPS
	{ "IPV6_UNICAST_HOPS",	IPPROTO_IPV6,IPV6_UNICAST_HOPS,sock_str_int },
#else
	{ "IPV6_UNICAST_HOPS",	0,			0,				NULL },
#endif
#ifdef	IPV6_V6ONLY
	{ "IPV6_V6ONLY",		IPPROTO_IPV6,IPV6_V6ONLY,	sock_str_flag },
#else
	{ "IPV6_V6ONLY",		0,			0,				NULL },
#endif
	{ "TCP_MAXSEG",			IPPROTO_TCP,TCP_MAXSEG,		sock_str_int },
	{ "TCP_NODELAY",		IPPROTO_TCP,TCP_NODELAY,	sock_str_flag },
#ifdef	SCTP_AUTOCLOSE
	{ "SCTP_AUTOCLOSE",		IPPROTO_SCTP,SCTP_AUTOCLOSE,sock_str_int },
#else
	{ "SCTP_AUTOCLOSE",		0,			0,				NULL },
#endif
#ifdef	SCTP_MAXBURST
	{ "SCTP_MAXBURST",		IPPROTO_SCTP,SCTP_MAXBURST,	sock_str_int },
#else
	{ "SCTP_MAXBURST",		0,			0,				NULL },
#endif
#ifdef	SCTP_MAXSEG
	{ "SCTP_MAXSEG",		IPPROTO_SCTP,SCTP_MAXSEG,	sock_str_int },
#else
	{ "SCTP_MAXSEG",		0,			0,				NULL },
#endif
#ifdef	SCTP_NODELAY
	{ "SCTP_NODELAY",		IPPROTO_SCTP,SCTP_NODELAY,	sock_str_flag },
#else
	{ "SCTP_NODELAY",		0,			0,				NULL },
#endif
	{ NULL,					0,			0,				NULL }
};

/* include checkopts3 */
static char	strres[128];

static char	*
sock_str_flag(union val *ptr, int len)
{
/* *INDENT-OFF* */
	if (len != sizeof(int))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	else
		snprintf(strres, sizeof(strres),
				 "%s", (ptr->i_val == 0) ? "off" : "on");
	return(strres);
/* *INDENT-ON* */
}
/* end checkopts3 */

static char	*
sock_str_int(union val *ptr, int len)
{
	if (len != sizeof(int))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	else
		snprintf(strres, sizeof(strres), "%d", ptr->i_val);
	return(strres);
}

static char	*
sock_str_linger(union val *ptr, int len)
{
	struct linger	*lptr = &ptr->linger_val;

	if (len != sizeof(struct linger))
		snprintf(strres, sizeof(strres),
				 "size (%d) not sizeof(struct linger)", len);
	else
		snprintf(strres, sizeof(strres), "l_onoff = %d, l_linger = %d",
				 lptr->l_onoff, lptr->l_linger);
	return(strres);
}

static char	*
sock_str_timeval(union val *ptr, int len)
{
	struct timeval	*tvptr = &ptr->timeval_val;

	if (len != sizeof(struct timeval))
		snprintf(strres, sizeof(strres),
				 "size (%d) not sizeof(struct timeval)", len);
	else
		snprintf(strres, sizeof(strres), "%d sec, %d usec",
				 tvptr->tv_sec, tvptr->tv_usec);
	return(strres);
}


void getSetOpts(int sockfd)
{
    printf("...getSetOpts()...\n");
    ssize_t n;
    char* clientMsg;
    char    buf[MAXLINE];
    socklen_t len;
    int fd;
    char *pt;
    int x = 1;
    char* optStr;
    int cliVal, newVal, cliLvl, optName;
//    char line[] = "SO_BROADCAST,0,1,6";
    printf("buf:\n");
    again:

        while ( (n = read(sockfd, buf, MAXLINE)) > 0)
        {
            //printf("%s", buf);
            char* line = strtok(buf, "\n"); //get a line from the buffer
           
            while(line)
            {
                printf("[%d] RECEIVED FROM CLIENT: %s\n", x, line);
                //strcpy(clientMsg, line);

                int i = 0;
                pt = strtok(line, ",");
                while (pt != NULL)  //parse lines by comma delimiter
                {
                    //printf("%s\n", pt);
                    switch(i) {
                        case 0: optStr = pt; break;
                        case 1: cliVal = atoi(pt); break;
                        case 2: cliLvl = atoi(pt); break;
                        case 3: optName = atoi(pt); break;
                    }
                    pt = strtok(NULL, ",");
                    i++;
                }
                //printf("optStr: %s\ncliVal: %d\ncliLvel: %d\noptName: %d\n", optStr, cliVal, cliLvl, optName);

                //set new value for server option
                switch (cliVal) {
                    case 0: newVal = 1; break;
                    case 1: newVal = 0; break;
                    default: newVal = cliVal * 3;
                }
                
                //set socket option with new value
                len = sizeof(newVal);
                if (setsockopt(sockfd, cliLvl, optName, &newVal, len) == -1) {
                    err_ret("setsockopt error");
                } else {
                    printf("sockopt %s set to %d\n", optStr, newVal);
                }
                printf("\n");

                line = strtok(NULL, "\n");
                x++;
            }
            //printf("END OF BUF\n");
            bzero(buf, sizeof(buf));    //clear buffer so lines aren't truncated
        }

        if (n < 0 && errno == EINTR)
            goto again;
        else if (n < 0) {
            err_sys("reply: read error");
        }
}

void printAll(int sockfd)
{
    printf("...print all socket options...\n");
            
    int fd;
    socklen_t len;
    struct sock_opts *ptr;

    for (ptr = sock_opts; ptr->opt_str != NULL; ptr++) {
        printf("%s: ", ptr->opt_str);
        if (ptr->opt_val_str == NULL)
            printf("(undefined)\n");
        else {
            switch(ptr->opt_level) {
            case SOL_SOCKET:
            case IPPROTO_IP:
            case IPPROTO_TCP:
                fd = Socket(AF_INET, SOCK_STREAM, 0);
                break;
#ifdef  IPV6
            case IPPROTO_IPV6:
                fd = Socket(AF_INET6, SOCK_STREAM, 0);
                break;
#endif
#ifdef  IPPROTO_SCTP
            case IPPROTO_SCTP:
                fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
                break;
#endif
            default:
                err_quit("Can't create fd for level %d\n", ptr->opt_level);
            }
                                                        
            len = sizeof(val);
            if (getsockopt(sockfd, ptr->opt_level, ptr->opt_name,
                           &val, &len) == -1) {
                err_ret("getsockopt error");
            } else {
               printf("default = %s\n", (*ptr->opt_val_str)(&val, len));
            }
            close(fd);
        }
    }
}

int
main(int argc, char **argv)
{
    printf("...starting server...\n");

 	int					listenfd, connfd;
 	pid_t				childpid;
 	socklen_t			clilen;
 	struct sockaddr_in	cliaddr, servaddr;
 
 	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
 
 	bzero(&servaddr, sizeof(servaddr));
 	servaddr.sin_family      = AF_INET;
 	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 	servaddr.sin_port        = htons(SERV_PORT);
 
 	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
 
 	Listen(listenfd, LISTENQ);
 
 	for ( ; ; ) {
 		clilen = sizeof(cliaddr);
 		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
 
        getSetOpts(connfd); //get client options and set servers accordingly
        printAll(connfd);   //print server options
 		Close(connfd);			/* parent closes connected socket */
        exit(0);
 	}
}

