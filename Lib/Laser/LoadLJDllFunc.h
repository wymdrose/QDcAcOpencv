//Copyright(c) 2014, KEYENCE Corporation. All rights reserved.
/** @file	
@brief LoadLJDllFunc.h
*/

#pragma once
#include "LJIF.h"
#include <QLibrary>

class CLoadLJDllFunc
{
private:
	HINSTANCE hDLL;		// Handle to Dll

public:
	CLoadLJDllFunc(void);
	~CLoadLJDllFunc(void);
	static CLoadLJDllFunc* GetInstance(void);
	void GetProcAddressAll();

	// Function pointer
	RC (__stdcall *LJIF_OpenDeviceUSB)();
	RC (__stdcall *LJIF_OpenDeviceETHER)(LJIF_OPENPARAM_ETHERNET* pOpenParam);
	RC (__stdcall *LJIF_CloseDevice)();
	
	RC (__stdcall *LJIF_GetHeadType)(BYTE* pHeadTypeA, int nCountHeadA, BYTE* pHeadTypeB, int nCountHeadB);
	RC (__stdcall *LJIF_SetRunMode)(BOOL bNotSaveSetting);
	RC (__stdcall *LJIF_SetCommMode)();
	RC (__stdcall *LJIF_SetStorageMode)();
	RC (__stdcall *LJIF_InitSetting)(LJIF_INITTYPE InitType, LJIF_SETTINGTARGET InitTarget);
	RC (__stdcall *LJIF_LoadProgramFile)(LPCTSTR szFilePath);
	RC (__stdcall *LJIF_SaveProgramFile)(LPCTSTR szFilePath);
	RC (__stdcall *LJIF_LoadEnvironmentFile)(LPCTSTR szFilePath);
	RC (__stdcall *LJIF_SaveEnvironmentFile)(LPCTSTR szFilePath);
	RC (__stdcall *LJIF_SetSettingItem)(LJIF_TYPE_SETTING nType , int nItem, int nTarget, void* pData, int nSizeOfData);
	RC (__stdcall *LJIF_GetSettingItem)(LJIF_TYPE_SETTING nType, int nItem, int nTarget, void* pData, int nSizeOfData);
	RC (__stdcall *LJIF_CopyProgram)(LJIF_SETTINGTARGET SettingFrom, LJIF_SETTINGTARGET SettingTo);
	RC (__stdcall *LJIF_ChangeProgram)(LJIF_SETTINGTARGET SettingTarget);
	RC (__stdcall *LJIF_ChangeProgramInCF)(LJIF_FILETARGET FileTarget, LJIF_SETTINGTARGET SettingTarget);
	RC (__stdcall *LJIF_GetActiveProgramNo)(BYTE* pData);
	RC (__stdcall *LJIF_ChangeDisplay)(LJIF_DISPLAY_TARGET DisplayTarget);
	RC (__stdcall *LJIF_SetKeyLock)(BOOL bIsOn);
	RC (__stdcall *LJIF_SetTrigger)(LJIF_HEAD Head);
	RC (__stdcall *LJIF_SetAutoZero)(BOOL bIsOn, LJIF_OUTNO Outno);
	RC (__stdcall *LJIF_SetTiming)(BOOL bIsOn, LJIF_OUTNO Outno);
	RC (__stdcall *LJIF_SetReset)();
	RC (__stdcall *LJIF_GetMeasureValue)(LJIF_MEASUREDATA* pData, int nCount);
	RC (__stdcall *LJIF_GetMeasureValueEx)(LJIF_MEASUREDATAEX* pData, int nCount);
	RC (__stdcall *LJIF_GetProfileData)(LJIF_PROFILETARGET ProfileTarget, LJIF_PROFILE_INFO* pProfInfo, float* pProfile, int nCount);
	RC (__stdcall *LJIF_GetPrecalcProfileData)(LJIF_PROFILETARGET ProfileTarget, LJIF_PROFILE_INFO* pProfInfo, float* pProfile, int nCount);
	RC (__stdcall *LJIF_ClearStorageData)();
	RC (__stdcall *LJIF_GetDataStorage)(LJIF_OUTNO Outno, int *pDataCount, LJIF_STIME* pTime, LJIF_DATA_STORAGE* pData, int nCount);
	RC (__stdcall *LJIF_GetProfileStorage)(LJIF_PROFILETARGET ProfileTarget, int nIndex, LJIF_STIME* pTime, LJIF_PROFILE_INFO* pProfInfo, LJIF_MEASUREDATA* pData , int nCountOfData , float* pProfile, int nCountOfProfile);
	RC (__stdcall *LJIF_StartStorage)();
	RC (__stdcall *LJIF_StopStorage)();
	RC (__stdcall *LJIF_GetStorageStatus)(LJIF_GET_STORAGE_STATUS* pData);
	RC (__stdcall *LJIF_SaveStorage)();
};
