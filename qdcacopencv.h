#ifndef QDCACOPENCV_H
#define QDCACOPENCV_H

#include <QtWidgets/QMainWindow>
#include "ui_qdcacopencv.h"

#include "global.h"

class QDcAcOpencv : public QMainWindow
{
	Q_OBJECT

public:
	QDcAcOpencv(QWidget *parent = 0);
	~QDcAcOpencv();

	void loadPara();
	void gPointInit();

public:
	void beforeTest();

	

private:
	QTableWidget* mpCurTableWidgetPara;
	std::shared_ptr<FileIo::CsvFile> mCurParaCsv;

private slots:
	void pushButtonStart();
	void pushButtonStop();
	void pushButtonPause();


private:
	QString mParaFile;
	std::shared_ptr<Drose::MyThread2> pmMyThread;
	Ui::QDcAcOpencvClass ui;
	
};

#endif // QDCACOPENCV_H
