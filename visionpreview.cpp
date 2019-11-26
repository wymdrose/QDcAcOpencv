#include "visionpreview.h"
#include "global.h"
#include <QFileDialog>
#include <QGraphicsScene>



VisionPreview::VisionPreview(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(100);

	gpChroma62000H->setVoltage("13.0");
	gpChroma62000H->confOutput(true);

	/*
	gpMindVision->snapshot(gExePath + "/Snapshot/tSnap");
	gpColor->linePicture(gExePath + "/Snapshot");

	QImage *image = new QImage();

	QString fileName = QFileDialog::getOpenFileName(
		this, "open image file",
		".",
		"Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
	if (fileName != "")
	{
		if (image->load(fileName))
		{
			QGraphicsScene *scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(*image));
			ui.graphicsView->setScene(scene);
			ui.graphicsView->resize(image->width() + 10, image->height() + 10);
			ui.graphicsView->show();
		}
	}
	
	*/

}

VisionPreview::~VisionPreview(){
	
}

void VisionPreview::closeEvent(QCloseEvent *event)
{
	timer->stop();
	gpChroma62000H->confOutput(false);
}

void VisionPreview::update(){

	gpMindVision->snapshot(gExePath + "/Snapshot/tSnap");
	MachineVision::linePicture(gExePath + "/Snapshot");

	QImage *image = new QImage();

	image->load(gExePath + "/Snapshot/tLine.BMP");

	QGraphicsScene *scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(*image));
	ui.graphicsView->setScene(scene);
	ui.graphicsView->resize(image->width() + 10, image->height() + 10);
	ui.graphicsView->show();

}