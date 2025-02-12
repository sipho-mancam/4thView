#include "processes_manager.hpp"

ProcessesManager::ProcessesManager(json globConfig)
	:globalConfig(globConfig)
{
	json system = globConfig["systemSettings"];
	std::vector<std::string> mods = system["registeredModules"];
	for (std::string& mod : mods)
	{
		processesStrNames.push_back(mod);
	}
}

ProcessesManager::~ProcessesManager()
{
	for (size_t i = 0; i < procInfoList.size(); i++)
	{
		// Close process and thread handles. 
		CloseHandle(procInfoList[i].hProcess);
		CloseHandle(procInfoList[i].hThread);
	}
}

void ProcessesManager::init()
{
	for (std::string& proc : processesStrNames)
	{
		__start_process(proc);
	}
}

void ProcessesManager::__start_process(std::string processName)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	json processInfo = globalConfig[processName];
	
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	std::wstring wProcessName = std::wstring(processName.begin(), processName.end()) + L".exe";

	if (!std::filesystem::exists(std::string(processName + ".exe")))
	{
		std::system("cd");
	}

	if (!CreateProcess(
		/*proc*/
		wProcessName.c_str(),
		NULL,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si,
		&pi))
	{
		std::cout << "Failed to start process: " << processName << " Error code: (" << GetLastError() << ")" << std::endl;
		throw std::exception("Failed to create process.");
	}

	procInfoList.push_back(pi);
	procStartupInfoList.push_back(si);
}
