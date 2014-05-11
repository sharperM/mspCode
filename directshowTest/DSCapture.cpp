#include "stdafx.h"
#include "DSCapture.h"
#include <math.h>

#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")
DSCapture::DSCapture()
:gnRecurse(0)
{
	// Read the list of devices to use from win.ini
	ZeroMemory(gcap.rgpmAudioMenu, sizeof(gcap.rgpmAudioMenu));
	ZeroMemory(gcap.rgpmVideoMenu, sizeof(gcap.rgpmVideoMenu));
	gcap.pmVideo = 0;
	gcap.pmAudio = 0;
	gcap.fMPEG2 = FALSE;
	gcap.fDeviceMenuPopulated = false;
	gcap.fWantPreview = TRUE;
	gcap.FrameRate = 30.0;
}

DSCapture::~DSCapture()
{
}

DSCapture& DSCapture::instance()
{
	static DSCapture singleton;
	return singleton;
}

void DSCapture::IMonRelease(IMoniker *&pm)
{
	if (pm)
	{
		pm->Release();
		pm = 0;
	}
}

// Make a graph builder object we can use for capture graph building
//
BOOL DSCapture::MakeBuilder()
{
	// we have one already
	if (gcap.pBuilder)
		return TRUE;

	gcap.pBuilder = new ISampleCaptureGraphBuilder();
	if (NULL == gcap.pBuilder)
	{
		return FALSE;
	}

	return TRUE;
}


// Make a graph object we can use for capture graph building
//
BOOL DSCapture::MakeGraph()
{
	// we have one already
	if (gcap.pFg)
		return TRUE;

	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (LPVOID *)&gcap.pFg);

	return (hr == NOERROR) ? TRUE : FALSE;
}


// put all installed video and audio devices in the menus
//
void DSCapture::AddDevicesToMenu()
{
	if (gcap.fDeviceMenuPopulated)
	{
		return;
	}
	gcap.fDeviceMenuPopulated = true;
	gcap.iNumVCapDevices = 0;

	UINT    uIndex = 0;
	HMENU   hMenuSub;
	HRESULT hr;
	BOOL bCheck = FALSE;

// 	hMenuSub = GetSubMenu(GetMenu(ghwndApp), 1);        // Devices menu

	// Clean the sub menu
// 	int iMenuItems = GetMenuItemCount(hMenuSub);
// 	if (iMenuItems == -1)
// 	{
// 		ErrMsg(TEXT("Error Cleaning Devices Menu"));
// 		return;
// 	}
// 	else if (iMenuItems > 0)
// 	{
// 		for (int i = 0; i < iMenuItems; i++)
// 		{
// 			RemoveMenu(hMenuSub, 0, MF_BYPOSITION);
// 		}
// 	}

	for (int i = 0; i < NUMELMS(gcap.rgpmVideoMenu); i++)
	{
		IMonRelease(gcap.rgpmVideoMenu[i]);
	}
	for (int i = 0; i < NUMELMS(gcap.rgpmAudioMenu); i++)
	{
		IMonRelease(gcap.rgpmAudioMenu[i]);
	}

	// enumerate all video capture devices
	ICreateDevEnum *pCreateDevEnum = 0;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)
	{
		ErrMsg(TEXT("Error Creating Device Enumerator"));
		return;
	}

	IEnumMoniker *pEm = 0;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)
	{
		ErrMsg(TEXT("Sorry, you have no video capture hardware.\r\n\r\n")
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
// 				AppendMenu(hMenuSub, MF_STRING, MENU_VDEVICE0 + uIndex,
// 					var.bstrVal);

				if (gcap.pmVideo != 0 && (S_OK == gcap.pmVideo->IsEqual(pM)))
					bCheck = TRUE;

// 				CheckMenuItem(hMenuSub, MENU_VDEVICE0 + uIndex,
// 					(bCheck ? MF_CHECKED : MF_UNCHECKED));
// 				EnableMenuItem(hMenuSub, MENU_VDEVICE0 + uIndex,
// 					(gcap.fCapturing ? MF_DISABLED : MF_ENABLED));
				bCheck = FALSE;

				SysFreeString(var.bstrVal);

				ASSERT(gcap.rgpmVideoMenu[uIndex] == 0);
				gcap.rgpmVideoMenu[uIndex] = pM;
				pM->AddRef();
			}
			pBag->Release();
		}

		pM->Release();
		uIndex++;
	}
	pEm->Release();

	gcap.iNumVCapDevices = uIndex;

	// separate the video and audio devices
// 	AppendMenu(hMenuSub, MF_SEPARATOR, 0, NULL);

EnumAudio:

	// enumerate all audio capture devices
	uIndex = 0;
	bCheck = FALSE;

	ASSERT(pCreateDevEnum != NULL);

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
// 				AppendMenu(hMenuSub, MF_STRING, MENU_ADEVICE0 + uIndex,
// 					var.bstrVal);
// 
				if (gcap.pmAudio != 0 && (S_OK == gcap.pmAudio->IsEqual(pM)))
					bCheck = TRUE;

