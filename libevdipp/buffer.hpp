#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <evdi_lib.h>
#include <vector>

class Buffer {
public:
    Buffer(int id, int width, int height, int stride);
    ~Buffer();
    evdi_buffer get() const;

private:
    std::vector<char> data;
    evdi_buffer buffer;
};

#endif