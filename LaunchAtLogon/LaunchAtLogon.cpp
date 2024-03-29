#include "stdafx.h"
#include "TaskSchedulerTask.h"

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

#define LIMIT_MIC_VOL_TASK_NAME     L"Limit Microphone Volume"
#define LIMIT_MIC_VOL_TASK_PROGRAM  L"LimitMicVol.exe"

int main()
{
  int retValue = 1;
  HRESULT hr;

  hr = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (FAILED(hr))
  {
    printf("CoInitializeEx failure. HR=0x%X\n", hr);
    return 1;
  }

  //  Set general COM security levels.
  hr = CoInitializeSecurity(
    NULL,
    -1,
    NULL,
    NULL,
    RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
    RPC_C_IMP_LEVEL_IMPERSONATE,
    NULL,
    0,
    NULL);
  if (SUCCEEDED(hr))
  {
    TaskSchedulerTask task;
    std::array<wchar_t, MAX_PATH> path;
    std::wstring programPath;
    if (!::GetModuleFileNameW(nullptr, path.data(),
      static_cast<DWORD>(path.max_size())))
    {
      printf("Cannot get module handle. errno: %d\n", GetLastError());
      goto exit;
    }

    programPath = std::wstring(path.begin(), path.end());
    auto it = programPath.find_last_of(L'\\');
    if (it != std::string::npos)
    {
      programPath = programPath.substr(0, it + 1);
    }
    programPath += LIMIT_MIC_VOL_TASK_PROGRAM;

    HANDLE file = CreateFile(programPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
      nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
    {
      printf("The target program does not exist or is not available. errno: %d",
        GetLastError());
      goto exit;
    }

    CloseHandle(file);

    if (task.Create(LIMIT_MIC_VOL_TASK_NAME, programPath.c_str(),
      TASK_TRIGGER_LOGON))
    {
      std::wcout << L"Task " << programPath.c_str() <<
        L" has been added successfully\n";

      retValue = 0;
    }
    else
    {
      printf("Task couldn't be created.\n");
      goto exit;
    }
  }
  else
  {
    printf("CoInitializeSecurity failure. HR=0x%X\n", hr);
  }

exit:
  ::CoUninitialize();
  return retValue;
}