// 				CheckMenuItem(hMenuSub, MENU_ADEVICE0 + uIndex,
// 					(bCheck ? MF_CHECKED : MF_UNCHECKED));
// 				EnableMenuItem(hMenuSub, MENU_ADEVICE0 + uIndex,
// 					(gcap.fCapturing ? MF_DISABLED : MF_ENABLED));
				bCheck = FALSE;

				SysFreeString(var.bstrVal);

				ASSERT(gcap.rgpmAudioMenu[uIndex] == 0);
				gcap.rgpmAudioMenu[uIndex] = pM;
				pM->AddRef();
			}
			pBag->Release();
		}
		pM->Release();
		uIndex++;
	}

	pEm->Release();
}

// Allow the user to choose a frame rate
//
void DSCapture::ChooseFrameRate(double rate)
{
	/*double rate ;*/

// 	DoDialog(ghwndApp, IDD_FrameRateDialog, (DLGPROC)FrameRateProc, 0);

	HRESULT hr = E_FAIL;

	// If somebody unchecks "use frame rate" it means we will no longer
	// tell the filter what frame rate to use... it will either continue
	// using the last one, or use some default, or if you bring up a dialog
	// box that has frame rate choices, it will obey them.

	// new frame rate?
	if (gcap.fUseFrameRate && gcap.FrameRate != rate)
	{
		gcap.FrameRate = rate;
		if (gcap.fPreviewing)
			StopPreview();

		// now tell it what frame rate to capture at.  Just find the format it
		// is capturing with, and leave everything else alone
		if (gcap.pVSC)
		{
			AM_MEDIA_TYPE *pmt;
			hr = gcap.pVSC->GetFormat(&pmt);

			// DV capture does not use a VIDEOINFOHEADER
			if (hr == NOERROR)
			{
				if (pmt->formattype == FORMAT_VideoInfo)
				{
					VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
					pvi->AvgTimePerFrame = (LONGLONG)(10000000 / gcap.FrameRate);

					hr = gcap.pVSC->SetFormat(pmt);
					if (hr != S_OK)
						ErrMsg(TEXT("%x: Cannot set new frame rate"), hr);
				}
				DeleteMediaType(pmt);
			}
		}

		if (hr != NOERROR)
			ErrMsg(TEXT("Cannot set frame rate for capture"));

		if (gcap.fWantPreview)  // we were previewing
			StartPreview();
	}
}

