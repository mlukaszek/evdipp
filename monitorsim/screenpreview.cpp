#include <QBoxLayout>
#include <QtDebug>
#include "screenpreview.h"

ScreenPreview::ScreenPreview(QEvdiScreen& screen, QWidget *parent)
: QWidget(parent)
, latestBufferId(-1)
, screen(screen)
{
	label = new QLabel(this);
	label->setScaledContents(true);
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->addWidget(label);
	layout->setSizeConstraint(QLayout::SetMinimumSize);
	setLayout(layout);

	connect(&screen, SIGNAL(screen_updated(int)), this, SLOT(latest_buffer(int)));
	
	createTimers(60, 10);
}

void ScreenPreview::createTimers(unsigned fps, unsigned screenshotEverySec)
{
    redrawTimer = new QTimer(this);
    connect(redrawTimer, SIGNAL(timeout()), this, SLOT(redraw()));
    redrawTimer->start(1000.0 / fps);
	
	if (screenshotEverySec) {
		screenshotTimer = new QTimer(this);
		connect(screenshotTimer, SIGNAL(timeout()), this, SLOT(screenshot()));
		screenshotTimer->start(1000 * screenshotEverySec);
	}
}

void ScreenPreview::latest_buffer(int buffer_id)
{
	latestBufferId = buffer_id;
}

void ScreenPreview::redraw()
{
	screen.update();	
	if (latestBufferId >= 0) {
		// TODO: be smarter and only refresh areas of the image that changed
		label->setPixmap(QPixmap::fromImage(screen.image_for_buffer(latestBufferId)));
		label->update();
	}
}

void ScreenPreview::screenshot()
{
	if (label->pixmap() && !label->pixmap()->isNull()) {
		label->pixmap()->save("screenshot.png");
	}
}
