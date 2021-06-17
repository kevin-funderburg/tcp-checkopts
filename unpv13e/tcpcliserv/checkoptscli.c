#include	"unp.h"
#include	<netinet/tcp.h>		/* for TCP_xxx defines */

#define ZEUS "147.26.231.156"
#define EROS "147.26.231.153"
#define LOCALHOST "127.0.0.1"

union val {
  int				i_val;
  long				l_val;
  struct linger		linger_val;
  struct timeval	timeval_val;
} val;

static char *sock_str_flag(union val *, int);
static char *sock_str_int(union val *, int);
static char *sock_str_linger(union val *, int);
static char *sock_str_timeval(union val *, int);

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
#ifdef  SO_USELOOPBACK
	{ "SO_USELOOPBACK",		SOL_SOCKET,	SO_USELOOPBACK,	sock_str_flag },
#else
	{ "SO_USELOOPBACK",	0,			0,				NULL },
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

// int
// main(int argc, char **argv)
// {
// 	int					sockfd;
// 	struct sockaddr_in	servaddr;
// 
// 	if (argc != 2)
// 		err_quit("usage: tcpcli <IPaddress>");
// 
// 	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
// 
// 	bzero(&servaddr, sizeof(servaddr));
// 	servaddr.sin_family = AF_INET;
// 	servaddr.sin_port = htons(SERV_PORT);
// 	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
// 
// 	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
// 
// 	str_cli(stdin, sockfd);		/* do it all */
// 
// 	exit(0);
// }


int
main(int argc, char **argv)
{
    printf("...starting client...\n");
    
	int					fd;
	socklen_t			len;
	struct sock_opts	*ptr;
 	int					sockfd;
 	struct sockaddr_in	servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];
    int                 sockopt;

 	if (argc != 2)
 		err_quit("usage: tcpcli <IPaddress>");
 
 	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

 	bzero(&servaddr, sizeof(servaddr));
 	servaddr.sin_family = AF_INET;
 	servaddr.sin_port = htons(SERV_PORT);
 	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
 
 	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    //strcpy(sendline, "testing a send line\n");

	for (ptr = sock_opts; ptr->opt_str != NULL; ptr++) {
        //bzero(sendline, sizeof(sendline));
//		printf("[CLIENT] %s: ", ptr->opt_str);
		if (ptr->opt_val_str == NULL)
			printf("(undefined)\n");
		else {
			switch(ptr->opt_level) {
			case SOL_SOCKET:
			case IPPROTO_IP:
			case IPPROTO_TCP:
				fd = Socket(AF_INET, SOCK_STREAM, 0);
				break;
#ifdef	IPV6
			case IPPROTO_IPV6:
				fd = Socket(AF_INET6, SOCK_STREAM, 0);
				break;
#endif
#ifdef	IPPROTO_SCTP
			case IPPROTO_SCTP:
				fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
				break;
#endif
			default:
				err_quit("Can't create fd for level %d\n", ptr->opt_level);
			}

			len = sizeof(val);
            //sockopt = getsockopt(fd, ptr->opt_level, ptr->opt_name,
			//			   &val, &len);

			if (getsockopt(fd, ptr->opt_level, ptr->opt_name,
						   &val, &len) == -1) {
				err_ret("getsockopt error");
			} else {
				printf("default = %s\n", (*ptr->opt_val_str)(&val, len));
			}
			close(fd);
		}

        //convert value of ptr to a string
        int length = snprintf( NULL, 0, ",%d", val);
        char* val_str = malloc(length + 1);
        snprintf(val_str, length + 1, ",%d", val);

        length = snprintf( NULL, 0, ",%d", ptr->opt_level);
        char* len_str = malloc(length + 1);
        snprintf(len_str, length + 1, ",%d", ptr->opt_level);
 
        length = snprintf( NULL, 0, ",%d", ptr->opt_name);
        char* name_str = malloc(length + 1);
        snprintf(name_str, length + 1, ",%d", ptr->opt_name);
       
        //concat ptr name and ptr value and separte with a comma
        strcpy(sendline, ptr->opt_str);
        strcat(sendline, val_str);
        strcat(sendline, len_str);
        strcat(sendline, name_str);
        strcat(sendline, "\n");
        printf("sendline: %s", sendline);
        Writen(sockfd, sendline, strlen(sendline)); //send line to server
        free(val_str);
	}
	exit(0);
}