/*----------------------------------------------------------------------------*\
|   ErrMsg - Opens a Message box with a error message in it.  The user can     |
|            select the OK button to continue                                  |
\*----------------------------------------------------------------------------*/
void DSCapture::ErrMsg(LPTSTR szFormat, ...)
{
	static TCHAR szBuffer[2048] = { 0 };
	const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
	const int LASTCHAR = NUMCHARS - 1;

	// Format the input string
	va_list pArgs;
	va_start(pArgs, szFormat);

	// Use a bounded buffer size to prevent buffer overruns.  Limit count to
	// character size minus one to allow for a NULL terminating character.
	HRESULT hr = StringCchVPrintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
	va_end(pArgs);

	// Ensure that the formatted string is NULL-terminated
	szBuffer[LASTCHAR] = TEXT('\0');

	MessageBox(ghwndApp, szBuffer, NULL,
		MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
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
BOOL DSCapture::BuildPreviewGraph()
{
	int cy, cyBorder;
	HRESULT hr;
	AM_MEDIA_TYPE *pmt;

	// we have one already
	if (gcap.fPreviewGraphBuilt)
		return TRUE;

	// No rebuilding while we're running
	if (gcap.fCapturing || gcap.fPreviewing)
		return FALSE;

	// We don't have the necessary capture filters
	if (gcap.pVCap == NULL)
		return FALSE;
	if (gcap.pACap == NULL && gcap.fCapAudio)
		return FALSE;

	// we already have another graph built... tear down the old one
	if (gcap.fCaptureGraphBuilt)
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

	if (gcap.fMPEG2)
	{
		hr = gcap.pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
			&MEDIATYPE_Stream, gcap.pVCap, NULL, NULL);
		if (FAILED(hr))
		{
			ErrMsg(TEXT("Cannot build MPEG2 preview graph!"));
		}

	}
	else
	{
		hr = gcap.pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
			&MEDIATYPE_Interleaved, gcap.pVCap, NULL, NULL);
		if (hr == VFW_S_NOPREVIEWPIN)
		{
			// preview was faked up for us using the (only) capture pin
			gcap.fPreviewFaked = TRUE;
		}
		else if (hr != S_OK)
		{
			// maybe it's DV?
			hr = gcap.pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
				&MEDIATYPE_Video, gcap.pVCap, NULL, NULL);
			if (hr == VFW_S_NOPREVIEWPIN)
			{
				// preview was faked up for us using the (only) capture pin
				gcap.fPreviewFaked = TRUE;
			}
			else if (hr != S_OK)
			{
				ErrMsg(TEXT("This graph cannot preview!"));
				gcap.fPreviewGraphBuilt = FALSE;
				return FALSE;
			}
		}

		//
		// Render the closed captioning pin? It could be a CC or a VBI category pin,
		// depending on the capture driver
		//

		if (gcap.fCapCC)
		{
			hr = gcap.pBuilder->RenderStream(&PIN_CATEGORY_CC, NULL,
				gcap.pVCap, NULL, NULL);
			if (hr != NOERROR)
			{
				hr = gcap.pBuilder->RenderStream(&PIN_CATEGORY_VBI, NULL,
					gcap.pVCap, NULL, NULL);
				if (hr != NOERROR)
				{
					ErrMsg(TEXT("Cannot render closed captioning"));
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

	hr = gcap.pFg->QueryInterface(IID_IVideoWindow, (void **)&gcap.pVW);
	if (hr != NOERROR)
	{
		ErrMsg(TEXT("This graph cannot preview properly"));
	}
	else
	{
		//Find out if this is a DV stream
		AM_MEDIA_TYPE * pmtDV;

		if (gcap.pVSC && SUCCEEDED(gcap.pVSC->GetFormat(&pmtDV)))
		{
			if (pmtDV->formattype == FORMAT_DvInfo)
			{
				// in this case we want to set the size of the parent window to that of
				// current DV resolution.
				// We get that resolution from the IVideoWindow.
				CComPtr<IBasicVideo> pBV;

				// If we got here, gcap.pVW is not NULL 
				ASSERT(gcap.pVW != NULL);
				hr = gcap.pVW->QueryInterface(IID_IBasicVideo, (void**)&pBV);

				if (SUCCEEDED(hr))
				{
					HRESULT hr1, hr2;
					long lWidth, lHeight;

					hr1 = pBV->get_VideoHeight(&lHeight);
					hr2 = pBV->get_VideoWidth(&lWidth);
					if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
					{
// 						ResizeWindow(lWidth, abs(lHeight));
					}
				}
			}
		}

		RECT rc;
		gcap.pVW->put_Owner((OAHWND)ghwndApp);    // We own the window now
		gcap.pVW->put_WindowStyle(WS_CHILD);    // you are now a child

		// give the preview window all our space but where the status bar is
		GetClientRect(ghwndApp, &rc);
		cyBorder = GetSystemMetrics(SM_CYBORDER);
		cy = /*statusGetHeight()*/ + cyBorder;
		rc.bottom -= cy;

		gcap.pVW->SetWindowPosition(0, 0, rc.right, rc.bottom); // be this big
		gcap.pVW->put_Visible(OATRUE);
	}

	// now tell it what frame rate to capture at.  Just find the format it
	// is capturing with, and leave everything alone but change the frame rate
	// No big deal if it fails.  It's just for preview
	// !!! Should we then talk to the preview pin?
	if (gcap.pVSC && gcap.fUseFrameRate)
	{
		hr = gcap.pVSC->GetFormat(&pmt);

		// DV capture does not use a VIDEOINFOHEADER
		if (hr == NOERROR)
		{
			if (pmt->formattype == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
				pvi->AvgTimePerFrame = (LONGLONG)(10000000 / gcap.FrameRate);

				hr = gcap.pVSC->SetFormat(pmt);
				if (hr != NOERROR)
					ErrMsg(TEXT("%x: Cannot set frame rate for preview"), hr);
			}
			DeleteMediaType(pmt);
		}
	}

	// make sure we process events while we're previewing!
	hr = gcap.pFg->QueryInterface(IID_IMediaEventEx, (void **)&gcap.pME);
	if (hr == NOERROR)
	{
		gcap.pME->SetNotifyWindow((OAHWND)ghwndApp, WM_FGNOTIFY, 0);
	}

	// potential debug output - what the graph looks like
	// DumpGraph(gcap.pFg, 1);

	// Add our graph to the running object table, which will allow
	// the GraphEdit application to "spy" on our graph
#ifdef REGISTER_FILTERGRAPH
	hr = AddGraphToRot(gcap.pFg, &g_dwGraphRegister);
	if (FAILED(hr))
	{
		ErrMsg(TEXT("Failed to register filter graph with ROT!  hr=0x%x"), hr);
		g_dwGraphRegister = 0;
	}
#endif

	// All done.
	gcap.fPreviewGraphBuilt = TRUE;
	return TRUE;
}


// Start previewing
//
BOOL DSCapture::StartPreview()
{
	// way ahead of you
	if (gcap.fPreviewing)
		return TRUE;

	if (!gcap.fPreviewGraphBuilt)
		return FALSE;

	// run the graph
	IMediaControl *pMC = NULL;
	HRESULT hr = gcap.pFg->QueryInterface(IID_IMediaControl, (void **)&pMC);
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
		ErrMsg(TEXT("Error %x: Cannot run preview graph"), hr);
		return FALSE;
	}

	gcap.fPreviewing = TRUE;
	return TRUE;
}


// stop the preview graph
//
BOOL DSCapture::StopPreview()
{
	// way ahead of you
	if (!gcap.fPreviewing)
	{
		return FALSE;
	}

	// stop the graph
	IMediaControl *pMC = NULL;
	HRESULT hr = gcap.pFg->QueryInterface(IID_IMediaControl, (void **)&pMC);
	if (SUCCEEDED(hr))
	{
		hr = pMC->Stop();
		pMC->Release();
	}
	if (FAILED(hr))
	{
		ErrMsg(TEXT("Error %x: Cannot stop preview graph"), hr);
		return FALSE;
	}

	gcap.fPreviewing = FALSE;

	// get rid of menu garbage
	InvalidateRect(ghwndApp, NULL, TRUE);

	return TRUE;
}

// Check the devices we're currently using and make filters for them
//
void DSCapture::ChooseDevices(IMoniker *pmVideo, IMoniker *pmAudio)
{
#define VERSIZE 40
#define DESCSIZE 80

	int versize = VERSIZE;
	int descsize = DESCSIZE;
	WCHAR wachVer[VERSIZE] = { 0 }, wachDesc[DESCSIZE] = { 0 };
	TCHAR tachStatus[VERSIZE + DESCSIZE + 5] = { 0 };


	// they chose a new device. rebuild the graphs
	if (gcap.pmVideo != pmVideo || gcap.pmAudio != pmAudio)
	{
		if (pmVideo)
		{
			pmVideo->AddRef();
		}
		if (pmAudio)
		{
			pmAudio->AddRef();
		}

		IMonRelease(gcap.pmVideo);
		IMonRelease(gcap.pmAudio);
		gcap.pmVideo = pmVideo;
		gcap.pmAudio = pmAudio;

		if (gcap.fPreviewing)
			StopPreview();
		if (gcap.fCaptureGraphBuilt || gcap.fPreviewGraphBuilt)
			TearDownGraph();

		FreeCapFilters();
		InitCapFilters();

		if (gcap.fWantPreview)   // were we previewing?
		{
			BuildPreviewGraph();
			StartPreview();
		}

// 		MakeMenuOptions();      // the UI choices change per device
	}

	// Set the check marks for the devices menu.
	int i;
	for (i = 0; i < NUMELMS(gcap.rgpmVideoMenu); i++)
	{
		if (gcap.rgpmVideoMenu[i] == NULL)
			break;

// 		CheckMenuItem(GetMenu(ghwndApp),
// 			MENU_VDEVICE0 + i,
// 			(S_OK == gcap.rgpmVideoMenu[i]->IsEqual(gcap.pmVideo)) ? MF_CHECKED : MF_UNCHECKED);
	}

	for (i = 0; i < NUMELMS(gcap.rgpmAudioMenu); i++)
	{
		if (gcap.rgpmAudioMenu[i] == NULL)
			break;
 
// 		CheckMenuItem(GetMenu(ghwndApp), MENU_ADEVICE0 + i,
// 			(S_OK == gcap.rgpmAudioMenu[i]->IsEqual(gcap.pmAudio)) ? MF_CHECKED : MF_UNCHECKED);
	}

	// Put the video driver name in the status bar - if the filter supports
	// IAMVideoCompression::GetInfo, that's the best way to get the name and
	// the version.  Otherwise use the name we got from device enumeration
	// as a fallback.
	if (gcap.pVC)
	{
		HRESULT hr = gcap.pVC->GetInfo(wachVer, &versize, wachDesc, &descsize,
			NULL, NULL, NULL, NULL);
		if (hr == S_OK)
		{
			// It's possible that the call succeeded without actually filling
			// in information for description and version.  If these strings
			// are empty, just display the device's friendly name.
			if (wcslen(wachDesc) && wcslen(wachVer))
			{
				hr = StringCchPrintf(tachStatus, VERSIZE + DESCSIZE + 5, TEXT("%s - %s\0"), wachDesc, wachVer);
// 				statusUpdateStatus(ghwndStatus, tachStatus);
				return;
			}
		}
	}

	// Since the GetInfo method failed (or the interface did not exist),
	// display the device's friendly name.
// 	statusUpdateStatus(ghwndStatus, gcap.wachFriendlyName);
}

void DSCapture::ChooseDevices(TCHAR *szVideo, TCHAR *szAudio)
{
	WCHAR wszVideo[1024], wszAudio[1024];

// 	StringCchCopyN(wszVideo, NUMELMS(wszVideo), szVideo, NUMELMS(wszVideo) - 1);
// 	StringCchCopyN(wszAudio, NUMELMS(wszAudio), szAudio, NUMELMS(wszAudio) - 1);
// 	wszVideo[1023] = wszAudio[1023] = 0;    // Null-terminate
// 
// 	IBindCtx *lpBC = 0;
	IMoniker *pmVideo = 0, *pmAudio = 0;
// 
// 	HRESULT hr = CreateBindCtx(0, &lpBC);
// 	if (SUCCEEDED(hr))
// 	{
// 		DWORD dwEaten;
// 		hr = MkParseDisplayName(lpBC, wszVideo, &dwEaten, &pmVideo);
// 		hr = MkParseDisplayName(lpBC, wszAudio, &dwEaten, &pmAudio);
// 
// 		lpBC->Release();
// 	}

	// Handle the case where the video capture device used for the previous session
	// is not available now.
	BOOL bFound = FALSE;

	if (pmVideo != NULL)
	{
		for (int i = 0; i < NUMELMS(gcap.rgpmVideoMenu); i++)
		{
			if (gcap.rgpmVideoMenu[i] != NULL &&
				S_OK == gcap.rgpmVideoMenu[i]->IsEqual(pmVideo))
			{
				bFound = TRUE;
				break;
			}
		}
	}

	if (!bFound)
	{
		if (gcap.iNumVCapDevices > 0)
		{
			IMonRelease(pmVideo);
			ASSERT(gcap.rgpmVideoMenu[0] != NULL);
			pmVideo = gcap.rgpmVideoMenu[0];
			pmVideo->AddRef();
		}
		else
			goto CleanUp;
	}

	ChooseDevices(pmVideo, pmAudio);

CleanUp:
	IMonRelease(pmVideo);
	IMonRelease(pmAudio);
}


// Tear down everything downstream of a given filter
void DSCapture::RemoveDownstream(IBaseFilter *pf)
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
						gcap.pFg->Disconnect(pTo);
						gcap.pFg->Disconnect(pP);
						gcap.pFg->RemoveFilter(pininfo.pFilter);
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


// Tear down everything downstream of the capture filters, so we can build
// a different capture graph.  Notice that we never destroy the capture filters
// and WDM filters upstream of them, because then all the capture settings
// we've set would be lost.
//
void DSCapture::TearDownGraph()
{
	SAFE_RELEASE(gcap.pSink);
	SAFE_RELEASE(gcap.pConfigAviMux);
	SAFE_RELEASE(gcap.pRender);
	SAFE_RELEASE(gcap.pME);
	SAFE_RELEASE(gcap.pDF);

	if (gcap.pVW)
	{
		// stop drawing in our window, or we may get wierd repaint effects
		gcap.pVW->put_Owner(NULL);
		gcap.pVW->put_Visible(OAFALSE);
		gcap.pVW->Release();
		gcap.pVW = NULL;
	}

	// destroy the graph downstream of our capture filters
	if (gcap.pVCap)
		RemoveDownstream(gcap.pVCap);
	if (gcap.pACap)
		RemoveDownstream(gcap.pACap);
	if (gcap.pVCap)
		gcap.pBuilder->ReleaseFilters();

	// potential debug output - what the graph looks like
	// if (gcap.pFg) DumpGraph(gcap.pFg, 1);

#ifdef REGISTER_FILTERGRAPH
	// Remove filter graph from the running object table
	if (g_dwGraphRegister)
	{
		RemoveGraphFromRot(g_dwGraphRegister);
		g_dwGraphRegister = 0;
	}
#endif

	gcap.fCaptureGraphBuilt = FALSE;
	gcap.fPreviewGraphBuilt = FALSE;
	gcap.fPreviewFaked = FALSE;
}



// create the capture filters of the graph.  We need to keep them loaded from
// the beginning, so we can set parameters on them and have them remembered
//
BOOL DSCapture::InitCapFilters()
{
	HRESULT hr = S_OK;
	BOOL f;

	gcap.fCCAvail = FALSE;  // assume no closed captioning support

	f = MakeBuilder();
	if (!f)
	{
		ErrMsg(TEXT("Cannot instantiate graph builder"));
		return FALSE;
	}

	//
	// First, we need a Video Capture filter, and some interfaces
	//
	gcap.pVCap = NULL;

	if (gcap.pmVideo != 0)
	{
		IPropertyBag *pBag;
		gcap.wachFriendlyName[0] = 0;

		hr = gcap.pmVideo->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if (SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;

			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{
				hr = StringCchCopyW(gcap.wachFriendlyName, sizeof(gcap.wachFriendlyName) / sizeof(gcap.wachFriendlyName[0]), var.bstrVal);
				SysFreeString(var.bstrVal);
			}

			pBag->Release();
		}

		hr = gcap.pmVideo->BindToObject(0, 0, IID_IBaseFilter, (void**)&gcap.pVCap);
	}

	if (gcap.pVCap == NULL)
	{
		ErrMsg(TEXT("Error %x: Cannot create video capture filter"), hr);
		goto InitCapFiltersFail;
	}

	//
	// make a filtergraph, give it to the graph builder and put the video
	// capture filter in the graph
	//

	f = MakeGraph();
	if (!f)
	{
		ErrMsg(TEXT("Cannot instantiate filtergraph"));
		goto InitCapFiltersFail;
	}

	hr = gcap.pBuilder->SetFiltergraph(gcap.pFg);
	if (hr != NOERROR)
	{
		ErrMsg(TEXT("Cannot give graph to builder"));
		goto InitCapFiltersFail;
	}

	// Add the video capture filter to the graph with its friendly name
	hr = gcap.pFg->AddFilter(gcap.pVCap, gcap.wachFriendlyName);
	if (hr != NOERROR)
	{
		ErrMsg(TEXT("Error %x: Cannot add vidcap to filtergraph"), hr);
		goto InitCapFiltersFail;
	}
	MyMethod();
	// Calling FindInterface below will result in building the upstream
	// section of the capture graph (any WDM TVTuners or Crossbars we might
	// need).

	// we use this interface to get the name of the driver
	// Don't worry if it doesn't work:  This interface may not be available
	// until the pin is connected, or it may not be available at all.
	// (eg: interface may not be available for some DV capture)
	hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Interleaved, gcap.pVCap,
		IID_IAMVideoCompression, (void **)&gcap.pVC);
	if (hr != S_OK)
	{
		hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
			&MEDIATYPE_Video, gcap.pVCap,
			IID_IAMVideoCompression, (void **)&gcap.pVC);
	}

	// !!! What if this interface isn't supported?
	// we use this interface to set the frame rate and get the capture size
	hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Interleaved,
		gcap.pVCap, IID_IAMStreamConfig, (void **)&gcap.pVSC);

	if (hr != NOERROR)
	{
		hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
			&MEDIATYPE_Video, gcap.pVCap,
			IID_IAMStreamConfig, (void **)&gcap.pVSC);
		if (hr != NOERROR)
		{
			// this means we can't set frame rate (non-DV only)
			ErrMsg(TEXT("Error %x: Cannot find VCapture:IAMStreamConfig"), hr);
		}
	}

	gcap.fCapAudioIsRelevant = TRUE;

	AM_MEDIA_TYPE *pmt;

	// default capture format
	if (gcap.pVSC && gcap.pVSC->GetFormat(&pmt) == S_OK)
	{
		// DV capture does not use a VIDEOINFOHEADER
		if (pmt->formattype == FORMAT_VideoInfo)
		{
			// resize our window to the default capture size
// 			ResizeWindow(HEADER(pmt->pbFormat)->biWidth,
// 				abs(HEADER(pmt->pbFormat)->biHeight));
		}
		if (pmt->majortype != MEDIATYPE_Video)
		{
			// This capture filter captures something other that pure video.
			// Maybe it's DV or something?  Anyway, chances are we shouldn't
			// allow capturing audio separately, since our video capture
			// filter may have audio combined in it already!
			gcap.fCapAudioIsRelevant = FALSE;
			gcap.fCapAudio = FALSE;
		}
		DeleteMediaType(pmt);
	}

	// we use this interface to bring up the 3 dialogs
	// NOTE:  Only the VfW capture filter supports this.  This app only brings
	// up dialogs for legacy VfW capture drivers, since only those have dialogs
	hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Video, gcap.pVCap,
		IID_IAMVfwCaptureDialogs, (void **)&gcap.pDlg);

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

