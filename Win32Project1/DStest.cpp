#include "stdafx.h"
#include "DStest.h"
#include<set>
#include <assert.h>

#pragma comment(lib, "strmiids")
#pragma comment(lib, "Vfw32.lib")
DStest::DStest()
: pBuilder(NULL), pVW(NULL), pME(NULL), pDF(NULL), pVC(NULL), pDlg(NULL),
pASC(NULL), pVSC(NULL), pRender(NULL), pVCap(NULL), pACap(NULL), pFg(NULL),
pSink(NULL), pConfigAviMux(NULL), pmAudio(NULL), pmVideo(NULL), 
fDeviceMenuPopulated(FALSE), iNumVCapDevices(0)
{
	m_bConnected = false;
	m_nWidth = 0;
	m_nHeight = 0;
	m_bLock = false;
	m_bChanged = false;
	//    m_pFrame = NULL;
	m_nBufferSize = 0;
	m_pNullFilter = NULL;
	m_pMediaEvent = NULL;
	m_pSampleGrabberFilter = NULL;
	m_pGraph = NULL;

	ZeroMemory(rgpmVideoMenu, sizeof(rgpmVideoMenu));
	ZeroMemory(rgpmAudioMenu, sizeof(rgpmAudioMenu));

	CoInitialize(NULL);
}
DStest::~DStest()
{
	CloseCamera();
	CoUninitialize();
}

void DStest::initCarema()
{

}

void DStest::startCapture()
{

}

void DStest::getPic()
{

}


HRESULT DStest::EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
{
	// Create the System Device Enumerator.
	ICreateDevEnum *pDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the category.
		hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
		}
		pDevEnum->Release();
	}
	return hr;
}


void DStest::DisplayDeviceInformation(IEnumMoniker *pEnum)
{
	IMoniker *pMoniker = NULL;

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		IPropertyBag *pPropBag;
		HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;
		}

		VARIANT var;
		VariantInit(&var);

		// Get description or friendly name.
		hr = pPropBag->Read(L"Description", &var, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
		}
		if (SUCCEEDED(hr))
		{
			printf("%S\n", var.bstrVal);
			VariantClear(&var);
		}

		hr = pPropBag->Write(L"FriendlyName", &var);

		// WaveInID applies only to audio capture devices.
		hr = pPropBag->Read(L"WaveInID", &var, 0);
		if (SUCCEEDED(hr))
		{
			printf("WaveIn ID: %d\n", var.lVal);
			VariantClear(&var);
		}

		hr = pPropBag->Read(L"DevicePath", &var, 0);
		if (SUCCEEDED(hr))
		{
			// The device path is not intended for display.
			printf("Device path: %S\n", var.bstrVal);
			VariantClear(&var);
		}

		pPropBag->Release();
		pMoniker->Release();
	}
}

HWND DStest::vfwCapture(HWND parent)
{
	HWND hWndC = capCreateCaptureWindow(
		"My Capture Window",								// 如果是Pop-up窗口的窗口名称
		WS_CHILD | WS_VISIBLE,								// 窗口类型
		0, 0, 160, 120,									// 窗口位置和尺寸
		parent,
		12345);

	while (capDriverConnect(hWndC, 0) != TRUE)				//Window Vista和Windows 7系统以上，需要不断尝试连接（不知为何）
	{
		continue;
	}

	CAPDRIVERCAPS CapDriverCaps = {};
	CAPSTATUS     CapStatus = {};

	capDriverGetCaps(hWndC, &CapDriverCaps, sizeof(CAPDRIVERCAPS));

	// Video source dialog box. 
	if (CapDriverCaps.fHasDlgVideoSource)
	{
		capDlgVideoSource(hWndC);
	}

	// Video format dialog box. 
	if (CapDriverCaps.fHasDlgVideoFormat)
	{
		capDlgVideoFormat(hWndC);

		// Are there new image dimensions?
		capGetStatus(hWndC, &CapStatus, sizeof (CAPSTATUS));

		// If so, notify the parent of a size change.
	}

	// Video display dialog box. 
	if (CapDriverCaps.fHasDlgVideoDisplay)
	{
		capDlgVideoDisplay(hWndC);
	}

	capGetStatus(hWndC, &CapStatus, sizeof (CAPSTATUS));

	SetWindowPos(hWndC, NULL, 0, 0, CapStatus.uiImageWidth,
		CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);
	SetWindowPos(parent, NULL, 0, 0, CapStatus.uiImageWidth,
		CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);

	LPBITMAPINFO lpbi;
	DWORD dwSize;

	dwSize = capGetVideoFormatSize(hWndC);
	lpbi = (LPBITMAPINFO)GlobalAllocPtr(GHND, dwSize);
	capGetVideoFormat(hWndC, lpbi, dwSize);



	CAPTUREPARMS CapParms;
	capCaptureGetSetup(hWndC, &CapParms, sizeof(CAPTUREPARMS));
	//设置帧数
	CapParms.fLimitEnabled = FALSE;
	//是否捕捉音频
	CapParms.fCaptureAudio = FALSE;
	//MCI Device支持
	CapParms.fMCIControl = TRUE;
	//设置窗口，如果为false，捕捉画面在桌面上
	CapParms.fYield = TRUE;
	//停止捕捉键设置
	CapParms.vKeyAbort = VK_ESCAPE;
	CapParms.fAbortLeftMouse = true;
	CapParms.fAbortRightMouse = FALSE;
	capCaptureGetSetup(hWndC, &CapParms, sizeof(CAPTUREPARMS));
	//设置预览时的比例
	capPreviewScale(hWndC, 66);
	//是否支持比例变化
	capPreviewScale(hWndC, FALSE);
	std::set<int, std::less<int>> aa;
	aa.insert(11);
	capPreviewRate(hWndC, 600);
	capPreview(hWndC, TRUE);

	return hWndC;

}

