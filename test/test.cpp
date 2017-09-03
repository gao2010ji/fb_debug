#include <iostream>
#include "file.h"
#include "fb_json.h"
#include "define_type_generated.h"

using namespace test::idl;

void test_json()
{
    std::string schema_file = "define_type.fbs";
    std::string include_path = "./";
    std::string json_file = "data.json";

    // {
    //     FB_JSON fbjson;
    //     fbjson.init(schema_file, include_path);
    //     std::string fb_data = fbjson.parse_json(json_file);
    //     printf("fb_data size = %lu\n", fb_data.size());

    //     File file("raw_data");
    //     file.write(fb_data);
    // }

    {
        flatbuffers::FlatBufferBuilder fbb;
        auto name = fbb.CreateString("hello");
        auto f_test_request = CreateTestRequest(fbb, name, 21);
        auto f_union_request = CreateUnionRequest(fbb, Command_TestRequest, f_test_request.Union());
        fbb.Finish(f_union_request);

        std::string request((char *)fbb.GetBufferPointer(), fbb.GetSize());
        File file("raw_data");
        file.write(request);
    }

    {
        File file("raw_data");
        std::string data;
        size_t len = file.read(data);
        printf("read data len = %lu\n", len);

        FB_JSON fbjson;
        fbjson.init(schema_file, include_path);
        std::string json = fbjson.dump_json((const uint8_t *)data.c_str());
        printf("%s\n", json.c_str());
    }
}

int main(int argc, char *argv[])
{
    test_json();
    return 0;
}

