#include "screen.hpp"
#include "debug.hpp"

Screen::Screen(const Evdi& evdi, std::vector<unsigned char>& edid, bool separateCursorEvents)
    : evdi(evdi)
    , buffersRegistered(false)
    , bufferToUpdate(0)
{
    rects.resize(16); // FIXME: remove magic const
    evdi.connect(edid.data(), edid.size());
    context.dpms_handler = dpms_handler;
    context.mode_changed_handler = mode_change_handler;
    context.update_ready_handler = update_ready_handler;
    context.crtc_state_handler = crtc_state_handler;
    context.cursor_set_handler = cursor_set_handler;
    context.cursor_move_handler = cursor_move_handler;
    context.user_data = this;
    if (separateCursorEvents) {
        enable_cursor_events();
    }
}

Screen::~Screen()
{
    evdi.disconnect();
}

int Screen::event_source() const
{
    return evdi.get_event_source();
}

void Screen::handle_events()
{
    evdi.handle_events(&context);
}

void Screen::enable_cursor_events() const
{
    evdi.enable_cursor_events();
}

void Screen::mode_change_handler(evdi_mode mode, void* user_data)
{
    static_cast<Screen*>(user_data)->on_mode_change(mode);
}

void Screen::dpms_handler(int dpms_mode, void* user_data)
{
    static_cast<Screen*>(user_data)->on_dpms_notification(dpms_mode);
}

void Screen::update_ready_handler(int buffer_to_be_updated, void* user_data)
{
    static_cast<Screen*>(user_data)->on_update_ready(buffer_to_be_updated);
}

void Screen::crtc_state_handler(int state, void* user_data)
{
    static_cast<Screen*>(user_data)->on_crtc_state_change(state);
}

void Screen::cursor_set_handler(evdi_cursor_set cursor_set, void* user_data)
{
    static_cast<Screen*>(user_data)->on_cursor_set(cursor_set);
}

void Screen::cursor_move_handler(evdi_cursor_move cursor_move, void* user_data)
{
    static_cast<Screen*>(user_data)->on_cursor_move(cursor_move);
}

void Screen::on_mode_change(evdi_mode mode)
{
    logging << "Mode change: " << mode.width << "x" << mode.height << "@" << mode.refresh_rate << "Hz "
            << mode.bits_per_pixel << "bpp fmt:" << mode.pixel_format << std::endl;

    buffersRegistered = false;
    for (const auto& [buffer_id, buffer] : buffers) {
        evdi.unregister_buffer(buffer_id);
    }

    buffers.clear();

    for (int i = 0; i < BUFFER_COUNT; ++i) {
        buffers[i] = std::make_unique<Buffer>(i, mode.width, mode.height, mode.bits_per_pixel / 8 * mode.width);
        evdi.register_buffer(buffers[i]->get());
    }
    buffersRegistered = true;
}

int Screen::on_update_ready(int buffer_to_be_updated)
{
    //logging << "Update ready for buffer " << buffer_to_be_updated << std::endl;
    int num_rects;
    evdi.grab_pixels(rects.data(), &num_rects);
    if (num_rects > 1) {
        // Do not bother logging single dirty rects, only log more interesting cases
        logging << "Have " << num_rects << " to redraw" << std::endl;

        for (size_t i = 0; i < num_rects; ++i) {
            logging << "[" << rects[i].x1 << "," << rects[i].y1 << ":" << rects[i].x2 << "," << rects[i].y2 << "] ";
        }
        logging << std::endl;
    }
    return num_rects;
}

void Screen::on_dpms_notification(int dpms_mode)
{
    logging << "DPMS event: " << dpms_mode << std::endl;
}

void Screen::on_crtc_state_change(int state)
{
    logging << "CRTC state change: " << state << std::endl;
}

void Screen::on_cursor_set(evdi_cursor_set cursor_set)
{
    logging << "Cursor " << (cursor_set.enabled ? "ON" : "OFF") << ": Hot[X,Y]=" << cursor_set.hot_x << "," << cursor_set.hot_y << std::endl;
}

void Screen::on_cursor_move(evdi_cursor_move cursor_move)
{
    logging << "Cursor moved to " << cursor_move.x << "," << cursor_move.y << std::endl;
}

void Screen::update()
{
    if (!buffersRegistered) {
        return;
    }

    //logging << "Asking to update buffer " << bufferToUpdate << std::endl;
    if (evdi.request_update(bufferToUpdate)) {
        on_update_ready(bufferToUpdate);
    }
    bufferToUpdate = (bufferToUpdate + 1) % BUFFER_COUNT;
}
