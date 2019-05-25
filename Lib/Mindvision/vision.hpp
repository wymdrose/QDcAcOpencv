
#pragma once

#include "Include/CameraDefine.H"
#include "Include/CameraApi.h"
#include <QRect>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include "../mythread.hpp"

/*
#ifdef _WINDOWS_
#undef _WINDOWS_
#endif

#include <afxwin.h>
*/

#ifdef _WIN64
#pragma comment(lib, "Lib/Mindvision/MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif

#define MAX_PATH          260



namespace MachineVision
{
	class MindVision : public QObject
	{
	//	Q_OBJECT
	public:
		MindVision()
		{
	//		connect(&pWorkThread->mThread, &QThread::started, this, &run);
		}
		
		void ProcessSnapRequest(BYTE* pImageData, tSdkFrameHead *pImageHead)
		{
//			if (m_SnapRequest == 0)
				return;
	//		InterlockedDecrement(&m_SnapRequest);

	//		static ULONG s_Index = 0;
	//		CString strFileName;
	//		GetCurrentDirectory(MAX_PATH, strFileName.GetBufferSetLength(MAX_PATH));
	//		strFileName.ReleaseBuffer();
		//	strFileName.AppendFormat("\\Snapshot%u", s_Index++);

		//	CameraSaveImage(m_hCamera, strFileName.GetBuffer(), pImageData, pImageHead, FILE_BMP, 100);
		}

		void run()
		{
			
			tSdkFrameHead 	sFrameInfo;
			BYTE*			pbyBuffer;
			CameraSdkStatus status;

			while (true)
			{
				if (CameraGetImageBufferPriority(m_hCamera, &sFrameInfo, &pbyBuffer, 1000,
					CAMERA_GET_IMAGE_PRIORITY_NEWEST) == CAMERA_STATUS_SUCCESS)
				{
					// The original data obtained will be converted into RGB format data, and meanwhile through the ISP module, the image will be processed with noise reduction, edge enhancement and color correction.
					// Most of our company's cameras, the original data are Bayer format
					status = CameraImageProcess(m_hCamera, pbyBuffer, m_pFrameBuffer, &sFrameInfo); // Continuous mode

					// resolution has changed, then refresh the background
					if (m_sFrInfo.iWidth != sFrameInfo.iWidth || m_sFrInfo.iHeight != sFrameInfo.iHeight)
					{
						m_sFrInfo.iWidth = sFrameInfo.iWidth;
						m_sFrInfo.iHeight = sFrameInfo.iHeight;
					//	InvalidateRect(NULL);
					}

					if (status == CAMERA_STATUS_SUCCESS)
					{
						//Call the SDK package display interface to display the image, you can also m_pFrameBuffer RGB data through other ways, such as directX, OpengGL, and so on.
						CameraImageOverlay(m_hCamera, m_pFrameBuffer, &sFrameInfo);
						CameraDisplayRGB24(m_hCamera, m_pFrameBuffer, &sFrameInfo);

						m_iDispFrameNum++;

						ProcessSnapRequest(m_pFrameBuffer, &sFrameInfo);
					}

					// After successfully calling CameraGetImageBuffer, you have to call CameraReleaseImageBuffer to release the obtained buffer.
					// Otherwise call CameraGetImageBuffer, the program will be suspended, that other threads call CameraReleaseImageBuffer to release the buffer
					CameraReleaseImageBuffer(m_hCamera, pbyBuffer);

					memcpy(&m_sFrInfo, &sFrameInfo, sizeof(tSdkFrameHead));
				}
				
				QApplication::processEvents();
			}
		}