// 		gcap.pCrossbar = NULL;

		if (SUCCEEDED(gcap.pVCap->EnumPins(&pins)))
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
// 							gcap.pCrossbar = new CCrossbar(pP, &hrCreate);
// 							if (!gcap.pCrossbar || FAILED(hrCreate))
// 								break;
// 
// 							hr = gcap.pCrossbar->GetInputCount(&gcap.NumberOfVideoInputs);
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
	if (gcap.fCapAudioIsRelevant == FALSE)
		goto SkipAudio;

	// create the audio capture filter, even if we are not capturing audio right
	// now, so we have all the filters around all the time.

	//
	// We want an audio capture filter and some interfaces
	//

	if (gcap.pmAudio == 0)
	{
		// there are no audio capture devices. We'll only allow video capture
		gcap.fCapAudio = FALSE;
		goto SkipAudio;
	}
	gcap.pACap = NULL;


	hr = gcap.pmAudio->BindToObject(0, 0, IID_IBaseFilter, (void**)&gcap.pACap);

	if (gcap.pACap == NULL)
	{
		// there are no audio capture devices. We'll only allow video capture
		gcap.fCapAudio = FALSE;
		ErrMsg(TEXT("Cannot create audio capture filter"));
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
		hr = gcap.pmAudio->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
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
		hr = gcap.pFg->AddFilter(gcap.pACap, wachAudioFriendlyName);
		if (hr != NOERROR)
		{
			ErrMsg(TEXT("Error %x: Cannot add audio capture filter to filtergraph"), hr);
			goto InitCapFiltersFail;
		}
	}

	// Calling FindInterface below will result in building the upstream
	// section of the capture graph (any WDM TVAudio's or Crossbars we might
	// need).

	// !!! What if this interface isn't supported?
	// we use this interface to set the captured wave format
	hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, gcap.pACap,
		IID_IAMStreamConfig, (void **)&gcap.pASC);

	if (hr != NOERROR)
	{
		ErrMsg(TEXT("Cannot find ACapture:IAMStreamConfig"));
	}

