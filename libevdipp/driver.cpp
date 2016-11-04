#include "driver.hpp"
#include <iostream>
#include <unistd.h>

Driver::Driver()
{
}

void Driver::add_screen(ScreenPtr screen)
{
    screens.push_back(screen);
    auto watcher = std::make_shared<ev::io>();
    watcher->set<Driver, &Driver::on_screen_event>(this);
    watcher->data = screen.get();
    watcher->start(screen->event_source(), ev::READ);
    screen_event_watchers.push_back(watcher);
}

void Driver::quit()
{
    std::cout << "Qutting..." << std::endl;
    ev::get_default_loop().break_loop(ev::ALL);
}

void Driver::on_screen_event(ev::io &w, int revents)
{
    static_cast<Screen*>(w.data)->handle_events();
}

void Driver::on_enter_pressed(ev::io &w, int revents)
{
    quit();
}

void Driver::on_sigint(ev::sig &w, int revents)
{
    quit();
}

void Driver::on_sync(ev::periodic &w, int revents)
{
    for (const auto& screen : screens) {
        screen->update();
    }
}

void Driver::drive(unsigned fps)
{
    ev::default_loop loop;

    stdin_watcher.set<Driver, &Driver::on_enter_pressed>(this);
    stdin_watcher.start(STDIN_FILENO, ev::READ);

    const double update_interval = 1.0 / fps;
    sync_timer.set<Driver, &Driver::on_sync>(this);
    sync_timer.start(0, update_interval);

    ev::sig sigint_handler;
    sigint_handler.set<Driver, &Driver::on_sigint>(this);
    sigint_handler.start(SIGINT);

    std::cout << "Press Enter or Ctrl+C to quit..." << std::endl;
    loop.run(0);
}
