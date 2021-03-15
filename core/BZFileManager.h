#pragma once
#include "BZUtils.h"
#include "BZCoder.h"

#include <fstream> 

#define MAX_PATH_SIZE 256



struct DataBaseFileHeadInfo {
	BZDate date;
	BZTime time;
};

struct DataBaseFileDataInfo {
	int32_t codeType;
	int32_t dataSzie;
};

struct DataBaseFileInfo {
	DataBaseFileHeadInfo headInfo;
	DataBaseFileDataInfo dataInfo;
};


class FileManager {
public:
	FileManager();
	~FileManager();
	BZState Init();
	BZState DeInit();

	BZState SetFilePath(char* path);
	BZState SaveData(void* srcData, int* length);
	char* LoadData(int* length);
	BZState LoadDataFinish();

private:

	//BZState GetHeadInfo(DataBaseFileHeadInfo* info);
	BZState ReadFile();
	BZState WriteFile();
	BZState CheckHeadInfo();
	BZState FillinHeadInfo();
	BZState FillinDataInfo();
	Coder* mCoder;
	char* filePath;
	DataBaseFileInfo* fileInfo;
	char* pFileData;
	char* pDataBaseData;
	int32_t dataLength;
};