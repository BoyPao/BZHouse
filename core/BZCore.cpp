#include "BZUtils.h"
#include "BZFileManager.h"
#include "BZDataBase.h"
#include "BZHouse.h"

enum MENU_LIST {
	SELECT_BY_ID,
	DISPLAY_SELECTED,
	LOAD_DATA,
	SAVE_DATA,
	DISPLAY_ALL,
	ADD_TEST_DATA,
	ADD_INPUT_DATA,
	DELETE_ALL,
	DELETE_SELECTED,
	EXIT
};

typedef BZHouseUint BZData;


BZState AddOneInputData(DataManager<BZData, BZHouseUintObject>* db)
{
	BZState result = BZ_SUCCESS;

	BZData u;
	char ch[BZ_MAX_HOUSE_NAME_SIZE] = { '\0' };
	int32_t intNum = 0;
	double douNum = 0.001;

	do {
		cin.clear();
		cin.ignore();
		BZLogi("Please input factory name:");
		cin >> ch;
	} while (cin.fail());
	strcpy_s(u.facName, ch);

	do {
		cin.clear();
		cin.ignore();
		BZLogi("Please input area:");
		cin >> intNum;
	} while (cin.fail());
	u.area = intNum;

	do {
		cin.clear();
		cin.ignore();
		BZLogi("Please input price:");
		cin >> douNum;
	} while (cin.fail());
	u.price = douNum;

	//need to develop map info setting
	u.mapUint.provinc.id = Zhejiang;
	strcpy_s(u.mapUint.provinc.name, "Zhejiang");
	u.mapUint.city.id = Hangzhou;
	strcpy_s(u.mapUint.city.name, "Hangzhou");
	u.mapUint.zoon.id = Zoon0;
	strcpy_s(u.mapUint.zoon.name, "Xiachengqu");

	do {
		cin.clear();
		cin.ignore();
		BZLogi("Please input longitude:");
		cin >> douNum;
	} while (cin.fail());
	u.mapUint.cp.longitude = douNum;

	do {
		cin.clear();
		cin.ignore();
		BZLogi("Please input latitude:");
		cin >> douNum;
	} while (cin.fail());
	u.mapUint.cp.latitude = douNum;

	result = db->Add(u);

	return result;
}

BZState TestAddTestData(DataManager<BZData, BZHouseUintObject>* db) {

	BZState result = BZ_SUCCESS;
	if (db) {
		BZData u;
		strcpy_s(u.facName, "JinShangWenLan");
		u.area = 100;
		u.price = 3.2;
		u.mapUint.provinc.id = Zhejiang;
		strcpy_s(u.mapUint.provinc.name, "Zhejiang");
		u.mapUint.city.id = Hangzhou;
		strcpy_s(u.mapUint.city.name, "Hangzhou");
		u.mapUint.zoon.id = Zoon0;
		strcpy_s(u.mapUint.zoon.name, "Xiachengqu");
		u.mapUint.cp.longitude = 120.168;
		u.mapUint.cp.latitude = 30.256;
		result = db->Add(u);

		strcpy_s(u.facName, "HouTaoYuan");
		u.area = 95;
		u.price = 5;
		u.mapUint.provinc.id = DirectControl;
		strcpy_s(u.mapUint.provinc.name, "None");
		u.mapUint.city.id = Beijing;
		strcpy_s(u.mapUint.city.name, "Beijing");
		u.mapUint.zoon.id = Zoon0;
		strcpy_s(u.mapUint.zoon.name, "Xichengqu");
		u.mapUint.cp.longitude = 116.367;
		u.mapUint.cp.latitude = 39.949;
		result = db->Add(u);

		strcpy_s(u.facName, "ChangChengXiaoQu");
		u.area = 115;
		u.price = 2.2;
		u.mapUint.provinc.id = Yunnan;
		strcpy_s(u.mapUint.provinc.name, "Yunnan");
		u.mapUint.city.id = Kunming;
		strcpy_s(u.mapUint.city.name, "Kunming");
		u.mapUint.zoon.id = Zoon0;
		strcpy_s(u.mapUint.zoon.name, "Wuhuaqu");
		u.mapUint.cp.longitude = 102.778;
		u.mapUint.cp.latitude = 24.902;
		result = db->Add(u);
	}
	else
	{
		result = BZ_INVALID_PARAM;
		BZLoge("db is nullptr!");
	}
	return result;
}