HRESULT DStest::test()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		IEnumMoniker *pEnum;
		ICaptureGraphBuilder2 *pCaptureGraph = NULL;
		IGraphBuilder *pGraph = NULL;                // Graph builder object

		IMediaControl *pControl = NULL;              // Media control object

		IFileSinkFilter *pSink = NULL;               // File sink object

		IBaseFilter *pAudioInputFilter = NULL;       // Audio Capture filter

		IBaseFilter *pVideoInputFilter = NULL;       // Video Capture filter

		IBaseFilter *pASFWriter = NULL;              // WM ASF File filter

		
		hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
		if (SUCCEEDED(hr))
		{
		 	DisplayDeviceInformation(pEnum);
		 	pEnum->Release();
		}
		hr = EnumerateDevices(CLSID_AudioInputDeviceCategory, &pEnum);
		if (SUCCEEDED(hr))
		{
		 	DisplayDeviceInformation(pEnum);
		 	pEnum->Release();
		}
		 
		 
		hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
		 		CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2,
		 		(void **)&pCaptureGraph);
		// 		if (FAILED(hr))
		// 		{
		// 			return hr;
		// 		}
		// 
		// 		hr = pCaptureGraph->SetOutputFileName(&MEDIASUBTYPE_Asf,
		// 
		// 		                                       L"C:\\MyWebcam.ASF", &pASFWriter, &pSink);
		// 
		// 
		// 
		// 
		// 		// Now configure the ASF Writer.
		// 
		// 		// Present the property pages for this filter.
		// 
		// 		 hr = ShowFilterPropertyPages(pASFWriter);
		// 
		// 		
		// 
		// 			  // Now get the Filter Graph Manager
		// 
		// 			  // that was created by the Capture Graph Builder.
		// 
		// 			  hr = pCaptureGraph->GetFiltergraph(&pGraph);
		// 
		// 		
		// 
		// 			  // Using QueryInterface on the graph builder,
		// 
		// 			  // get the Media Control object.
		// 
		// 			  hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
		// 
		// 		 if (FAILED(hr))
		// 
		// 		 {
		// 
		// 			 printf("ERROR - Could not create the Media Control object.");
		// 
		// 			 pGraph->Release();        // Clean up after ourselves
		// 
		// 			 CoUninitialize();         // And uninitalize COM
		// 
		// 			 return hr;
		// 
		// 		 }
		// 
		// 	
		// 
		// 			 // Get an audio capture filter.
		// 
		// 			 // There are several to choose from,
		// 
		// 			 // so we need to enumerate them, pick one, and
		// 
		// 			 // then add the audio capture filter to the filter graph.
		// 
		// 			 hr = GetAudioInputFilter(&pAudioInputFilter, L"Logitech");
		// 
		// 	  if (SUCCEEDED(hr)) {
		// 
		// 			   hr = pGraph->AddFilter(pAudioInputFilter, L"Webcam Audio Capture");
		// 
		// 			
		// 		}
		// 
		// 		
		// 			 // Now create the video input filter from the webcam.
		// 
		// 			 hr = GetVideoInputFilter(&pVideoInputFilter, L"Logitech");
		// 
		// 		 if (SUCCEEDED(hr)) {
		// 
		// 			   hr = pGraph->AddFilter(pVideoInputFilter, L"Webcam Video Capture");
		// 
		// 			
		// 		}
		// 
		// 		
		// 
		// 		 // Use another method of the Capture Graph Builder
		// 
		// 		 // to provide a render path for video preview.
		// 
		// 		 IBaseFilter *pIntermediate = NULL;
		// 
		// 		 hr = pCaptureGraph->RenderStream(&PIN_CATEGORY_PREVIEW,
		// 
		// 		                                  &MEDIATYPE_Video, pVideoInputFilter, NULL, NULL);
		// 
		// 		
		// 
		// 		 // Now add the video capture to the output file.
		// 
		// 		 hr = pCaptureGraph->RenderStream(&PIN_CATEGORY_CAPTURE,
		// 
		// 		                                   &MEDIATYPE_Video, pVideoInputFilter, NULL, pASFWriter);
		// 
		// 		
		// 
		// 		  // And do the same for the audio.
		// 
		// 		  hr = pCaptureGraph->RenderStream(&PIN_CATEGORY_CAPTURE,
		// 
		// 		                                   &MEDIATYPE_Audio, pAudioInputFilter, NULL, pASFWriter);
		// 		  if (SUCCEEDED(hr))
		// 
		// 		  {
		// 
		// 			  // Run the graph.
		// 
		// 			      hr = pControl->Run();
		// 
		// 			  if (SUCCEEDED(hr))
		// 
		// 				   {
		// 
		// 				     // Wait patiently for completion of the recording.
		// 
		// 				      wprintf(L"Started recording...press Enter to stop recording.\n");
		// 
		// 				
		// 
		// 			     // Wait for completion.
		// 
		// 			     char ch;
		// 
		// 			     ch = getchar(); // We wait for keyboard input
		// 
		// 			   }
		// 
		// 			
		// 
		// 			   // And stop the filter graph.
		// 
		// 			   hr = pControl->Stop();
		// 
		// 			   wprintf(L"Stopped recording.\n"); // To the console
		// 
		// 			
		// 
		// 			    // Before we finish, save the filter graph to a file.
		// 
		// 			    SaveGraphFile(pGraph, L"C:\\MyGraph.GRF");
		// 
		// 			  }
		// 
		// 			
		// 
		// 			 // Now release everything and clean up.
		// 
		// 			 pSink->Release();
		// 
		// 			 pASFWriter->Release();
		// 
		// 			 pVideoRenderer->Release();
		// 
		// 			 pVideoInputFilter->Release();
		// 
		// 			 pAudioInputFilter->Release();
		// 
		// 			 pControl->Release();
		// 
		// 			 pGraph->Release();
		// 
		// 			 pCaptureGraph->Release();
		// 
		// 			 CoUninitialize();
		// 

		

	}
	return 0;
}