SkipAudio:

	// Can this filter do closed captioning?
	IPin *pPin;
	hr = gcap.pBuilder->FindPin(gcap.pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_VBI,
		NULL, FALSE, 0, &pPin);
	if (hr != S_OK)
		hr = gcap.pBuilder->FindPin(gcap.pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_CC,
		NULL, FALSE, 0, &pPin);
	if (hr == S_OK)
	{
		pPin->Release();
		gcap.fCCAvail = TRUE;
	}
	else
	{
		gcap.fCapCC = FALSE;    // can't capture it, then
	}

	// potential debug output - what the graph looks like
	// DumpGraph(gcap.pFg, 1);

	return TRUE;

InitCapFiltersFail:
	FreeCapFilters();
	return FALSE;
}


// all done with the capture filters and the graph builder
//
void DSCapture::FreeCapFilters()
{
	SAFE_RELEASE(gcap.pFg);
	if (gcap.pBuilder)
	{
		delete gcap.pBuilder;
		gcap.pBuilder = NULL;
	}
	SAFE_RELEASE(gcap.pVCap);
	SAFE_RELEASE(gcap.pACap);
	SAFE_RELEASE(gcap.pASC);
	SAFE_RELEASE(gcap.pVSC);
	SAFE_RELEASE(gcap.pVC);
	SAFE_RELEASE(gcap.pDlg);

// 	if (gcap.pCrossbar)
// 	{
// 		delete gcap.pCrossbar;
// 		gcap.pCrossbar = NULL;
// 	}
}

