#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "BZUtils.h"
#include "BZHouse.h"
#include "BZFileManager.h"

using namespace std;

//#define NUM_BIT 3
#define KEY_USED true
#define KEY_NOT_USED false
#define INVALED_ID -1
//typedef BZHouseUint BZDataPackageUint;
bool CountNotUsedKey(bool val);

template<typename BZDataUint, class BZDataUintCtlObject>
class DataManager {
public:
	DataManager();
	~DataManager();

	BZState Init();
	BZState	DeInit();

	BZState Add(BZDataUint uint);
	FileManager* GetFileManager();

	BZState SaveData();
	BZState LoadData();
	BZState DisplaySelectedData();
	BZState DisplayAll();
	BZState SelectDataUintById(int32_t id);
	BZState SelectLastDataUint();
	BZState DeleteSelectedData();
	BZState ClearAll();
	BZState	GetSelectedId(int32_t* id);

private:
	BZState CheckContainer();
	BZState UpdateDataBase(void* data, int32_t num);
	unordered_map<int32_t, BZDataUint>* mDataBase;
	vector<bool>* mDataList;
	BZDataUintCtlObject* mUintCtlObject;
	int32_t mSelectedId;
	FileManager* mFileManager;
	bool inited;
	//char NUM[NUM_BIT];
};


template<typename BZDataUint, class BZDataUintCtlObject>
DataManager<BZDataUint, BZDataUintCtlObject>::DataManager() :
	mDataBase(nullptr),
	mDataList(nullptr),
	mUintCtlObject(nullptr),
	mSelectedId(INVALED_ID),
	mFileManager(nullptr),
	inited(false)
{

}

