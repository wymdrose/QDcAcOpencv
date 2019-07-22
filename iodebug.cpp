#include "iodebug.h"
#include <QTimer>
#include "global.h"

IoDebug::IoDebug(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QTimer* timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(ioScan()));
	timer->start(500);
	
	for (size_t i = 0; i < 8; i++){
		pCheckBoxSout[i] = findChild<QCheckBox*>("checkBoxSout_" + QString::number(i + 1));
		
		connect(pCheckBoxSout[i], &QCheckBox::clicked, [this, i](){
			ioMutex.lock();
			pCheckBoxSout[i]->isChecked() ? gpDmc1380->SetOutput(i + 1, 0) : gpDmc1380->SetOutput(i + 1, 1);
			ioMutex.unlock();
		});
	}

	for (size_t i = 0; i < 16; i++){

		pCheckBoxGout[i] = findChild<QCheckBox*>("checkBoxGout_" + QString::number(i + 1));
		
		connect(pCheckBoxGout[i], &QCheckBox::clicked, [this, i](){
			ioMutex.lock();	
			pCheckBoxGout[i]->isChecked() ? gpDmc1380->SetOutput(i + 9, 0) : gpDmc1380->SetOutput(i + 9, 1);
			ioMutex.unlock();
		});
	}
}

IoDebug::~IoDebug()
{

}

void IoDebug::ioScan(){
	ioMutex.lock();
	for (size_t i = 0; i < 15; i++){
		
		QCheckBox* pCheckBox = findChild<QCheckBox*>("checkBoxSin_" + QString::number(i + 1));	
		if (!gpDmc1380->GetInput(i + 1)){
			pCheckBox->setChecked(true);
			pCheckBox->setStyleSheet("QCheckBox{color:green}");
		}
		else{
			pCheckBox->setChecked(false);
			pCheckBox->setStyleSheet("QCheckBox{color:}");
		}
	}

	for (size_t i = 0; i < 16; i++){

		QCheckBox* pCheckBox = findChild<QCheckBox*>("checkBoxGin_" + QString::number(i + 1));
		if (!gpDmc1380->GetInput(i + 16)){
			pCheckBox->setChecked(true);
			pCheckBox->setStyleSheet("QCheckBox{color:green}");
		}
		else{
			pCheckBox->setChecked(false);
			pCheckBox->setStyleSheet("QCheckBox{color:}");
		}
	}
	
	for (size_t i = 0; i < 8; i++){

		QCheckBox* pCheckBox = findChild<QCheckBox*>("checkBoxSout_" + QString::number(i + 1));
		if (!gpDmc1380->GetOutput(i + 1)){
			pCheckBox->setChecked(true);
			pCheckBox->setStyleSheet("QCheckBox{color:green}");
		}
		else{
			pCheckBox->setChecked(false);
			pCheckBox->setStyleSheet("QCheckBox{color:}");
		}
	}

	for (size_t i = 0; i < 16; i++){

		QCheckBox* pCheckBox = findChild<QCheckBox*>("checkBoxGout_" + QString::number(i + 1));
		if (!gpDmc1380->GetOutput(i + 9)){
			pCheckBox->setChecked(true);
			pCheckBox->setStyleSheet("QCheckBox{color:green}");
		}
		else{
			pCheckBox->setChecked(false);
			pCheckBox->setStyleSheet("QCheckBox{color:}");
		}
	}
	ioMutex.unlock();

	QApplication::processEvents();
}