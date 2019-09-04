#pragma once

const int INPUT_1 = 2;
const int INPUT_2 = 3;
const int DELAY = 4;
const int CHECK_1 = 5;
const int CHECK_2 = 6;
const int CHECK_3 = 7;
const int RESULT = 0;

const int WT230_CH1 = 1;
const int WT230_CH3 = 3;
const float RESISTANCE = 10000;
const float PULSE_PER_VOLT = 346.55;

struct ValueS
{
	QString so, is, tu, to;
};

enum CmdType{

	SET,CHECK,UP,DOWN
};

struct CmdSet
{
	QString cmd;
	CmdType type;
	ValueS values;
};

#define inputChroma62000H() \
	gpChroma62000H->setVoltage(QString::number(_getValueM(gTestVector[i][INPUT_1]) + 0.5));\
	gpChroma62000H->confOutput(true);\
	tMeasure = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, settings.value("Channel/KsChroma").toString());\
	if (tMeasure < _getValueL(gTestVector[i][INPUT_1],"+-") || tMeasure > _getValueR(gTestVector[i][INPUT_1],"+-")){\
	gpSignal->showMsgSignal(gpUi->textBrowser, QString("inputChroma62000H()...Error->%0:[%1-%2]").arg(tMeasure).arg(_getValueL(gTestVector[i][INPUT_1],"+-")).arg(_getValueR(gTestVector[i][INPUT_1],"+-")));\
	bError = true;\
		}\
	measureItem = gpUi->tableWidgetTest->item(i, INPUT_1);\
	gpSignal->textSignal(measureItem, QStringLiteral("DC电压:%1 V").arg(tMeasure));\
	tDataList.append(QString("%1").arg(tMeasure));

#define setKs8600() \
	gpItechIt8600->setRemote();\
	gpItechIt8600->setPower(QString::number(_getValueM(gTestVector[i][INPUT_2])));\
	gpSignal->showMsgSignal(gpUi->textBrowser, QString("setKs8600():%0").arg(_getValueM(gTestVector[i][INPUT_2])));\
	QThread::msleep(500);\
	gpItechIt8600->setInput(true);\
	QThread::msleep(3000);\
	gpWt230->getPower(WT230_CH3,tMeasure);\
	if (tMeasure < _getValueL(gTestVector[i][INPUT_2],"+-") || tMeasure > _getValueR(gTestVector[i][INPUT_2],"+-")){\
	gpSignal->showMsgSignal(gpUi->textBrowser, QString("gpWt230->getPower()...Error->%0 [%1-%2]").arg(tMeasure).arg(_getValueL(gTestVector[i][INPUT_2],"+-")).arg(_getValueR(gTestVector[i][INPUT_2],"+-")));\
	bError = true;\
		}\
	measureItem = gpUi->tableWidgetTest->item(i, INPUT_2);\
	gpSignal->textSignal(measureItem, QStringLiteral("AC负载: %1 w").arg(tMeasure));\
	tDataList.append(QString("%1").arg(tMeasure));

#define checkKs8600_VOLT() \
	Sleep(gTestVector[i][DELAY].toInt());\
	tMeasure = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageAc, settings.value("Channel/Ks8600").toString());\
	if (tMeasure < _getValueL(gTestVector[i][CHECK_1],"-") || tMeasure > _getValueR(gTestVector[i][CHECK_1],"-")){\
	gpSignal->showMsgSignal(gpUi->textBrowser, QString("checkKs8600_VOLT()...Error->%0 [%1-%2]").arg(tMeasure).arg(_getValueL(gTestVector[i][CHECK_1],"-")).arg(_getValueR(gTestVector[i][CHECK_1],"-")));\
	bError = true;\
					}\
	measureItem = gpUi->tableWidgetTest->item(i, CHECK_1);\
	gpSignal->textSignal(measureItem, QStringLiteral("AC电压: %1 V").arg(tMeasure));\
	tDataList.append(QString("%1").arg(tMeasure));

#define checkKs8600_FREQ() \
	tMeasure = gpKs34970A_2A->getMeasure(gpKs34970A_2A->frequency, settings.value("Channel/Ks8600").toString());\
	if (tMeasure < _getValueL(gTestVector[i][CHECK_2],"-") || tMeasure > _getValueR(gTestVector[i][CHECK_2],"-")){\
	gpSignal->showMsgSignal(gpUi->textBrowser, QString("checkKs8600_FREQ()...Error.->%0 [%1-%2]").arg(tMeasure).arg(_getValueL(gTestVector[i][CHECK_2],"-")).arg(_getValueR(gTestVector[i][CHECK_2],"-")));\
	bError = true;\
					}\
	measureItem = gpUi->tableWidgetTest->item(i, CHECK_2);\
	gpSignal->textSignal(measureItem, QStringLiteral("AC频率: %1").arg(tMeasure));\
	tDataList.append(QString("%1").arg(tMeasure));

#define checkKs8600_CURR() \
	tMeasure = gpKs34970A_2A->getMeasure(gpKs34970A_2A->currentAc, settings.value("Channel/Ks8600").toString());\
	if (tMeasure < _getValueL(gTestVector[i][CHECK_3],"-") || tMeasure > _getValueR(gTestVector[i][CHECK_3],"-")){\
	gpSignal->showMsgSignal(gpUi->textBrowser, QString("checkKs8600_CURR()...Error."));\
	bError = true;\
					}\
	measureItem = gpUi->tableWidgetTest->item(i, CHECK_3);\
	gpSignal->textSignal(measureItem, QStringLiteral("测试电流: %1").arg(tMeasure));\
	tDataList.append(QString("%1").arg(tMeasure));

#define DATA_LIST_ok() \
	tDataList.push_back(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));\
	tDataList.push_back("ok");\
	tCsv.append(tDataList);\
	tTxt.append(tDataList);

#define DATA_LIST_ng() \
	tDataList.push_back(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));\
	tDataList.push_back("ng");\
	tCsv.append(tDataList);\
	tTxt.append(tDataList);

enum RunState{
	RUN, STOP, PAUSE
};
