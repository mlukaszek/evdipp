#include <cstddef>
#include "evdi.hpp"

Evdi::Evdi()
: handle(EVDI_INVALID_HANDLE)
{}

Evdi::Evdi(int devnum)
: handle(evdi_open(devnum))
{
    
}

Evdi::~Evdi()
{
    evdi_close(handle);
}

Evdi::operator bool() const
{
    return handle != EVDI_INVALID_HANDLE;
}

int Evdi::get_event_source() const
{
    return evdi_get_event_ready(handle);
}

void Evdi::handle_events(evdi_event_context *context) const
{
    evdi_handle_events(handle, context);
}

void Evdi::connect(const unsigned char* edid, const unsigned edid_length) const
{
    evdi_connect(handle, edid, edid_length);
}

void Evdi::disconnect() const
{
    evdi_disconnect(handle);
}

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

void Evdi::grab_pixels(evdi_rect *rects, int *num_rects) const
{
    evdi_grab_pixels(handle, rects, num_rects);
}
