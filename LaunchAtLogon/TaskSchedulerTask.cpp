#include "stdafx.h"
#include "TaskSchedulerTask.h"
#include <atlbase.h>

TaskSchedulerTask::TaskSchedulerTask()
{
}


TaskSchedulerTask::~TaskSchedulerTask()
{
}

bool TaskSchedulerTask::Create(LPCWSTR taskName, LPCWSTR program,
  TASK_TRIGGER_TYPE2 flags)
{
  HRESULT hr = E_UNEXPECTED;

  do
  {
    //  ------------------------------------------------------
    //  Create an instance of the Task Service. 
    CComPtr<ITaskService> pService;
    hr = CoCreateInstance(CLSID_TaskScheduler,
      NULL,
      CLSCTX_INPROC_SERVER,
      IID_ITaskService,
      (void**)&pService);
    if (FAILED(hr))
    {
      break;
    }

    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(),
      _variant_t(), _variant_t());
    if (FAILED(hr))
    {
      break;
    }

    //  ------------------------------------------------------
    //  Get the pointer to the root task folder.  This folder will hold the
    //  new task that is registered.
    CComPtr<ITaskFolder> pRootFolder;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr))
    {
      printf("Cannot get Root Folder pointer: HR=0x%x", hr);
      return 1;
    }

    //  If the same task exists, remove it.
    pRootFolder->DeleteTask(_bstr_t(taskName), 0);

    //  Create the task builder object to create the task.
    ITaskDefinition *pTask = NULL;
    hr = pService->NewTask(0, &pTask);

    pService.Release();  // COM clean up.  Pointer is no longer used.
    if (FAILED(hr))
    {
      break;
    }

    //  ------------------------------------------------------
    //  Get the registration info for setting the identification.
    CComPtr<IRegistrationInfo> pRegInfo;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr))
    {
      break;
    }

    hr = pRegInfo->put_Author(L"John Mautari (LimitMicVol)");
    pRegInfo.Release();
    if (FAILED(hr))
    {
      break;
    }

    //  ------------------------------------------------------
    //  Create the settings for the task
    CComPtr<ITaskSettings> pSettings;
    hr = pTask->get_Settings(&pSettings);
    if (FAILED(hr))
    {
      break;
    }

    //  Set setting values for the task. 
    hr = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
    pSettings.Release();
    if (FAILED(hr))
    {
      break;
    }

    //  ------------------------------------------------------
    //  Get the trigger collection to insert the logon trigger.
    CComPtr<ITriggerCollection> pTriggerCollection;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr))
    {
      break;
    }

    //  Add the logon trigger to the task.
    CComPtr<ITrigger> pTrigger;
    hr = pTriggerCollection->Create(flags, &pTrigger);
    if (FAILED(hr))
    {
      break;
    }

    CComPtr<ILogonTrigger> pLogonTrigger;
    hr = pTrigger->QueryInterface(
      IID_ILogonTrigger, (void**)&pLogonTrigger);
    if (FAILED(hr))
    {
      break;
    }

    hr = pLogonTrigger->put_Id(_bstr_t(L"Trigger1"));
    if (FAILED(hr))
    {
      break;
    }

    //  Define the user.  The task will execute when the user logs on.
    //  The specified user must be a user on this computer.  
    //hr = pLogonTrigger->put_UserId(_bstr_t(L"MR-PC\\MR"));
    if (FAILED(hr))
    {
      break;
    }

    //  ------------------------------------------------------
    //  Add an Action to the task.
    CComPtr<IActionCollection> pActionCollection;

    //  Get the task action collection pointer.
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr))
    {
      break;
    }

    //  Create the action, specifying that it is an executable action.
    CComPtr<IAction> pAction;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    if (FAILED(hr))
    {
      break;
    }

    CComPtr<IExecAction> pExecAction;
    //  QI for the executable task pointer.
    hr = pAction->QueryInterface(IID_PPV_ARGS(&pExecAction));
    if (FAILED(hr))
    {
      break;
    }

    //  Set the path of the executable to notepad.exe.
    hr = pExecAction->put_Path(_bstr_t(program));
    if (FAILED(hr))
    {
      break;
    }

    //  ------------------------------------------------------
    //  Save the task in the root folder.
    hr = pRootFolder->RegisterTaskDefinition(
      _bstr_t(taskName),
      pTask,
      TASK_CREATE_OR_UPDATE,
      _variant_t(L"Builtin\\Users"),
      _variant_t(),
      TASK_LOGON_GROUP,
      _variant_t(L""),
      nullptr);
    if (FAILED(hr))
    {
      break;
    }

  } while (false);

  return SUCCEEDED(hr) ? true : false;
}
