#pragma once
#include "BZUtils.h"

//#define SHIFT_BIT 3

enum CODE_TYPE {
	TYPE_ONE = 0,
	TYPE_NONE,
	TYPE_NUM
};

enum OPERATE {
	ENCODE = 0,
	DECODE,
	INVALED_OP
};

enum SHIFT_BIT {
	ONE_BIT = 0x1,
	TWO_BIT = 0x3,
	THREE_BIT = 0x7,
	FOUR_BIT = 0x15
};

class Coder {
public:
	BZState Init();
	BZState EnCode(void* data, int32_t length);
	BZState DeCode(void* data, int32_t length);
	BZState GetCodeType(int32_t* type);
	BZState SetCodeType(int32_t* type);
private:

	BZState ShiftCode(void* data, int32_t length, OPERATE op);
	CODE_TYPE codeType;
};