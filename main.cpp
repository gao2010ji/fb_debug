#include "file.h"
#include "fb_json.h"
#include "tcp_connector.h"

using namespace phxrpc;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("usage: config full path, json full path\n");
        return 0;
    }

    Config cfg;
    if(!cfg.InitConfig(argv[1]))
    {
        printf("load config file failed\n");
        return -1;
    }

    std::string json_file(argv[2]);
    char value[512];
    cfg.ReadItem("FB_JSON", "schema_file", value, sizeof(value), "error");
    std::string shcema_file(value);
    cfg.ReadItem("FB_JSON", "inclde_path", value, sizeof(value), "error");
    std::string include_path(value);
    // cfg.ReadItem("FB_JSON", "json_file", value, sizeof(value), "error");
    // std::string json_file(value);
    cfg.ReadItem("REMOTE_SERVER", "ip", value, sizeof(value), "error");
    std::string ip(value);
    int port = 0;
    cfg.ReadItem("REMOTE_SERVER", "port", &port, 0);
    if (shcema_file.empty() || shcema_file == "error" ||
        include_path.empty() || include_path == "error" ||
        json_file.empty() || json_file == "error" ||
        ip.empty() || ip == "error" ||
        port == 0)
    {
        printf("config value error\n");
        return -1;
    }
    printf(
        "schema file: %s\n"
        "include path: %s\n"
        "remote ip: %s\n"
        "remote port %d\n",
        shcema_file.c_str(),
        include_path.c_str(),
        ip.c_str(),
        port);
    
    FB_JSON fb_json;
    if(!fb_json.init(shcema_file, include_path))
    {
        printf("load schema file error\n");
        return -1;
    }
    std::string parse_json = fb_json.parse_json(json_file);
    if(parse_json.size() == 0)
    {
        printf("parse json file failed\n");
        return -1;
    }

    // send to server
    TcpConnector connector;
    if(connector.init(ip, port) != 0)
    {
        printf("init connector error\n");
        return -1;
    }
    std::string fb_data;
    connector.send_and_recv(parse_json, fb_data);
    if(fb_data.size() == 0)
    {
        printf("nothing recv from server\n");
        return -1;
    }
    std::string dump_json = fb_json.dump_json((uint8_t *)fb_data.c_str());
    printf("\n%s\n", dump_json.c_str());

    return 0;
}