void DStest::CloseCamera()
{
	if (m_bConnected)
		m_pMediaControl->Stop();

	m_pGraph = NULL;
	m_pDeviceFilter = NULL;
	m_pMediaControl = NULL;
	m_pSampleGrabberFilter = NULL;
	m_pGrabberInput = NULL;
	m_pGrabberOutput = NULL;
	m_pCameraOutput = NULL;
	m_pMediaEvent = NULL;
	m_pNullFilter = NULL;
	m_pNullInputPin = NULL;


	//if (m_pFrame)
	//    cvReleaseImage(&m_pFrame);
	if (!this->m_image)
	{
		;
	}

	m_bConnected = false;
	m_nWidth = 0;
	m_nHeight = 0;
	m_bLock = false;
	m_bChanged = false;
	m_nBufferSize = 0;
}

bool DStest::OpenCamera(int nCamID, bool bDisplayProperties /*= true*/, int nWidth /*= 320*/, int nHeight /*= 240*/)
{

	HRESULT hr = S_OK;

	CoInitialize(NULL);
	// Create the Filter Graph Manager.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void**)&m_pGraph);

// 	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
// 		IID_IBaseFilter, (LPVOID *)&m_pSampleGrabberFilter);
	
	hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
	hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void**)&m_pMediaEvent);

// 	hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
// 		IID_IBaseFilter, (LPVOID*)&m_pNullFilter);


	hr = m_pGraph->AddFilter(m_pNullFilter, L"NullRenderer");

// 	hr = m_pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&m_pSampleGrabber);

	AM_MEDIA_TYPE   mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;
	mt.formattype = FORMAT_VideoInfo;
// 	hr = m_pSampleGrabber->SetMediaType(&mt);
	MYFREEMEDIATYPE(mt);

	m_pGraph->AddFilter(m_pSampleGrabberFilter, L"Grabber");

	// Bind Device Filter.  We know the device because the id was passed in
// 	BindFilter(nCamID, &m_pDeviceFilter);
	m_pGraph->AddFilter(m_pDeviceFilter, NULL);

	CComPtr<IEnumPins> pEnum;
	m_pDeviceFilter->EnumPins(&pEnum);

	hr = pEnum->Reset();
	hr = pEnum->Next(1, &m_pCameraOutput, NULL);

	pEnum = NULL;
	m_pSampleGrabberFilter->EnumPins(&pEnum);
	pEnum->Reset();
	hr = pEnum->Next(1, &m_pGrabberInput, NULL);

	pEnum = NULL;
	m_pSampleGrabberFilter->EnumPins(&pEnum);
	pEnum->Reset();
	pEnum->Skip(1);
	hr = pEnum->Next(1, &m_pGrabberOutput, NULL);

	pEnum = NULL;
	m_pNullFilter->EnumPins(&pEnum);
	pEnum->Reset();
	hr = pEnum->Next(1, &m_pNullInputPin, NULL);

	//SetCrossBar();

	if (bDisplayProperties)
	{
		CComPtr<ISpecifyPropertyPages> pPages;

		HRESULT hr = m_pCameraOutput->QueryInterface(IID_ISpecifyPropertyPages, (void**)&pPages);
		if (SUCCEEDED(hr))
		{
			PIN_INFO PinInfo;
			m_pCameraOutput->QueryPinInfo(&PinInfo);

			CAUUID caGUID;
			pPages->GetPages(&caGUID);

			OleCreatePropertyFrame(NULL, 0, 0,
				L"Property Sheet", 1,
				(IUnknown **)&(m_pCameraOutput.p),
				caGUID.cElems,
				caGUID.pElems,
				0, 0, NULL);
			CoTaskMemFree(caGUID.pElems);
			PinInfo.pFilter->Release();
		}
		pPages = NULL;
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////////
		// 加入由 lWidth和lHeight设置的摄像头的宽和高 的功能，默认320*240
		// by flymanbox @2009-01-24
		//////////////////////////////////////////////////////////////////////////////
		int _Width = nWidth, _Height = nHeight;
		IAMStreamConfig*   iconfig;
		iconfig = NULL;
		hr = m_pCameraOutput->QueryInterface(IID_IAMStreamConfig, (void**)&iconfig);

		AM_MEDIA_TYPE* pmt;
		if (iconfig->GetFormat(&pmt) != S_OK)
		{
			//printf("GetFormat Failed ! \n");
			return false;
		}

		VIDEOINFOHEADER*   phead;
		if (pmt->formattype == FORMAT_VideoInfo)
		{
			phead = (VIDEOINFOHEADER*)pmt->pbFormat;
			phead->bmiHeader.biWidth = _Width;
			phead->bmiHeader.biHeight = _Height;
			if ((hr = iconfig->SetFormat(pmt)) != S_OK)
			{
				return false;
			}

		}

		iconfig->Release();
		iconfig = NULL;
		MYFREEMEDIATYPE(*pmt);
	}

	hr = m_pGraph->Connect(m_pCameraOutput, m_pGrabberInput);
	hr = m_pGraph->Connect(m_pGrabberOutput, m_pNullInputPin);

	if (FAILED(hr))
	{
		switch (hr)
		{
		case VFW_S_NOPREVIEWPIN:
			break;
		case E_FAIL:
			break;
		case E_INVALIDARG:
			break;
		case E_POINTER:
			break;
		}
	}

