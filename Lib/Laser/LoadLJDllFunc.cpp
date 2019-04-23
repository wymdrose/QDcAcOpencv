//Copyright(c) 2014, KEYENCE Corporation. All rights reserved.
/** @file	
@brief LoadLJDllFunc.cpp
*/

//#include "StdAfx.h"
#include "LoadLJDllFunc.h"
#include <qdebug.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif



CLoadLJDllFunc::CLoadLJDllFunc(void)
{

	hDLL = LoadLibrary(L"LJIF.dll");

	auto a = ::GetLastError();

	if (hDLL != NULL)
	{
		LJIF_OpenDeviceUSB	       = ( RC (__stdcall *)() )GetProcAddress(hDLL, "LJIF_OpenDeviceUSB");
		LJIF_OpenDeviceETHER       = ( RC (__stdcall *)(LJIF_OPENPARAM_ETHERNET* pOpenParam) )GetProcAddress(hDLL, "LJIF_OpenDeviceETHER");
		LJIF_CloseDevice	       = ( RC (__stdcall *)() )GetProcAddress(hDLL, "LJIF_CloseDevice");

		LJIF_GetHeadType           = ( RC (__stdcall *)(BYTE* pHeadTypeA, int nCountHeadA, BYTE* pHeadTypeB, int nCountHeadB))GetProcAddress(hDLL, "LJIF_GetHeadType");
		LJIF_SetRunMode            = ( RC (__stdcall *)(BOOL bNotSaveSetting))GetProcAddress(hDLL, "LJIF_SetRunMode");
		LJIF_SetCommMode           = ( RC (__stdcall *)())GetProcAddress(hDLL, "LJIF_SetCommMode");
		LJIF_SetStorageMode        = ( RC (__stdcall *)())GetProcAddress(hDLL, "LJIF_SetStorageMode");
		LJIF_InitSetting           = ( RC (__stdcall *)(LJIF_INITTYPE InitType, LJIF_SETTINGTARGET InitTarget))GetProcAddress(hDLL, "LJIF_InitSetting");
		LJIF_LoadProgramFile       = ( RC (__stdcall *)(LPCTSTR szFilePath))GetProcAddress(hDLL, "LJIF_LoadProgramFile");
		LJIF_SaveProgramFile       = ( RC (__stdcall *)(LPCTSTR szFilePath))GetProcAddress(hDLL, "LJIF_SaveProgramFile");
		LJIF_LoadEnvironmentFile   = ( RC (__stdcall *)(LPCTSTR szFilePath))GetProcAddress(hDLL, "LJIF_LoadEnvironmentFile");
		LJIF_SaveEnvironmentFile   = ( RC (__stdcall *)(LPCTSTR szFilePath))GetProcAddress(hDLL, "LJIF_SaveEnvironmentFile");
		LJIF_SetSettingItem        = ( RC (__stdcall *)(LJIF_TYPE_SETTING nType , int nItem, int nTarget, void* pData, int nSizeOfData))GetProcAddress(hDLL, "LJIF_SetSettingItem");
		LJIF_GetSettingItem        = ( RC (__stdcall *)(LJIF_TYPE_SETTING nType, int nItem, int nTarget, void* pData, int nSizeOfData))GetProcAddress(hDLL, "LJIF_GetSettingItem");
		LJIF_CopyProgram           = ( RC (__stdcall *)(LJIF_SETTINGTARGET SettingFrom, LJIF_SETTINGTARGET SettingTo))GetProcAddress(hDLL, "LJIF_CopyProgram");
		LJIF_ChangeProgram         = ( RC (__stdcall *)(LJIF_SETTINGTARGET SettingTarget))GetProcAddress(hDLL, "LJIF_ChangeProgram");
		LJIF_ChangeProgramInCF     = ( RC (__stdcall *)(LJIF_FILETARGET FileTarget, LJIF_SETTINGTARGET SettingTarget))GetProcAddress(hDLL, "LJIF_ChangeProgramInCF");
		LJIF_GetActiveProgramNo    = ( RC (__stdcall *)(BYTE* pData))GetProcAddress(hDLL, "LJIF_GetActiveProgramNo");
		LJIF_ChangeDisplay         = ( RC (__stdcall *)(LJIF_DISPLAY_TARGET DisplayTarget))GetProcAddress(hDLL, "LJIF_ChangeDisplay");
		LJIF_SetKeyLock            = ( RC (__stdcall *)(BOOL bIsOn))GetProcAddress(hDLL, "LJIF_SetKeyLock");
		LJIF_SetTrigger            = ( RC (__stdcall *)(LJIF_HEAD Head))GetProcAddress(hDLL, "LJIF_SetTrigger");
		LJIF_SetAutoZero           = ( RC (__stdcall *)(BOOL bIsOn, LJIF_OUTNO Outno))GetProcAddress(hDLL, "LJIF_SetAutoZero");
		LJIF_SetTiming             = ( RC (__stdcall *)(BOOL bIsOn, LJIF_OUTNO Outno))GetProcAddress(hDLL, "LJIF_SetTiming");
		LJIF_SetReset              = ( RC (__stdcall *)())GetProcAddress(hDLL, "LJIF_SetReset");
		LJIF_GetMeasureValue       = ( RC (__stdcall *)(LJIF_MEASUREDATA* pData, int nCount))GetProcAddress(hDLL, "LJIF_GetMeasureValue");
		LJIF_GetMeasureValueEx     = ( RC (__stdcall *)(LJIF_MEASUREDATAEX* pData, int nCount))GetProcAddress(hDLL, "LJIF_GetMeasureValueEx");
		LJIF_GetProfileData        = ( RC (__stdcall *)(LJIF_PROFILETARGET ProfileTarget, LJIF_PROFILE_INFO* pProfInfo, float* pProfile, int nCount))GetProcAddress(hDLL, "LJIF_GetProfileData");
		LJIF_GetPrecalcProfileData = ( RC (__stdcall *)(LJIF_PROFILETARGET ProfileTarget, LJIF_PROFILE_INFO* pProfInfo, float* pProfile, int nCount))GetProcAddress(hDLL, "LJIF_GetPrecalcProfileData");
		LJIF_ClearStorageData      = ( RC (__stdcall *)())GetProcAddress(hDLL, "LJIF_ClearStorageData");
		LJIF_GetDataStorage        = ( RC (__stdcall *)(LJIF_OUTNO Outno, int *pDataCount, LJIF_STIME* pTime, LJIF_DATA_STORAGE* pData, int nCount))GetProcAddress(hDLL, "LJIF_GetDataStorage");
		LJIF_GetProfileStorage     = ( RC (__stdcall *)(LJIF_PROFILETARGET ProfileTarget, int nIndex, LJIF_STIME* pTime, LJIF_PROFILE_INFO* pProfInfo, LJIF_MEASUREDATA* pData , int nCountOfData , float* pProfile, int nCountOfProfile))GetProcAddress(hDLL, "LJIF_GetProfileStorage");
		LJIF_StartStorage          = ( RC (__stdcall *)())GetProcAddress(hDLL, "LJIF_StartStorage");
		LJIF_StopStorage           = ( RC (__stdcall *)())GetProcAddress(hDLL, "LJIF_StopStorage");
		LJIF_GetStorageStatus      = ( RC (__stdcall *)(LJIF_GET_STORAGE_STATUS* pData))GetProcAddress(hDLL, "LJIF_GetStorageStatus");
		LJIF_SaveStorage           = ( RC (__stdcall *)())GetProcAddress(hDLL, "LJIF_SaveStorage");
	}
	else
	{
		qDebug() << "Error: LoadLibrary: LJIF.dll " << "\r";
	}
}

CLoadLJDllFunc::~CLoadLJDllFunc(void)
{
	FreeLibrary(hDLL);
}

CLoadLJDllFunc* CLoadLJDllFunc::GetInstance(void)
{
	static CLoadLJDllFunc m_loadLJDllFunc;
	return &m_loadLJDllFunc;
}