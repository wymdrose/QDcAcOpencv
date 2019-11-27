
#pragma once

#include "global.h"
#include "mydefine.h"

namespace ParaConfig
{
	inline float _getDcVolt(){
		gpWt230->tabChannel(WT230_CH1);
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, settings.value("Channel/KsChroma").toString());
	}
	
	inline float _getAcVolt(){
		gpWt230->tabChannel(WT230_CH3);
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageAc, settings.value("Channel/Ks8600").toString());
	}

	inline void _setAcSource(float volt){
		gpDmc1380->AbsMove(0, PULSE_PER_VOLT * volt);
	}

	inline float _getAcSource(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageAc, settings.value("Channel/KsAcSource").toString());
	}

	inline float _getAcFreq(){
		/*
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->frequency, settings.value("Channel/Ks8600").toString());
		*/
		float value = gpWt230->getFreq();
		value = gpWt230->getFreq();
		return value;
	}

	inline float _getResistance(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->resistance, settings.value("Channel/resistance").toString());
	}

	inline float _getDcCurr(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		float tCurr(0.0);
		tCurr = gpKs34970A_2A->getDcmVolt(settings.value("Channel/KsResistanceVolt").toString()) * RESISTANCE;
		return fabs(tCurr);
	}
	
	inline float _getFanValue(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);

		QVector<float> vValues;

		for (size_t i = 0; i < 2; i++)
		{
			float tValue = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, settings.value("Channel/Fan").toString());
		
			if (tValue > 0)
			{
				vValues.append(tValue);
			}
		}

		float values(0.0);
		for (size_t i = 0; i < vValues.length(); i++)
		{
			values += vValues[i];
		}
		
		return values / vValues.length();
	}

	inline float _getGroundValue(int index){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);

		if (index == 1)
		{
			return gpKs34970A_2A->getMeasure(gpKs34970A_2A->resistance, settings.value("Channel/Ground").toString());
		}
		else if (index == 2)
		{
			return gpKs34970A_2A->getMeasure(gpKs34970A_2A->resistance, settings.value("Channel/Ground2").toString());
		}
		
	//	return gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageAc, settings.value("Channel/Ground").toString());
	}

	inline void _pushKey(int keyNo, int delay){
		gpDmc1380->SetOutput(8 + keyNo, 0);
		QThread::msleep(delay);
		gpDmc1380->SetOutput(8 + keyNo, 1);
		QThread::msleep(100);
	}

	inline void _relay(int relayNo, QString on_off)
	{
		int relayIo = (on_off == "ON") ? 0 : 1 ;
		gpDmc1380->SetOutput(8 + 9 + relayNo, relayIo);
		QThread::msleep(300);
	}

	inline void _dialCode(int dialNo, QString on_off)
	{
		if (dialNo == 1 && on_off == "OFF")
		{
			gpDmc1380->SetOutput(8 + 3, 0);
			gpDmc1380->SetOutput(8 + 4, 1);
		}
		else if (dialNo == 1 && on_off == "ON")
		{
			gpDmc1380->SetOutput(8 + 3, 1);
			gpDmc1380->SetOutput(8 + 4, 0);
		}

		else if (dialNo == 2 && on_off == "OFF")
		{
			gpDmc1380->SetOutput(8 + 5, 0);
			gpDmc1380->SetOutput(8 + 6, 1);
		}
		else if (dialNo == 2 && on_off == "ON")
		{
			gpDmc1380->SetOutput(8 + 5, 1);
			gpDmc1380->SetOutput(8 + 6, 0);
		}
	}

	float _getValueM(const QString tString){
		return tString.split("+-")[0].toFloat();
	}

	float _getValueL(const QString tString, const QString split){
		if (split != "+-" || split != "-"){
			return tString.right(tString.size() - 1).toFloat();
		}

		return tString.split(split)[0].toFloat() - tString.split(split)[1].toFloat();
	}

	float _getValueR(const QString tString, const QString split){
		if (split != "+-" || split != "-"){
			return tString.right(tString.size() - 1).toFloat();
		}

		return tString.split(split)[0].toFloat() + tString.split(split)[1].toFloat();
	}

	QString inline _getLedString(){
		gpMindVision->snapshot(gExePath + "/Snapshot/tSnap");
		gpMindVision->snapshot(gExePath + "/Snapshot/tSnap");
	
		/*
		Mat roi;
		MachineVision::roiSnap(gExePath + "/Snapshot/tSnap.BMP", roi);
		*/

		QString tResult("");
		TubeKnn::getTubeValue(gExePath + "/Snapshot/tSnap.BMP", tResult, 5, 5, 50);

		qDebug() << tResult;
		return tResult;
	}

	float inline _getLedValue(bool bPoint = false)
	{
		if (!bPoint){
			return _getLedString().toFloat();
		}
		else{
			return _getLedString().toFloat() / 10.0;
		}
	}

	bool _lightProcess(CmdSet& set, QString& msg)
	{
		VideoCapture capture(0);
		if (!capture.isOpened())
		{
			cout << "cannot open the camera.";
			cin.get();
			return -1;
		}

		Mat edges;

		Mat frame;
		capture >> frame;
		if (frame.empty())
		{
			printf("--(!) No captured frame -- Break!");
			return false;                
		}
		else
		{
			/*
			cvtColor(frame, edges, CV_BGR2GRAY);
			blur(edges, edges, Size(7, 7));
			Canny(edges, edges, 0, 30, 3);
			*/
			imshow("读取被边缘后的视频", frame);
		}

		QString lightPath = gExePath + "/Light/light.jpg";
		imwrite(lightPath.toStdString(), frame);



		int hsvG[] = { 35, 77, 43, 255, 46, 255 };   //green
		int hsvY[] = { 26, 34, 43, 255, 46, 255 };   //yellow
		int hsvO[] = { 11, 25, 43, 255, 46, 255 };   //orange
		int hsvR1[] = { 0, 10, 43, 255, 46, 255 };   //RED
		int hsvR2[] = { 156, 180, 43, 255, 46, 255 };   //RED
		
	
		if (set.values.so == "RED")
		{
			if (gpColor->checkColor(hsvR1, frame) || gpColor->checkColor(hsvR2, frame))
			{
				set.values.is = "RED";
				return true;
			}
		}
		else if (set.values.so == "GREEN")
		{
			if (gpColor->checkColor(hsvG, frame))
			{
				set.values.is = "GREEN";
				return true;
			}
		}
		else if (set.values.so == "ORANGE")
		{
			if (gpColor->checkColor(hsvO, frame) || gpColor->checkColor(hsvY, frame))
			{
				set.values.is = "ORANGE";
				return true;
			}
		}
		
		
		return false;
		
		/*
		double resis[3];
		for (size_t i = 0; i < 3; i++)
		{
			resis[i] = _getResistance();
			Sleep(500);
		}

		gpSignal->showMsgSignal(gpUi->textBrowser, QString("_lightProcess:%0,%1,%2").arg(resis[0]).arg(resis[1]).arg(resis[2]));

		if (set.so == "FLASH")
		{
			if (qAbs(resis[0] - resis[1]) > 1000000 || qAbs(resis[1] - resis[2]) > 1000000
				|| qAbs(resis[0] - resis[2]) > 1000000)
			{
				return true;
			}
		}
		else
		{
			if (qAbs(resis[0] - resis[1]) < 1000000 || qAbs(resis[1] - resis[2]) < 1000000
				|| qAbs(resis[0] - resis[2]) < 1000000)
			{
				return true;
			}
		}

		return false;
		*/
	}

	bool _speProcess(ValueS set, QString& msg)
	{
		if (set.so == "1")
		{
			gpSignal->showMsgSignal(gpUi->textBrowser, "_speProcess");

			float tVac = _getAcVolt();
			float tAdc = _getDcCurr();

			msg += QStringLiteral("Vac:%0 ").arg(tVac);
			msg += QStringLiteral("Adc:%0 ").arg(tAdc);

			if (124 <= tVac && tVac <= 125 && tAdc < 189.4){return true;}
			else if (123 <= tVac && tVac <= 124 && tAdc < 186.4){ return true; }
			else if (122 <= tVac && tVac <= 123 && tAdc < 183.4){ return true; }
			else if (121 <= tVac && tVac <= 122 && tAdc < 180.4){ return true; }
			else if (120 <= tVac && tVac <= 121 && tAdc < 177.4){ return true; }
			else if (119 <= tVac && tVac <= 120 && tAdc < 174.5){ return true; }
			else if (118 <= tVac && tVac <= 119 && tAdc < 171.5){ return true; }
			else if (117 <= tVac && tVac <= 118 && tAdc < 168.6){ return true; }
			else if (116 <= tVac && tVac <= 117 && tAdc < 165.8){ return true; }
			else if (115 <= tVac && tVac <= 116 && tAdc < 162.9){ return true; }
			else if (114 <= tVac && tVac <= 115 && tAdc < 160.1){ return true; }
			else if (113 <= tVac && tVac <= 114 && tAdc < 157.1){ return true; }
			else if (112 <= tVac && tVac <= 113 && tAdc < 154.5){ return true; }
			else if (111 <= tVac && tVac <= 112 && tAdc < 151.8){ return true; }
			else if (110 <= tVac && tVac <= 111 && tAdc < 149.1){ return true; }
		}
		else
		{
			return true;
		}

		return true;
	}

	bool inline _checkSet(ValueS set)
	{
		if (set.tu != "")
		{
			if (set.is.toFloat() < set.tu.toFloat())
			{
				return false;
			}
		}

		if (set.to != "")
		{
			if (set.is.toFloat() > set.to.toFloat())
			{
				return false;
			}
		}

		if (set.tu == "" && set.to == "")
		{
			if (set.is != set.so)
			{
				return false;
			}
		}

		return true;
	}

	bool _dclProcess(CmdSet& cmdSet, QString& msg)
	{
		if (cmdSet.type == SET)
		{
			msg += QStringLiteral("[设置DC负载:%0] ").arg(cmdSet.values.so);

			if (cmdSet.values.so == "ON")
			{
				gpItechIt8800->setInput("ON");
				return true;
			}
			else if (cmdSet.values.so == "OFF")
			{
				gpItechIt8800->setCurrent("0");
				gpItechIt8800->setInput("OFF");
				return true;
			}
			else
			{
				gpItechIt8800->setCurrent(cmdSet.values.so);
				gpItechIt8800->setInput("ON");
			}
		}
		else if (cmdSet.type == CHECK)
		{

		//	gpWt230->getPower(WT230_CH3, cmdSet.values.is);
			cmdSet.values.is = QString::number(_getDcCurr());

			msg += QStringLiteral("DC负载%0A ").arg(cmdSet.values.is);

			if (_checkSet(cmdSet.values) == false)
			{
				return false;
			}
		}

		return true;
	}

	bool __aclPrepare(CmdSet& cmdSet, QString& msg)
	{
		
		float diff(0.0);

		for (size_t i = 0; i < 3; i++)
		{		
			gpChroma63800->setPower(QString::number(cmdSet.values.so.toFloat() + diff * 0.8));
			gpChroma63800->setInput("ON");
			Sleep(3000);
			gpWt230->getPower(WT230_CH3, cmdSet.values.is);
		
			cmdSet.values.is = QString::number(cmdSet.values.is.toFloat());

			if (cmdSet.values.is.toFloat() < 1)
			{
				continue;
			}

			float diff = cmdSet.values.so.toFloat() - cmdSet.values.is.toFloat();

			if (diff < 7)
			{
				return true;
			}

		}

		msg += "Error: __aclPrepare()";

		return false;

	}

	bool _aclProcess(CmdSet& cmdSet, QString& msg)
	{
		if (cmdSet.type == SET)
		{
			msg += QStringLiteral("[设置AC负载:%0] ").arg(cmdSet.values.so);

			if (cmdSet.values.so == "ON")
			{
				gpChroma63800->setInput("ON");
				return true;
			}
			else if (cmdSet.values.so == "OFF")
			{
				gpChroma63800->setPower("0");
				gpChroma63800->setInput("OFF");
				return true;
			}
			else
			{
				if (cmdSet.cmd.right(1) == "A")
				{
				//	gpChroma63800->setCurrent(cmdSet.values.so);
				}
				else if (cmdSet.cmd.right(1) == "R")
				{
					gpChroma63800->setResistance(cmdSet.values.so);
					gpChroma63800->setInput("ON");
				}
				else
				{
					if (false == __aclPrepare(cmdSet, msg))
					{
						return false;
					}				
				}
			
			}
		}
		else if (cmdSet.type == CHECK)
		{
			if (cmdSet.cmd.right(1) == "A")
			{

			}
			else if (cmdSet.cmd.right(1) == "R")
			{

			}
			else
			{
				gpWt230->getPower(WT230_CH3, cmdSet.values.is);
				cmdSet.values.is = QString::number(cmdSet.values.is.toFloat());
				msg += QStringLiteral("AC负载%0W ").arg(cmdSet.values.is);
			}
			
			if (_checkSet(cmdSet.values) == false)
			{
				return false;
			}
		}

		return true;
	}

	bool _acsPrepare(CmdSet& cmdSet, QString& msg)
	{
		if (cmdSet.values.so == "OFF")
		{
			msg += QStringLiteral("[AC输入:OFF] ");

			gpDmc1380->SetOutput(8 + 16, 1);
			return true;
		}

		float curValue(0.0);
		float setValue = cmdSet.values.so.toFloat();

		for (size_t i = 0; i < 20; i++)
		{
			curValue = _getAcSource();

			if (curValue == 0)
			{
				continue;
			}

			if (fabs(curValue - setValue) < 1)
			{
				break;
			}

			while (!gpDmc1380->CheckDone(0))
			{
				Sleep(10);
			}

			gpDmc1380->SetPos(0, 0);
			gpDmc1380->RelMove(0, 100 * (setValue - curValue), 3000);
		}

		curValue = _getAcSource();
		cmdSet.values.is = QString::number(curValue);

		if (curValue > cmdSet.values.so.toFloat() + 1)
		{
			return false;
		}

		if (cmdSet.cmd.right(2) == "on")
		{
			gpDmc1380->SetOutput(8 + 16, 0);
		}
		
		return true;
	}

	bool _ledProcess(CmdSet& cmdSet, QString& msg)
	{
		QString tLed;

		tLed = _getLedString();

		for (size_t i = 0; i < 3; i++)
		{
			if (tLed.length() != cmdSet.values.so.length())
			{
				tLed = _getLedString();
			}
			else
			{
				break;
			}
		}
		if (tLed.length() > 3)
		{
			tLed = tLed.left(3);
		}
		//
		if (cmdSet.values.so.contains("."))
		{
			tLed.insert(2, ".");
		}

		if (tLed == "FL0")
		{
			tLed = "FLo";
		}

		msg += QStringLiteral("LED:%0 ").arg(tLed);
		cmdSet.values.is = tLed;

		if (_checkSet(cmdSet.values) == false)
		{
			bool question = false;

			gpSignal->showBlockSignal("LED?", cmdSet.values.so, question);

			if (!question){
				msg += cmdSet.values.so;
				return false;
			}
		}

		if (cmdSet.values.is == "Ab5")
		{
			cmdSet.values.is = "AbS";
		}

		if (cmdSet.values.is == "F1")
		{
			cmdSet.values.is = "FI";
		}

		return true;
	}

	bool _cmdParse(CmdSet& cmdSet, QString& msg)
	{
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);

		if (cmdSet.cmd == "DCV")
		{
			cmdSet.unit = "V";
			if (cmdSet.type == SET)
			{
				msg += QStringLiteral("[设置DC电压:%0V] ").arg(cmdSet.values.so);
				gpChroma62000H->setVoltage(cmdSet.values.so);
				gpChroma62000H->confOutput(true);
				Sleep(500);
			//	gpChroma62000H->setVoltage(QString::number(2 * cmdSet.values.so.toFloat() - _getDcVolt()));
				 
			}
			else if (cmdSet.type == CHECK)
			{
				cmdSet.values.is = QString::number(_getDcVolt(), 'f', 2);

				msg += QStringLiteral("DC电压%0V ").arg(cmdSet.values.is);

				if (_checkSet(cmdSet.values) == false)
				{
					goto Error;
				}
			}
			else if (cmdSet.type == UP)
			{
				gpChroma62000H->setVoltage(QString::number(gpChroma62000H->getVolt() + cmdSet.values.so.toFloat()));
				gpChroma62000H->confOutput(true);
			}
			else if (cmdSet.type == DOWN)
			{
				gpChroma62000H->setVoltage(QString::number(gpChroma62000H->getVolt() - cmdSet.values.so.toFloat()));
				gpChroma62000H->confOutput(true);
			}			
		}
		else if (cmdSet.cmd.left(3) == "ACL")
		{
			cmdSet.unit = "W";
			if (_aclProcess(cmdSet, msg) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "DCL")
		{
			cmdSet.unit = "A";
			if (_dclProcess(cmdSet, msg) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "ACV")
		{
			cmdSet.unit = "V";
			cmdSet.values.is = QString::number(_getAcVolt());;
			msg += QStringLiteral("AC电压%0V ").arg(cmdSet.values.is);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "ACF")
		{
			cmdSet.unit = "Hz";
			cmdSet.values.is = QString::number(_getAcFreq());
			msg += QStringLiteral("AC频率%0 ").arg(cmdSet.values.is);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd.left(3) == "ACS")
		{
			cmdSet.unit = "V";
			if (_acsPrepare(cmdSet, msg) == false)
			{
				msg += QStringLiteral("AC输入电压:") + cmdSet.values.is;
				goto Error;
			}
			msg += QStringLiteral("AC输入电压:") + cmdSet.values.is;
		}
		else if (cmdSet.cmd.left(4) == "GOUT")
		{
			int io = (cmdSet.values.so == "ON") ? 0 : 1;
			int index = cmdSet.cmd.right(cmdSet.cmd.length() - 4).toInt();
		
			gpDmc1380->SetOutput(8 + index, io);
		}
		else if (cmdSet.cmd.left(5) == "RELAY")
		{
			_relay(cmdSet.cmd.right(1).toInt(), cmdSet.values.so);
		}
		else if (cmdSet.cmd == "KEY1")
		{
			_pushKey(1, cmdSet.values.so.toInt());
		}
		else if (cmdSet.cmd == "KEY2")
		{
			_pushKey(2, cmdSet.values.so.toInt());
		}
		else if (cmdSet.cmd == "DIA1")
		{
			_dialCode(1, cmdSet.values.so);
		}
		else if (cmdSet.cmd == "DIA2")
		{
			_dialCode(2, cmdSet.values.so);
		}
		else if (cmdSet.cmd == "WAIT")
		{
			int delay = cmdSet.values.so.toInt();
			if (delay >= 3000)
			{
				gpMytimer->countdownSignal(delay/1000);
			}
			else
			{
				Sleep(delay);
			}		
		}
		else if (cmdSet.cmd == "LED")
		{
			cmdSet.unit = "-";
			if (false == _ledProcess(cmdSet, msg))
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "DCC")
		{
			cmdSet.unit = "A";
			float tDcc = _getDcCurr();
			msg += QStringLiteral("DCC:%0 ").arg(tDcc);
			cmdSet.values.is = QString::number(tDcc);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "LED-DCC")
		{
			cmdSet.unit = "A";
			float tLed = _getLedValue();
			float tDcc = _getDcCurr();
		
			msg += QStringLiteral("LED:%0 ").arg(tLed);
			msg += QStringLiteral("DCC:%0 ").arg(tDcc);

			QString tDiff = QString::number(qAbs(tLed - tDcc), 'f', 3);

			cmdSet.values.is = tDiff;
			if (_checkSet(cmdSet.values) == false)
			{
				cmdSet.values.is = QString("%1-%2=%3").arg(tLed).arg(tDcc).arg(tDiff);
				goto Error;
			}
		//	cmdSet.values.is = QString::number(tLed) + "-" + QString::number(tDcc);
			cmdSet.values.is = QString("%1-%2=%3").arg(tLed).arg(tDcc).arg(tDiff);
		}
		else if (cmdSet.cmd == "ACV-ACS")
		{
			cmdSet.unit = "V";
			QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
			float tAcDiff = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageAc, settings.value("Channel/acDiff").toString());
			msg += QStringLiteral("acDiff:%0 ").arg(tAcDiff);
			
			cmdSet.values.is = QString::number(tAcDiff, 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}

			/*
			float tACV = _getAcVolt();
			float tACS = _getAcSource();

			msg += QStringLiteral("AC输出:%0 ").arg(tACV);
			msg += QStringLiteral("AC输入:%0 ").arg(tACS);

			cmdSet.values.is = QString::number(qAbs(tACV - tACS), 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				cmdSet.values.is = QString::number(tACV, 'f', 0) + "-" + QString::number(tACS, 'f', 0);
				goto Error;
			}

			cmdSet.values.is = QString::number(tACV, 'f', 0) + "-" + QString::number(tACS, 'f', 0);
			*/
		}
		else if (cmdSet.cmd == "SPE")
		{
			if (_speProcess(cmdSet.values, msg) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "MSG")
		{
			bool question = false;
			
			gpSignal->showBlockSignal("", cmdSet.values.so, question);
			
			if (!question){
				msg += cmdSet.values.so;
				goto Error;
			}
		}
		else if (cmdSet.cmd == "VOICE")
		{
			cmdSet.unit = "dB";
			float tVoice(0);
			for (size_t i = 0; i < 4; i++)
			{
				Sleep(1300);
				tVoice = gpWst60m485->getVoice() / 10.0;
				cmdSet.values.is = QString("%1").arg(tVoice);

				if (_checkSet(cmdSet.values) == true)
				{
					msg += QStringLiteral("蜂鸣器:%0 ").arg(tVoice);
					return true;
				}
			}

			msg += QStringLiteral("蜂鸣器:%0 ").arg(tVoice);

			bool question = false;
			QString info = cmdSet.values.tu.isEmpty() ? QStringLiteral("蜂鸣器停止？") : QStringLiteral("蜂鸣器响？");
			gpSignal->showBlockSignal(QStringLiteral("蜂鸣器？"), info, question);

			if (!question){
				msg += cmdSet.values.so;
				goto Error;
			}

		}
		else if (cmdSet.cmd == "LIGHT")
		{
			for (size_t i = 0; i < 3; i++)
			{
				if (_lightProcess(cmdSet, msg))
				{
					return true;
				}
			}
			goto Error;
		}
		else if (cmdSet.cmd == "FAN")
		{
			cmdSet.unit = "m/s";
			float tFan = _getFanValue();
			msg += QStringLiteral("FAN:%0 ").arg(tFan);
			cmdSet.values.is = QString("%1").arg(tFan);

			if (_checkSet(cmdSet.values) == false)
			{
				bool question = false;
				QString info = cmdSet.values.tu.isEmpty() ? QStringLiteral("风速小于设定值？") : QStringLiteral("风速大于设定值？");
				gpSignal->showBlockSignal("FAN?", info, question);

				if (!question){
					msg += cmdSet.values.so;
					goto Error;
				}
			}
		}
		else if (cmdSet.cmd == "GROUND")
		{
			cmdSet.unit = "ohm";
			float tValue = _getGroundValue(1);
			msg += QStringLiteral("地线:%0 ").arg(tValue);
			cmdSet.values.is = QString("%1").arg(tValue);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "GROUND2" || cmdSet.cmd == "ground2" || cmdSet.cmd == "Ground2")
		{
			cmdSet.unit = "ohm";
			float tValue = _getGroundValue(2);
			msg += QStringLiteral("地线:%0 ").arg(tValue);
			cmdSet.values.is = QString("%1").arg(tValue);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "Efficiency")
		{
			cmdSet.unit = "%";
			QString tAcl;
			gpWt230->getPower(WT230_CH3, tAcl);

			float tDcv = _getDcVolt();
			float tDcc = _getDcCurr();

			float tEff = 100 * tAcl.toFloat() / (tDcv * tDcc);

			msg += QStringLiteral("Efficiency:%0 ").arg(tEff);
			cmdSet.values.is = QString("%1").arg(tEff);

			if (_checkSet(cmdSet.values) == false)
			{ 
				goto Error;
			}
		}
		else if (cmdSet.cmd == "CEfficiency")
		{
			cmdSet.unit = "%";
			float tDcv = _getDcVolt();
			float tDcc = _getDcCurr();

			QString tPower;
			gpWt230->getPower(WT230_CH1, tPower);

			float tCEff = 100 * tDcv * tDcc / tPower.toFloat();

			msg += QStringLiteral("CEfficiency:%0 ").arg(tCEff);
			cmdSet.values.is = QString("%1").arg(tCEff);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "USBC")
		{
			cmdSet.unit = "A";
			float tCurr(0.0);
			tCurr = gpKs34970A_2A->getDcmVolt("109") * RESISTANCE;

			msg += QStringLiteral("USBC:%0 ").arg(fabs(tCurr));
			cmdSet.values.is = QString("%1").arg(fabs(tCurr));

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "USB-1")
		{
			cmdSet.unit = "V";
			float tValue = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, "111");
			msg += QStringLiteral("USB-1:%0 ").arg(tValue);

			cmdSet.values.is = QString::number(tValue, 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "USB-2")
		{
			cmdSet.unit = "V";
			float tValue = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, "112");
			msg += QStringLiteral("USB-2:%0 ").arg(tValue);

			cmdSet.values.is = QString::number(tValue, 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}

		}
		else if (cmdSet.cmd == "USB-3")
		{
			cmdSet.unit = "V";
			float tValue = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, "113");
			msg += QStringLiteral("USB-3:%0 ").arg(tValue);

			cmdSet.values.is = QString::number(tValue, 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "AuxiliaryDCV1")
		{
			cmdSet.unit = "V";
			float tValue = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, "114");
			msg += QStringLiteral("AuxiliaryDCV1:%0 ").arg(tValue);

			cmdSet.values.is = QString::number(tValue, 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "AuxiliaryDCV2")
		{
			cmdSet.unit = "V";
			float tValue = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, "115");
			msg += QStringLiteral("AuxiliaryDCV2:%0 ").arg(tValue);

			cmdSet.values.is = QString::number(tValue, 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "AuxiliaryACV1")
		{
			cmdSet.unit = "V";
			float tValue = gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageAc, "116");
			msg += QStringLiteral("AuxiliaryACV1:%0 ").arg(tValue);

			cmdSet.values.is = QString::number(tValue, 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else
		{
			msg += QStringLiteral(" 无效命令 :%0").arg(cmdSet.cmd);
			return false;
		}

		return true;

	Error:
		msg += QStringLiteral(" NG:%0[%1,%2] ").arg(cmdSet.values.so).arg(cmdSet.values.tu).arg(cmdSet.values.to);
		return false;
	}

	inline void _valueSetLoad(ValueS& set, const QString cmdR)
	{
		if (cmdR.contains("+-"))
		{
			set.so = cmdR.split("+-")[0];
			set.tu = QString("%0").arg(set.so.toFloat() - cmdR.split("+-")[1].toFloat());
			set.to = QString("%0").arg(set.so.toFloat() + cmdR.split("+-")[1].toFloat());
		}
		else if (cmdR.contains("~"))
		{
			set.tu = QString("%0").arg(cmdR.split("~")[0].toFloat());
			set.to = QString("%0").arg(cmdR.split("~")[1].toFloat());
			set.so = QString::number((set.tu.toFloat() + set.to.toFloat()) * 0.5);
		}
		else if (cmdR.contains("<"))
		{
			set.so = "";
			set.tu = "";
			set.to = cmdR.right(cmdR.size()-1);
		}
		else if (cmdR.contains(">"))
		{
			set.so = "";
			set.tu = cmdR.right(cmdR.size() - 1);
			set.to = "";
		}
		else
		{
			set.so = cmdR;
			set.tu = "";
			set.to = "";
		}
	}

	void cmdLoad(CmdSet& cmdSet, QString oneCmd)
	{
		if (oneCmd.contains(":"))
		{
			cmdSet.type = SET;
			cmdSet.cmd = oneCmd.split(":")[0];
			_valueSetLoad(cmdSet.values, oneCmd.split(":")[1]);
		}
		else if (oneCmd.contains("="))
		{
			cmdSet.type = CHECK;
			cmdSet.cmd = oneCmd.split("=")[0];
			_valueSetLoad(cmdSet.values, oneCmd.split("=")[1]);
		}
		else if (oneCmd.contains("++"))
		{
			cmdSet.type = UP;
			cmdSet.cmd = oneCmd.split("++")[0];
			_valueSetLoad(cmdSet.values, oneCmd.split("++")[1]);

		}
		else if (oneCmd.contains("--"))
		{
			cmdSet.type = DOWN;
			cmdSet.cmd = oneCmd.split("--")[0];
			_valueSetLoad(cmdSet.values, oneCmd.split("--")[1]);
		}
		else if (oneCmd.contains("?"))
		{
			cmdSet.type = QUESTION;
		}
	}

	inline void cmdConver(CmdSet& CmdSet, QString& dataline)
	{
		if (CmdSet.cmd == "DCV")
		{
			CmdSet.cmd = "Vdc_in";
		}
		else if (CmdSet.cmd == "ACV")
		{
			CmdSet.cmd = "Vac_out";
		}
		else if (CmdSet.cmd == "ACF")
		{
			CmdSet.cmd = "Frequency";
		}
		else if (CmdSet.cmd == "DCC")
		{
			CmdSet.cmd = "Idc_in";
		}
		else
		{

		}
	}

	bool _cellParse(QTableWidgetItem *measureItem, QString cell, QString& msg, QStringList& dataList)
	{
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);

		cell.replace("[", "");
		cell.remove(" ");
		QStringList tList = cell.split("]");

		msg.clear();

		for (size_t i = 0; i < tList.size(); i++)
		{
			if (tList[i].isEmpty()){ continue; }

			CmdSet tCmdSet;

			cmdLoad(tCmdSet, tList[i]);

			//	
			QString dataline;

			if (_cmdParse(tCmdSet, msg) == false)
			{
				if (tCmdSet.type == CHECK)
				{
					cmdConver(tCmdSet, dataline);

					dataline += tCmdSet.cmd;

					for (size_t i = 0; i < 15 - tCmdSet.cmd.length(); i++)
					{
						dataline += " ";
					}
					
					dataline += tCmdSet.values.tu;
					for (size_t i = 0; i < 10 - tCmdSet.values.tu.length(); i++)
					{
						dataline += " ";
					}

					dataline += tCmdSet.values.to;
					for (size_t i = 0; i < 11 - tCmdSet.values.to.length(); i++)
					{
						dataline += " ";
					}

					dataline += tCmdSet.values.is;
					for (size_t i = 0; i < 17 - tCmdSet.values.is.length(); i++)
					{
						dataline += " ";
					}

					//
					dataline += tCmdSet.unit;
					for (size_t i = 0; i < 7 - tCmdSet.unit.length(); i++)
					{
						dataline += " ";
					}

					dataline += "*FAIL*";
					dataList.append(dataline);
				}
				
				return false;
			}

			if (tCmdSet.type == CHECK)
			{
				cmdConver(tCmdSet, dataline);

				dataline += tCmdSet.cmd;

				for (size_t i = 0; i < 15 - tCmdSet.cmd.length(); i++)
				{
					dataline += " ";
				}

				dataline += tCmdSet.values.tu;
				for (size_t i = 0; i < 10 - tCmdSet.values.tu.length(); i++)
				{
					dataline += " ";
				}

				dataline += tCmdSet.values.to;
				for (size_t i = 0; i < 11 - tCmdSet.values.to.length(); i++)
				{
					dataline += " ";
				}

				dataline += tCmdSet.values.is;
				for (size_t i = 0; i < 17 - tCmdSet.values.is.length(); i++)
				{
					dataline += " ";
				}

				dataline += tCmdSet.unit;
				for (size_t i = 0; i < 7 - tCmdSet.unit.length(); i++)
				{
					dataline += " ";
				}

				dataline += "PASS";
				dataList.append(dataline);
			}

			gpSignal->textSignal(measureItem, msg);
			gpSignal->colorSignal(measureItem, QColor(0, 255, 0), 1);
		}

		return true;
	}
};









