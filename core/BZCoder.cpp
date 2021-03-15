#include "BZCoder.h"

BZState Coder::Init()
{
	BZState result = BZ_SUCCESS;

	codeType = TYPE_ONE;

	return result;
}

BZState Coder::GetCodeType(int32_t* type)
{
	BZState result = BZ_SUCCESS;

	*type = codeType;

	return result;
}

BZState Coder::SetCodeType(int32_t* type)
{
	BZState result = BZ_SUCCESS;

	switch (*type) {
	case TYPE_ONE:
		codeType = TYPE_ONE;
		break;
	default:
		result = BZ_INVALID_PARAM;
		BZLoge("Invaled code type! %d", type);
		break;
	}

	return result;
}

BZState Coder::EnCode(void* data, int32_t length)
{
	BZState result = BZ_SUCCESS;

	if (data) {
		switch (codeType) {
		case TYPE_ONE:
			result = ShiftCode(data, length, ENCODE);
			break;
		case TYPE_NONE:
		default:
			break;
		}
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("data is null! %d", result);
	}

	return result;
}

BZState Coder::DeCode(void* data, int32_t length)
{
	BZState result = BZ_SUCCESS;

	if (data) {
		switch (codeType) {
		case TYPE_ONE:
			result = ShiftCode(data, length, DECODE);
			break;
		case TYPE_NONE:
		default:
			break;
		}
	}
	else {
		result = BZ_INVALID_PARAM;
		BZLoge("data is null! %d", result);
	}

	return result;
}

BZState Coder::ShiftCode(void* data, int32_t length, OPERATE op)
{
	BZState result = BZ_SUCCESS;
	char container;

	switch (op)
	{
	case ENCODE:
		for (int32_t i = 0; i < length; i++) {
			//BZLogi("++++ %d", static_cast<char*>(data)[i]);
			container = ((((static_cast<char*>(data)[i]) & 0x7) << 5) & 0xE0) +
				((((static_cast<char*>(data)[i]) & 0xF8) >> 3) & 0x1F);
			static_cast<char*>(data)[i] = container;
			//BZLogi("---- %d", static_cast<char*>(data)[i]);
		}
		break;
	case DECODE:
		for (int32_t i = 0; i < length; i++) {
			container = ((((static_cast<char*>(data)[i]) & 0x1F) << 3) & 0xF8) +
				((((static_cast<char*>(data)[i]) & 0xE0) >> 5) & 0x7);
			static_cast<char*>(data)[i] = container;
		}
		break;
	default:
		break;
	}

	return result;
}