
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
		gpDmc1380->RelMove(0, PULSE_PER_VOLT * volt);
	}

	inline float _getAcSource(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageAc, settings.value("Channel/KsAcSource").toString());
	}

	inline float _getAcFreq(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->frequency, settings.value("Channel/Ks8600").toString());
	}

	inline float _getResistance(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->resistance, settings.value("Channel/resistance").toString());
	}

	inline float _getDcCurr(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, settings.value("Channel/KsResistanceVolt").toString()) * RESISTANCE;
	}
	
	inline float _getFanValue(){
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);
		return gpKs34970A_2A->getMeasure(gpKs34970A_2A->voltageDc, settings.value("Channel/Fan").toString());
	}

	inline void _pushKey(int keyNo, int delay){
		gpDmc1380->SetOutput(8 + keyNo, 0);
		QThread::msleep(delay);
		gpDmc1380->SetOutput(8 + keyNo, 1);
		QThread::msleep(500);
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
		Sleep(300);
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

	bool _lightProcess(ValueS set, QString& msg)
	{
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
			gpWt230->getPower(WT230_CH3, cmdSet.values.is);

			msg += QStringLiteral("DC负载%0A ").arg(cmdSet.values.is);

			if (_checkSet(cmdSet.values) == false)
			{
				return false;
			}
		}

		return true;
	}

	bool _aclProcess(CmdSet& cmdSet, QString& msg)
	{
		if (cmdSet.type == SET)
		{

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
				else
				{
					gpChroma63800->setPower(cmdSet.values.so);
				}
				
				gpChroma63800->setInput("ON");
			}
		}
		else if (cmdSet.type == CHECK)
		{
			if (cmdSet.cmd.right(1) == "A")
			{

			}
			else
			{
				gpWt230->getPower(WT230_CH3, cmdSet.values.is);
				msg += QStringLiteral("AC负载%0W ").arg(cmdSet.values.is);
			}
			
			if (_checkSet(cmdSet.values) == false)
			{
				return false;
			}
		}

		return true;
	}

	bool _acsProcess(ValueS values)
	{
		gpDmc1380->SetOutput(8 + 16, 1);

		if (values.so == "OFF")
		{
			gpDmc1380->SetOutput(8 + 16, 1);
			return true;
		}

		float curValue(0.0);
		float setValue = values.so.toFloat();
		
		for (size_t i = 0; i < 100; i++)
		{
			curValue = _getAcSource();

			if (fabs(curValue - setValue) < 3)
			{
				break;
			}

			while (!gpDmc1380->CheckDone(0))
			{
				Sleep(300);
			}
			
			gpDmc1380->SetPos(0,0);
			gpDmc1380->RelMove(0, 500 * (setValue - curValue) / fabs(curValue - setValue), 5000);	
		}

		values.is = QString::number(_getAcSource());
		
		if (_checkSet(values) == false)
		{
			return false;
		}

		gpDmc1380->SetOutput(8 + 16, 0);

		return true;
	}

	bool _cmdParse(CmdSet& cmdSet, QString& msg)
	{
		QSettings settings(gExePath + "/cfg/paraHardware.ini", QSettings::IniFormat);

		if (cmdSet.cmd == "DCV")
		{
			if (cmdSet.type == SET)
			{
				gpChroma62000H->setVoltage(cmdSet.values.so);
				gpChroma62000H->confOutput(true);
				Sleep(500);
				gpChroma62000H->setVoltage(QString::number(2 * cmdSet.values.so.toFloat() - _getDcVolt()));
				 
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
			if (_aclProcess(cmdSet, msg) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "DCL")
		{
			if (_dclProcess(cmdSet, msg) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "ACV")
		{
			cmdSet.values.is = QString::number(_getAcVolt());;
			msg += QStringLiteral("AC电压%0V ").arg(cmdSet.values.is);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "ACF")
		{
			cmdSet.values.is = QString::number(_getAcFreq());
			msg += QStringLiteral("AC频率%0 ").arg(cmdSet.values.is);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "ACS")
		{
			if (_acsProcess(cmdSet.values) == false)
			{
				goto Error;
			}
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
			if (delay > 3000)
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
			auto tLed = _getLedString();
 		
			for (size_t i = 0; i < 3; i++)
			{
				if (tLed.isEmpty())
				{
					tLed = _getLedString();
				}
				else
				{
					break;
				}
			}
			

			msg += QStringLiteral("LED:%0 ").arg(tLed);		

			cmdSet.values.is = tLed;
		
			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}			
		}
		else if (cmdSet.cmd == "DCC")
		{
			float tDcc = _getDcCurr();
			msg += QStringLiteral("DCC:%0 ").arg(tDcc);
			cmdSet.values.is = tDcc;

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "LED-DCC")
		{
			float tLed = _getLedValue();
			float tDcc = _getDcCurr();

			msg += QStringLiteral("LED:%0 ").arg(tLed);
			msg += QStringLiteral("DCC:%0 ").arg(tDcc);

			cmdSet.values.is = QString::number(qAbs(tLed - tDcc), 'f', 0);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
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
		else if (cmdSet.cmd == "LIGHT")
		{
			if (_lightProcess(cmdSet.values, msg) == false)
			{
				goto Error;
			}
		}
		else if (cmdSet.cmd == "FAN")
		{
			float tFan = _getFanValue();
			msg += QStringLiteral("FAN:%0 ").arg(tFan);
			cmdSet.values.is = QString("%1").arg(tFan);

			if (_checkSet(cmdSet.values) == false)
			{
				goto Error;
			}
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
			if (_cmdParse(tCmdSet, msg) == false)
			{
				dataList.append(tCmdSet.values.is);
				return false;
			}

			dataList.append(tCmdSet.values.is);

			gpSignal->textSignal(measureItem, msg);
			gpSignal->colorSignal(measureItem, QColor(0, 255, 0), 1);
		}

		return true;
	}
};