template<typename BZDataUint, class BZDataUintCtlObject>
DataManager<BZDataUint, BZDataUintCtlObject>::~DataManager()
{
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::Init()
{
	BZState result = BZ_SUCCESS;

	if (mFileManager == nullptr) {
		mFileManager = new FileManager;
		if (mFileManager) {
			result = mFileManager->Init();
			if (!SUCCESS(result)) {
				BZLoge("Failed to init file manager! %d", result);
			}
		}
		else {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc for file manager failed! %d", result);
		}
	}

	if (SUCCESS(result)) {
		mDataBase = new unordered_map<int32_t, BZDataUint>;
		if (mDataBase == nullptr) {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc for DataBase failed! %d", result);
		}
	}

	if (SUCCESS(result)) {
		mDataList = new vector<bool>;
		if (mDataList == nullptr) {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc for DataList failed! %d", result);
		}
	}

	if (SUCCESS(result)) {
		mUintCtlObject = new BZDataUintCtlObject;
		if (mUintCtlObject) {
			mSelectedId = INVALED_ID;
			result = mUintCtlObject->Init();
		}
		else {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc for Uint failed! %d", result);
		}
	}
	inited = true;

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::DeInit()
{
	BZState result = BZ_SUCCESS;

	if (inited) {
		if (mFileManager) {
			result = mFileManager->DeInit();
		}

		if (mDataBase) {
			delete mDataBase;
			mDataBase = nullptr;
		}

		if (mDataList) {
			delete mDataList;
			mDataList = nullptr;
		}

		if (mUintCtlObject) {
			delete mUintCtlObject;
			mUintCtlObject = nullptr;
			mSelectedId = INVALED_ID;
		}

		inited = false;
	}
	else {
		result = BZ_FAILED;
		BZLoge("DataBase is not inited! %d", result);
	}

	return result;
}


template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::CheckContainer() {
	BZState result = BZ_SUCCESS;

	if (mDataBase == nullptr) {
		result = BZ_FAILED;
		BZLoge("mDataBase is null! %d", result);
	}

	if (mDataList == nullptr) {
		result = BZ_FAILED;
		BZLoge("mDataList is null! %d", result);
	}

	if (SUCCESS(result)) {
		int numNotUsedKey = count_if(mDataList->begin(), mDataList->end(), CountNotUsedKey);
		if (mDataBase->size() + numNotUsedKey != mDataList->size()) {
			result = BZ_FAILED;
			BZLoge("mDataList cannot match mDataList! %d", result);
		}
	}
	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::SaveData()
{
	BZState result = BZ_SUCCESS;

	int32_t charLength = 0;

	result = CheckContainer();

	if (SUCCESS(result)) {
		if (mDataBase->size() * sizeof(BZDataUint) % sizeof(char) != 0) {
			charLength = mDataBase->size() * sizeof(BZDataUint) / sizeof(char) + 1;
		}
		else {
			charLength = mDataBase->size() * sizeof(BZDataUint) / sizeof(char);
		}

		char* data = (char*)malloc(mDataBase->size() * sizeof(BZDataUint));

		if (data) {
			typename unordered_map<int32_t, BZDataUint>::iterator mI;
			vector<bool>::iterator vI;
			for (mI = mDataBase->begin(), vI = mDataList->begin(); mI != mDataBase->end(); mI++, vI++) {
				memcpy(data + ((vI - mDataList->begin()) * sizeof(BZDataUint)), &(mI->second), sizeof(BZDataUint));
			}
			if (SUCCESS(result)) {
				result = mFileManager->SaveData((void*)data, &charLength);
			}
			free(data);
		}
		else {
			result = BZ_MEMORY_ERROR;
			BZLoge("Malloc failed! %d", result);
		}
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::LoadData()
{
	BZState result = BZ_SUCCESS;

	int32_t charLength = 0;
	char* data = nullptr;

	result = CheckContainer();

	if (SUCCESS(result)) {
		data = mFileManager->LoadData(&charLength);
		if (data == nullptr) {
			result = BZ_FAILED;
		}
	}

	if (SUCCESS(result) && data && charLength > 0) {
		int32_t num = 0;
		if (charLength * sizeof(char) % sizeof(BZDataUint) != 0) {
			result = BZ_UNEXPECT_ERROR;
			BZLoge("Data size should be check! it should be integral times of (PackageUint + num[NUM_BIT]!");
		}
		else {
			num = charLength * sizeof(char) / sizeof(BZDataUint);
		}
		result = UpdateDataBase((void*)data, num);
		if (SUCCESS(result))
		{
			result = mFileManager->LoadDataFinish();
		}
	}
	else {
		BZLoge("Failed to load data! %d", result);
	}

	if (SUCCESS(result))
	{
		result = SelectLastDataUint();
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::SelectDataUintById(int32_t id)
{
	BZState result = BZ_SUCCESS;

	result = CheckContainer();

	if (!mUintCtlObject && SUCCESS(result)) {
		result = BZ_FAILED;
		BZLoge("mUintCtlObject is nullptr! %d", result);
	}

	if (SUCCESS(result)) {
		if (mDataBase->empty()) {
			mSelectedId = INVALED_ID;
			result = mUintCtlObject->UpdateData(nullptr);
			BZLogi("Database has not data.");
		}
		else {
			if (mDataBase->find(id) == mDataBase->end()) {
				result = BZ_INVALID_PARAM;
				BZLogw("Cannot find invaled id:%d", id);
			}
			else {
				mSelectedId = id;
				result = mUintCtlObject->UpdateData(&(mDataBase->at(mSelectedId)));
			}
		}
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::SelectLastDataUint()
{
	BZState result = BZ_SUCCESS;
	if (mDataList && mDataBase) {
		int32_t index = INVALED_ID;
		vector<bool>::iterator vI;
		for (vI = mDataList->end() - 1; vI != mDataList->begin(); vI--) {
			if (*vI) {
				index = vI - mDataList->begin();
				break;
			}
		}
		if (vI == mDataList->begin() && *(mDataList->begin())) {
			index = mDataList->begin() - mDataList->begin();
		}
		result = SelectDataUintById(index);
	}
	else {
		result = BZ_FAILED;
		BZLoge("Database is not inited");
	}
	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::UpdateDataBase(void* data, int32_t num)
{
	BZState result = BZ_SUCCESS;

	result = ClearAll();

	if (SUCCESS(result)) {
		if (data == nullptr || num < 0) {
			result = BZ_INVALID_PARAM;
			BZLoge("data is null! %d", result);
		}
	}

	if (SUCCESS(result)) {
		BZDataUint uint;
		auto vI = mDataList->begin();
		for (int32_t i = 0; i < num; i++) {
			memcpy(&uint,
				static_cast<char*>(data) + i * (sizeof(BZDataUint)),
				sizeof(BZDataUint));
			mDataBase->insert(make_pair(vI - mDataList->begin(), uint));
			BZLogi("###### num:%d", vI - mDataList->begin());
			mDataList->push_back(KEY_USED);
			vI = mDataList->end();
		}
		BZLogi("sizeof DataBase:%d", mDataBase->size());
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
FileManager* DataManager<BZDataUint, BZDataUintCtlObject>::GetFileManager()
{
	return mFileManager;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::Add(BZDataUint uint)
{
	BZState result = BZ_SUCCESS;

	result = CheckContainer();

	int32_t index;
	if (SUCCESS(result))
	{
		vector<bool>::iterator vI = find(mDataList->begin(), mDataList->end(), KEY_NOT_USED);
		if (vI == mDataList->end()) {
			index = mDataBase->size();
			mDataBase->insert(make_pair(index, uint));
			mDataList->push_back(KEY_USED);
		}
		else {
			index = vI - mDataList->begin();
			mDataBase->insert(make_pair(index, uint));
			*vI = KEY_USED;
		}
		result = SelectDataUintById(index);

	}
	else {
		result = BZ_FAILED;
		BZLoge("Data Manager is not inited! %d", result);
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::DeleteSelectedData()
{
	BZState result = BZ_SUCCESS;

	result = CheckContainer();

	if (SUCCESS(result)) {
		int32_t deleteId = mSelectedId;
		if (mUintCtlObject) {
			if (!mDataBase->empty()) {
				if ((mDataList->size() - mSelectedId) > 0 && (mDataList->size() - mSelectedId) <= mDataList->size()) {
					mDataBase->erase(deleteId);
					mDataList->at(deleteId) = KEY_NOT_USED;
					result = SelectLastDataUint();
					BZLogi("%d is deleted", deleteId);
				}
				else {
					BZLogw("current selection %d is Invalied", deleteId);
				}
			}
			else {
				BZLogi("DataBase is empty %d", result);
			}
		}
		else {
			result = BZ_FAILED;
			BZLoge("mUintCtlObject is nullptr! %d", result);
		}
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::DisplaySelectedData()
{
	BZState result = BZ_SUCCESS;

	result = CheckContainer();

	if (SUCCESS(result)) {
		if (mUintCtlObject) {
			if (!mDataBase->empty()) {
				BZLogi(" ");
				BZLogi("Id:%d", mSelectedId);
				result = mUintCtlObject->Display();
			}
			else {
				BZLogi("DataBase is empty %d", result);
			}
		}
		else {
			result = BZ_FAILED;
			BZLoge("mUintCtlObject is nullptr! %d", result);
		}
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::DisplayAll()
{
	BZState result = BZ_SUCCESS;

	result = CheckContainer();

	if (SUCCESS(result)) {
		if (mDataBase->empty()) {
			BZLogi("DataBase is empty %d", result);
		}
		else {
			for (auto mI = mDataBase->begin(); mI != mDataBase->end(); mI++) {
				result = SelectDataUintById(mI->first);
				if (SUCCESS(result)) {
					result = DisplaySelectedData();
				}
				else {
					result = BZ_FAILED;
					BZLoge("Select Object failed! key:%d %d", mI->first, result);
					break;
				}
			}
		}
	}

	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState DataManager<BZDataUint, BZDataUintCtlObject>::ClearAll()
{
	BZState result = BZ_SUCCESS;

	result = CheckContainer();

	if (SUCCESS(result)) {

		if (mUintCtlObject) {
			mDataBase->clear();
			mDataList->clear();
			mSelectedId = INVALED_ID;
		}
		else {
			result = BZ_FAILED;
			BZLoge("mUintCtlObject is nullptr! %d", result);
		}
	}
	return result;
}

template<typename BZDataUint, class BZDataUintCtlObject>
BZState	DataManager<BZDataUint, BZDataUintCtlObject>::GetSelectedId(int32_t* id)
{
	BZState result = BZ_SUCCESS;

	if (id) {
		*id = mSelectedId;
	}
	else {
		result = BZ_FAILED;
		BZLoge("Input is nullptr! %d", result);
	}

	return result;
}
