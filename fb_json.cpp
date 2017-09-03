#include "string"
#include "fb_json.h"

FB_JSON::FB_JSON()
{

}

FB_JSON::~FB_JSON()
{

}

bool FB_JSON::init(const std::string &schema_file, const std::string &include_path)
{
    std::string load_buffer;
    bool ok = flatbuffers::LoadFile(schema_file.c_str(), true, &load_buffer);
    if(!ok)
    {
        printf("load file error\n");
        return false;
    }

    const char *include_directories[] = { include_path.c_str(), nullptr };
    ok = parser_.Parse(load_buffer.c_str(), include_directories);
    if(!ok)
    {
        printf("parse file error: %s\n", parser_.error_.c_str());
        return false;
    }

    init_ = true;
    return true;
}

std::string FB_JSON::parse_json(const std::string &json_file)
{
    if(!init_)
    {
        printf("not init");
        return nullptr;
    }

    std::string load_buffer;
    bool ok = flatbuffers::LoadFile(json_file.c_str(), true, &load_buffer);
    if(!ok)
    {
        printf("load file error\n");
        return nullptr;
    }

    //const char *include_directories[] = { include_path.c_str(), nullptr };
    ok = parser_.Parse(load_buffer.c_str(), nullptr);
    if(!ok)
    {
        printf("parse file error: %s\n", parser_.error_.c_str());
        return nullptr;
    }
    return std::string((char *)parser_.builder_.GetBufferPointer(), parser_.builder_.GetSize());
}

std::string FB_JSON::dump_json(const uint8_t *fb_data)
{
    if(!init_)
    {
        printf("not init");
        return "";
    }

    std::string jsongen;
    if (!GenerateText(parser_, fb_data, &jsongen))
    {
      printf("Couldn't serialize parsed data to JSON!\n");
    }

    return jsongen;
}