// make sure the preview window inside our window is as big as the
// dimensions of captured video, or some capture cards won't show a preview.
// (Also, it helps people tell what size video they're capturing)
// We will resize our app's window big enough so that once the status bar
// is positioned at the bottom there will be enough room for the preview
// window to be w x h
//

void DSCapture::ResizeWindow(int w, int h)
{
	RECT rcW, rcC;
	int xExtra, yExtra;
	int cyBorder = GetSystemMetrics(SM_CYBORDER);

	gnRecurse++;

	GetWindowRect(ghwndApp, &rcW);
	GetClientRect(ghwndApp, &rcC);
	xExtra = rcW.right - rcW.left - rcC.right;
	yExtra = rcW.bottom - rcW.top - rcC.bottom + cyBorder /*+ statusGetHeight()*/;

	rcC.right = w;
	rcC.bottom = h;
	SetWindowPos(ghwndApp, NULL, 0, 0, rcC.right + xExtra,
		rcC.bottom + yExtra, SWP_NOZORDER | SWP_NOMOVE);

	// we may need to recurse once.  But more than that means the window cannot
	// be made the size we want, trying will just stack fault.
	//
	if (gnRecurse == 1 && ((rcC.right + xExtra != rcW.right - rcW.left && w > GetSystemMetrics(SM_CXMIN)) ||
		(rcC.bottom + yExtra != rcW.bottom - rcW.top)))
		ResizeWindow(w, h);

	gnRecurse--;
}

