#pragma once
//Regular lib
#include <string>
#include <iostream>
#include <time.h>
#include <unordered_map>

#include "BZLog.h"

#define SUCCESS(s) ((s) == BZ_SUCCESS ? true : false)

enum BZState {
	BZ_SUCCESS = 0,
	BZ_INVALID_PARAM,
	BZ_MEMORY_ERROR,
	BZ_FAILED,
	BZ_UNEXPECT_ERROR
};

struct BZDate {
	int32_t year;
	int32_t month;
	int32_t day;
};

struct BZTime
{
	int32_t hour;
	int32_t min;
	int32_t second;
};