#pragma once
#include "BZUtils.h"
#include "BZMap.h"

#define BZ_MAX_HOUSE_NAME_SIZE 20

struct BZHouseUint {
	char facName[BZ_MAX_HOUSE_NAME_SIZE];
	double price;	//1 equals ten thousands RMB
	int32_t area;
	BZMapUint mapUint;
	//need to develop
};

class BZHouseUintObject :public BZMapUintObject {
public:

	BZState Init();
	BZState SetHouseInfo(void* data);

	BZState GetHouseInfo(void* data);
	BZState UpdateMapInfo();
	BZState UpdateTotalPrice();
	BZState Display();
	BZState UpdateData(void* ptr);

	double GetTotalPrice() { return totalPrice; }

private:

	BZHouseUint* pHouse;
	double totalPrice;

};