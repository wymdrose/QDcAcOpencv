#ifndef VISIONPREVIEW_H
#define VISIONPREVIEW_H

#include <QWidget>
#include "ui_visionpreview.h"
#include <QTimer>

class VisionPreview : public QWidget
{
	Q_OBJECT

public:
	VisionPreview(QWidget *parent = 0);
	~VisionPreview();

	void closeEvent(QCloseEvent *event);

	private slots:
	void update();

private:
	QTimer *timer;
	Ui::VisionPreview ui;
};

#endif // VISIONPREVIEW_H
