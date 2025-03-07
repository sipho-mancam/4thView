#include "processes_manager.hpp"

ProcessesManager::ProcessesManager(json globConfig, std::shared_ptr<spdlog::logger>logger)
	:globalConfig(globConfig),
	logger(logger)
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
	logger->info("Initiating manager shutdown ...");
	for (size_t i = 0; i < procInfoList.size(); i++)
	{
		// Close process and thread handles. 
		CloseHandle(procInfoList[i].hProcess);
		CloseHandle(procInfoList[i].hThread);
	}

	logger->info("Manager shutdown complete ...");
}

void ProcessesManager::init()
{
	for (std::string& proc : processesStrNames)
	{
		__start_process(proc);
	}
	logger->info("Started {} modules.", processesStrNames.size());
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
		logger->error("Failed to start process: {} Error Code: ({})" , processName,  GetLastError() );
		throw std::exception("Failed to create process.");
	}

	procInfoList.push_back(pi);
	procStartupInfoList.push_back(si);
}
