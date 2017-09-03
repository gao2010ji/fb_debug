#ifndef _FB_JSON_H
#define _FB_JSON_H

#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>
#include <flatbuffers/flatbuffers.h>

class FB_JSON
{
public:
    FB_JSON();
    ~FB_JSON();

public:
    bool init(const std::string &schema_file, const std::string &include_path);
    std::string parse_json(const std::string &json_file);
    std::string dump_json(const uint8_t *fb_data);

private:
    flatbuffers::Parser parser_;
    bool init_ = false;
};

#endif