#ifndef EVDI_HPP
#define EVDI_HPP

#include <string>
#include <functional>
#include <evdi_lib.h>

class Evdi {
public:
    using LogHandler = std::function<void(const std::string&)>;
    static LogHandler log_handler;

    Evdi();
    Evdi(int devnum);
    Evdi(const char* parent);
    ~Evdi();
    operator bool() const;

    void connect(const unsigned char* edid, const unsigned edid_length) const;
    void disconnect() const;

    void register_buffer(evdi_buffer buffer) const;
    void unregister_buffer(int buffer_id) const;

    bool request_update(int buffer_id) const;
    void grab_pixels(evdi_rect* rects, int* num_rects) const;

    int get_event_source() const;
    void handle_events(evdi_event_context* context) const;
    void enable_cursor_events() const;

    static void add() { evdi_add_device(); }

private:
    evdi_handle handle;
    static unsigned char sample_edid[];

    void log(const std::string& message);
    static void dispatch_log(void* user_data, const char* fmt, ...);
};

#endif