// 	m_pSampleGrabber->SetBufferSamples(TRUE);
// 	m_pSampleGrabber->SetOneShot(TRUE);
// 
// 	hr = m_pSampleGrabber->GetConnectedMediaType(&mt);
	if (FAILED(hr))
		return false;

	VIDEOINFOHEADER *videoHeader;
	videoHeader = reinterpret_cast<VIDEOINFOHEADER*>(mt.pbFormat);
	m_nWidth = videoHeader->bmiHeader.biWidth;
	m_nHeight = videoHeader->bmiHeader.biHeight;
	m_bConnected = true;

	pEnum = NULL;
	return true;

}

HRESULT DStest::test2()
{
	HRESULT hr;
	return 0;
}
//枚举设备 填充 rgpmAudioMenu，rgpmVideoMenu
void DStest::AddDevicesToMenu()
{
	if (fDeviceMenuPopulated)
	{
		return;
	}
	fDeviceMenuPopulated = true;
	iNumVCapDevices = 0;

	UINT    uIndex = 0;
	HRESULT hr;
	BOOL bCheck = FALSE;
	

	for (int i = 0; i < NUMELMS(rgpmVideoMenu); i++)
	{
		IMonRelease(rgpmVideoMenu[i]);
	}
	for (int i = 0; i < NUMELMS(rgpmAudioMenu); i++)
	{
		IMonRelease(rgpmAudioMenu[i]);
	}

	// enumerate all video capture devices
	ICreateDevEnum *pCreateDevEnum = 0;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)
	{
		OutputDebugString(TEXT("Error Creating Device Enumerator"));
		return;
	}

	IEnumMoniker *pEm = 0;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)
	{
		OutputDebugString(TEXT("Sorry, you have no video capture hardware.\r\n\r\n")
			TEXT("Video capture will not function properly."));
		goto EnumAudio;
	}

	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;

	while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
	{
		IPropertyBag *pBag = 0;

		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if (SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{

				if (pmVideo != 0 && (S_OK == pmVideo->IsEqual(pM)))
					bCheck = TRUE;

				bCheck = FALSE;

				SysFreeString(var.bstrVal);

				assert(rgpmVideoMenu[uIndex] == 0);
				rgpmVideoMenu[uIndex] = pM;
				pM->AddRef();
			}
			pBag->Release();
		}

		pM->Release();
		uIndex++;
	}
	pEm->Release();

	iNumVCapDevices = uIndex;


EnumAudio:

	// enumerate all audio capture devices
	uIndex = 0;
	bCheck = FALSE;

	assert(pCreateDevEnum != NULL);

	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEm, 0);
	pCreateDevEnum->Release();
	if (hr != NOERROR)
		return;
	pEm->Reset();

	while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if (SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{
				if (pmAudio != 0 && (S_OK == pmAudio->IsEqual(pM)))
					bCheck = TRUE;

				bCheck = FALSE;

				SysFreeString(var.bstrVal);

				assert(rgpmAudioMenu[uIndex] == 0);
				rgpmAudioMenu[uIndex] = pM;
				pM->AddRef();
			}
			pBag->Release();
		}
		pM->Release();
		uIndex++;
	}

	pEm->Release();
}

void DStest::IMonRelease(IMoniker *&pm)
{
	if (pm)
	{
		pm->Release();
		pm = 0;
	}
}

void DStest::ChooseDevices(IMoniker *pmVideo, IMoniker *pmAudio)
{
#define VERSIZE 40
#define DESCSIZE 80

	int versize = VERSIZE;
	int descsize = DESCSIZE;
	WCHAR wachVer[VERSIZE] = { 0 }, wachDesc[DESCSIZE] = { 0 };
	TCHAR tachStatus[VERSIZE + DESCSIZE + 5] = { 0 };


	// they chose a new device. rebuild the graphs
	if (this->pmVideo != pmVideo || this->pmAudio != pmAudio)
	{
		if (pmVideo)
		{
			pmVideo->AddRef();
		}
		if (pmAudio)
		{
			pmAudio->AddRef();
		}

		IMonRelease(pmVideo);
		IMonRelease(pmAudio);
		this->pmVideo = pmVideo;
		this->pmAudio = pmAudio;

	 	if (fPreviewing)
	 		StopPreview();
		if (fCaptureGraphBuilt || fPreviewGraphBuilt)
			TearDownGraph();

		FreeCapFilters();
		InitCapFilters();

		if (fWantPreview)   // were we previewing?
		{
			//BuildPreviewGraph();
			StartPreview();
		}

		//MakeMenuOptions();      // the UI choices change per device
	}

	// Set the check marks for the devices menu.


	// Put the video driver name in the status bar - if the filter supports
	// IAMVideoCompression::GetInfo, that's the best way to get the name and
	// the version.  Otherwise use the name we got from device enumeration
	// as a fallback.
	if (pVC)
	{
		HRESULT hr = pVC->GetInfo(wachVer, &versize, wachDesc, &descsize,
			NULL, NULL, NULL, NULL);
		if (hr == S_OK)
		{
			// It's possible that the call succeeded without actually filling
			// in information for description and version.  If these strings
			// are empty, just display the device's friendly name.
			if (wcslen(wachDesc) && wcslen(wachVer))
			{
				hr = StringCchPrintf(tachStatus, VERSIZE + DESCSIZE + 5, TEXT("%s - %s\0"), wachDesc, wachVer);
				//statusUpdateStatus(ghwndStatus, tachStatus);
				return;
			}
		}
	}

	// Since the GetInfo method failed (or the interface did not exist),
	// display the device's friendly name.
	//statusUpdateStatus(ghwndStatus, wachFriendlyName);
}


