#pragma once
#pragma once
#include "BZUtils.h"

#define BZ_MAX_MAP_NAME_SIZE 20

enum CITY_ID {
	Beijing = 0,
	Tianjin,
	Shanghai,
	Chongqing,
	Hangzhou,
	Kunming,
	CITY_NUM
};

enum PROVINCE_ID {
	DirectControl = 0,
	Zhejiang,
	Yunnan,
	PROVINCE_NUM
};

enum ZOON_ID
{
	Zoon0 = 0,
	Zoon1 = 1,
	Zoon_Max
};

struct BZMapUintID {
	int32_t id;
	char name[BZ_MAX_MAP_NAME_SIZE];
};

struct BZMapUintCoordinate {
	double longitude;
	double latitude;
};

struct BZMapUint {
	BZMapUintID provinc;
	BZMapUintID city;
	BZMapUintID zoon;
	BZMapUintCoordinate cp;
};

class BZMapUintObject {
public:
	BZState SetMapInfo(void* data);
	BZState GetMapInfo(void* data);
	BZState UpdateDirectControl();
	bool isDirectControl() { return directControl; };

	BZState SetMap(void* ptr);
private:
	BZMapUint* pMap;
	bool directControl;
};