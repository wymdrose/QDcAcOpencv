#ifndef IODEBUG_H
#define IODEBUG_H

#include <QWidget>
#include "ui_iodebug.h"

class IoDebug : public QWidget
{
	Q_OBJECT

public:
	IoDebug(QWidget *parent = 0);
	~IoDebug();

private slots:
	void ioScan();

private:
	QCheckBox* pCheckBoxSout[8];
	QCheckBox* pCheckBoxGout[16];
	QMutex ioMutex;
	Ui::IoDebug ui;
};

#endif // IODEBUG_H
