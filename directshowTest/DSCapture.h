#include "stdafx.h"
#include "SampleCGB.h"
#include <atlbase.h>
#import "qedit.dll" raw_interfaces_only named_guids

EXTERN_C const CLSID CLSID_NullRenderer;
EXTERN_C const CLSID CLSID_SampleGrabber;

class DSCapture
{
public:
	DSCapture();
	~DSCapture();
	static DSCapture& instance();
	BOOL BuildPreviewGraph();
	BOOL StartPreview();
	BOOL StopPreview();
	void AddDevicesToMenu();
	void ChooseFrameRate(double rate);
	void ErrMsg(LPTSTR szFormat, ...);
	void ChooseDevices(TCHAR *szVideo, TCHAR *szAudio);
	void ChooseDevices(IMoniker *pmVideo, IMoniker *pmAudio);
	BOOL InitCapFilters();
	void FreeCapFilters();
	BOOL MakeBuilder();
	BOOL MakeGraph();
	void IMonRelease(IMoniker *&pm);
	void RemoveDownstream(IBaseFilter *pf);
	void TearDownGraph();
	void ResizeWindow(int w, int h);
	HWND ghwndApp;

	void openCamara(HWND parent);
	void captureImage();

	void MyMethod();

	void setWindowVisble(bool bVisible);
	void OnClose();
	IBaseFilter *pSampleGrabberFilter = NULL;
	DexterLib::ISampleGrabber *pSampleGrabber = NULL;
	IBaseFilter *pNullRenderer = NULL;
	IMediaControl *pMediaControl = NULL;
	char *pBuffer = NULL;

private:
	struct _capstuff
	{
		WCHAR wszCaptureFile[_MAX_PATH];
		WORD wCapFileSize;  // size in Meg
		ISampleCaptureGraphBuilder *pBuilder;
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

		int iVideoInputMenuPos;
		LONG NumberOfVideoInputs;
		HMENU hMenuPopup;
		int iNumVCapDevices;
	} gcap;

	int gnRecurse;
};

