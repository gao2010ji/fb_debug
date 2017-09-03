#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include "define_type_generated.h"

using namespace test::idl;

int main()
{

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6667);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sockfd, 5);

    while (true)
    {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof(c_addr);
        bzero(&c_addr, c_len);
        int client_fd = accept(sockfd, (struct sockaddr *)&addr, &c_len);
        if (client_fd == -1)
        {
            printf("accept error\n");
            return -1;
        }

        char ip[40] = {0};
        int port;
        inet_ntop(AF_INET, &addr.sin_addr, ip, 40);
        port = ntohs(addr.sin_port);
        printf("from %s:%d\n", ip, port);

        char buf[1024 * 10];
        int ret = recv(client_fd, buf, sizeof(buf), 0);
        if(ret == -1)
        {
            break;
        }

        printf("recv len: %d\n", ret);
        flatbuffers::FlatBufferBuilder fbb;
        auto name = fbb.CreateString("hello client");
        auto f_test_request = CreateTestRequest(fbb, name, 21);
        auto f_union_request = CreateUnionRequest(fbb, Command_TestRequest, f_test_request.Union());
        fbb.Finish(f_union_request);
        ret = send(client_fd, fbb.GetBufferPointer(), fbb.GetSize(), 0);
        printf("send len: %d\n", ret);
        
        close(client_fd);
    }
}
