#pragma once
#include "BZUtils.h"

#define LOG_ON 1

void LogWithInfo(const char* str, ...);

enum SYSTEM_TYPES {
	WINDOWS_SYSTEM = 1,
	LINUX_SYSTEM
};

#define SYSTEM_TYPE WINDOWS_SYSTEM


#define IS_WINDOWS() (SYSTEM_TYPE == WINDOWS_SYSTEM ? 1 : 0)
#define IS_LINUX() (SYSTEM_TYPE == LINUX_SYSTEM ? 1 : 0)

#define LOG_FORMATE "%d:%s:"

#define BZLoge(str, ...)	LogWithInfo("[E] " LOG_FORMATE str, __LINE__, __func__, ##__VA_ARGS__)
#define BZLogw(str, ...)	LogWithInfo("[W] " LOG_FORMATE str, __LINE__, __func__, ##__VA_ARGS__)
#define BZLogi(str, ...)	LogWithInfo("[I] " LOG_FORMATE str, __LINE__, __func__, ##__VA_ARGS__)