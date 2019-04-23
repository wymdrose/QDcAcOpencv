
#include "qdcacopencv.h"
#include <QPushButton>
#include <keysight.hpp>
#include <tubeknn.hpp>
#include <xmltree.hpp>
#include <Mindvision/vision.hpp>
#include <QGroupBox>

QDcAcOpencv::QDcAcOpencv(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

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

void QDcAcOpencv::pushButtonStart()
{
	MachineVision::MindVision m;
	m.initCamera();
	m.snapshot();


	Treexml::XmlSet t;

	t.loadXmlToTree(ui.treeWidget);

	QString tValue;
	t.getOneParaValue("AXIS1", "path", tValue);
	t.deleteOnePara("AXIS1", "path");
	t.updateOnePara("AXIS0", "path", "111111");
	t.updateOnePara("AXIS1", "path", "222222");
	t.updateOnePara("AXIS0", "acc", "500");
	t.updateOnePara("AXIS2", "path", "222222");

	std::cout << TubeKnn::getTubeValue("test5.png").toStdString();

	std::shared_ptr<Ks34970A_2A> mpKs34970A_2A = std::make_shared<Ks34970A_2A>(1, 9600);
	mpKs34970A_2A->init();
	
}
