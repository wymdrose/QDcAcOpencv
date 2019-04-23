// LJIF.h : LJIF.DLL  Main header file 

#pragma once

/*
#ifndef __AFXWIN_H__
	#error 
#endif
*/

#include <winsock.h>

#define EXP __declspec(dllimport)

extern	"C"
{
/// Return Code List
typedef enum {

	RC_OK = 0x0000,				// Completed without errors

	// Communication error from controller notification
	RC_NAK_FIRST	= 0x1000,
	RC_NAK_COMMAND,				// Command error
	RC_NAK_COMMAND_LENGTH,		// Command length error
	RC_NAK_TIMEOUT,				// Time out
	RC_NAK_CHECKSUM,			// Check sum error
	RC_NAK_INVALID_STATE,		// Status error
	RC_NAK_OTHER,				// Other errors
	RC_NAK_LAST		= 0x1fff,

	// Communication DLL error notification
	RC_ERR_OPEN_DEVICE = 0x2000,// Failed to open device
	RC_ERR_NO_DEVICE,			// Device is not open
	RC_ERR_SEND,				// Command sending error
	RC_ERR_RECEIVE,				// Response receiving error
	RC_ERR_TIMEOUT,				// Time out
	RC_ERR_CHECKSUM,			// Check sum error
	RC_ERR_PARAMETER,			// Parameter error
	RC_ERR_NODATA,				// No data
	RC_ERR_NOMEMORY,			// No free memory
	RC_ERR_OPEN_FILE,			// Failed to open file
	RC_ERR_INVALID_FILE,		// Target file does not exist
	RC_ERR_READ_FILE,			// Failed to load file
	RC_ERR_WRITE_FILE,			// Failed to write file

	RC_ERR_DISCONNECT,			// Connection may be disconnected
	RC_ERR_UNKNOWN,				// Undefined error

	RC_NO_GET_HEAD_TYPE	= 0x0100 //Unable to acquire head type information error
} RC;

/// Parameters when opening the Ethernet device
typedef struct {
	IN_ADDR		IPAddress;
} LJIF_OPENPARAM_ETHERNET;

/// Type of initialization
typedef enum {
	LJIF_INITTYPE_PROGRAM				= 0x00,		// Program
	LJIF_INITTYPE_WITHOUT_ENVIRONMENT	= 0x01,		// All except environment settings
	LJIF_INITTYPE_ALL					= 0x02		// All settings
} LJIF_INITTYPE;
/// Target of setting
typedef enum {
	LJIF_SETTINGTARGET_PROGRAM_00	= 0x00,		// Program No.0
	LJIF_SETTINGTARGET_PROGRAM_01	= 0x01,		// Program No.1
	LJIF_SETTINGTARGET_PROGRAM_02	= 0x02,		// Program No.2
	LJIF_SETTINGTARGET_PROGRAM_03	= 0x03,		// Program No.3
	LJIF_SETTINGTARGET_PROGRAM_04	= 0x04,		// Program No.4
	LJIF_SETTINGTARGET_PROGRAM_05	= 0x05,		// Program No.5
	LJIF_SETTINGTARGET_PROGRAM_06	= 0x06,		// Program No.6
	LJIF_SETTINGTARGET_PROGRAM_07	= 0x07,		// Program No.7
	LJIF_SETTINGTARGET_PROGRAM_08	= 0x08,		// Program No.8
	LJIF_SETTINGTARGET_PROGRAM_09	= 0x09,		// Program No.9
	LJIF_SETTINGTARGET_PROGRAM_10	= 0x0A,		// Program No.10
	LJIF_SETTINGTARGET_PROGRAM_11	= 0x0B,		// Program No.11
	LJIF_SETTINGTARGET_PROGRAM_12	= 0x0C,		// Program No.12
	LJIF_SETTINGTARGET_PROGRAM_13	= 0x0D,		// Program No.13
	LJIF_SETTINGTARGET_PROGRAM_14	= 0x0E,		// Program No.14
	LJIF_SETTINGTARGET_PROGRAM_15	= 0x0F,		// Program No.15
	LJIF_SETTINGTARGET_ENVIRONMENT	= 0x00,		// Environment setting
	LJIF_SETTINGTARGET_PROGRAM_ALL	= 0xFF		// All programs
} LJIF_SETTINGTARGET;	
/// Setting type
typedef enum {
	LJIF_TYPE_TRIGGER			= 0x01,	// Trigger setting
	LJIF_TYPE_HEAD				= 0x02,	// Head setting
	LJIF_TYPE_PROFILE			= 0x03,	// Profile setting
	LJIF_TYPE_OUT				= 0x06,	// OUT setting
	LJIF_TYPE_COMMON			= 0x07,	// Common setting
	LJIF_TYPE_ENVIRONMENT		= 0x09	// Environment setting
}LJIF_TYPE_SETTING;
// Trigger setting
typedef enum {	
	LJIF_ITEM_TRIGGER_MODE		= 0x00,	// Trigger mode
	LJIF_ITEM_INTERFERENCE		= 0x01,	// Mutual interference prevention
	LJIF_ITEM_AUTOMATIC			= 0x02,	// Multiple trigger ON/OFF
	LJIF_ITEM_AUTO_SETTING		= 0x03,	// Multiple trigger setting
	LJIF_ITEM_TRIGGER_SYNC		= 0x04,	// Trigger synch
	LJIF_ITEM_ASAP_TRIGGER		= 0x05,	// ASAP Trigger
    LJIF_ITEM_ASAP_MODE			= 0x06	// ASAP Mode
}LJIF_ITEM_TRIGGER;
// Head setting
typedef enum {	
	LJIF_ITEM_MEAS_RANGE_X		= 0x00,	// Measurement range (X direction)
	LJIF_ITEM_MEAS_RANGE_Z_FROM	= 0x01,	// Measurement range (Z direction: 
	LJIF_ITEM_MEAS_RANGE_Z_TO	= 0x02,	// Measurement range (Z direction: Stop)
	LJIF_ITEM_SAMPLING_TIME		= 0x03,	// Sampling time
	LJIF_ITEM_MFL_MODE			= 0x06,	// MFL(AUTO/MANUAL)
	LJIF_ITEM_MFL_MANUAL		= 0x07,	// MFL MANUAL range
	LJIF_ITEM_MFL_TUNING		= 0x08,	// MFL calibration
	LJIF_ITEM_ALARM_TIMES		= 0x09,	// No. of errors
	LJIF_ITEM_ALARM_LEVEL		= 0x0A,	// Alarm level
	LJIF_ITEM_FILTER			= 0x0B	// Filter
}LJIF_ITEM_HEAD;
// Profile setting
typedef enum {	
	LJIF_ITEM_CALCULATION		= 0x04,	// Calculation
	LJIF_ITEM_DELAY_COUNT		= 0x05,	// Delay count
	LJIF_ITEM_SMOOTHING			= 0x06,	// Smoothing
	LJIF_ITEM_AVERAGING			= 0x07	// Averaging
}LJIF_ITEM_PROFILE;
// OUT setting
typedef enum {	
	LJIF_ITEM_ALARM			= 0x03,	// Measurement value alarm
	LJIF_ITEM_SCALING		= 0x04,	// Scaling setting
	LJIF_ITEM_AVERAGE		= 0x05,	// Average measurement value
	LJIF_ITEM_MODE			= 0x06,	// Measurement mode
	LJIF_ITEM_OFFSET		= 0x07,	// Offset
	LJIF_ITEM_TOLERANCE		= 0x08,	// Tolerance
	LJIF_ITEM_UNIT			= 0x09	// Minimum display unit
}LJIF_ITEM_OUT;
// Common setting
typedef enum {						
	LJIF_ITEM_TIMING			= 0x00,	// TIMING	
	LJIF_ITEM_ZERO				= 0x01,	// ZERO	
	LJIF_ITEM_BINARY			= 0x02,	// Binary output	
	LJIF_ITEM_ANA_CH			= 0x03,	// Analog output CH setting
	LJIF_ITEM_ANA_SCALING		= 0x04,	// Analog output scaling
	LJIF_ITEM_ANA_CYCLE			= 0x05,	// Analog output profile update cycle	
	LJIF_ITEM_STORAGE			= 0x06,	// Storage setting	
	LJIF_ITEM_DATA_STORAGE		= 0x09,	// Data storage setting	
	LJIF_ITEM_PROFILE_STORAGE 	= 0x0A	// Profile storage setting
}LJIF_ITEM_COMMON;
// Environment setting
typedef enum {	
	LJIF_ITEM_ETHERNET_IP_ADDRESS	= 0x03,	// Ethernet IP address 
	LJIF_ITEM_ETHERNET_SUBNET_MASK	= 0x04,	// Ethernet subnet mask
	LJIF_ITEM_ETHERNET_GATEWAY		= 0x05,	// Ethernet gateway
	LJIF_ITEM_STROBE_CYCLE			= 0x07,	// Strobe time
	LJIF_ITEM_LANGUAGE				= 0x08,	// Language setting
	LJIF_ITEM_CABLE_EXTEND_MODE		= 0x0A	// Cable extension mode
}LJIF_ITEM_ENVIRONMENT;

/// The file No. to use for the new program
typedef enum {
 LJIF_FILE_CONFIG_00 = 0x00,  // File No.00
 LJIF_FILE_CONFIG_01 = 0x01,  // File No.01
 LJIF_FILE_CONFIG_02 = 0x02,  // File No.02
 LJIF_FILE_CONFIG_03 = 0x03,  // File No.03
 LJIF_FILE_CONFIG_04 = 0x04,  // File No.04
 LJIF_FILE_CONFIG_05 = 0x05,  // File No.05
 LJIF_FILE_CONFIG_06 = 0x06,  // File No.06
 LJIF_FILE_CONFIG_07 = 0x07,  // File No.07
 LJIF_FILE_CONFIG_08 = 0x08,  // File No.08
 LJIF_FILE_CONFIG_09 = 0x09   // File No.09
} LJIF_FILETARGET;
/// The screen to display
typedef enum {
	LJIF_DISPLAY_MEASURE_ALL = 0x00,  // Number, Multiple screens
	LJIF_DISPLAY_MEASURE_OUTNO_1  = 0x01,  //Number, Single screen with OUT1
	LJIF_DISPLAY_MEASURE_OUTNO_2  = 0x02,  //Number, Single screen with OUT2
	LJIF_DISPLAY_MEASURE_OUTNO_3  = 0x03,  //Number, Single screen with OUT3
	LJIF_DISPLAY_MEASURE_OUTNO_4  = 0x04,  //Number, Single screen with OUT4
	LJIF_DISPLAY_MEASURE_OUTNO_5  = 0x05,  //Number, Single screen with OUT5
	LJIF_DISPLAY_MEASURE_OUTNO_6  = 0x06,  //Number, Single screen with OUT6
	LJIF_DISPLAY_MEASURE_OUTNO_7  = 0x07,  //Number, Single screen with OUT7
	LJIF_DISPLAY_MEASURE_OUTNO_8  = 0x08,  //Number, Single screen with OUT8
	LJIF_DISPLAY_PROFILE_HEAD_A   = 0x0A,  //Profile display - Head A    
	LJIF_DISPLAY_PROFILE_HEAD_B   = 0x0B,  //Profile display - Head B
	LJIF_DISPLAY_PROFILE_CALC     = 0x0C   //Profile display - After calculation
}LJIF_DISPLAY_TARGET;
/// The target head
typedef enum {
	LJIF_HEAD_A		= 0x00,		// Head A
	LJIF_HEAD_B		= 0x01,		// Head B
	LJIF_HEAD_BOTH	= 0x02,		// Both heads
	LJIF_HEAD_CALC	= 0x03		// After calculation
} LJIF_HEAD;
/// OUT setting
typedef enum {
	LJIF_OUTNO_1		= 0x01,		// OUT1
	LJIF_OUTNO_2		= 0x02,		// OUT2
	LJIF_OUTNO_3		= 0x04,		// OUT3
	LJIF_OUTNO_4		= 0x08,		// OUT4
	LJIF_OUTNO_5		= 0x10,		// OUT5
	LJIF_OUTNO_6		= 0x20,		// OUT6
	LJIF_OUTNO_7		= 0x40,		// OUT7
	LJIF_OUTNO_8		= 0x80,		// OUT8
	LJIF_OUTNO_ALL		= 0xff		// All OUT
} LJIF_OUTNO;
//Measurement value structure
typedef enum {
	LJIF_FLOATRESULT_VALID,			// Valid data
	LJIF_FLOATRESULT_RANGEOVER_P,	// Overrange at the plus side
	LJIF_FLOATRESULT_RANGEOVER_N,	// Overrange at the minus side
	LJIF_FLOATRESULT_WAITING		// Comparator standby
} LJIF_FLOATRESULT;
typedef struct {
	LJIF_FLOATRESULT FloatResult; // Indicates whether data is valid
	float fValue;				  // Measurement value when the value is "LJIF_FLOATRESULT_VALID". Indicates an invalid value in other situations.
} LJIF_MEASUREDATA;
//Measurement value2 structure
typedef enum {    
	LJIF_GO,				// GO
	LJIF_HI,				// HI
	LJIF_LO,				// LO
	LJIF_WAITINGRESULT		// WAITING
} LJIF_MEASURERESULT;
typedef struct {    
	LJIF_FLOATRESULT FloatResult;		// Indicates whether data is valid
	LJIF_MEASURERESULT MeasureResult;	// GO/HI/LO/WAITING
	float fValue;						// Measurement value when the value is "LJIF_FLOATRESULT_VALID". Indicates an invalid value in other situations.
} LJIF_MEASUREDATAEX;
//X coordinate information structure
typedef struct {
	float fXStart;	//Starting point for the first point
	float fXPitch;	//X pitch for the data
}LJIF_PROFILE_INFO;
/// Time structure
typedef struct {
	WORD						wYear;						// Year 2000-2099
	WORD						wMonth;						// Month, 1/12 (Month/Day)
	WORD						wDayOfWeek;					// Day of the week 0-6 (Sunday - Saturday)
	WORD						wDay;						// Day 1/31 (Month/Day)
	WORD						wHour;						// Hour 0-23
	WORD						wMinute;					// Minute 0-59
	WORD						wSecond;					// Second 0-59
	WORD						wMillisecond;				// Millisecond
} LJIF_STIME;
//Data storage structure 
typedef struct {
	DWORD						dwDateTime;					// Data and time for storing the measurement value
	LJIF_MEASUREDATA			MeasureData;				// Measurement value
} LJIF_DATA_STORAGE;
/// The profile to acquire
typedef enum {
	LJIF_PROFILETARGET_HEAD_A	= 0x00,		// Head A
	LJIF_PROFILETARGET_HEAD_B	= 0x01,		// Head B
	LJIF_PROFILETARGET_HEAD_BOTH = 0x02,	// Both heads
	LJIF_PROFILETARGET_CALC		= 0x03,		// After calculation
} LJIF_PROFILETARGET;
/// Get Storage Status
typedef struct {
	LONG						lSetting;					// Storage setting
	LONG						lStorageNum;				// Amount of storage data
	LONG						lStatus;					// Storage status
} LJIF_GET_STORAGE_STATUS;

/// Device	/////////////////////////////////////////////////////////////////////////

/// USB Open Device 
EXP RC  WINAPI LJIF_OpenDeviceUSB();
/// Ether Open Device 
EXP RC WINAPI LJIF_OpenDeviceETHER(IN LJIF_OPENPARAM_ETHERNET* pOpenParam);
/// Closes the currently open device
EXP RC WINAPI LJIF_CloseDevice();

/// Command/////////////////////////////////////////////////////////////////////////

/// Get Head Type Information 
EXP RC WINAPI LJIF_GetHeadType(OUT BYTE* pHeadTypeA, IN int nCountHeadA, OUT BYTE* pHeadTypeB, IN int nCountHeadB);
/// Move to Measurement Mode 
EXP RC WINAPI LJIF_SetRunMode(IN BOOL bNotSaveSetting);
/// Move to Communication Mode 
EXP RC WINAPI LJIF_SetCommMode();
/// Move to Storage Mode 
EXP RC WINAPI LJIF_SetStorageMode();
/// Initialize Settings 
EXP RC WINAPI LJIF_InitSetting(IN LJIF_INITTYPE InitType, IN LJIF_SETTINGTARGET InitTarget);
/// Load Program File
EXP RC WINAPI LJIF_LoadProgramFile(IN LPCTSTR szFilePath);
/// Save Program File 	
EXP RC WINAPI LJIF_SaveProgramFile(IN LPCTSTR szFilePath);
/// Load Environment Setting File 			
EXP RC WINAPI LJIF_LoadEnvironmentFile(IN LPCTSTR szFilePath);
/// Save Environment Setting File 				
EXP RC WINAPI LJIF_SaveEnvironmentFile(IN LPCTSTR szFilePath);
/// Set Specified Parameter 
EXP RC WINAPI LJIF_SetSettingItem(IN LJIF_TYPE_SETTING nType , IN int nItem, IN int nTarget, IN void* pData, IN int nSizeOfData);
/// Get Specified Parameter 
EXP RC WINAPI LJIF_GetSettingItem(IN LJIF_TYPE_SETTING nType, IN int nItem, IN int nTarget, OUT void* pData, IN int nSizeOfData);
/// Copy Program 
EXP RC WINAPI LJIF_CopyProgram(IN LJIF_SETTINGTARGET SettingFrom, IN LJIF_SETTINGTARGET SettingTo);
/// Change Program 
EXP RC WINAPI LJIF_ChangeProgram(IN LJIF_SETTINGTARGET SettingTarget);
///Change Program with Saved Program in the CF 
EXP RC WINAPI LJIF_ChangeProgramInCF(IN LJIF_FILETARGET FileTarget, IN LJIF_SETTINGTARGET SettingTarget);
///Get Active Program No.
EXP RC WINAPI LJIF_GetActiveProgramNo(OUT BYTE* pData);
/// Change Measurement Screen 
EXP RC WINAPI LJIF_ChangeDisplay(IN LJIF_DISPLAY_TARGET DisplayTarget);
/// Set Keylock 
EXP RC WINAPI LJIF_SetKeyLock(IN BOOL bIsOn);
/// Trigger 
EXP RC WINAPI LJIF_SetTrigger(IN LJIF_HEAD Head);
/// Auto Zero
EXP RC WINAPI LJIF_SetAutoZero(IN BOOL bIsOn, IN LJIF_OUTNO Outno);
/// Timing
EXP RC WINAPI LJIF_SetTiming(IN BOOL bIsOn, IN LJIF_OUTNO Outno);
/// Reset
EXP RC WINAPI LJIF_SetReset();
/// Get Measurement Value
EXP RC WINAPI LJIF_GetMeasureValue(OUT LJIF_MEASUREDATA* pData, IN int nCount);
/// Get Measurement Value2
EXP RC WINAPI LJIF_GetMeasureValueEx(OUT LJIF_MEASUREDATAEX* pData, IN int nCount);
/// Get Profile Data
EXP RC WINAPI LJIF_GetProfileData(IN LJIF_PROFILETARGET ProfileTarget, OUT LJIF_PROFILE_INFO* pProfInfo, OUT float* pProfile, IN int nCount);
/// Get Precalculation Profile Data 
EXP RC WINAPI LJIF_GetPrecalcProfileData(IN LJIF_PROFILETARGET ProfileTarget, OUT LJIF_PROFILE_INFO* pProfInfo, OUT float* pProfile, IN int nCount);
/// Clear Storage Data 
EXP RC WINAPI LJIF_ClearStorageData();
/// Get Data from Data Storage
EXP RC WINAPI LJIF_GetDataStorage(IN LJIF_OUTNO Outno, OUT int *pDataCount, OUT LJIF_STIME* pTime, OUT LJIF_DATA_STORAGE* pData, IN int nCount);
/// Get Data from Profile Storage
EXP RC WINAPI LJIF_GetProfileStorage(IN LJIF_PROFILETARGET ProfileTarget, IN int nIndex, OUT LJIF_STIME* pTime, OUT LJIF_PROFILE_INFO* pProfInfo, OUT LJIF_MEASUREDATA* pData , IN int nCountOfData , OUT float* pProfile, IN int nCountOfProfile);
/// Start Storage
EXP RC WINAPI LJIF_StartStorage();
/// Stop Storage
EXP RC WINAPI LJIF_StopStorage();
/// Get Storage Status
EXP RC WINAPI LJIF_GetStorageStatus(OUT LJIF_GET_STORAGE_STATUS* pData);
/// Save Storage Data
EXP RC WINAPI LJIF_SaveStorage();

}//extern	"C"
