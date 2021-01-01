#ifndef SCREENPREVIEW_H
#define SCREENPREVIEW_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPixmap>

#include "qevdiscreen.h"

class ScreenPreview
    : public QWidget {
    Q_OBJECT;

public:
    ScreenPreview(QEvdiScreen& screen, QWidget* parent = 0);
    void paintEvent(QPaintEvent*);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void save_screenshot();

private slots:
    void latest_buffer(int buffer_id);
    void update_screen();
    void update_cursor(QImage cursorImage, bool enabled, int hot_x, int hot_y);
    void move_cursor(int x, int y);

private:
    void createTimer(unsigned fps);

    QTimer* updateTimer;

    QImage cursorImage;
    bool cursorEnabled;
    QPoint cursorPosition;
    QPoint cursorHotpoint;

    int latestBufferId;
    QEvdiScreen& screen;
};

#endif