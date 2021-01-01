#ifndef QTSCREEN_H
#define QTSCREEN_H

#include <QObject>
#include <QImage>
#include <QSocketNotifier>
#include <libevdipp/screen.hpp>
#include <libevdipp/evdi.hpp>

class QEvdiScreen
    : public QObject,
      public Screen {
    Q_OBJECT;

public:
    QEvdiScreen(const Evdi& evdi, std::vector<unsigned char> edid, bool separateseparateCursorEventsursorUpdates = true);

    void on_mode_change(evdi_mode mode);
    void on_dpms_notification(int dpms_mode);
    int on_update_ready(int buffer_to_be_updated);
    void on_cursor_set(evdi_cursor_set cursor_set);
    void on_cursor_move(evdi_cursor_move cursor_move);

    QImage& image_for_buffer(int num);

signals:
    void screen_updated(int buffer_id);
    void cursor_changed(QImage image, bool enabled, int hot_x, int hot_y);
    void cursor_moved(int x, int y);

private slots:
    void dispatchScreenEvents();

private:
    static QImage make_qimage_wrapper(const std::unique_ptr<Buffer>& buffer);
    QImage noContent;
    std::map<int, QImage> images;
    QSocketNotifier screenEventNotifier;
    bool enabled;
};

#endif