// all done with the capture filters and the graph builder
//
void DStest::FreeCapFilters()
{
	SAFE_RELEASE(pFg);
 	if (pBuilder)
 	{//todo
 		delete pBuilder;
 		pBuilder = NULL;
 	}
	SAFE_RELEASE(pVCap);
	SAFE_RELEASE(pACap);
	SAFE_RELEASE(pASC);
	SAFE_RELEASE(pVSC);
	SAFE_RELEASE(pVC);
	SAFE_RELEASE(pDlg);

// 	if (pCrossbar)
// 	{
// 		delete pCrossbar;
// 		pCrossbar = NULL;
// 	}
}

// create the capture filters of the graph.  We need to keep them loaded from
// the beginning, so we can set parameters on them and have them remembered
//
BOOL DStest::InitCapFilters()
{
	HRESULT hr = S_OK;
	BOOL f;

	fCCAvail = FALSE;  // assume no closed captioning support

	f = MakeBuilder();
	if (!f)
	{
		OutputDebugString(TEXT("Cannot instantiate graph builder"));
		return FALSE;
	}

	//
	// First, we need a Video Capture filter, and some interfaces
	//
	pVCap = NULL;

	if (pmVideo != 0)
	{
		IPropertyBag *pBag;
		wachFriendlyName[0] = 0;

		hr = pmVideo->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if (SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;

			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{
				hr = StringCchCopyW(wachFriendlyName, sizeof(wachFriendlyName) / sizeof(wachFriendlyName[0]), var.bstrVal);
				SysFreeString(var.bstrVal);
			}

			pBag->Release();
		}

		hr = pmVideo->BindToObject(0, 0, IID_IBaseFilter, (void**)&pVCap);
	}

	if (pVCap == NULL)
	{
		////ErrMsg(TEXT("Error %x: Cannot create video capture filter"), hr);
		goto InitCapFiltersFail;
	}

	//
	// make a filtergraph, give it to the graph builder and put the video
	// capture filter in the graph
	//

	f = MakeGraph();
	if (!f)
	{
		////ErrMsg(TEXT("Cannot instantiate filtergraph"));
		goto InitCapFiltersFail;
	}

	hr = pBuilder->SetFiltergraph(pFg);
	if (hr != NOERROR)
	{
		////ErrMsg(TEXT("Cannot give graph to builder"));
		goto InitCapFiltersFail;
	}

	// Add the video capture filter to the graph with its friendly name
	hr = pFg->AddFilter(pVCap, wachFriendlyName);
	if (hr != NOERROR)
	{
		////ErrMsg(TEXT("Error %x: Cannot add vidcap to filtergraph"), hr);
		goto InitCapFiltersFail;
	}

	// Calling FindInterface below will result in building the upstream
	// section of the capture graph (any WDM TVTuners or Crossbars we might
	// need).

	// we use this interface to get the name of the driver
	// Don't worry if it doesn't work:  This interface may not be available
	// until the pin is connected, or it may not be available at all.
	// (eg: interface may not be available for some DV capture)
	hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Interleaved, pVCap,
		IID_IAMVideoCompression, (void **)&pVC);
	if (hr != S_OK)
	{
		hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
			&MEDIATYPE_Video, pVCap,
			IID_IAMVideoCompression, (void **)&pVC);
	}

	// !!! What if this interface isn't supported?
	// we use this interface to set the frame rate and get the capture size
	hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Interleaved,
		pVCap, IID_IAMStreamConfig, (void **)&pVSC);

	if (hr != NOERROR)
	{
		hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
			&MEDIATYPE_Video, pVCap,
			IID_IAMStreamConfig, (void **)&pVSC);
		if (hr != NOERROR)
		{
			// this means we can't set frame rate (non-DV only)
			////ErrMsg(TEXT("Error %x: Cannot find VCapture:IAMStreamConfig"), hr);
		}
	}

	fCapAudioIsRelevant = TRUE;

	AM_MEDIA_TYPE *pmt;

	// default capture format
	if (pVSC && pVSC->GetFormat(&pmt) == S_OK)
	{
		// DV capture does not use a VIDEOINFOHEADER
		if (pmt->formattype == FORMAT_VideoInfo)
		{
			pmt->majortype;
			// resize our window to the default capture size
// 			ResizeWindow(HEADER(pmt->pbFormat)->biWidth,
// 				ABS(HEADER(pmt->pbFormat)->biHeight));
		}
		if (pmt->majortype != MEDIATYPE_Video)
		{
			// This capture filter captures something other that pure video.
			// Maybe it's DV or something?  Anyway, chances are we shouldn't
			// allow capturing audio separately, since our video capture
			// filter may have audio combined in it already!
			fCapAudioIsRelevant = FALSE;
			fCapAudio = FALSE;
		}
		//DeleteMediaType(pmt);
	}

	// we use this interface to bring up the 3 dialogs
	// NOTE:  Only the VfW capture filter supports this.  This app only brings
	// up dialogs for legacy VfW capture drivers, since only those have dialogs
	hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Video, pVCap,
		IID_IAMVfwCaptureDialogs, (void **)&pDlg);

	// Use the crossbar class to help us sort out all the possible video inputs
	// The class needs to be given the capture filters ANALOGVIDEO input pin
	{
		IPin        *pP = 0;
		IEnumPins   *pins = 0;
		ULONG        n;
		PIN_INFO     pinInfo;
		BOOL         Found = FALSE;
		IKsPropertySet *pKs = 0;
		GUID guid;
		DWORD dw;
		BOOL fMatch = FALSE;

		//pCrossbar = NULL;

		if (SUCCEEDED(pVCap->EnumPins(&pins)))
		{
			while (!Found && (S_OK == pins->Next(1, &pP, &n)))
			{
				if (S_OK == pP->QueryPinInfo(&pinInfo))
				{
					if (pinInfo.dir == PINDIR_INPUT)
					{
						// is this pin an ANALOGVIDEOIN input pin?
						if (pP->QueryInterface(IID_IKsPropertySet,
							(void **)&pKs) == S_OK)
						{
							if (pKs->Get(AMPROPSETID_Pin,
								AMPROPERTY_PIN_CATEGORY, NULL, 0,
								&guid, sizeof(GUID), &dw) == S_OK)
							{
								if (guid == PIN_CATEGORY_ANALOGVIDEOIN)
									fMatch = TRUE;
							}
							pKs->Release();
						}

						if (fMatch)
						{
							HRESULT hrCreate = S_OK;
// 							pCrossbar = new CCrossbar(pP, &hrCreate);
// 							if (!pCrossbar || FAILED(hrCreate))
// 								break;
// 
// 							hr = pCrossbar->GetInputCount(&NumberOfVideoInputs);
							Found = TRUE;
						}
					}
					pinInfo.pFilter->Release();
				}
				pP->Release();
			}
			pins->Release();
		}
	}

	// there's no point making an audio capture filter
	if (fCapAudioIsRelevant == FALSE)
		goto SkipAudio;

	// create the audio capture filter, even if we are not capturing audio right
	// now, so we have all the filters around all the time.

	//
	// We want an audio capture filter and some interfaces
	//

	if (pmAudio == 0)
	{
		// there are no audio capture devices. We'll only allow video capture
		fCapAudio = FALSE;
		goto SkipAudio;
	}
	pACap = NULL;


	hr = pmAudio->BindToObject(0, 0, IID_IBaseFilter, (void**)&pACap);

	if (pACap == NULL)
	{
		// there are no audio capture devices. We'll only allow video capture
		fCapAudio = FALSE;
		////ErrMsg(TEXT("Cannot create audio capture filter"));
		goto SkipAudio;
	}

	//
	// put the audio capture filter in the graph
	//
	{
		WCHAR wachAudioFriendlyName[256];
		IPropertyBag *pBag;

		wachAudioFriendlyName[0] = 0;

		// Read the friendly name of the filter to assist with remote graph
		// viewing through GraphEdit
		hr = pmAudio->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if (SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;

			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{
				hr = StringCchCopyW(wachAudioFriendlyName, 256, var.bstrVal);
				SysFreeString(var.bstrVal);
			}

			pBag->Release();
		}

		// We'll need this in the graph to get audio property pages
		hr = pFg->AddFilter(pACap, wachAudioFriendlyName);
		if (hr != NOERROR)
		{
			////ErrMsg(TEXT("Error %x: Cannot add audio capture filter to filtergraph"), hr);
			goto InitCapFiltersFail;
		}
	}

	// Calling FindInterface below will result in building the upstream
	// section of the capture graph (any WDM TVAudio's or Crossbars we might
	// need).

	// !!! What if this interface isn't supported?
	// we use this interface to set the captured wave format
	hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, pACap,
		IID_IAMStreamConfig, (void **)&pASC);

	if (hr != NOERROR)
	{
		////ErrMsg(TEXT("Cannot find ACapture:IAMStreamConfig"));
	}

