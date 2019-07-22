#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qdcacopencv.h"
#include <windows.h>
#include <memory>

#include <mysignal.h>
#include <filelib.h>
#include <tubeknn.h>
#include <keysight.h>
#include <yokogawa.h>
#include <chroma.h>
#include <itech.h>
#include <colorExtract.h>
#include <mindvision.h>
#include <xmlwidget.h>
#include <leadshine.h>
#include <wensn.h>

#include <QPushButton>
#include <QGroupBox>
#include <mythread.h>
#include <QProcess>


#include <QSettings>
#include <mysqlite.h>
#include <QtSql\QSqlTableModel>

#pragma comment(lib, "KeySight.lib")
#pragma comment(lib, "chroma.lib")
#pragma comment(lib, "TubeKnn.lib")

extern  Ui::QDcAcOpencvClass* gpUi;
extern  QString gExePath;
extern  std::shared_ptr<Drose::MySignalUi> gpSignal;
extern  std::shared_ptr<OpencvApi::ColorExtract> gpColor;
extern  std::shared_ptr<InstrumentApi::Chroma62000H> gpChroma62000H;
extern  std::shared_ptr<InstrumentApi::Chroma63800> gpChroma63800;
extern  std::shared_ptr<InstrumentApi::ItechIt8600> gpItechIt8600;
extern  std::shared_ptr<InstrumentApi::ItechIt8800> gpItechIt8800;
extern  std::shared_ptr<InstrumentApi::Ks34970A_2A> gpKs34970A_2A;
extern  std::shared_ptr<InstrumentApi::Wst60m485> gpWst60m485;
extern  std::shared_ptr<MotionCtrl::Dmc1380>   gpDmc1380;
extern  std::shared_ptr<MachineVision::MindVision>   gpMindVision;
extern  std::shared_ptr<InstrumentApi::Wt230>   gpWt230;
extern  std::vector<QStringList> gParaVector;
extern  std::vector<QStringList> gTestVector;
extern  std::vector<QStringList> gParaVector2[30];
extern  std::vector<std::vector<QStringList>> gTestVector2;

#define GLOBAL \
Ui::QDcAcOpencvClass* gpUi;\
QString gExePath;\
std::shared_ptr<Drose::MySignalUi> gpSignal;\
std::shared_ptr<OpencvApi::ColorExtract> gpColor;\
std::shared_ptr<InstrumentApi::Chroma62000H> gpChroma62000H;\
	std::shared_ptr<InstrumentApi::Chroma63800> gpChroma63800;\
std::shared_ptr<InstrumentApi::ItechIt8600> gpItechIt8600;\
std::shared_ptr<InstrumentApi::ItechIt8800> gpItechIt8800;\
std::shared_ptr<InstrumentApi::Ks34970A_2A> gpKs34970A_2A;\
std::shared_ptr<InstrumentApi::Wst60m485>   gpWst60m485;\
std::shared_ptr<MotionCtrl::Dmc1380>   gpDmc1380;\
std::shared_ptr<MachineVision::MindVision>   gpMindVision;\
std::shared_ptr<InstrumentApi::Wt230>   gpWt230;\
std::vector<QStringList> gParaVector;\
std::vector<QStringList> gTestVector;\
std::vector<QStringList> gParaVector2[30];\
std::vector<std::vector<QStringList>> gTestVector2;
