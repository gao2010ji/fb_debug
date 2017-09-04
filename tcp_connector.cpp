#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include "tcp_connector.h"

TcpConnector::TcpConnector()
    : sockfd_(0)
    , init_(false)
{

}

TcpConnector::~TcpConnector()
{
    close(sockfd_);
}

int TcpConnector::init(const std::string &ip, int port)
{
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ == -1)
    {
        printf("init socket error\n");
        return -1;
    }

    struct sockaddr_in remote_addr;
    bzero(&remote_addr, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &remote_addr.sin_addr);

    int ret = connect(sockfd_, (struct sockaddr *)&remote_addr, sizeof(remote_addr));
    if (ret != 0)
    {
        printf("can't connect remote server\n");
        return -1;
    }

    init_ = true;
    return 0;
}

int TcpConnector::send_and_recv(const std::string &sendbuf, std::string &recvbuf)
{
    if(!init_)
    {
        printf("TcpConnector is not init\n");
        return -1;
    }

    int len = 0;
    len = send(sockfd_, sendbuf.c_str(), sendbuf.size(), 0);
    printf("send data len = %d\n", len);
    if(len == -1 || (uint32_t)len != sendbuf.size())
    {
        printf("send data error = %d, errno = %s, socket = %d\n", len, strerror(errno), sockfd_);
        return -1;
    }

    char data[1024 * 10];
    int buf_len = sizeof(data);
    memset(data, 0, sizeof(data));
    int count = 0;
    while(true)
    {
        len = recv(sockfd_, &data[count], buf_len - count, 0);
        if(len == -1)
        {
            printf("recv error\n");
            break;
        }
        else if(len == 0 || count >= buf_len)
        {
            printf("recv data len = %d\n", count);
            break;
        }
        else
        {
            // null
        }

        count += len;
    }

    try
    {
        recvbuf.resize(count);
        memcpy((char *)recvbuf.data(), data, count);
    }
    catch (const std::exception &e)
    {
        printf("catch exception: %s\n", e.what());
        return -1;
    }

    return 0;
}
