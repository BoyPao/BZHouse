#include <fstream>

#include "BZFileManager.h"

#define SYSTEM_YEAR_OFFSET	1900
#define SYSTEM_MONTH_OFFSET	1
#define BEIJING_HOUR_OFFSET 8 

FileManager::FileManager() :
	mCoder(nullptr),
	filePath(nullptr),
	fileInfo(nullptr),
	pFileData(nullptr),
	pDataBaseData(nullptr),
	dataLength(0)
{
	//BZState result = this->Init();
}

FileManager::~FileManager()
{
	//BZState result = this->DeInit();
}

BZState FileManager::Init()
{
	BZState result = BZ_SUCCESS;

	if (mCoder)
	{
		result = BZ_UNEXPECT_ERROR;
		BZLoge("Coder singleTon needed! Unexpect error! %d", result);
	}
	else {
		mCoder = new Coder;
		if (mCoder)
		{
			result = mCoder->Init();
		}
		else {
			result = BZ_MEMORY_ERROR;
			mCoder = nullptr;
			BZLoge("Create coder failed! %d", result);
		}
	}

	if (SUCCESS(result)) {
		filePath = (char*)malloc(MAX_PATH_SIZE * sizeof(char));
		if (filePath) {
			memset(filePath, '\0', MAX_PATH_SIZE);
			/*for (int32_t i = 0; i < MAX_PATH_SIZE; i++) {
				filePath[i] = '\0';
			}*/
		}
		else {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc file path failed! %d", result);
		}
	}

	if (SUCCESS(result)) {
		fileInfo = (DataBaseFileInfo*)malloc(sizeof(DataBaseFileInfo));
		if (fileInfo == nullptr) {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc head info failed! %d", result);
		}
	}

	pFileData = nullptr;
	pDataBaseData = nullptr;
	dataLength = 0;

	return result;
}

BZState FileManager::DeInit()
{
	BZState result = BZ_SUCCESS;

	if (mCoder) {
		delete mCoder;
		mCoder = nullptr;
	}
	else {
		BZLogw("Coder is null! %d", result);
	}

	if (filePath) {
		free(filePath);
		filePath = nullptr;
	}
	else {
		BZLogw("File path is null! %d", result);
	}

	if (fileInfo) {
		free(fileInfo);
		fileInfo = nullptr;
	}
	else {
		BZLogw("Head info is null! %d", result);
	}

	if (pFileData)
	{
		free(pFileData);
		pFileData = nullptr;
	}

	if (pDataBaseData) {
		pDataBaseData = nullptr;
	}

	dataLength = 0;

	return result;
}

BZState FileManager::SetFilePath(char* path)
{
	BZState result = BZ_SUCCESS;
	if (path == nullptr) {
		result = BZ_INVALID_PARAM;
		BZLoge("Set file path failed!");
	}

	if (SUCCESS(result)) {
		int32_t length = 0;
		while (length < MAX_PATH_SIZE - 1) {
			if (path[length] != '\0') {
				length++;
			}
			else {
				break;
			}
		}

		if ((length + 1) != MAX_PATH_SIZE - 1) {
			memcpy(filePath, path, length);
			BZLogi("Set file path:%s", filePath);
		}
		else {
			result = BZ_INVALID_PARAM;
			BZLoge("Param oversize!");
		}
	}
	return result;
}

BZState FileManager::ReadFile()
{
	BZState result = BZ_SUCCESS;

	std::ifstream inputFile(filePath, std::ios::in | std::ios::binary);
	if (inputFile.fail()) {
		result = BZ_FAILED;
		BZLoge("Open file failed! %d %s", result, filePath);
		return result;
	}

	inputFile.read((char*)fileInfo, sizeof(DataBaseFileInfo));
	result = CheckHeadInfo();

	if (SUCCESS(result)) {
		pFileData = (char*)malloc(fileInfo->dataInfo.dataSzie * sizeof(char));
		if (pFileData) {
			inputFile.seekg(sizeof(DataBaseFileInfo), std::ios::beg);
			inputFile.read(pFileData, fileInfo->dataInfo.dataSzie);
			mCoder->DeCode(pFileData, fileInfo->dataInfo.dataSzie);
		}
		else {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc for data failed! %d", result);
		}
	}
	inputFile.close();

	return result;
}

