
#include "qdcacopencv.h"
#include "devdebug.h"
#include "visionpreview.h"
#include "iodebug.h"
#include "mydefine.h"
#include <QFileDialog>
#include "paraparse.hpp"
#include "scansn.h"

GLOBAL

extern int loginMode;

QMutex runStateMutex;
RunState gRunState = STOP;
using namespace FileIo;
using namespace ParaConfig;

inline void _runStateUpdate(RunState);

QDcAcOpencv::QDcAcOpencv(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	if (loginMode == 1){
		ui.tabWidget->setTabEnabled(1, false);
	}
	else{

	}

	gExePath = QCoreApplication::applicationDirPath();
	QSettings settings(gExePath + "/cfg/cfg.ini", QSettings::IniFormat);
	mParaFile = gExePath + "/cfg/" + settings.value("Para/paraFile").toString();

	//
	gPointInit();
	loadPara();
	
//	ui.tableWidgetTest->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//	ui.tableWidgetPara1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);	

	//
	connect(ui.pushButtonStart, &QPushButton::clicked, [this]() { pushButtonStart(); });
	connect(ui.pushButtonPause, &QPushButton::clicked, [this]() { pushButtonPause();});
	connect(ui.pushButtonStop, &QPushButton::clicked, [this]() { pushButtonStop();});
	connect(ui.pushButtonPara, &QPushButton::clicked, [this]() {
		
		mParaFile = QFileDialog::getOpenFileName(NULL, QStringLiteral("参数列表"), gExePath + "/cfg/", "*.csv");	
		ui.labelPara->setText(mParaFile.section('/', -1, -1));
		loadPara();
	});
	//
	connect(ui.actionMindVision_platform, &QAction::triggered, [this]() {
		std::shared_ptr<QProcess> pPro = std::make_shared<QProcess>();
		QString tagDir = "\"" + gExePath + "/MindVision/MVDCP.exe" + "\"";
		pPro->startDetached(tagDir);
	});
	
	connect(ui.actionVision, &QAction::triggered, [this]() {		
		VisionPreview *pw = new VisionPreview();
		pw->show();

	});

	connect(ui.actionDevDebug, &QAction::triggered, [this]() {
		
		DevDebug *pDialog = new DevDebug();
		pDialog->show();
	});
	
	connect(ui.actionXml, &QAction::triggered, [this]() {

		xmlwidget *pWidget = new xmlwidget();
	});
	
	connect(ui.actionIo, &QAction::triggered, [this]() {

		IoDebug *pWidget = new IoDebug();
		pWidget->show();
	});

	connect(ui.pushButtonParaSave, &QPushButton::clicked, [this]() { 
				
		mCurParaCsv->clear();

		for (size_t i = 0; i < mpCurTableWidgetPara->rowCount(); i++)
		{
			QStringList tList;
			
			if (mpCurTableWidgetPara->item(i, 0)->checkState() == Qt::Checked){
				tList.append("#" + mpCurTableWidgetPara->item(i, 0)->text());
			}
			else{
				tList.append(mpCurTableWidgetPara->item(i, 0)->text());
			}

			for (size_t j = 1; j < mpCurTableWidgetPara->columnCount(); j++)
			{
				tList.append(mpCurTableWidgetPara->item(i, j)->text());
			}

			mCurParaCsv->append(tList);
		}
	
		loadPara();
	});

	//
}

QDcAcOpencv::~QDcAcOpencv()
{
	gpWt230->close();
}

