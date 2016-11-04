#ifndef EVDI_HPP
#define EVDI_HPP

#include <evdi_lib.h>

class Evdi {
public:
    Evdi();
    Evdi(int devnum);
    ~Evdi();
    operator bool() const;

    void connect(const unsigned char* edid, const unsigned edid_length) const;
    void disconnect() const;

    void register_buffer(evdi_buffer buffer) const;
    void unregister_buffer(int buffer_id) const;

    bool request_update(int buffer_id) const;
    void grab_pixels(evdi_rect *rects, int *num_rects) const;

    int get_event_source() const;
    void handle_events(evdi_event_context *context) const;

    static void add() { evdi_add_device(); }

private:
    evdi_handle handle;
};

#endif