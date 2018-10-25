// LimitMicVol.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LimitMicVol.h"

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING	100
#define WINDOW_WIDTH	380
#define WINDOW_HEIGHT	150

// Global Variables:
HINSTANCE hInst;                                // current instance
HANDLE hMutex;
HWND hWnd;
HWND hTrackVolume;
HWND hLabel;

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

std::wstring sAppConfigPath;

NOTIFYICONDATA niData = {};
HANDLE hQuitEvent = nullptr;
HANDLE hSetVolumeThread = nullptr;
double fMaxVolume = MAX_MIC_VOLUME;

DWORD CALLBACK SetVolumeThread(LPVOID);
void LoadConfig();
void SaveConfig();
void UpdateVolume();
bool PickDevice(IMMDevice **DeviceToUse);
void Quitting();

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	hMutex = CreateMutex(nullptr, TRUE, INSTANCE_MUTEX);
	if (hMutex == nullptr || (hMutex != nullptr && GetLastError() == ERROR_ALREADY_EXISTS))
	{
		if (hMutex != nullptr)
		{
			CloseHandle(hMutex);
		}
		return 0;
	}

    // TODO: Place code here.
	LoadConfig();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LIMITMICVOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, SW_HIDE))
    {
        return FALSE;
    }

	hQuitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	hSetVolumeThread = CreateThread(nullptr, 0, &SetVolumeThread, nullptr, 0,
		nullptr);

    HACCEL hAccelTable = LoadAccelerators(hInstance, 
		MAKEINTRESOURCE(IDC_LIMITMICVOL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	CloseHandle(hMutex);
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LIMITMICVOL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   INITCOMMONCONTROLSEX ice = {};
   ice.dwSize = sizeof(ice);
   ice.dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES;
   InitCommonControlsEx(&ice);

   DWORD dwStyle = WS_TILEDWINDOW &~WS_MAXIMIZEBOX;
   dwStyle &= ~WS_THICKFRAME;

   hWnd = CreateWindowW(szWindowClass, szTitle, dwStyle,
      CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr,
	   hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   hTrackVolume = CreateWindowEx(
	   0,
	   TRACKBAR_CLASS,
	   _TEXT("Trackbar Control"),
	   WS_CHILD | WS_VISIBLE |
	   TBS_ENABLESELRANGE | TBS_AUTOTICKS |
	   TBS_BOTH | TBS_HORZ |
	   TBS_FIXEDLENGTH,
	   15, 15,
	   300, 50,
	   hWnd,
	   NULL,
	   hInstance,
	   NULL
   );
   SendMessage(hTrackVolume, TBM_SETRANGE,
	   (WPARAM)TRUE,
	   (LPARAM)MAKELONG(0, 100));
   SendMessage(hTrackVolume, TBM_SETPOS,
	   (WPARAM)TRUE,
	   (LPARAM)(fMaxVolume * 100.0));
   SendMessage(hTrackVolume, TBM_SETPAGESIZE,
	   0,
	   (LPARAM)1);
   SendMessage(hTrackVolume, TBM_SETTICFREQ,
	   (WPARAM)10,
	   (LPARAM)0);

   hLabel = CreateWindowEx(
	   0,
	   WC_STATIC,
	   _TEXT("Text Control"),
	   WS_CHILD | WS_VISIBLE,
	   320, 28,
	   30, 20,
	   hWnd,
	   NULL,
	   hInstance,
	   NULL
   );

   niData.cbSize = sizeof(NOTIFYICONDATA);
   niData.uID = MY_TRAY_ICON_ID;
   niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   niData.hIcon =
	   (HICON)LoadImage(hInstance,
		   MAKEINTRESOURCE(IDI_ICON1),
		   IMAGE_ICON,
		   GetSystemMetrics(SM_CXSMICON),
		   GetSystemMetrics(SM_CYSMICON),
		   LR_DEFAULTCOLOR);
   niData.hWnd = hWnd;
   niData.uCallbackMessage = WM_TRAY_ICON_MESSAGE;
   wcsncpy_s(niData.szTip, ARRAYSIZE(niData.szTip), szTitle, _TRUNCATE);
   Shell_NotifyIcon(NIM_ADD, &niData);

   UpdateVolume();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

VOID WINAPI TBNotifications(
	WPARAM wParam,  // wParam of WM_HSCROLL message 
	HWND hwndTrack, // handle of trackbar window 
	UINT iSelMin,   // minimum value of trackbar selection 
	UINT iSelMax)   // maximum value of trackbar selection 
{
	DWORD dwPos;    // current position of slider 
	WORD msg = LOWORD(wParam);
	switch (msg) {

	case TB_ENDTRACK:
	case TB_THUMBPOSITION:
	case TB_THUMBTRACK:

		dwPos = static_cast<DWORD>(SendMessage(hwndTrack, TBM_GETPOS, 0, 0));   

		if (dwPos > iSelMax)
			SendMessage(hwndTrack, TBM_SETPOS,
			(WPARAM)TRUE,       // redraw flag 
				(LPARAM)iSelMax);

		else if (dwPos < iSelMin)
			SendMessage(hwndTrack, TBM_SETPOS,
			(WPARAM)TRUE,       // redraw flag 
				(LPARAM)iSelMin);

		fMaxVolume = double(dwPos) / 100.0;
		UpdateVolume();

		break;

	default:

		break;
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_QUERYENDSESSION:
		Quitting();
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_HSCROLL:
		TBNotifications(wParam, hTrackVolume, 0, 100);
		break;
    case WM_DESTROY:
		Quitting();
        PostQuitMessage(0);
        break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			ShowWindow(hWnd, SW_HIDE);
		}
		break;
	case WM_TRAY_ICON_MESSAGE:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowWindow(hWnd, SW_RESTORE);
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void SetVolumeLoop(IAudioEndpointVolume* volumeControl)
{
	DWORD dwResult;
	HRESULT hr;
	int errorCount = 0;
	bool changed;
	float volume;

	for (;;)
	{
		dwResult = WaitForSingleObject(hQuitEvent, SET_VOLUME_INTERVAL);
		if (dwResult != WAIT_TIMEOUT)
		{
			break;
		}

		changed = false;
		hr = volumeControl->GetMasterVolumeLevelScalar(&volume);
		if (FAILED(hr))
		{
			errorCount++;
			if (errorCount > 10)
			{
				break;
			}
		}
		else
		{
			errorCount = 0;
		}

		if (volume > fMaxVolume)
		{
			volume = static_cast<float>(fMaxVolume);
			changed = true;
		}

		if (changed)
		{
			hr = volumeControl->SetMasterVolumeLevelScalar(volume, NULL);
			if (FAILED(hr))
			{
				errorCount++;
				if (errorCount > 10)
				{
					break;
				}
			}
			else
			{
				errorCount = 0;
			}
		}
	}
}

DWORD CALLBACK SetVolumeThread(LPVOID pData)
{
	HRESULT hr;
	CComPtr<IMMDevice> endPoint;
	CComPtr<IAudioClient> audioClient;
	CComPtr<IAudioEndpointVolume> volumeControl;

	do
	{
		if (FAILED(::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
		{
			break;
		}

		if (!PickDevice(&endPoint))
		{
			break;
		}
		
		hr = endPoint->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL,
			reinterpret_cast<void **>(&audioClient));
		if (FAILED(hr))
		{
			break;
		}

		hr = endPoint->Activate(__uuidof(IAudioEndpointVolume),
			CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&volumeControl);
		if (FAILED(hr))
		{
			break;
		}

		SetVolumeLoop(volumeControl);
	} while (false);

	::CoUninitialize();
	return 0;
}

void UpdateVolume()
{
	std::wstring title = szTitle;
	std::wstring vol = std::to_wstring(static_cast<int>(fMaxVolume * 100.0));
	title += L" - Max: " + vol;

	SetWindowText(hWnd, title.c_str());

	wcsncpy_s(niData.szTip, ARRAYSIZE(niData.szTip), title.c_str(), _TRUNCATE);
	Shell_NotifyIcon(NIM_MODIFY, &niData);

	SetWindowText(hLabel, vol.c_str());

	SaveConfig();
}

//
//  Based on the input switches, pick the specified device to use.
//
bool PickDevice(IMMDevice **DeviceToUse)
{
	HRESULT hr;
	CComPtr<IMMDeviceEnumerator> deviceEnumerator;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
	if (FAILED(hr))
	{
		return false;
	}

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole,
		DeviceToUse);
	return SUCCEEDED(hr) ? true : false;
}

void Quitting()
{
	if (niData.cbSize > 0)
	{
		Shell_NotifyIcon(NIM_DELETE, &niData);
		niData.cbSize = 0;
	}
	SetEvent(hQuitEvent);
}

// Config
void LoadConfig()
{
	auto flags = KF_FLAG_SIMPLE_IDLIST | KF_FLAG_DONT_VERIFY | KF_FLAG_NO_ALIAS;
	PWSTR path_ptr = nullptr;
	if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, flags, nullptr,
		&path_ptr) != S_OK)
	{
		return;
	}

	sAppConfigPath = path_ptr;
	CoTaskMemFree(path_ptr);

	sAppConfigPath += L"\\LimitMicVol";
	CreateDirectory(sAppConfigPath.c_str(), nullptr);

	sAppConfigPath += L"\\config.ini";

	std::ifstream cfgfile(sAppConfigPath.c_str(), std::ifstream::in);
	if (cfgfile.is_open())
	{
		std::string line;
		if (std::getline(cfgfile, line))
		{
			std::string value(line.begin(), line.end());
			int maxVolume = std::stoi(value);
			if (maxVolume > 0 && maxVolume <= 100)
			{
				fMaxVolume = double(maxVolume) / 100.0;
			}
		}
	}
}

void SaveConfig()
{
	std::ofstream cfgfile(sAppConfigPath.c_str(), std::ofstream::out);
	if (cfgfile.is_open())
	{
		int volume = static_cast<int>(fMaxVolume * 100.0);
		std::string line = std::to_string(volume);
		cfgfile.write(line.c_str(), line.size());
	}
}