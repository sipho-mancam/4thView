#pragma once
#include "types.hpp"
#include <filesystem>
#include <algorithm>

#define MAX_BUFFER_SIZE 18000

class persistentFrameStore
{
public:
	persistentFrameStore();
	~persistentFrameStore();

	std::vector<json> loadStore();
	void dumpFrame(json frame);
	size_t size();

private:
	std::vector<json> frames;
	int maxStoreSize, framePtr;
	std::string storeDirPath;
	bool storeOpen;

	void openStore();
	void closeStore();

	void __delete_all__();

};

