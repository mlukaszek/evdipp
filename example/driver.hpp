#include "libevdipp/screen.hpp"
#include <ev++.h>
#include <vector>

class Driver
{
public:
    Driver();
    void add_screen(ScreenPtr screen);
    void drive(unsigned fps);

private:
    void on_screen_event(ev::io &w, int revents);
    void on_enter_pressed(ev::io &w, int revents);
    void on_sigint(ev::sig &w, int revents);
    void on_sync(ev::periodic &w, int revents);
    void quit();

    ev::io stdin_watcher;
    ev::periodic sync_timer;

    std::vector<std::shared_ptr<ev::io>> screen_event_watchers;
    std::vector<ScreenPtr> screens;
};
