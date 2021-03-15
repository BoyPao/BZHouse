#include "BZHouse.h"

BZState BZHouseUintObject::Init()
{
	BZState result = BZ_SUCCESS;

	this->pHouse = nullptr;
	this->totalPrice = 0;

	return result;
}

BZState BZHouseUintObject::SetHouseInfo(void* data)
{
	BZState result = BZ_SUCCESS;
	if (data) {
		if (pHouse) {
			memcpy(pHouse, data, sizeof(BZHouseUint));
			UpdateTotalPrice();
		}
		else {
			result = BZ_FAILED;
			BZLoge("Set house info failed! pHouse is nullptr %d", result);
		}
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("Set house info failed! %d", result);
	}
	return result;
}

BZState BZHouseUintObject::GetHouseInfo(void* data)
{
	BZState result = BZ_SUCCESS;
	if (data) {
		if (pHouse) {
			memcpy(data, pHouse, sizeof(BZHouseUint));
		}
		else {
			result = BZ_FAILED;
			BZLoge("Set house info failed! pHouse is nullptr %d", result);
		}
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("Get house info failed! %d", result);
	}
	return result;
}

BZState BZHouseUintObject::UpdateMapInfo()
{
	BZState result = BZ_SUCCESS;

	if (pHouse) {
		result = SetMap(&(pHouse->mapUint));
	}
	else {
		result = SetMap(nullptr);
	}

	return result;
}

BZState BZHouseUintObject::UpdateTotalPrice()
{
	BZState result = BZ_SUCCESS;

	if (pHouse) {
		totalPrice = (pHouse->price) * (pHouse->area);
	}
	else {
		totalPrice = 0;
	}

	return result;
}

BZState BZHouseUintObject::Display()
{
	BZState result = BZ_SUCCESS;

	if (pHouse) {
		BZLogi("<------------------------------>");
		BZLogi("House: %s", pHouse->facName);
		BZLogi("Area: %d m2", pHouse->area);
		BZLogi("Price: %.2lf wRMR/m2	Total around: %.2lf wRMB", pHouse->price, GetTotalPrice());
		if (isDirectControl()) {
			BZLogi("Place: %s %s",
				pHouse->mapUint.city.name,
				pHouse->mapUint.zoon.name);
		}
		else {
			BZLogi("Place: %s %s %s",
				pHouse->mapUint.provinc.name,
				pHouse->mapUint.city.name,
				pHouse->mapUint.zoon.name);
		}
		BZLogi("Place: (%.3lf, %.3lf )", pHouse->mapUint.cp.longitude, pHouse->mapUint.cp.latitude);
		BZLogi("<------------------------------>");
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("Get house info failed! %d", result);
	}

	return result;
}

BZState BZHouseUintObject::UpdateData(void* ptr)
{
	BZState result = BZ_SUCCESS;

	if (ptr) {
		pHouse = static_cast<BZHouseUint*>(ptr);
	}
	else {
		pHouse = nullptr;
		//result = BZ_INVALID_PARAM;
		//BZLoge("ptr is nullptr! %d", result);
	}

	result = UpdateTotalPrice();

	if (SUCCESS(result)) {
		result = UpdateMapInfo();
	}

	return result;
}