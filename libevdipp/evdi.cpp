#include <cstddef>
#include <cstdint>
#include <cstdarg>
#include <limits>
#include <vector>
#include <iostream>
#include "evdi.hpp"

Evdi::LogHandler Evdi::log_handler = [](const std::string&) {};

unsigned char Evdi::sample_edid[] = {
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x31, 0xd8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x16, 0x01, 0x03, 0x6d, 0x1b, 0x14, 0x78,
    0xea, 0x5e, 0xc0, 0xa4, 0x59, 0x4a, 0x98, 0x25, 0x20, 0x50, 0x54, 0x01,
    0x00, 0x00, 0x45, 0x40, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xa0, 0x0f, 0x20, 0x00, 0x31, 0x58,
    0x1c, 0x20, 0x28, 0x80, 0x14, 0x00, 0x15, 0xd0, 0x10, 0x00, 0x00, 0x1e,
    0x00, 0x00, 0x00, 0xff, 0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x23,
    0x30, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x3b,
    0x3d, 0x24, 0x26, 0x05, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x00, 0x00, 0x00, 0xfc, 0x00, 0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x53,
    0x56, 0x47, 0x41, 0x0a, 0x20, 0x20, 0x00, 0xc2
};

Evdi::Evdi()
{
    evdi_set_logging({ .function = &Evdi::dispatch_log, .user_data = this });
    handle = evdi_open_attached_to(NULL);
}

Evdi::Evdi(int devnum)
{
    evdi_set_logging({ .function = &Evdi::dispatch_log, .user_data = this });
    handle = evdi_open(devnum);
}

Evdi::Evdi(const char* parent)
{
    evdi_set_logging({ .function = &Evdi::dispatch_log, .user_data = this });
    handle = evdi_open_attached_to(parent);
}

Evdi::~Evdi() { evdi_close(handle); }

Evdi::operator bool() const { return handle != EVDI_INVALID_HANDLE; }

int Evdi::get_event_source() const { return evdi_get_event_ready(handle); }

void Evdi::handle_events(evdi_event_context* context) const
{
    evdi_handle_events(handle, context);
}

void Evdi::enable_cursor_events(bool enable) const
{
    evdi_enable_cursor_events(handle, enable);
}

void Evdi::connect(const unsigned char* edid,
    const unsigned edid_length) const
{
    if (edid) {
        evdi_connect(handle, edid, edid_length,
            std::numeric_limits<uint32_t>::max());
    } else {
        evdi_connect(handle, sample_edid, 128,
            std::numeric_limits<uint32_t>::max());
    }
}

void Evdi::disconnect() const { evdi_disconnect(handle); }

void Evdi::register_buffer(evdi_buffer buffer) const
{
    evdi_register_buffer(handle, buffer);
}

void Evdi::unregister_buffer(int buffer_id) const
{
    evdi_unregister_buffer(handle, buffer_id);
}

bool Evdi::request_update(int buffer_id) const
{
    return evdi_request_update(handle, buffer_id);
}

void Evdi::grab_pixels(evdi_rect* rects, int* num_rects) const
{
    evdi_grab_pixels(handle, rects, num_rects);
}

void Evdi::dispatch_log(void* user_data, const char* fmt, ...)
{
    constexpr size_t MAX_LEN = 255;
    static std::vector<char> buffer(MAX_LEN, '\0');

    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer.data(), MAX_LEN, fmt, args);
    va_end(args);

    log_handler(std::string(buffer.data()));
}
