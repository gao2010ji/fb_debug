#ifndef _TCP_CONNECTOR_H_
#define _TCP_CONNECTOR_H_

#include <string>

class TcpConnector
{
public:
    TcpConnector();
    ~TcpConnector();

public:
    int init(const std::string &ip, int port);
    int send_and_recv(const std::string &sendbuf, std::string &recvbuf);

private:
    int sockfd_;
    bool init_;
};

#endif