void DSCapture::openCamara(HWND parent)
{
	ghwndApp = parent;
	AddDevicesToMenu();
	ChooseDevices(TEXT(""), TEXT(""));
	StartPreview();
}

void DSCapture::captureImage()
{
	HRESULT hr;
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	hr = gcap.pFg->QueryInterface(IID_IMediaControl,
		(void**)&pMediaControl);
	if (hr != S_OK)
		return;

	// Run graph
	while (1)
	{
		hr = pMediaControl->Run();

		// Hopefully, the return value was S_OK or S_FALSE
		if (hr == S_OK) break; // graph is now running
		if (hr == S_FALSE) continue; // graph still preparing to run

		// If the Run function returned something else,
		// there must be a problem
		fprintf(stderr, "Error: %u\n", hr);
	}
	long buffer_size = 0;
	while (1)
	{
		// Passing in a NULL pointer signals that we're just checking
		// the required buffer size; not looking for actual data yet.
		hr = pSampleGrabber->GetCurrentBuffer(&buffer_size, NULL);

		// Keep trying until buffer_size is set to non-zero value.
		if (hr == S_OK && buffer_size != 0) break;

		// If the return value isn't S_OK or VFW_E_WRONG_STATE
		// then something has gone wrong. VFW_E_WRONG_STATE just
		// means that the filter graph is still starting up and
		// no data has arrived yet in the sample grabber filter.
		if (hr != S_OK && hr != VFW_E_WRONG_STATE)
			return;
// 			exit_message("Could not get buffer size", 1);
	}
	pBuffer = new char[buffer_size];
	if (!pBuffer)
		return;//exit_message("Could not allocate data buffer for image", 1);
	// Retrieve image data from sample grabber buffer
	hr = pSampleGrabber->GetCurrentBuffer(
		&buffer_size, (long*)pBuffer);
	if (hr != S_OK)
		OutputDebugString(TEXT("Could not get buffer data from sample grabber\n"));
	// Get the media type from the sample grabber filter
	hr = pSampleGrabber->GetConnectedMediaType(
		(DexterLib::_AMMediaType *)&mt);
	if (hr != S_OK) OutputDebugString(TEXT("Could not get media type"));

	// Retrieve format information
	VIDEOINFOHEADER *pVih = NULL;
	if ((mt.formattype == FORMAT_VideoInfo) &&
		(mt.cbFormat >= sizeof(VIDEOINFOHEADER)) &&
		(mt.pbFormat != NULL))
	{
		// Get video info header structure from media type
		pVih = (VIDEOINFOHEADER*)mt.pbFormat;

		// Print the resolution of the captured image
		fprintf(stderr, "Capture resolution: %dx%d\n",
			pVih->bmiHeader.biWidth,
			pVih->bmiHeader.biHeight);

		// Create bitmap structure
		long cbBitmapInfoSize = mt.cbFormat - SIZE_PREHEADER;
		BITMAPFILEHEADER bfh;
		ZeroMemory(&bfh, sizeof(bfh));
		bfh.bfType = 'MB'; // Little-endian for "BM".
		bfh.bfSize = sizeof(bfh)+buffer_size + cbBitmapInfoSize;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+cbBitmapInfoSize;

		// Open output file
		HANDLE hf = CreateFile(TEXT("avarge.bmp"), GENERIC_WRITE,
			FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		if (hf == INVALID_HANDLE_VALUE)
			OutputDebugString(TEXT("Error opening output file"));

		// Write the file header.
		DWORD dwWritten = 0;
		WriteFile(hf, &bfh, sizeof(bfh), &dwWritten, NULL);
		WriteFile(hf, HEADER(pVih),
			cbBitmapInfoSize, &dwWritten, NULL);

		// Write pixel data to file
		WriteFile(hf, pBuffer, buffer_size, &dwWritten, NULL);
		CloseHandle(hf);
	}
	else
	{
		OutputDebugString(TEXT("Wrong media type"));
	}

	// Free the format block
	if (mt.cbFormat != 0)
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL)
	{
		// pUnk should not be used.
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
	delete pBuffer;
}

void DSCapture::MyMethod()
{
	HRESULT hr;
	hr = CoCreateInstance(CLSID_SampleGrabber, NULL,
		CLSCTX_INPROC_SERVER, IID_IBaseFilter,
		(void**)&pSampleGrabberFilter);
	if (hr != S_OK)
		return;
	hr = pSampleGrabberFilter->QueryInterface(
		DexterLib::IID_ISampleGrabber, (void**)&pSampleGrabber);
	if (hr != S_OK)
		return;
	hr = pSampleGrabber->SetBufferSamples(TRUE);
	if (hr != S_OK)
		return;
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;
	hr = pSampleGrabber->SetMediaType((DexterLib::_AMMediaType *)&mt);
	if (hr != S_OK)
		return;
	hr = gcap.pFg->AddFilter(pSampleGrabberFilter, L"SampleGrab");
	if (hr != S_OK)
		return;
	hr = CoCreateInstance(CLSID_NullRenderer, NULL,
		CLSCTX_INPROC_SERVER, IID_IBaseFilter,
		(void**)&pNullRenderer);
	if (hr != S_OK)
		return;
	hr = gcap.pFg->AddFilter(pNullRenderer, L"NullRender");
	if (hr != S_OK)
		return;
	hr = gcap.pBuilder->RenderStream(
		&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,
		gcap.pVCap, pSampleGrabberFilter, pNullRenderer);
	if (hr != S_OK)
		return;
}

void DSCapture::setWindowVisble(bool bVisible)
{
	gcap.pVW->put_Visible(bVisible ? OATRUE : OAFALSE);
}