void QDcAcOpencv::loadPara()
{
	gpSignal->showMsgSignal(gpUi->textBrowser, "loadPara()...");

	mCurParaCsv = std::shared_ptr<FileIo::CsvFile>(new FileIo::CsvFile(mParaFile));
	mpCurTableWidgetPara = ui.tableWidgetPara1;
	ui.stackedWidgetPara->setCurrentIndex(0);
	
	mCurParaCsv->get(gParaVector);
	mpCurTableWidgetPara->setColumnCount(gParaVector[0].size());
	mpCurTableWidgetPara->setRowCount(gParaVector.size());

	QRegExp tRegExp("[0-9]{0,2}.[0-9]{0,2}");
	
	for (size_t i = 0; i < gParaVector.size(); i++){
		QTableWidgetItem *check = new QTableWidgetItem;
		
		if (gParaVector[i][0].contains("#")){
			check->setCheckState(Qt::Checked);
			check->setText(gParaVector[i][0].right(gParaVector[i][0].size() - 1));
		}
		else if (tRegExp.exactMatch(gParaVector[i][0])){
			check->setCheckState(Qt::Unchecked);
			check->setText(gParaVector[i][0]);
		}
		
		mpCurTableWidgetPara->setItem(i, 0, check);

		for (size_t j = 1; j < gParaVector[0].size(); j++){
			mpCurTableWidgetPara->setItem(i, j, new QTableWidgetItem(gParaVector[i][j]));
		}
	}
	
	//
	for (size_t i = 0; i < 30; i++){
		gParaVector2[i].clear();
	}

	gTestVector.clear();
	gTestVector2.clear();
	gTestVector2.resize(gParaVector.size());

	
	for (size_t i = 0; i < gParaVector.size(); i++){
		
		if (gParaVector[i][0].contains("#")){
			gTestVector.push_back(gParaVector[i]);

			QStringList tStepList = gParaVector[i][0].split(".");
			gParaVector2[tStepList[0].toInt()].push_back(gParaVector[i]);
			
			gTestVector2[tStepList[0].toInt()].push_back(gParaVector[i]);
			gTestVector2.push_back(gTestVector2[tStepList[0].toInt()]);
		}
	}

	//
	gpUi->tableWidgetTest->clear();
	//
	gpUi->tableWidgetTest->setRowCount(gTestVector.size());
	gpUi->tableWidgetTest->setColumnCount(gTestVector[0].size() + 1);
	gpUi->progressBar->setRange(0, gTestVector.size());
	gpUi->progressBar->setValue(0);
	
	for (size_t i = 0; i < gTestVector.size(); i++){	

		gpUi->tableWidgetTest->setItem(i, 0, new QTableWidgetItem("#"));

		gpUi->tableWidgetTest->setItem(i, 1, new QTableWidgetItem(gTestVector[i][0].right(gTestVector[i][0].size()-1)));
		
		for (size_t j = 2; j < gTestVector[0].size(); j++)
		{	
			gpUi->tableWidgetTest->setItem(i, j, new QTableWidgetItem(gTestVector[i][j - 1]));
		}
	}


	QStringList headers;

	headers << QStringLiteral("结果") << QStringLiteral("序号") << QStringLiteral("作业步骤") << QStringLiteral("测试要点");

	for (size_t i = 4; i < gTestVector[0].size() + 1; i++)
	{
		headers << "*";
	}

	gpUi->tableWidgetTest->setHorizontalHeaderLabels(headers);

	for (size_t i = 0; i < gTestVector[0].size(); i++)
	{
		gpUi->tableWidgetPara1->resizeColumnToContents(i);
		gpUi->tableWidgetTest->resizeColumnToContents(i);
	}
	


	/*
	int lineNo(0);
	for (size_t i = 0; i < gParaVector.size(); i++)
	{
		for (size_t j = 0; j < gTestVector2[i].size(); j++, lineNo++)
		{
			for (size_t k = 0; k < gTestVector2[i][j].size(); k++)
			{
				gpUi->tableWidgetTest->setItem(lineNo, k, new QTableWidgetItem(gTestVector2[i][j][k]));
			}
			
		}
	}
	*/
}

void QDcAcOpencv::gPointInit(){
	
	QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
	

	gpUi = &ui;
	gpSignal = std::make_shared<Drose::MySignalUi>();
	gpColor = std::make_shared<OpencvApi::ColorExtract>();
	gpChroma62000H = std::make_shared<InstrumentApi::Chroma62000H>(settings.value("uartNo/chroma62000H").toInt(), 115200);
	gpChroma62000H->init(settings.value("GPIB/chroma62000H").toInt());

	gpChroma63800 = std::make_shared<InstrumentApi::Chroma63800>(settings.value("uartNo/chroma63800").toInt(), 57600);
	gpChroma63800->init(settings.value("GPIB/chroma63800").toInt());
	
	/*
	gpItechIt8600 = std::make_shared<InstrumentApi::ItechIt8600>(settings.value("netIp/itechIt8600").toString(), 30000);
	gpItechIt8600->init();
	*/
	gpItechIt8800 = std::make_shared<InstrumentApi::ItechIt8800>(settings.value("uartNo/itechIt8800").toInt(), 9600);
	gpItechIt8800->init();
	

	gpKs34970A_2A = std::make_shared<InstrumentApi::Ks34970A_2A>(settings.value("uartNo/ks34970A").toInt(), 57600);
	gpKs34970A_2A->init();

	gpWst60m485 = std::make_shared<InstrumentApi::Wst60m485>(settings.value("uartNo/wst60m485").toInt(), 9600);
	gpWst60m485->init();

	gpDmc1380 = std::make_shared<MotionCtrl::Dmc1380>();
	gpDmc1380->Init();
	//
	gpMindVision = std::make_shared<MachineVision::MindVision>();
	gpMindVision->initCamera();

	gpWt230 = std::make_shared<InstrumentApi::Wt230>();
	gpWt230->init(settings.value("GPIB/Wt230").toInt());

	//
	gpSignal->showMsgSignal(gpUi->textBrowser, "gPointInit()...");
}

