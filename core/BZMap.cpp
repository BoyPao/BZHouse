#include "BZMap.h"

BZState BZMapUintObject::SetMapInfo(void* data)
{
	BZState result = BZ_SUCCESS;
	if (data) {
		if (pMap) {
			memcpy(pMap, data, sizeof(BZMapUint));
			UpdateDirectControl();
		}
		else {
			result = BZ_FAILED;
			BZLoge("Set map info failed! Map is nullptr %d", result);
		}
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("Set map info failed! %d", result);
	}
	return result;
}

BZState BZMapUintObject::GetMapInfo(void* data)
{
	BZState result = BZ_SUCCESS;
	if (data) {
		if (pMap) {
			memcpy(data, pMap, sizeof(BZMapUint));
		}
		else {
			result = BZ_FAILED;
			BZLoge("Set map info failed! Map is nullptr %d", result);
		}
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("Get map info failed! %d", result);
	}
	return result;
}

BZState BZMapUintObject::UpdateDirectControl()
{
	BZState result = BZ_SUCCESS;

	if (pMap) {
		switch (pMap->city.id) {
		case Beijing:
		case Tianjin:
		case Shanghai:
		case Chongqing:
			directControl = true;
			break;
		default:
			directControl = false;
			break;
		}
		//BZLogi("%d %s directControl:%d", pMap->city.id, pMap->city.name, directControl);
	}
	else {
		directControl = false;
		//result = BZ_FAILED;
		//BZLoge("mMap is mullptr! %d", result);
	}
	return result;
}

BZState BZMapUintObject::SetMap(void* ptr)
{
	BZState result = BZ_SUCCESS;

	if (ptr)
	{
		pMap = static_cast<BZMapUint*>(ptr);
	}
	else {
		pMap = nullptr;
		//result = BZ_INVALID_PARAM;
		//BZLoge("ptr is nullptr! %d", result);
	}

	if (SUCCESS(result)) {
		result = UpdateDirectControl();
	}

	return result;
}
