#ifndef DEVDEBUG_H
#define DEVDEBUG_H

#include <QDialog>
#include "ui_devdebug.h"

class DevDebug : public QDialog
{
	Q_OBJECT

public:
	DevDebug(QWidget *parent = 0);
	~DevDebug();

private:
	Ui::DevDebug ui;
};

#endif // DEVDEBUG_H