SkipAudio:

	// Can this filter do closed captioning?
	IPin *pPin;
	hr = pBuilder->FindPin(pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_VBI,
		NULL, FALSE, 0, &pPin);
	if (hr != S_OK)
		hr = pBuilder->FindPin(pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_CC,
		NULL, FALSE, 0, &pPin);
	if (hr == S_OK)
	{
		pPin->Release();
		fCCAvail = TRUE;
	}
	else
	{
		fCapCC = FALSE;    // can't capture it, then
	}

	// potential debug output - what the graph looks like
	// DumpGraph(pFg, 1);

	return TRUE;

InitCapFiltersFail:
	FreeCapFilters();
	return FALSE;
}


// Make a graph builder object we can use for capture graph building
//
BOOL DStest::MakeBuilder()
{
	// we have one already
	if (pBuilder)
		return TRUE;

 	pBuilder = new ISampleCaptureGraphBuilder();
	if (NULL == pBuilder)
	{
		return FALSE;
	}

	return TRUE;
}


// Make a graph object we can use for capture graph building
//
BOOL DStest::MakeGraph()
{
	// we have one already
	if (pFg)
		return TRUE;

	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (LPVOID *)&pFg);

	return (hr == NOERROR) ? TRUE : FALSE;
}



// build the preview graph!
//
// !!! PLEASE NOTE !!!  Some new WDM devices have totally separate capture
// and preview settings.  An application that wishes to preview and then
// capture may have to set the preview pin format using IAMStreamConfig on the
// preview pin, and then again on the capture pin to capture with that format.
// In this sample app, there is a separate page to set the settings on the
// capture pin and one for the preview pin.  To avoid the user
// having to enter the same settings in 2 dialog boxes, an app can have its own
// UI for choosing a format (the possible formats can be enumerated using
// IAMStreamConfig) and then the app can programmatically call IAMStreamConfig
// to set the format on both pins.
//
BOOL DStest::BuildPreviewGraph(HWND parent)
{
	int cy, cyBorder;
	HRESULT hr;
	AM_MEDIA_TYPE *pmt;

	// we have one already
	if (fPreviewGraphBuilt)
		return TRUE;

	// No rebuilding while we're running
	if (fCapturing || fPreviewing)
		return FALSE;

	// We don't have the necessary capture filters
	if (pVCap == NULL)
		return FALSE;
	if (pACap == NULL && fCapAudio)
		return FALSE;

	// we already have another graph built... tear down the old one
	if (fCaptureGraphBuilt)
		TearDownGraph();

	//
	// Render the preview pin - even if there is not preview pin, the capture
	// graph builder will use a smart tee filter and provide a preview.
	//
	// !!! what about latency/buffer issues?

	// NOTE that we try to render the interleaved pin before the video pin, because
	// if BOTH exist, it's a DV filter and the only way to get the audio is to use
	// the interleaved pin.  Using the Video pin on a DV filter is only useful if
	// you don't want the audio.

	if (fMPEG2)
	{
		hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
			&MEDIATYPE_Stream, pVCap, NULL, NULL);
		if (FAILED(hr))
		{
			//ErrMsg(TEXT("Cannot build MPEG2 preview graph!"));
		}

	}
	else
	{
		hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
			&MEDIATYPE_Interleaved, pVCap, NULL, NULL);
		if (hr == VFW_S_NOPREVIEWPIN)
		{
			// preview was faked up for us using the (only) capture pin
			fPreviewFaked = TRUE;
		}
		else if (hr != S_OK)
		{
			// maybe it's DV?
			hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
				&MEDIATYPE_Video, pVCap, NULL, NULL);
			if (hr == VFW_S_NOPREVIEWPIN)
			{
				// preview was faked up for us using the (only) capture pin
				fPreviewFaked = TRUE;
			}
			else if (hr != S_OK)
			{
				//ErrMsg(TEXT("This graph cannot preview!"));
				fPreviewGraphBuilt = FALSE;
				return FALSE;
			}
		}

		//
		// Render the closed captioning pin? It could be a CC or a VBI category pin,
		// depending on the capture driver
		//

		if (fCapCC)
		{
			hr = pBuilder->RenderStream(&PIN_CATEGORY_CC, NULL,
				pVCap, NULL, NULL);
			if (hr != NOERROR)
			{
				hr = pBuilder->RenderStream(&PIN_CATEGORY_VBI, NULL,
					pVCap, NULL, NULL);
				if (hr != NOERROR)
				{
					//ErrMsg(TEXT("Cannot render closed captioning"));
				}
			}
		}
	}

	//
	// Get the preview window to be a child of our app's window
	//

	// This will find the IVideoWindow interface on the renderer.  It is
	// important to ask the filtergraph for this interface... do NOT use
	// ICaptureGraphBuilder2::FindInterface, because the filtergraph needs to
	// know we own the window so it can give us display changed messages, etc.

	hr = pFg->QueryInterface(IID_IVideoWindow, (void **)&pVW);
	if (hr != NOERROR)
	{
		//ErrMsg(TEXT("This graph cannot preview properly"));
	}
	else
	{
		//Find out if this is a DV stream
		AM_MEDIA_TYPE * pmtDV;

		if (pVSC && SUCCEEDED(pVSC->GetFormat(&pmtDV)))
		{
			if (pmtDV->formattype == FORMAT_DvInfo)
			{
				// in this case we want to set the size of the parent window to that of
				// current DV resolution.
				// We get that resolution from the IVideoWindow.
				CComPtr<IBasicVideo> pBV;

				// If we got here, pVW is not NULL 
				assert(pVW != NULL);
				hr = pVW->QueryInterface(IID_IBasicVideo, (void**)&pBV);

				if (SUCCEEDED(hr))
				{
					HRESULT hr1, hr2;
					long lWidth, lHeight;

					hr1 = pBV->get_VideoHeight(&lHeight);
					hr2 = pBV->get_VideoWidth(&lWidth);
					if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
					{
						//ResizeWindow(lWidth, abs(lHeight));
					}
				}
			}
		}

		RECT rc;
		pVW->put_Owner((OAHWND)parent);    // We own the window now
		pVW->put_WindowStyle(WS_CHILD);    // you are now a child

		// give the preview window all our space but where the status bar is
		GetClientRect(parent, &rc);
		cyBorder = GetSystemMetrics(SM_CYBORDER);
// 		cy = statusGetHeight() + cyBorder;
		cy = cyBorder;
		rc.bottom -= cy;

		pVW->SetWindowPosition(0, 0, rc.right, rc.bottom); // be this big
		pVW->put_Visible(OATRUE);
	}

	// now tell it what frame rate to capture at.  Just find the format it
	// is capturing with, and leave everything alone but change the frame rate
	// No big deal if it fails.  It's just for preview
	// !!! Should we then talk to the preview pin?
	if (pVSC && fUseFrameRate)
	{
		hr = pVSC->GetFormat(&pmt);

		// DV capture does not use a VIDEOINFOHEADER
		if (hr == NOERROR)
		{
			if (pmt->formattype == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
				pvi->AvgTimePerFrame = (LONGLONG)(10000000 / FrameRate);

				hr = pVSC->SetFormat(pmt);
				if (hr != NOERROR)
					;
					//ErrMsg(TEXT("%x: Cannot set frame rate for preview"), hr);
			}
			//DeleteMediaType(pmt);
		}
	}

	// make sure we process events while we're previewing!
	hr = pFg->QueryInterface(IID_IMediaEventEx, (void **)&pME);
	if (hr == NOERROR)
	{
		//pME->SetNotifyWindow((OAHWND)ghwndApp, WM_FGNOTIFY, 0);
	}

	// potential debug output - what the graph looks like
	// DumpGraph(pFg, 1);

	// Add our graph to the running object table, which will allow
	// the GraphEdit application to "spy" on our graph
