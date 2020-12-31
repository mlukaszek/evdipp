#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <map>
#include <memory>
#include "evdi.hpp"
#include "buffer.hpp"

class Screen {
public:
    Screen(const Evdi& evdi, std::vector<unsigned char>& edid, bool separateCursorEvents = false);
    ~Screen();
    void update();

    int event_source() const;
    void handle_events();
    void enable_cursor_events() const;

protected:
    virtual void on_mode_change(evdi_mode mode);
    virtual int on_update_ready(int buffer_to_be_updated);
    virtual void on_dpms_notification(int dpms_mode);
    virtual void on_crtc_state_change(int state);
    virtual void on_cursor_set(evdi_cursor_set cursor_set);
    virtual void on_cursor_move(evdi_cursor_move cursor_move);

    const size_t BUFFER_COUNT = 2;
    std::map<int, std::unique_ptr<Buffer>> buffers;
    std::vector<evdi_rect> rects;

private:
    static void mode_change_handler(evdi_mode mode, void* user_data);
    static void dpms_handler(int dpms_mode, void* user_data);
    static void update_ready_handler(int buffer_to_be_updated, void* user_data);
    static void crtc_state_handler(int state, void* user_data);
    static void cursor_set_handler(evdi_cursor_set cursor_set, void* user_data);
    static void cursor_move_handler(evdi_cursor_move cursor_move, void* user_data);

    bool buffersRegistered;
    size_t bufferToUpdate;

    const Evdi& evdi;
    evdi_event_context context;
};

typedef std::shared_ptr<Screen> ScreenPtr;

#endif
