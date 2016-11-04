#include <iostream>
#include "buffer.hpp"
#include "debug.hpp"

Buffer::Buffer(int id, int width, int height, int stride)
{
    logging << "Creating buffer id:" << id << " size " << width << "x" << height
         << " stride " << stride << std::endl;
    buffer.id = id;
    buffer.width = width;
    buffer.height = height;
    buffer.stride = stride;
    data.resize(height * stride, 0);
    buffer.buffer = data.data();
}

Buffer::~Buffer()
{
    logging << "Destroying buffer with id:" << buffer.id << std::endl;
}

evdi_buffer Buffer::get() const
{
    return buffer;
}