#ifdef REGISTER_FILTERGRAPH
	hr = AddGraphToRot(pFg, &g_dwGraphRegister);
	if (FAILED(hr))
	{
		//ErrMsg(TEXT("Failed to register filter graph with ROT!  hr=0x%x"), hr);
		g_dwGraphRegister = 0;
	}
#endif

	// All done.
	fPreviewGraphBuilt = TRUE;
	return TRUE;
}


// Start previewing
//
BOOL DStest::StartPreview()
{
	// way ahead of you
	if (fPreviewing)
		return TRUE;

	if (!fPreviewGraphBuilt)
		return FALSE;

	// run the graph
	IMediaControl *pMC = NULL;
	HRESULT hr = pFg->QueryInterface(IID_IMediaControl, (void **)&pMC);
	if (SUCCEEDED(hr))
	{
		hr = pMC->Run();
		if (FAILED(hr))
		{
			// stop parts that ran
			pMC->Stop();
		}
		pMC->Release();
	}
	if (FAILED(hr))
	{
		//ErrMsg(TEXT("Error %x: Cannot run preview graph"), hr);
		return FALSE;
	}

	fPreviewing = TRUE;
	return TRUE;
}


// stop the preview graph
//
BOOL DStest::StopPreview()
{
	// way ahead of you
	if (!fPreviewing)
	{
		return FALSE;
	}

	// stop the graph
	IMediaControl *pMC = NULL;
	HRESULT hr = pFg->QueryInterface(IID_IMediaControl, (void **)&pMC);
	if (SUCCEEDED(hr))
	{
		hr = pMC->Stop();
		pMC->Release();
	}
	if (FAILED(hr))
	{
		//ErrMsg(TEXT("Error %x: Cannot stop preview graph"), hr);
		return FALSE;
	}

	fPreviewing = FALSE;

	// get rid of menu garbage
	//InvalidateRect(, NULL, TRUE);

	return TRUE;
}

