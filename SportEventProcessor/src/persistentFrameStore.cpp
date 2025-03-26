#include "persistentFrameStore.hpp"

persistentFrameStore::persistentFrameStore()
	: maxStoreSize(MAX_BUFFER_SIZE), storeOpen(false), framePtr(0)
{
	json globalConfig = KEvents::__load_config__();
	json moduleConfig = globalConfig["SportEventProcessor"];
	storeDirPath = moduleConfig["persistent_store_dir"];
}


persistentFrameStore::~persistentFrameStore()
{
	this->closeStore();
}

std::vector<json> persistentFrameStore::loadStore()
{
	if (!std::filesystem::exists(storeDirPath))
	{
		return frames;
	}

	std::filesystem::directory_iterator directoryIterator(storeDirPath);

	std::vector<std::filesystem::directory_entry> entries;
	for (std::filesystem::directory_entry entry : directoryIterator)
	{
		entries.push_back(entry);
	}

	std::sort(entries.begin(), entries.end(), [](std::filesystem::directory_entry a, std::filesystem::directory_entry b) {
		return a.last_write_time() < b.last_write_time();
	});

	for (std::filesystem::directory_entry entry : entries)
	{
		std::ifstream file(entry.path());
		json frame = json::parse(file);
		frames.push_back(frame);
	}

	closeStore();
	return frames;
}

void persistentFrameStore::dumpFrame(json frame)
{
	if (!storeOpen)
	{
		openStore();
	}
	std::string frame_name = storeDirPath + "frame_"+ std::to_string(framePtr) + ".json";
	std::string frameData = frame.dump();
	std::ofstream file(frame_name);
	file.write(const_cast<char*>(frameData.c_str()), frameData.size());
	framePtr++;

	if (framePtr >= maxStoreSize)
	{
		int idx = maxStoreSize - framePtr;
		frame_name = storeDirPath + "frame_" + std::to_string(idx) + ".json";
		std::filesystem::remove(frame_name);
	}
}

size_t persistentFrameStore::size()
{
	return frames.size();
}

void persistentFrameStore::openStore()
{
	__delete_all__();
	if (!std::filesystem::exists(storeDirPath))
	{
		std::filesystem::create_directories(storeDirPath);
	}
	storeOpen = true;
	framePtr = 0;
}

void persistentFrameStore::closeStore()
{
	storeOpen = false;
	__delete_all__();
	framePtr = 0;
}

void persistentFrameStore::__delete_all__()
{
	if (std::filesystem::exists(storeDirPath))
	{
		std::filesystem::remove_all(storeDirPath);
	}
	frames.clear();
}
