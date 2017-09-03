#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "file.h"

File::File(const std::string &file)
    : file_(file)
{

}

File::~File()
{
}

size_t File::read(std::string &buf)
{
    if (!fs_in_.is_open())
    {
        fs_in_.clear();
        fs_in_.open(file_.c_str(), std::ios::in | std::ios::binary);
    }

    fs_in_.seekg(0, fs_in_.end);
    int length = fs_in_.tellg();
    fs_in_.seekg(0, fs_in_.beg);

    try
    {
        char *p = new char[length];
        fs_in_.read(p, length);
        buf.resize(length);
        memcpy((void *)buf.data(), p, length);
        delete[] p;
    }
    catch (std::exception &e)
    {
        std::cout << "read error: " << e.what() << std::endl;
        length = 0;
    }

    fs_in_.close();
    return length;
}

size_t File::write(const std::string &buf)
{
    if (!fs_out_.is_open())
    {
        fs_out_.clear();
        fs_out_.open(file_.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
    }

    fs_out_.write(buf.c_str(), buf.size());
    fs_out_.close();
    return 0;
}