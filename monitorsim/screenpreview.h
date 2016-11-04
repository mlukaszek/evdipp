#ifndef SCREENPREVIEW_H
#define SCREENPREVIEW_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPixmap>

#include "qevdiscreen.h"

class ScreenPreview
: public QWidget
{
    Q_OBJECT;

public:
    ScreenPreview(QEvdiScreen& screen, QWidget *parent = 0);

private slots:
    void latest_buffer(int buffer_id);
	void redraw();
	void screenshot();

private:
	void createTimers(unsigned fps, unsigned screenshotEverySec = 0);
	
    QTimer *redrawTimer;
	QTimer *screenshotTimer;

	QLabel *label;
	int latestBufferId;
	QEvdiScreen& screen;
};

#endif