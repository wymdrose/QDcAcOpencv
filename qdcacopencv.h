#ifndef QDCACOPENCV_H
#define QDCACOPENCV_H

#include <QtWidgets/QMainWindow>
#include "ui_qdcacopencv.h"
#include <mythread.hpp>

class QDcAcOpencv : public QMainWindow
{
	Q_OBJECT

public:
	QDcAcOpencv(QWidget *parent = 0);
	~QDcAcOpencv();

private slots:
	void pushButtonStart();

private:
	Drose::MyThread2 mMyThread;
	Ui::QDcAcOpencvClass ui;
	
};

#endif // QDCACOPENCV_H