void _afterTest(bool result){
	
	
	for (size_t i = 0; i < gTestVector[0].size(); i++)
	{
		gpUi->tableWidgetPara1->resizeColumnToContents(i);
		gpUi->tableWidgetTest->resizeColumnToContents(i);
	}
	

	gpChroma62000H->setVoltage("0");
	gpChroma62000H->setCurrent("0");
	gpChroma62000H->confOutput(false);


	gpSignal->colorSignal(gpUi->pushButtonStart, "QPushButton{background:}");

	gpSignal->textSignal(gpUi->labelResult, result ? QStringLiteral("OK") : QStringLiteral("NG"));
	gpSignal->colorSignal(gpUi->labelResult, result ? "QLabel{background:lightgreen}" : "QLabel{background:red}");
	gpSignal->showDialogSignal("Result", result ? QStringLiteral("OK") : QStringLiteral("NG"));

	gpSignal->textSignal(gpUi->textBrowser, result ? "Passed." :"Failed.");
		
	if (result){
	
	}
	else{
		
	}
	//
	gpChroma63800->setInput("OFF");

	//
	_runStateUpdate(STOP);
	gpSignal->ableSignal(gpUi->pushButtonStart, true);
}

inline void _runStateUpdate(RunState state){
	runStateMutex.lock();
	gRunState = state;
	runStateMutex.unlock();
}

