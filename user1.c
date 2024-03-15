// include all the headers
#include "msocket.h"

int main(int argc, char *argv[])
{

    int sock_mtpfd = m_socket(AF_INET, SOCK_MTP, 0);

    printf("Created %d id\n", sock_mtpfd);

    if (m_bind(sock_mtpfd, "loopback", atoi(argv[1]), "loopback", atoi(argv[2])) > 0)
    {
        printf("success\n");

    }
     
    char buff[1024];
    strcpy(buff,"Hello");
    if(m_sendto(sock_mtpfd,buff,sizeof(buff))>=0)
    {
        printf("have send\n");
    }

    sleep(3);











    
    m_close(sock_mtpfd);

    return 0;
}