// Tear down everything downstream of the capture filters, so we can build
// a different capture graph.  Notice that we never destroy the capture filters
// and WDM filters upstream of them, because then all the capture settings
// we've set would be lost.
//
void DStest::TearDownGraph()
{
	SAFE_RELEASE(pSink);
	SAFE_RELEASE(pConfigAviMux);
	SAFE_RELEASE(pRender);
	SAFE_RELEASE(pME);
	SAFE_RELEASE(pDF);

	if (pVW)
	{
		// stop drawing in our window, or we may get wierd repaint effects
		pVW->put_Owner(NULL);
		pVW->put_Visible(OAFALSE);
		pVW->Release();
		pVW = NULL;
	}

	// destroy the graph downstream of our capture filters
	if (pVCap)
		RemoveDownstream(pVCap);
	if (pACap)
		RemoveDownstream(pACap);
	if (pVCap)
		pBuilder->ReleaseFilters();

	// potential debug output - what the graph looks like
	// if (pFg) DumpGraph(pFg, 1);

#ifdef REGISTER_FILTERGRAPH
	// Remove filter graph from the running object table
	if (g_dwGraphRegister)
	{
		RemoveGraphFromRot(g_dwGraphRegister);
		g_dwGraphRegister = 0;
	}
#endif

	fCaptureGraphBuilt = FALSE;
	fPreviewGraphBuilt = FALSE;
	fPreviewFaked = FALSE;
}


// Tear down everything downstream of a given filter
void DStest::RemoveDownstream(IBaseFilter *pf)
{
	IPin *pP = 0, *pTo = 0;
	ULONG u;
	IEnumPins *pins = NULL;
	PIN_INFO pininfo;

	if (!pf)
		return;

	HRESULT hr = pf->EnumPins(&pins);
	pins->Reset();

	while (hr == NOERROR)
	{
		hr = pins->Next(1, &pP, &u);
		if (hr == S_OK && pP)
		{
			pP->ConnectedTo(&pTo);
			if (pTo)
			{
				hr = pTo->QueryPinInfo(&pininfo);
				if (hr == NOERROR)
				{
					if (pininfo.dir == PINDIR_INPUT)
					{
						RemoveDownstream(pininfo.pFilter);
						pFg->Disconnect(pTo);
						pFg->Disconnect(pP);
						pFg->RemoveFilter(pininfo.pFilter);
					}
					pininfo.pFilter->Release();
				}
				pTo->Release();
			}
			pP->Release();
		}
	}

	if (pins)
		pins->Release();
}


