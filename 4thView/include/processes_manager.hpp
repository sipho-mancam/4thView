#pragma once
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;

/**
* @brief 
* This class deals with the management of processes that are started by this
* application, since this application is the systems entry point.
*/
class ProcessesManager
{
public:
	ProcessesManager(json globalConfig);
	~ProcessesManager();
	void init();


private:
	void __start_process(std::string processName);

	std::vector<std::string> processesStrNames;
	std::vector<PROCESS_INFORMATION> procInfoList;
	std::vector<STARTUPINFO> procStartupInfoList;

	json globalConfig;
};



