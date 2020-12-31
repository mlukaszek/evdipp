#include <cstddef>
#include <cstdint>
#include <cstdarg>
#include <limits>
#include <vector>
#include <iostream>
#include "evdi.hpp"

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

void Evdi::enable_cursor_events() const
{
    evdi_enable_cursor_events(handle);
}

void Evdi::connect(const unsigned char* edid,
    const unsigned edid_length) const
{
    evdi_connect(handle, edid, edid_length,
        std::numeric_limits<uint32_t>::max());
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

void Evdi::log(const std::string& message)
{
    std::cout << "evdipp: " << message << std::endl;
}

void Evdi::dispatch_log(void* user_data, const char* fmt, ...)
{
    constexpr size_t MAX_LEN = 255;
    static std::vector<char> buffer(MAX_LEN, '\0');

    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer.data(), MAX_LEN, fmt, args);
    va_end(args);

    static_cast<Evdi*>(user_data)->log(std::string(buffer.data()));
}