int32_t SelectUintById(DataManager<BZData, BZHouseUintObject>* db)
{
	int32_t id = -1;
	do {
		cin.clear();
		cin.ignore();
		BZLogi("Please input target id:");
		cin >> id;
	} while (cin.fail());

	db->SelectDataUintById(id);
	db->GetSelectedId(&id);

	return id;
}

int main() {
	BZState result = BZ_SUCCESS;

	DataManager<BZData, BZHouseUintObject> db;

	result = db.Init();

	if (SUCCESS(result)) {
		char path[] = "F:\\code\\BZHouse\\testData\\DataBase.txt";
		FileManager* fm;
		fm = db.GetFileManager();
		fm->SetFilePath(path);
	}
	
	int selectionNum = 0;
	bool needRun = true;
	bool needLoad = false;
	bool needSave = false;
	bool needSaveFirst = false;
	bool needDeleteAll = false;
	if (SUCCESS(result)) {
		while (needRun) {
			BZLogi(" ");
			BZLogi("===============================");
			BZLogi("=========== BZ Menu ===========");
			BZLogi("= %d. Select Uint by id", SELECT_BY_ID);
			BZLogi("= %d. Display selected data", DISPLAY_SELECTED);
			BZLogi("= %d. Load data", LOAD_DATA);
			BZLogi("= %d. Save data", SAVE_DATA);
			BZLogi("= %d. Display all data", DISPLAY_ALL);
			BZLogi("= %d. Add test data", ADD_TEST_DATA);
			BZLogi("= %d. Add Input data", ADD_INPUT_DATA);
			BZLogi("= %d. Delete all", DELETE_ALL);
			BZLogi("= %d. Delect selected data", DELETE_SELECTED);
			BZLogi("= %d. Exit", EXIT);
			BZLogi("=========== BZ Menu ===========");
			BZLogi("Please input your selection num:");

			cin >> selectionNum;

			if (cin.fail()) {
				cin.clear();
				cin.ignore();
			}
			else {
				switch (selectionNum)
				{
				case SELECT_BY_ID:
					BZLogi("id:%d is selected", SelectUintById(&db));
					break;
				case DISPLAY_SELECTED:
					result = db.DisplaySelectedData();
					break;
				case LOAD_DATA:
					needLoad = false;
					BZLogi("Data haven't saved will be lost");
					BZLogi("Continue to load? Input: 0 or 1");
					cin >> needLoad;
					if (needLoad) {
						result = db.LoadData();
						BZLogi("Load Data");
					}
					break;
				case SAVE_DATA:
					needSave = false;
					BZLogi("Save data will cover old data base file.");
					BZLogi("Continue to save? Input: 0 or 1");
					cin >> needSave;
					if (needSave) {
						result = db.SaveData();
						BZLogi("Save Data");
					}
					break;
				case DISPLAY_ALL:
					result = db.DisplayAll();
					break;

				case ADD_TEST_DATA:
					TestAddTestData(&db);
					BZLogi("Add test data");
					break;
				case ADD_INPUT_DATA:
					result = AddOneInputData(&db);
					break;
				case DELETE_ALL:
					needDeleteAll = false;
					BZLogi("Do you want to delete all data.");
					BZLogi("Input: 0 or 1");
					cin >> needDeleteAll;
					if (needDeleteAll) {
						result = db.ClearAll();
						BZLogi("All deleted");
					}
					break;
				case DELETE_SELECTED:
					result = db.DeleteSelectedData();
					break;
				case EXIT:
					needSaveFirst = false;
					BZLogi("Do you want to save data before exit.");
					BZLogi("Input: 0 or 1");
					cin >> needSaveFirst;
					if (needSaveFirst) {
						result = db.SaveData();
						BZLogi("Save Data");
					}
					result = db.DeInit();
					needRun = false;
					break;
				default:
					BZLogw("= Invalied input!");
					break;
				}
			}
		}
	}
	else
	{
		BZLoge("result:%d", BZ_FAILED);
	}
	db.DeInit();

	return 0;
}