		bool initCamera()
		{
			tSdkCameraDevInfo sCameraList[10];
			int iCameraNums;
			CameraSdkStatus status;
			QRect rect;
			tSdkCameraCapbility sCameraInfo;

			iCameraNums = 10;

			if (CameraEnumerateDevice(sCameraList, &iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
			{
				qDebug() << "No camera was found!";
				return false;
			}

			if ((status = CameraInit(&sCameraList[0], -1, -1, &m_hCamera)) != CAMERA_STATUS_SUCCESS)
			{
				QString msg;
				msg = QString("Failed to init the camera! Error code is %1").arg(status);
				qDebug() << msg + "，Because:" + CameraGetErrorString(status);
				return false;
			}

			CameraGetCapability(m_hCamera, &sCameraInfo);

			m_pFrameBuffer = (unsigned char *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax * 4, 16);

			Q_ASSERT(m_pFrameBuffer);

			if (sCameraInfo.sIspCapacity.bMonoSensor)
				CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);

//			CameraDisplayInit(m_hCamera, (HWND)mPreview->winId());
			
//			mPreview->setGeometry(rect);
			
			CameraSetDisplaySize(m_hCamera, rect.right() - rect.left(), rect.bottom() - rect.top());

//			CameraCreateSettingPage(m_hCamera, GetSafeHwnd(),
//				sCameraList[0].acFriendlyName, CameraSettingPageCallback, (PVOID)this, 0);

#ifdef USE_CALLBACK_GRAB_IMAGE //If you want to use the callback function, define this macro USE_CALLBACK_GRAB_IMAGE
			//Set the callback for image capture
			SDK_TRACE(CameraSetCallbackFunction(m_hCamera, GrabImageCallback, (PVOID)this, NULL), gLanguage ? "设置图像抓取的回调函数" : "Set image grab call back function");
#else
			
			//m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, this, 0, &m_threadID);
			//ASSERT(m_hDispThread);
			//SetThreadPriority(m_hDispThread, THREAD_PRIORITY_HIGHEST);
			
			pWorkThread->start();
#endif
			//Tell the camera begin to sendding image			
			CameraPlay(m_hCamera);		

			//
			CameraSetTriggerMode(m_hCamera, 1);

			return true;
		}

		void snapshot()
		{

		//	CameraPause(m_hCamera);
			CameraPlay(m_hCamera);

			tSdkFrameHead FrameInfo;
			BYTE *pRawBuffer;
			BYTE *pRgbBuffer;
			QString sFileName;
			tSdkImageResolution sImageSize;
			CameraSdkStatus status;
			QString msg;
			memset(&sImageSize, 0, sizeof(tSdkImageResolution));
			sImageSize.iIndex = 0xff;
			// CameraSetResolutionForSnap set the resolution when capturing, and the preview can be the same, can also be different.
			//sImageSize.iIndex = 0xff; sImageSize.iwidth and sImageSize.iHeight are set to 0, indicating the same resolution as when previewing.
			// If you want a separate resolution when capturing, please refer to our Snapshot routine. Or see the SDK manual for a detailed description of the CameraSetResolutionForSnap interface
			CameraSetResolutionForSnap(m_hCamera, &sImageSize);

			//
			CameraSoftTrigger(m_hCamera);

			// CameraSnapToBuffer capture a frame of image data to the buffer, the buffer by the SDK internal application, the successful call, you need
			// !!!!!! Note: CameraSnapToBuffer will switch the resolution to take pictures, which is slower. For real-time processing, use the CameraGetImageBuffer function to take a map or callback function.
			if ((status = CameraSnapToBuffer(m_hCamera, &FrameInfo, &pRawBuffer, 1000)) != CAMERA_STATUS_SUCCESS)
			{
				qDebug() << "Function:[CameraSnapToBuffer] failed!";
				msg = QString("Error Code:%1. Get more information about the error in CameraDefine.h").arg(status);
				qDebug() << msg;
				
				qDebug() << "Snapshot failed,is camera in pause mode?";
				return;
			}
			else
			{
				msg = QString("Function:[%s] SUCCESS!") + "CameraSnapToBuffer";
				qDebug() << msg;
				msg = QString("Description:%s.") + "Capture a image to the buffer in snapshot mode";
				qDebug() << msg;
				qDebug() << " ";

				// After successful capture, save to file
				QString msg;
				char sCurpath[MAX_PATH];
				QString strTime = QTime::currentTime().toString();
				sFileName = QDir::currentPath() + "/Snapshot/a.jpg";
				
				// Apply a buffer, used to convert the raw data obtained to RGB data, and also get the image processing effect
				pRgbBuffer = (BYTE *)CameraAlignMalloc(FrameInfo.iWidth*FrameInfo.iHeight * 4, 16);
				//Process the raw data,and get the return image in RGB format
				CameraImageProcess(m_hCamera, pRawBuffer, pRgbBuffer, &FrameInfo);
				qDebug() << "process the raw data to rgb data";

				//Release the buffer which get from CameraSnapToBuffer or CameraGetImageBuffer
				CameraReleaseImageBuffer(m_hCamera, pRawBuffer);
				qDebug() << "Release the buffer malloced by CameraSnapToBuffer or CameraGetImageBuffer";

				// CameraSaveImage Save the image, here only demonstrates how to save the BMP image. If you need to save into other formats, such as JPG, PNG, RAW, etc.,
				// Please refer to our Snapshot routine. Or see the SDK manual for a detailed description of the CameraSaveImage interface
				if ((status = CameraSaveImage(m_hCamera, const_cast<char*>(sFileName.toStdString().c_str()), pRgbBuffer, &FrameInfo, FILE_BMP, 100)) != CAMERA_STATUS_SUCCESS)
				{
					qDebug() << "Function:[CameraSaveImage] failed!";
					msg = QString("Error Code:%1. Get more information about the error in CameraDefine.h").arg(status);
					qDebug() << msg;
					qDebug() << " ";
					msg = QString("Failed to save image to [%1] ,please check the path").arg(sFileName);
					qDebug() << msg;
				}
				else
				{
					msg = QString("Function:[%s] SUCCESS!") + "CameraSaveImage";
					qDebug() << msg;
					msg = QString("Description:%s.") + "Save the image data in the buffer to the file";
					qDebug() << msg;
					qDebug() << " ";

					msg = QString("Snapshot one image to file:[%1.BMP]").arg(sFileName);
					qDebug() << msg;
				}

				CameraAlignFree(pRgbBuffer);

			}
		}

		std::shared_ptr <Drose::MyThread2> pWorkThread = std::make_shared<Drose::MyThread2>();

	private:
//		CStatic m_cPreview;
		CameraHandle m_hCamera;
		int gLanguage = 0;
		unsigned char * m_pFrameBuffer; // Buffer for converting raw image data to RGB
		tSdkFrameHead m_sFrInfo;
//		LONG m_SnapRequest = 0;
		int m_iDispFrameNum = 0;
	};
};