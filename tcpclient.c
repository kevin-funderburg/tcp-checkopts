#include "unp.h"

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while(fgets(sendline, MAXLINE, fp) != NULL)         //read line of text
    {
        Writen(sockfd, sendline, strlen(sendline));     //send line to server

        if (readline(sockfd, recvline, MAXLINE == 0)    //read line echoed from server
            err_quit("str_cli: server terminated prematurely");

        Fputs(recvline, stdout);    //write line from server to standard output
    }
}


int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2)
        err_quit("usage: tcpcli <IPaddress>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    str_cli(stdin, sockfd);     //do it all

    exit(0);
}
