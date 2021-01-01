#include <libevdipp/evdi.hpp>
#include <libevdipp/buffer.hpp>
#include "qevdiscreen.h"

QEvdiScreen::QEvdiScreen(const Evdi& evdi, std::vector<unsigned char> edid, bool separateCursorEvents)
    : Screen(evdi, edid)
    , screenEventNotifier(event_source(), QSocketNotifier::Read)
    , enabled(false)
{
    noContent = QImage(1, 1, QImage::Format_RGB32);
    noContent.fill(Qt::black);
    connect(&screenEventNotifier, SIGNAL(activated(int)), this, SLOT(dispatchScreenEvents()));
    if (separateCursorEvents) {
        Screen::enable_cursor_events();
    }
}

void QEvdiScreen::dispatchScreenEvents()
{
    handle_events();
}

void QEvdiScreen::on_dpms_notification(int dpms_mode)
{
    switch (dpms_mode) {
    case 0:
        enabled = true;
        break;
    case 3:
        enabled = false;
        break;
    default:
        break;
    }
}

void QEvdiScreen::on_mode_change(evdi_mode mode)
{
    enabled = false;

    // let the parent class re-allocate and re-register buffers
    Screen::on_mode_change(mode);

    // update the QImage wrappers to the buffers
    for (const auto& buffer : buffers) {
        images[buffer.first] = make_qimage_wrapper(buffer.second);
    }

    enabled = true;
}

void QEvdiScreen::on_cursor_set(evdi_cursor_set cursor_set)
{
    constexpr size_t ARGB32_FOURCC = 0x34325241;
    if (cursor_set.pixel_format == ARGB32_FOURCC) {
        auto cursor_image = QImage((const uchar*)cursor_set.buffer, cursor_set.width, cursor_set.height, QImage::Format_ARGB32);
        emit cursor_changed(cursor_image, cursor_set.enabled, cursor_set.hot_x, cursor_set.hot_y);
    }
}

void QEvdiScreen::on_cursor_move(evdi_cursor_move cursor_move)
{
    emit cursor_moved(cursor_move.x, cursor_move.y);
}

QImage QEvdiScreen::make_qimage_wrapper(const std::unique_ptr<Buffer>& buffer)
{
    evdi_buffer buf = buffer->get();
    // Qt apps will use this for access to screen data - this makes no copies and takes no ownership
    return QImage((const uchar*)buf.buffer, buf.width, buf.height, buf.stride, QImage::Format_RGB32);
}

int QEvdiScreen::on_update_ready(int buffer_to_be_updated)
{
    int rectCount = Screen::on_update_ready(buffer_to_be_updated);
    if (rectCount) {
        // TODO: this should also inform about the actual areas that changed
        emit screen_updated(buffer_to_be_updated);
    }
    return rectCount;
}

QImage& QEvdiScreen::image_for_buffer(int num)
{
    return enabled ? images[num] : noContent;
}
