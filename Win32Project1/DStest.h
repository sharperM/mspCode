#include <windows.h>
#include "dshowutil.h"
#include <Vfw.h>
#include <atlbase.h>

#include <gdiplus.h>

#include "SampleCGB.h"

using namespace Gdiplus;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#endif


#define MYFREEMEDIATYPE(mt)    {if ((mt).cbFormat != 0)        \
					{CoTaskMemFree((PVOID)(mt).pbFormat);    \
					(mt).cbFormat = 0;                        \
					(mt).pbFormat = NULL;                    \
					}                                            \
if ((mt).pUnk != NULL)                        \
				{                                            \
				(mt).pUnk->Release();                    \
				(mt).pUnk = NULL;                        \
				}}

class DStest{
private:
	
private:
	//    IplImage * m_pFrame;
	Image* m_image;
	bool m_bConnected;
	int m_nWidth;
	int m_nHeight;
	bool m_bLock;
	bool m_bChanged;
	long m_nBufferSize;

	CComPtr<IGraphBuilder> m_pGraph;
	CComPtr<IBaseFilter> m_pDeviceFilter;
	CComPtr<IMediaControl> m_pMediaControl;
	CComPtr<IBaseFilter> m_pSampleGrabberFilter;
	//CComPtr<ISampleGrabber> m_pSampleGrabber;
	CComPtr<IPin> m_pGrabberInput;
	CComPtr<IPin> m_pGrabberOutput;
	CComPtr<IPin> m_pCameraOutput;
	CComPtr<IMediaEvent> m_pMediaEvent;
	CComPtr<IBaseFilter> m_pNullFilter;
	CComPtr<IPin> m_pNullInputPin;
	ICreateDevEnum *pCreateDevEnum = 0;

	ISampleCaptureGraphBuilder *pBuilder;
	//ISampleGrabber*	psamplebuilder
	IVideoWindow *pVW;
	IMediaEventEx *pME;
	IAMDroppedFrames *pDF;
	IAMVideoCompression *pVC;
	IAMVfwCaptureDialogs *pDlg;
	IAMStreamConfig *pASC;      // for audio cap
	IAMStreamConfig *pVSC;      // for video cap
	IBaseFilter *pRender;
	IBaseFilter *pVCap, *pACap;
	IGraphBuilder *pFg;
	IFileSinkFilter *pSink;
	IConfigAviMux *pConfigAviMux;
	int  iMasterStream;
	BOOL fCaptureGraphBuilt;
	BOOL fPreviewGraphBuilt;
	BOOL fCapturing;
	BOOL fPreviewing;
	BOOL fMPEG2;
	BOOL fCapAudio;
	BOOL fCapCC;
	BOOL fCCAvail;
	BOOL fCapAudioIsRelevant;
	bool fDeviceMenuPopulated;

	IMoniker *rgpmVideoMenu[10];
	IMoniker *rgpmAudioMenu[10];
	IMoniker *pmVideo;
	IMoniker *pmAudio;

	double FrameRate;
	BOOL fWantPreview;
	long lCapStartTime;
	long lCapStopTime;
	WCHAR wachFriendlyName[120];
	BOOL fUseTimeLimit;
	BOOL fUseFrameRate;
	DWORD dwTimeLimit;
	int iFormatDialogPos;
	int iSourceDialogPos;
	int iDisplayDialogPos;
	int iVCapDialogPos;
	int iVCrossbarDialogPos;
	int iTVTunerDialogPos;
	int iACapDialogPos;
	int iACrossbarDialogPos;
	int iTVAudioDialogPos;
	int iVCapCapturePinDialogPos;
	int iVCapPreviewPinDialogPos;
	int iACapCapturePinDialogPos;
	long lDroppedBase;
	long lNotBase;
	BOOL fPreviewFaked;
// 	CCrossbar *pCrossbar;
	int iVideoInputMenuPos;
	LONG NumberOfVideoInputs;
//	HMENU hMenuPopup;
	int iNumVCapDevices;

public:
	bool OpenCamera(int nCamID, bool bDisplayProperties = true, int nWidth = 320, int nHeight = 240);
	void CloseCamera();
//抓取一帧，返回的IplImage不可手动释放！
//返回图像数据的为RGB模式的Top-down(第一个字节为左上角像素)，即IplImage::origin=0(IPL_ORIGIN_TL)
//    IplImage * QueryFrame();
	Image* QueryFrame2();

public:
	DStest();
	~DStest();
	static DStest& instance();
	void initCarema();
	void startCapture();
	void getPic();
	
	HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum);
	void DisplayDeviceInformation(IEnumMoniker *pEnum);
	HWND vfwCapture(HWND parent);
	HRESULT test();

	HRESULT test2(HWND parent);
	void AddDevicesToMenu();
	void IMonRelease(IMoniker *&pm);
	void ChooseDevices(IMoniker *pmVideo, IMoniker *pmAudio, HWND parent);
	void ChooseDevices(HWND parent);
	void FreeCapFilters();
	BOOL InitCapFilters();
	BOOL MakeGraph();
	BOOL MakeBuilder();
	BOOL BuildPreviewGraph(HWND parent);
	BOOL StartPreview();
	BOOL StopPreview();
	void TearDownGraph();
	void RemoveDownstream(IBaseFilter *pf);
 };