BZState FileManager::CheckHeadInfo()
{
	BZState result = BZ_SUCCESS;
	if (fileInfo) {
		//need to develop
		if (fileInfo->dataInfo.dataSzie < 0) {
			result = BZ_UNEXPECT_ERROR;
		}
		else if (fileInfo->dataInfo.dataSzie == 0) {
			BZLogw("File data size is 0. Skip data read");
		}
		BZLogi("HeadInfo:%d-%d-%d %d:%d:%d",
			fileInfo->headInfo.date.year,
			fileInfo->headInfo.date.month,
			fileInfo->headInfo.date.day,
			fileInfo->headInfo.time.hour,
			fileInfo->headInfo.time.min,
			fileInfo->headInfo.time.second);
		BZLogi("DataInfo: codeType:%d dataSize:%d",
			fileInfo->dataInfo.codeType,
			fileInfo->dataInfo.dataSzie);
	}
	else {
		result = BZ_FAILED;
	}
	return result;
}

BZState FileManager::WriteFile()
{
	BZState result = BZ_SUCCESS;

	result = FillinHeadInfo();

	if (SUCCESS(result)) {
		result = FillinDataInfo();
	}

	if (SUCCESS(result)) {
		std::ofstream outputFile(filePath, std::ios::binary);
		outputFile.write((char*)fileInfo, sizeof(DataBaseFileInfo));
		if (pDataBaseData) {
			mCoder->EnCode(pDataBaseData, fileInfo->dataInfo.dataSzie);
			outputFile.write((char*)pDataBaseData, fileInfo->dataInfo.dataSzie);
		}
		else {
			BZLogw("Data is null. Skip data write. %d", result);
		}
		outputFile.close();
	}

	return result;
}

BZState FileManager::FillinHeadInfo()
{
	BZState result = BZ_SUCCESS;

	time_t systemTime;
	time(&systemTime);
	tm pTime;
	gmtime_s(&pTime, &systemTime);

	if (fileInfo) {
		fileInfo->headInfo.date.year = pTime.tm_year + SYSTEM_YEAR_OFFSET;
		fileInfo->headInfo.date.month = pTime.tm_mon + SYSTEM_MONTH_OFFSET;
		fileInfo->headInfo.date.day = pTime.tm_mday;
		fileInfo->headInfo.time.hour = pTime.tm_hour + BEIJING_HOUR_OFFSET;
		fileInfo->headInfo.time.min = pTime.tm_min;
		fileInfo->headInfo.time.second = pTime.tm_sec;
		BZLogi("Time:%d-%d-%d %d:%d:%d",
			fileInfo->headInfo.date.year,
			fileInfo->headInfo.date.month,
			fileInfo->headInfo.date.day,
			fileInfo->headInfo.time.hour,
			fileInfo->headInfo.time.min,
			fileInfo->headInfo.time.second);
	}
	else {
		result = BZ_UNEXPECT_ERROR;
		BZLoge("Get system time failed! %d", result);
	}
	return result;
}

BZState FileManager::FillinDataInfo()
{
	BZState result = BZ_SUCCESS;
	if (fileInfo && mCoder) {
		mCoder->GetCodeType(&fileInfo->dataInfo.codeType);
		BZLogi("Code type:%d %d", fileInfo->dataInfo.codeType, result);
	}
	else {
		result = BZ_UNEXPECT_ERROR;
		BZLoge("Faile to fill data info! %d", result);
	}

	if (SUCCESS(result)) {
		if (pDataBaseData) {
			fileInfo->dataInfo.dataSzie = dataLength;
		}
		else {
			fileInfo->dataInfo.dataSzie = 0;
		}
	}

	return result;
}

char* FileManager::LoadData(int* length)
{
	BZState result = BZ_SUCCESS;

	result = ReadFile();

	if (SUCCESS(result)) {
		*length = fileInfo->dataInfo.dataSzie;
		return pFileData;
	}
	else {
		*length = -1;
		return nullptr;
	}
}

BZState FileManager::LoadDataFinish()
{
	BZState result = BZ_SUCCESS;
	if (pFileData) {
		free(pFileData);
		pFileData = nullptr;
	}
	else {
		result = BZ_UNEXPECT_ERROR;
		BZLoge("pFileData is nullptr! it should not be. %d", result);
	}
	return result;
}

BZState FileManager::SaveData(void* srcData, int* length)
{
	BZState result = BZ_SUCCESS;

	dataLength = *length;
	if (srcData) {
		pDataBaseData = static_cast<char*>(srcData);
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("srcData is null %d", result);
	}

	if (SUCCESS(result)) {
		result = WriteFile();
	}

	return result;
}