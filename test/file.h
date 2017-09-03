#ifndef _FILE_H_
#define _FILE_H_

#include <string>
#include <fstream>

class File
{
public:
    explicit File(const std::string &file);
    ~File();

public:
    size_t read(std::string &buf);
    size_t write(const std::string &buf);

private:
    std::string file_;
    std::fstream fs_in_;
    std::fstream fs_out_;
};

#endif