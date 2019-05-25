
#include "qdcacopencv.h"
#include <QPushButton>
#include <keysight.hpp>
#include <tubeknn.hpp>
#include <xmltree.hpp>
#include <colorExtract.hpp>
#include <Mindvision/vision.hpp>
#include <QGroupBox>
#include <mysignal.hpp>
#include <mythread.hpp>

#include "global.hpp"

using namespace Global;

QDcAcOpencv::QDcAcOpencv(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//
	Global::mpUi = &ui;
	gpSignal = std::make_shared<Drose::MySignalUi>();
	gpColor = std::make_shared<OpencvApi::ColorExtract>();

	//
	connect(ui.pushButtonStart, &QPushButton::clicked, [this]() { pushButtonStart(); });
	connect(ui.treeWidget, &QTreeWidget::itemClicked, [this]() { 

		QTreeWidgetItem* item = ui.treeWidget->currentItem();
		QTreeWidgetItem* parent = item->parent();
		if (item->text(1).isEmpty())
		{
			ui.groupBoxId->setTitle(item->text(0));
			ui.lineEditKey->setText("");
			ui.lineEditValue->setText("");
		}
		else
		{
			ui.groupBoxId->setTitle(parent->text(0));
			ui.lineEditKey->setText(item->text(0));
			ui.lineEditValue->setText(item->text(1));
		}		

		 });

	connect(ui.pushButtonUpdate, &QPushButton::clicked, [this]() {
		Treexml::XmlSet t;
		t.updateOnePara(ui.groupBoxId->title(), ui.lineEditKey->text(), ui.lineEditValue->text());
		t.loadXmlToTree(ui.treeWidget);
	});

	connect(ui.pushButtonDelete, &QPushButton::clicked, [this]() { 
		Treexml::XmlSet t;
		t.deleteOnePara(ui.groupBoxId->title(), ui.lineEditKey->text());
		t.loadXmlToTree(ui.treeWidget);
	});
}

QDcAcOpencv::~QDcAcOpencv()
{

}

void Drose::MyThread2::run()
{

	for (size_t i = 0; i < 11; i++)
	{
		gpColor->getOne(OpencvApi::HSV[i], 1);
	}

	gpColor->getOne(OpencvApi::HSV[4], 1);
	gpColor->getOne(OpencvApi::HSV[7], 0);
	gpColor->getOne(OpencvApi::HSV[9], 0);


	while (true)
	{ 
		_sleep(1000);
		gpSignal->showMsgSignal(mpUi->textBrowser, "******");
		qDebug() << "thread thread:" << QThread::currentThreadId();
	}
	
}

void QDcAcOpencv::pushButtonStart()
{
	
	mMyThread.start();
		
	gpSignal->showDialogSignal("hello");
	gpSignal->colorSignal(ui.pushButtonStart, "QPushButton{background:yellow}");
	
	/*
	OpencvApi::ColorExtract* pColor = new OpencvApi::ColorExtract();
	pColor->get();

	*/

	//
//	MachineVision::MindVision* pm = new MachineVision::MindVision();
//	QObject::connect(&pm->pWorkThread->mThread, &QThread::started, pm, &MachineVision::MindVision::run);

//	pm->initCamera();
//	pm->snapshot();

	/*
	Treexml::XmlSet t;

	t.loadXmlToTree(ui.treeWidget);

	QString tValue;
	t.getOneParaValue("AXIS1", "path", tValue);
	t.deleteOnePara("AXIS1", "path");
	t.updateOnePara("AXIS0", "path", "111111");
	t.updateOnePara("AXIS1", "path", "222222");
	t.updateOnePara("AXIS0", "acc", "500");
	t.updateOnePara("AXIS2", "path", "222222");

	QString re = TubeKnn::getTubeValue("a.jpg.BMP");
	std::cout << re.toFloat();
	*/


	/*
	std::shared_ptr<Ks34970A_2A> mpKs34970A_2A = std::make_shared<Ks34970A_2A>(3, 57600);
	mpKs34970A_2A->init();

	float tfValue;
	mpKs34970A_2A->getMeasure(mpKs34970A_2A->voltageAc, "101", tfValue);
	mpKs34970A_2A->getMeasure(mpKs34970A_2A->voltageDc, "101", tfValue);
	mpKs34970A_2A->getMeasure(mpKs34970A_2A->currentAc, "101", tfValue);
	mpKs34970A_2A->getMeasure(mpKs34970A_2A->currentDc, "101", tfValue);
	mpKs34970A_2A->getMeasure(mpKs34970A_2A->frequency, "101", tfValue);
	*/

	while (true)
	{
		QApplication::processEvents();
	}
	
}