void Drose::MyThread2::run(){

	gpSignal->showMsgSignal(gpUi->textBrowser, "run start.");

	QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
	CsvFile tCsv(gExePath + "/Log/Data_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv");
	
	QSettings cfg(gExePath + "/cfg/cfg.ini", QSettings::IniFormat);
	QDir *folder = new QDir;
	QString logsPath = cfg.value("Para/logPath").toString();
	QString curDir = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	if (!folder->exists(logsPath + curDir))
	{
		folder->mkdir(logsPath + curDir);
	}

	
	TxtFile tTxt(logsPath + curDir + "/" + gpUi->lineEditSn->text() + ".txt");


	/*
	std::vector<QStringList> tLineAll;
	tCsv.get(tLineAll);
	if (tLineAll.size() == 0){
		QStringList tHead = {"Sn","DcVolt","AcLoad"};
	}
	*/

	QStringList tDataList;
	tDataList.append("*" + gpUi->lineEditSn->text() + "*");

	bool bError = false;	
	QStringList msgErrorList;

	for (size_t i = 0; i < gTestVector.size(); i++){
		
		if (gRunState == STOP){
			gpSignal->textSignal(gpUi->textBrowser, "Pushbutton Stop Clicked."); 
			goto FlagError;
		}

		if (gRunState == PAUSE){
			QThread::msleep(500);			
			--i;
			continue;
		}
				
		gpSignal->textSignal(gpUi->textBrowser, QString("Step %1").arg(gTestVector[i][0]));

		QTableWidgetItem *measureItem;
		float tMeasure;
		int repeat = 0;

		for (size_t j = 2; j < gTestVector[0].size(); j++)
		{
			if (!gTestVector[i][j].contains("[") || !gTestVector[i][j].contains("]"))
			{
				continue;
			}

			QString msgError;
			measureItem = gpUi->tableWidgetTest->item(i, j + 1);

			gpChroma62000H->setCurrent("200");

			if (gTestVector[i][j].contains("&"))
			{
				int tLength = gTestVector[i][j].length();
				gTestVector[i][j].remove("&");
				repeat = tLength - gTestVector[i][j].length();
			}
			
			if (_cellParse(measureItem, gTestVector[i][j], msgError, tDataList) == false)
			{
				if (repeat--)
				{
					j--;
					gpSignal->showMsgSignal(gpUi->textBrowser, QString("repeat...").arg(repeat));
					continue;
				}

				gpSignal->textSignal(measureItem, msgError);
				gpSignal->colorSignal(measureItem, QColor(255, 0, 0), 1);

				gpSignal->showDialogSignal("NG Msg", msgError);
				bError = true;
				break;
			}
		}

		QTableWidgetItem *checkItem = gpUi->tableWidgetTest->item(i, 0);

		if (bError){
			gpSignal->colorSignal(checkItem, QColor(255, 0, 0), 0);
			gpSignal->textSignal(checkItem, QStringLiteral("NG"));
			goto FlagError;
		}
		gpSignal->colorSignal(checkItem, QColor(0, 255, 0), 0);
		gpSignal->proBarSignal(gpUi->progressBar, i + 1);

		gpSignal->textSignal(checkItem, QStringLiteral("PASS"));
		gpSignal->showMsgSignal(gpUi->textBrowser, "..............Passed.");
		gpSignal->textSignal(gpUi->textBrowser, "");
	}

	DATA_LIST_ok();

	_afterTest(true);
	
	return;

FlagError:	
	DATA_LIST_ng();
	_afterTest(false);

				
	/*
	while (true)
	{ 
		DatabaseCover::MySqLite tSqlite(gExePath + "/DROSE.db");

		QVector<QStringList> tQvector;
		tQvector.append({ "date_time", "'" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +"'" });
		tQvector.append({ "sn", "3333333" });
		tSqlite.insert("drose", tQvector);
		tSqlite.select();

		QSqlTableModel* model = new QSqlTableModel(this);

		model->setTable("drose");
		gpUi->tableView->setModel(model);
		model->select();

		_sleep(1000);
		gpSignal->showMsgSignal(gpUi->textBrowser, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
		qDebug() << "thread thread:" << QThread::currentThreadId();
	}
	*/
	
}

void QDcAcOpencv::pushButtonPause(){
	static bool bGoOn = true;
	bGoOn = !bGoOn;

	if (bGoOn){
		_runStateUpdate(RUN);
		gpSignal->textSignal(ui.pushButtonPause, QStringLiteral("暂停"));
		gpSignal->colorSignal(ui.pushButtonPause, "QPushButton{background:}");
	}
	else{
		_runStateUpdate(PAUSE);
		gpSignal->textSignal(ui.pushButtonPause, QStringLiteral("继续"));
		gpSignal->colorSignal(ui.pushButtonPause, "QPushButton{background:yellow}");
	}

}

void QDcAcOpencv::pushButtonStop(){

	_runStateUpdate(STOP);
	ui.pushButtonStart->setEnabled(true);
	ui.pushButtonStart->setStyleSheet("QPushButton{background:}");
}

void QDcAcOpencv::beforeTest(){
	
	ui.lineEditSn->clear();
	ui.textBrowser->clear();
	ui.pushButtonStart->setEnabled(false);
	ui.labelResult->clear();
	loadPara();

	gpSignal->colorSignal(gpUi->labelResult, "QLabel{background:}");
	gpSignal->colorSignal(ui.pushButtonStart, "QPushButton{background:lightgreen}");

	if (gpUi->pushButtonPause->text() == QStringLiteral("继续")){
		pushButtonPause();
	}

	_runStateUpdate(RUN);
}

void QDcAcOpencv::pushButtonStart()
{
	beforeTest();

	//
	QString scan;
	ScanDlg t(scan, 12);
	t.exec();
	gpSignal->textSignal(gpUi->lineEditSn, scan);

	if (scan.size()<12)
	{
		QMessageBox::information(this, "", QStringLiteral("sn 错误"));
		return;
	}


	//
	std::shared_ptr<Drose::MyThread2> pMyThread = std::make_shared<Drose::MyThread2>();
	connect(&pMyThread->mThread, &QThread::started, &*pMyThread, &Drose::MyThread2::run);
	pmMyThread = pMyThread;
	

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
	*/
		

	/*
	while (true)
	{
		QApplication::processEvents();
	}
	*/
}
