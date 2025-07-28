#include "pxlibext.h"
#include "JSONDefs.h"
#include <fstream>


std::set<int> PXLibExt::FIELDTYPES_OUTPUT_STRING;
std::set<int> PXLibExt::FIELDTYPES_INTERNAL_STRING;
std::set<int> PXLibExt::FIELDTYPES_INTERNAL_LONG;
std::set<int> PXLibExt::FIELDTYPES_INTERNAL_DOUBLE;
std::set<int> PXLibExt::FIELDTYPES_INTERNAL_BLOB;



std::string PXLibExt::OutputRecordsVector(const std::vector<std::unordered_map<std::string, std::string>>& recordsVector, bool addWhitespace, bool ifNoRecordsOutputEmptyArray)
{
	std::stringstream outputStream;

	if (ifNoRecordsOutputEmptyArray || recordsVector.size() > 0)
	{
		outputStream << JSONDefs::ADS;

		std::vector<std::unordered_map<std::string, std::string>>::const_iterator recordsVectorIter = recordsVector.begin();
		for (; recordsVectorIter != recordsVector.end(); recordsVectorIter++)
		{
			if (recordsVectorIter != recordsVector.begin())
			{
				outputStream << JSONDefs::OAS;
			}

			if (addWhitespace)
			{
				outputStream << JSONDefs::WSNL;
			}

			std::stringstream recordOutputStream;

			std::unordered_map<std::string, std::string> fieldsKVPMap = *recordsVectorIter;

			if (fieldsKVPMap.size() > 0)
			{
				recordOutputStream << JSONDefs::ODS;

				std::unordered_map<std::string, std::string>::const_iterator fieldsKVPMapIter = fieldsKVPMap.begin();
				for (; fieldsKVPMapIter != fieldsKVPMap.end(); fieldsKVPMapIter++)
				{
					if (fieldsKVPMapIter != fieldsKVPMap.begin())
					{
						recordOutputStream << JSONDefs::EOS;
					}

					if (addWhitespace)
					{
						recordOutputStream << JSONDefs::WSSP;
					}


					std::stringstream fieldOutputStream;

					std::string fieldName = (*fieldsKVPMapIter).first;
					std::string valueStr = (*fieldsKVPMapIter).second;

					fieldOutputStream << JSONDefs::EDS << fieldName << JSONDefs::EDF; // Name element - always string (delimited with double quotes)
					fieldOutputStream << JSONDefs::NVS; // Name value separator
					fieldOutputStream << valueStr; // Value element - should be pre-delimited if it's a string(eque) type

					recordOutputStream << fieldOutputStream.str();
				}


				if (addWhitespace)
				{
					recordOutputStream << JSONDefs::WSSP;
				}

				recordOutputStream << JSONDefs::ODF;

			}

			outputStream << recordOutputStream.str();
		}

		if (addWhitespace)
		{
			outputStream << JSONDefs::WSNL;
		}

		outputStream << JSONDefs::ADF;

	}

	return outputStream.str();
}

//bool PXLibExt::GetFieldValueString(pxdoc_t* pxdoc, char px_ftype, int px_flen, int px_fdc, char* readBuffer, int offset, std::string& valueStr)
//{
//	bool success = false;
//	valueStr = "";
//
//	bool weAllocedReadBuffer = false;
//	if (readBuffer == NULL)
//	{
//		readBuffer = (char*)calloc(PXLibExtDefs::READBUFFER_BYTESSIZE, 1); // Just a buffer for reading data.
//		weAllocedReadBuffer = true;
//	}
//
//	if (readBuffer == NULL)
//	{
//		throw std::exception("Could not allocate readBuffer.");
//	}
//
//	if (pxdoc == NULL)
//	{
//		throw std::invalid_argument("Argument pxdoc is null.");
//	}
//
//	bool isStringValueType = true;
//
//	switch (px_ftype)
//	{
//	case pxfAlpha:
//	{
//		isStringValueType = true;
//		char* value;
//		if (0 < PX_get_data_alpha(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			valueStr = std::string(value);
//			pxdoc->free(pxdoc, value);
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfDate: {
//		isStringValueType = true;
//		long value;
//		if (0 < PX_get_data_long(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			//struct tm valueTm {};
//			//valueTm = PX_date2tm(pxdoc, value);
//			valueStr = PX_date2stdstring(pxdoc, value);
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfShort: {
//		isStringValueType = false;
//		short int value;
//		if (0 < PX_get_data_short(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			valueStr = std::to_string(value);
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfAutoInc:
//	case pxfLong: {
//		isStringValueType = false;
//		long value;
//		if (0 < PX_get_data_long(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			valueStr = std::to_string(value);
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfTimestamp: {
//		isStringValueType = true;
//		double value;
//		if (0 < PX_get_data_double(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			//struct tm valueTm {};
//			//valueTm = PX_timestamp2tm(pxdoc, value);
//			valueStr = PX_timestamp2stdstring(pxdoc, value);
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfTime: {
//		isStringValueType = true;
//		long value;
//		if (0 < PX_get_data_long(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			//struct tm valueTm {};
//			//valueTm = PX_time2tm(pxdoc, value);
//			valueStr = PX_time2stdstring(pxdoc, value);
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfCurrency:
//	case pxfNumber: {
//		isStringValueType = false;
//		double value;
//		if (0 < PX_get_data_double(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			valueStr = std::to_string(value);
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfLogical: {
//		isStringValueType = false;
//		char value;
//		if (0 < PX_get_data_byte(pxdoc, &readBuffer[offset], px_flen, &value))
//		{
//			success = true;
//			valueStr = value ? PXLibExtDefs::LOGICALFIELD_TRUE : PXLibExtDefs::LOGICALFIELD_FALSE;
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfBLOb:
//	case pxfGraphic:
//	case pxfOLE:
//	case pxfMemoBLOb:
//	case pxfFmtMemoBLOb:
//	{
//		isStringValueType = true;
//		char* value;
//		int mod_nr, size, ret;
//		if (px_ftype == pxfGraphic)
//		{
//			ret = PX_get_data_graphic(pxdoc, &readBuffer[offset], px_flen, &mod_nr, &size, &value);
//		}
//		else
//		{
//			ret = PX_get_data_blob(pxdoc, &readBuffer[offset], px_flen, &mod_nr, &size, &value);
//		}
//		if (ret > 0)
//		{
//			success = true;
//			if (value)
//			{
//				valueStr = std::string(value);
//				pxdoc->free(pxdoc, value);
//			}
//			else
//			{
//			}
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfBCD:
//	{
//		isStringValueType = false;
//		char* value;
//		int gotBCDRet;
//		if (0 < (gotBCDRet = PX_get_data_bcd(pxdoc, (unsigned char*)(&readBuffer[offset]), px_fdc, &value)))
//		{
//			valueStr = std::string(value);
//			pxdoc->free(pxdoc, value);
//			success = true;
//		}
//		else if (gotBCDRet == 0)
//		{
//			success = true;
//		}
//		else
//		{
//		}
//		break;
//	}
//	case pxfBytes:
//	{
//		isStringValueType = false;
//		valueStr = PXLibExtDefs::FIELD_NULL;
//	}
//	break;
//	default:
//	{
//		isStringValueType = false;
//		valueStr = PXLibExtDefs::FIELD_NULL;
//	}
//	break;
//	}
//
//	if (isStringValueType)
//	{
//		valueStr = ReplaceString(valueStr, PXLibExtDefs::REVERSESOLIDUS_UNESCAPED, PXLibExtDefs::REVERSESOLIDUS_ESCAPED); // Escape Reverse Solidus - do this first always!
//		valueStr = ReplaceString(valueStr, PXLibExtDefs::SOLIDUS_UNESCAPED, PXLibExtDefs::SOLIDUS_ESCAPED); // Escape Solidus
//		valueStr = ReplaceString(valueStr, PXLibExtDefs::QUOTEMARK_UNESCAPED, PXLibExtDefs::QUOTEMARK_ESCAPED); // Escape Quote Mark
//		valueStr = PXLibExtDefs::STRINGFIELD_DELIMITER + valueStr + PXLibExtDefs::STRINGFIELD_DELIMITER;
//	}
//	else
//	{
//		if (valueStr.length() == 0)
//		{
//			valueStr = PXLibExtDefs::FIELD_NULL;
//		}
//	}
//
//	if (weAllocedReadBuffer)
//	{
//		if (readBuffer != NULL)
//		{
//			free(readBuffer);
//			readBuffer = NULL;
//		}
//
//	}
//
//	return success;
//}


void PXLibExt::InitFieldTypeHashes()
{

	if (FIELDTYPES_OUTPUT_STRING.begin() == FIELDTYPES_OUTPUT_STRING.end())
	{
		FIELDTYPES_OUTPUT_STRING.insert(pxfAlpha);
		FIELDTYPES_OUTPUT_STRING.insert(pxfDate);
		FIELDTYPES_OUTPUT_STRING.insert(pxfTimestamp);
		FIELDTYPES_OUTPUT_STRING.insert(pxfTime);
		FIELDTYPES_OUTPUT_STRING.insert(pxfBLOb);
		FIELDTYPES_OUTPUT_STRING.insert(pxfGraphic);
		FIELDTYPES_OUTPUT_STRING.insert(pxfOLE);
		FIELDTYPES_OUTPUT_STRING.insert(pxfMemoBLOb);
		FIELDTYPES_OUTPUT_STRING.insert(pxfFmtMemoBLOb);
		FIELDTYPES_OUTPUT_STRING.insert(pxfBytes);
	}
	if (FIELDTYPES_INTERNAL_STRING.begin() == FIELDTYPES_INTERNAL_STRING.end())
	{
		FIELDTYPES_INTERNAL_STRING.insert(pxfAlpha);
		FIELDTYPES_INTERNAL_STRING.insert(pxfBLOb);
		FIELDTYPES_INTERNAL_STRING.insert(pxfGraphic);
		FIELDTYPES_INTERNAL_STRING.insert(pxfOLE);
		FIELDTYPES_INTERNAL_STRING.insert(pxfMemoBLOb);
		FIELDTYPES_INTERNAL_STRING.insert(pxfFmtMemoBLOb);
		FIELDTYPES_INTERNAL_STRING.insert(pxfBCD);
		FIELDTYPES_INTERNAL_STRING.insert(pxfBytes);
	}
	if (FIELDTYPES_INTERNAL_LONG.begin() == FIELDTYPES_INTERNAL_LONG.end())
	{
		FIELDTYPES_INTERNAL_LONG.insert(pxfDate);
		FIELDTYPES_INTERNAL_LONG.insert(pxfShort);
		FIELDTYPES_INTERNAL_LONG.insert(pxfAutoInc);
		FIELDTYPES_INTERNAL_LONG.insert(pxfLong);
		FIELDTYPES_INTERNAL_LONG.insert(pxfTime);
		FIELDTYPES_INTERNAL_LONG.insert(pxfLogical);
	}
	if (FIELDTYPES_INTERNAL_DOUBLE.begin() == FIELDTYPES_INTERNAL_DOUBLE.end())
	{
		FIELDTYPES_INTERNAL_DOUBLE.insert(pxfTimestamp);
		FIELDTYPES_INTERNAL_DOUBLE.insert(pxfCurrency);
		FIELDTYPES_INTERNAL_DOUBLE.insert(pxfNumber);
	}
	if (FIELDTYPES_INTERNAL_BLOB.begin() == FIELDTYPES_INTERNAL_BLOB.end())
	{
		FIELDTYPES_INTERNAL_BLOB.insert(pxfBLOb);
		FIELDTYPES_INTERNAL_BLOB.insert(pxfGraphic);
		FIELDTYPES_INTERNAL_BLOB.insert(pxfOLE);
		FIELDTYPES_INTERNAL_BLOB.insert(pxfMemoBLOb);
		FIELDTYPES_INTERNAL_BLOB.insert(pxfFmtMemoBLOb);
	}

}

bool PXLibExt::IsStringOutputType(int px_ftype)
{
	InitFieldTypeHashes();
	return FIELDTYPES_OUTPUT_STRING.find(px_ftype) != FIELDTYPES_OUTPUT_STRING.end();
}

bool PXLibExt::IsStringInternalType(int px_ftype)
{
	InitFieldTypeHashes();
	return FIELDTYPES_INTERNAL_STRING.find(px_ftype) != FIELDTYPES_INTERNAL_STRING.end();
}

bool PXLibExt::IsDoubleInternalType(int px_ftype)
{
	InitFieldTypeHashes();
	return FIELDTYPES_INTERNAL_LONG.find(px_ftype) != FIELDTYPES_INTERNAL_LONG.end();
}

bool PXLibExt::IsLongInternalType(int px_ftype)
{
	InitFieldTypeHashes();
	return FIELDTYPES_INTERNAL_DOUBLE.find(px_ftype) != FIELDTYPES_INTERNAL_DOUBLE.end();
}

bool PXLibExt::IsBlobInternalType(int px_ftype)
{
	InitFieldTypeHashes();
	return FIELDTYPES_INTERNAL_BLOB.find(px_ftype) != FIELDTYPES_INTERNAL_BLOB.end();
}

char PXLibExt::ToBase64Char(int v)
{
	char c;
	switch (v)
	{
	case 0: c = 'A';break;case 1: c = 'B';break;case 2: c = 'C';break;case 3: c = 'D';break;
	case 4: c = 'E';break;case 5: c = 'F';break;case 6: c = 'G';break;case 7: c = 'H';break;
	case 8: c = 'I';break;case 9: c = 'J';break;case 10: c = 'K';break;case 11: c = 'L';break;
	case 12: c = 'M';break;case 13: c = 'N';break;case 14: c = 'O';break;case 15: c = 'P';break;
	case 16: c = 'Q';break;case 17: c = 'R';break;case 18: c = 'S';break;case 19: c = 'T';break;
	case 20: c = 'U';break;case 21: c = 'V';break;case 22: c = 'W';break;case 23: c = 'X';break;
	case 24: c = 'Y';break;case 25: c = 'Z';break;case 26: c = 'a';break;case 27: c = 'b';break;
	case 28: c = 'c';break;case 29: c = 'd';break;case 30: c = 'e';break;case 31: c = 'f';break;
	case 32: c = 'g';break;case 33: c = 'h';break;case 34: c = 'i';break;case 35: c = 'j';break;
	case 36: c = 'k';break;case 37: c = 'l';break;case 38: c = 'm';break;case 39: c = 'n';break;
	case 40: c = 'o';break;case 41: c = 'p';break;case 42: c = 'q';break;case 43: c = 'r';break;
	case 44: c = 's';break;case 45: c = 't';break;case 46: c = 'u';break;case 47: c = 'v';break;
	case 48: c = 'w';break;case 49: c = 'x';break;case 50: c = 'y';break;case 51: c = 'z';break;
	case 52: c = '0';break;case 53: c = '1';break;case 54: c = '2';break;case 55: c = '3';break;
	case 56: c = '4';break;case 57: c = '5';break;case 58: c = '6';break;case 59: c = '7';break;
	case 60: c = '8';break;case 61: c = '9';break;case 62: c = '+';break;case 63: c = '/';break;
	}
	return c;
}

std::string PXLibExt::ToBase64String(const char* input, int len)
{
	unsigned int padding = 0;
	std::vector<int> v;
	bool lenProvided = len >= 0;
	for (unsigned int i = 0; (lenProvided && i < len) || (!lenProvided && input[i] != '\0'); i += 3)
	{
		v.push_back((input[i] & 0xFC) >> 2);
		if ((lenProvided && i + 1 == len) || (!lenProvided && input[i + 1] == '\0'))
		{
			padding = 3;
			break;
		}
		v.push_back(((input[i] & 0x03) << 4) | ((input[i + 1] & 0xF0) >> 4));
		if ((lenProvided && i + 2 == len) || (!lenProvided && input[i + 2] == '\0'))
		{
			padding = 2;
			break;
		}
		v.push_back(((input[i + 1] & 0x0F) << 2) | ((input[i + 2] & 0xC0) >> 6));
		if ((lenProvided && i + 3 == len) || (!lenProvided && input[i + 3] == '\0'))
		{
			padding = 1;
			break;
		}
		v.push_back(input[i + 2] & 0x3F);
	}
	std::ostringstream oss;
	for (unsigned int i = 0; i < v.size(); i++)
	{
		oss << ToBase64Char(v[i]);
	}
	for (; padding > 0; padding--)
	{
		oss << "=";
	}
	return oss.str();
}


bool PXLibExt::GetFieldValueString(pxdoc_t* pxdoc, pxval_t* data, std::string& valueStr)
{
	bool success = false;
	valueStr = "";

	if (data == NULL)
	{
		throw std::invalid_argument("Argument data is null.");
	}

	int px_ftype = data->type;

	bool isStringValueType = IsStringOutputType(px_ftype);

	switch (px_ftype)
	{
	case pxfAlpha:
	{
		//isStringValueType = true;
		char* value = data->value.str.val;
		if (value != NULL)
		{
			success = true;
			valueStr = std::string(value);
		}
		else
		{
		}
		break;
	}
	case pxfDate: {
		//isStringValueType = true;
		long value = data->value.lval;
		if (true)
		{
			success = true;
			valueStr = PX_date2stdstring(pxdoc, value);
		}
		else
		{
		}
		break;
	}
	case pxfShort: {
		//isStringValueType = false;
		short int value = data->value.lval;
		if (true)
		{
			success = true;
			valueStr = std::to_string(value);
		}
		else
		{
		}
		break;
	}
	case pxfAutoInc:
	case pxfLong: {
		//isStringValueType = false;
		long value = data->value.lval;
		if (true)
		{
			success = true;
			valueStr = std::to_string(value);
		}
		else
		{
		}
		break;
	}
	case pxfTimestamp: {
		//isStringValueType = true;
		double value = data->value.dval;
		if (true)
		{
			success = true;
			valueStr = PX_timestamp2stdstring(pxdoc, value);
		}
		else
		{
		}
		break;
	}
	case pxfTime: {
		//isStringValueType = true;
		long value = data->value.lval;
		if (true)
		{
			success = true;
			valueStr = PX_time2stdstring(pxdoc, value);
		}
		else
		{
		}
		break;
	}
	case pxfCurrency:
	case pxfNumber: {
		//isStringValueType = false;
		double value = data->value.dval;
		if (true)
		{
			success = true;
			valueStr = std::to_string(value);
		}
		else
		{
		}
		break;
	}
	case pxfLogical: {
		//isStringValueType = false;
		char value = data->value.lval;
		if (true)
		{
			success = true;
			valueStr = value ? PXLibExtDefs::LOGICALFIELD_TRUE : PXLibExtDefs::LOGICALFIELD_FALSE;
		}
		else
		{
		}
		break;
	}
	case pxfBLOb:
	case pxfGraphic:
	case pxfOLE:
	case pxfMemoBLOb:
	case pxfFmtMemoBLOb:
	case pxfBytes:
	{
		//isStringValueType = true;
		int strLen = data->value.str.len;
		char* strVal = data->value.str.val;
		if (strVal != NULL)
		{
			char* value = (char*)calloc(sizeof(char), strLen + 1); // Got to ensure at least one terminating char (blobs aren't guaranteed to have terminator).
			if (value != NULL)
			{
				memcpy(value, strVal, sizeof(char) * strLen);
				success = true;
				if (value != NULL)
				{
					bool isMemoField = px_ftype == pxfMemoBLOb || px_ftype == pxfFmtMemoBLOb;

					if (isMemoField)
					{
						valueStr = std::string(value);
					}
					else
					{ 
						valueStr = ToBase64String(value, strLen + 1); // You need to specify the len, if you just pass the char* it might have an early terminator.
					}
					free(value);
					value = NULL;
				}
				else
				{
				}
			}
			else
			{
			}
		}
		break;
	}
	case pxfBCD:
	{
		//isStringValueType = false;
		char* value = data->value.str.val;
		if (value != NULL)
		{
			valueStr = std::string(value);
			success = true;
		}
		else
		{
		}
		break;
	}
	default:
	{
		//isStringValueType = false;
		valueStr = PXLibExtDefs::FIELD_NULL;
	}
	break;
	}

	if (isStringValueType)
	{
		valueStr = ReplaceString(valueStr, PXLibExtDefs::REVERSESOLIDUS_UNESCAPED, PXLibExtDefs::REVERSESOLIDUS_ESCAPED); // Escape Reverse Solidus - do this first always!
		valueStr = ReplaceString(valueStr, PXLibExtDefs::SOLIDUS_UNESCAPED, PXLibExtDefs::SOLIDUS_ESCAPED); // Escape Solidus
		valueStr = ReplaceString(valueStr, PXLibExtDefs::QUOTEMARK_UNESCAPED, PXLibExtDefs::QUOTEMARK_ESCAPED); // Escape Quote Mark
		valueStr = PXLibExtDefs::STRINGFIELD_DELIMITER + valueStr + PXLibExtDefs::STRINGFIELD_DELIMITER;
	}
	else
	{
		if (valueStr.length() == 0)
		{
			valueStr = PXLibExtDefs::FIELD_NULL;
		}
	}

	return success;
}






bool PXLibExt::SetFieldValueString(pxdoc_t* pxdoc, pxval_t* data, const std::string valueStr, int maxStrFieldLen)//, int bCDDecLen)
{
	bool success = false;

	if (data == NULL)
	{
		throw std::invalid_argument("Argument data is null.");
	}

	int px_ftype = data->type;

	if (IsStringInternalType(px_ftype))
	{
		std::string valueStrTrimmed = valueStr;
		if (maxStrFieldLen > 0 && valueStrTrimmed.length() > maxStrFieldLen)
		{
			valueStrTrimmed = valueStrTrimmed.substr(0, maxStrFieldLen);
		}
		if (data->value.str.val == NULL)
		{
			data->value.str.val = (char*)pxdoc->malloc(pxdoc, maxStrFieldLen + 1, "Allocate memory for field data.");
		}
		strcpy(data->value.str.val, valueStrTrimmed.c_str());
		data->value.str.val[maxStrFieldLen] = '\0';
		//if (px_ftype == pxfBCD)
		//{
		//	data->value.str.len = bCDDecLen;
		//}
		//else
		{
			data->value.str.len = maxStrFieldLen;
		}
		success = true;
	}
	else
	{


		switch (px_ftype)
		{
			//case pxfAlpha:
			//{
			//	std::string valueStrTrimmed = valueStr;
			//	if (maxFieldLen > 0 && valueStrTrimmed.length() > maxFieldLen)
			//	{
			//		valueStrTrimmed = valueStrTrimmed.substr(0, maxFieldLen);
			//	}
			//	strcpy(data->value.str.val, valueStrTrimmed.c_str());
			//	success = true;
			//	break;
			//}
			case pxfDate: {
				data->value.lval = PX_stdstring2date(valueStr);
				success = true;
				break;
			}
			case pxfShort: {
				data->value.lval = std::stoi(valueStr);
				success = true;
				break;
			}
			case pxfAutoInc:
			case pxfLong: {
				data->value.lval = std::stoi(valueStr);
				success = true;
				break;
			}
			case pxfTimestamp: {
				data->value.dval = PX_stdstring2timestamp(valueStr);
				success = true;
				break;
			}
			case pxfTime: {
				data->value.lval = PX_stdstring2time(valueStr);
				success = true;
				break;
			}
			case pxfCurrency:
			case pxfNumber: {
				data->value.dval = std::stod(valueStr);
				success = true;
				break;
			}
			case pxfLogical: {
				data->value.lval = valueStr == PXLibExtDefs::LOGICALFIELD_TRUE ? 1 : 0;
				success = true;
				break;
			}
			//case pxfBLOb:
			//case pxfGraphic:
			//case pxfOLE:
			//case pxfMemoBLOb:
			//case pxfFmtMemoBLOb:
			//{
			//	std::string valueStrTrimmed = valueStr;
			//	if (maxFieldLen > 0 && valueStrTrimmed.length() > maxFieldLen)
			//	{
			//		valueStrTrimmed = valueStrTrimmed.substr(0, maxFieldLen);
			//	}
			//	strcpy(data->value.str.val, valueStrTrimmed.c_str());
			//	success = true;
			//	break;
			//}
			//case pxfBCD:
			//{
			//	std::string valueStrTrimmed = valueStr;
			//	if (maxFieldLen > 0 && valueStrTrimmed.length() > maxFieldLen)
			//	{
			//		valueStrTrimmed = valueStrTrimmed.substr(0, maxFieldLen);
			//	}
			//	strcpy(data->value.str.val, valueStrTrimmed.c_str());
			//	success = true;
			//	break;
			//}
			//case pxfBytes:
			//{
			//	std::string valueStrTrimmed = valueStr;
			//	if (maxFieldLen > 0 && valueStrTrimmed.length() > maxFieldLen)
			//	{
			//		valueStrTrimmed = valueStrTrimmed.substr(0, maxFieldLen);
			//	}
			//	strcpy(data->value.str.val, valueStrTrimmed.c_str());
			//	success = true;
			//	break;
			//}
			default: {
				break;
			}
		}

	}


	return success;
}




std::string PXLibExt::ReplaceString(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}








/// <summary>
/// Read the primary key field names.
/// </summary>
/// <param name="tableFilePath">The file path of the Paradox table</param>
/// <param name="delimiter">The string delimiter between each field name, default comma.</param>
/// <returns>The primary key field names.</returns>
std::string PXLibExt::GetPrimaryKeyFieldNames(std::string tableFilePath, std::string delimiter)
{
	bool primaryKeyFieldNamesOnly = true;
	return GetFieldNames(tableFilePath, delimiter, primaryKeyFieldNamesOnly);
}

/// <summary>
/// Read the field names.
/// </summary>
/// <param name="tableFilePath">The file path of the Paradox table</param>
/// <param name="delimiter">The string delimiter between each field name, default comma.</param>
/// <param name="primaryKeyFieldNamesOnly">If you just want primary key field names.</param>
/// <returns>The field names.</returns>
std::string PXLibExt::GetFieldNames(std::string tableFilePath, std::string delimiter, bool primaryKeyFieldNamesOnly)
{

	std::string output = "";

	bool outputRecordNumber = true;

	std::vector<std::string> fieldNamesVector;

	try
	{
		PX_boot();
		pxdoc_t* pxdoc = PX_new();

		if (pxdoc == NULL) { throw new std::exception("Could not create new pxdoc"); }

		bool openedPXDocFile = PX_open_file(pxdoc, tableFilePath.c_str()) >= 0;

		if (openedPXDocFile)
		{
			try
			{


				pxhead_t* pxh = pxdoc->px_head;

				pxfield_t* pxf = pxh->px_fields;
				int fieldsCount = pxh->px_numfields;
				int primarykeyfieldsCount = pxh->px_primarykeyfields;
				int numberOfFieldsToIterate = primaryKeyFieldNamesOnly ? primarykeyfieldsCount : fieldsCount;

				for (int i = 0; i < numberOfFieldsToIterate; i++)
				{
					fieldNamesVector.push_back(pxf->px_fname);
					pxf++;
				}


			}
			catch (...)
			{
				if (pxdoc != NULL)
				{
					PX_close(pxdoc);
					PX_delete(pxdoc);
				}
				throw;
			}
		}

		if (pxdoc != NULL)
		{
			PX_close(pxdoc);
			PX_delete(pxdoc);
		}

	}
	catch (...)
	{
		try
		{
			PX_shutdown();
		}
		catch (...)
		{
		}
		throw;
	}

	try
	{
		PX_shutdown();
	}
	catch (...)
	{
		// We don't care??
	}

	std::ostringstream oss;

	if (!fieldNamesVector.empty())
	{

		// Convert all but the last element to avoid a trailing ","
		std::copy(fieldNamesVector.begin(), fieldNamesVector.end() - 1, std::ostream_iterator<std::string>(oss, delimiter.c_str()));

		// Now add the last element with no delimiter
		oss << fieldNamesVector.back();
	}

	output = oss.str();

	return output;

};











/// <summary>
/// Read a whole table and output.
/// </summary>
/// <param name="tableFilePath"></param>
/// <returns>Rows read from the table.</returns>
std::string PXLibExt::ReadAllJSON(std::string tableFilePath)
{
	int recordNumberStart = 0;
	int recordsToReadCount = -1;
	return ReadJSON(tableFilePath, recordNumberStart, recordsToReadCount);
}

std::string PXLibExt::ReadJSON(std::string tableFilePath, int recordNumberStart, int recordsToReadCount)
{
	std::string output = "";

	std::vector<std::unordered_map<std::string, std::string>> recordsVector = Read(tableFilePath, recordNumberStart, recordsToReadCount);

	output = PXLibExt::OutputRecordsVector(recordsVector);

	return output;

}


/// <summary>
/// Read a table and output.
/// </summary>
/// <param name="tableFilePath">The file path of the Paradox table</param>
/// <param name="recordNumberStart">The first record you want to read. Default 0 (first row).</param>
/// <param name="recordsToReadCount">The amount of records you want to read. If negative we assume all.</param>
/// <returns>Rows read from the table.</returns>
std::vector<std::unordered_map<std::string, std::string>> PXLibExt::Read(std::string tableFilePath, int recordNumberStart, int recordsToReadCount)
{

	bool outputRecordNumber = true;

	std::vector<std::unordered_map<std::string, std::string>> recordsVector;


	try
	{
		PX_boot();
		pxdoc_t* pxdoc = PX_new();
		pxdoc_t* pindex = NULL; //PX_new();
		

		if (pxdoc == NULL) { throw new std::exception("Could not create new pxdoc"); }

		std::string indexFilePath = tableFilePath.substr(0, tableFilePath.length() - 2) + "PX"; // Remove DB add PX
		std::string blobFilePath = tableFilePath.substr(0, tableFilePath.length() - 2) + "MB"; // Remove DB add MB


		bool openedPXDocFile = false;
		if (FileExists(tableFilePath))
		{
			openedPXDocFile = PX_open_file(pxdoc, tableFilePath.c_str()) >= 0;
		}

		bool openedPXIndexFile = false;
		bool readPXIndexFile = false;
		bool addedPXIndexFile = false;
		bool wrotePXIndexFile = false;
		
		if (FileExists(indexFilePath))
		{
			pindex = PX_new();
			openedPXIndexFile = PX_open_file(pindex, indexFilePath.c_str()) >= 0;
		}

		bool openedPXBlobFile = false;


		if (openedPXDocFile && pxdoc != NULL)
		{

			try
			{

				if (openedPXIndexFile && pindex != NULL)
				{
					readPXIndexFile = PX_read_primary_index(pindex) >= 0;
				}
				if (readPXIndexFile && pindex != NULL && pxdoc != NULL)
				{
					addedPXIndexFile = PX_add_primary_index(pxdoc, pindex) >= 0;
				}




				pxhead_t* pxh = pxdoc->px_head;
				pxval_t** dataptr = NULL;

				int totalNumRecords = pxh->px_numrecords; // The number of records
				int numFields = pxh->px_numfields;

				bool hasBlobField = false;
				for (int i = 0; !hasBlobField && i < numFields; i++)
				{
					pxfield_t* pxf = pxh->px_fields + i;
					hasBlobField = hasBlobField || IsBlobInternalType(pxf->px_ftype);
				}
				if (hasBlobField)
				{
					if (FileExists(blobFilePath))
					{
						openedPXBlobFile = PX_set_blob_file(pxdoc, blobFilePath.c_str()) >= 0;
					}
				}



				//char* readBuffer = (char*)calloc(PXLibExtDefs::READBUFFER_BYTESSIZE, 1); // Just a buffer for reading data.

				if (recordNumberStart >= totalNumRecords)
				{
					// Should we throw exception instead??
					recordsToReadCount = 0;
				}
				else if (recordsToReadCount < 0 || (recordsToReadCount + recordNumberStart > totalNumRecords))
				{
					recordsToReadCount = totalNumRecords - recordNumberStart;
					if (recordsToReadCount < 0) // If it's still less than zero, something weird going on.
					{
						// Should we throw exception instead??
						recordsToReadCount = 0;
					}
				}

				int recordsVectorInitSize = recordsToReadCount;
				int recordNumberEnd = recordNumberStart + recordsToReadCount;

				recordsVector.reserve(recordsVectorInitSize);


				for (int j = recordNumberStart; j < recordNumberEnd; j++)
				{
					std::unordered_map<std::string, std::string> fieldsKVPMap;

					std::string recordOutput = "";
					dataptr = PX_retrieve_record(pxdoc, j);

					if (dataptr != NULL)
					{


						int numFields = pxh->px_numfields;

						int fieldsVectorInitSize = numFields;
						if (outputRecordNumber)
						{
							fieldsVectorInitSize++;
						}

						fieldsKVPMap.reserve(fieldsVectorInitSize);

						for (int i = 0; i < numFields; i++)
						{
							pxfield_t* pxf = pxh->px_fields + i;


							std::string fieldName = std::string(pxf->px_fname);


							bool nonEmptyFieldName = fieldName.length() > 0;

							if (nonEmptyFieldName)
							{

								pxval_t* data = dataptr[i];
								std::string valueStr = "";

								if (PXLibExt::GetFieldValueString(pxdoc, data, valueStr))
								{

									std::pair<std::string, std::string> nameValueKVP = { fieldName, valueStr };

									fieldsKVPMap.insert(nameValueKVP);

								}
							}

						}

						// Do we need to do this? Or will pxdoc clean it up itself.
						for (int i = 0; i < numFields; i++)
						{
							pxfield_t* pxf = pxh->px_fields + i;
							bool isStringFieldType = IsStringInternalType(pxf->px_ftype);

							if (dataptr[i] != NULL)
							{
								if (isStringFieldType && dataptr[i]->value.str.val != NULL)
								{
									pxdoc->free(pxdoc, dataptr[i]->value.str.val);
									dataptr[i]->value.str.val = NULL;
								}
								pxdoc->free(pxdoc, dataptr[i]);
							}
							dataptr[i] = NULL;
						}
						pxdoc->free(pxdoc, dataptr); // Do we need to do this? Or will pxdoc clean it up itself.
						dataptr = NULL;

					}
					else
					{
						// We couldn't get this record - do we care?
					}

					if (fieldsKVPMap.size() > 0)
					{
						if (outputRecordNumber)
						{

							std::pair<std::string, std::string> nameValueKVP = { PXLibExtDefs::FIELDNAME_READ_RECORDNUMBER, std::to_string(j) };

							fieldsKVPMap.insert(nameValueKVP);

						}


						recordsVector.push_back(fieldsKVPMap);
					}

				}

				//if (readBuffer != NULL)
				//{
				//	free(readBuffer);
				//	readBuffer = NULL;
				//}


			}
			catch (...)
			{
				if (pxdoc != NULL)
				{
					PX_close(pxdoc);
					PX_delete(pxdoc);
					pxdoc = NULL;
				}
				if (pindex != NULL)
				{
					PX_close(pindex);
					PX_delete(pindex);
					pindex = NULL;
				}
				// blob should auto close
				throw;
			}
		}

		if (pxdoc != NULL)
		{
			PX_close(pxdoc);
			PX_delete(pxdoc);
			pxdoc = NULL;
		}
		if (pindex != NULL)
		{
			PX_close(pindex);
			PX_delete(pindex);
			pindex = NULL;
		}
		// blob should auto close

	}
	catch (...)
	{
		try
		{
			PX_shutdown();
		}
		catch (...)
		{
		}
		throw;
	}

	try
	{
		PX_shutdown();
	}
	catch (...)
	{
		// We don't care??
	}

	return recordsVector;

}










bool PXLibExt::FileExists(const std::string& filePath)
{
	std::ifstream f(filePath.c_str());
	return f.good();
}

bool PXLibExt::Write(std::string tableFilePath, std::unordered_map<std::string, std::string> rowKVPs, int& recordNumber)
{
	bool ret = false;


	try
	{
		PX_boot();
		pxdoc_t* pxdoc = PX_new();
		pxdoc_t* pindex = NULL; //PX_new();

		if (pxdoc == NULL) { throw new std::exception("Could not create new pxdoc"); }

		std::string indexFilePath = tableFilePath.substr(0, tableFilePath.length() - 2) + "PX"; // Remove DB add PX
		std::string blobFilePath = tableFilePath.substr(0, tableFilePath.length() - 2) + "MB"; // Remove DB add MB
		

		int updatedRecordNumber = -1;
		int insertedRecordNumber = -1;


		bool openedPXDocFile = false;
		if (FileExists(tableFilePath))
		{
			openedPXDocFile = PX_open_file(pxdoc, tableFilePath.c_str()) >= 0;
		}

		bool openedPXIndexFile = false;
		bool readPXIndexFile = false;
		bool addedPXIndexFile = false;
		bool wrotePXIndexFile = false;
		if (FileExists(indexFilePath))
		{
			pindex = PX_new();
			openedPXIndexFile = PX_open_file(pindex, indexFilePath.c_str()) >= 0;
		}

		bool openedPXBlobFile = false;

		if (openedPXDocFile && pxdoc != NULL)
		{

			try
			{
				if (openedPXIndexFile && pindex != NULL)
				{
					readPXIndexFile = PX_read_primary_index(pindex) >= 0;
				}
				if (readPXIndexFile && pindex != NULL && pxdoc != NULL)
				{
					addedPXIndexFile = PX_add_primary_index(pxdoc, pindex) >= 0;
				}


				pxhead_t* pxh = pxdoc->px_head;
				pxval_t** dataptr = NULL;

				int totalNumRecords = pxh->px_numrecords; // The number of records
				int numFields = pxh->px_numfields;


				bool hasBlobField = false;
				for (int i = 0; !hasBlobField && i < numFields; i++)
				{
					pxfield_t* pxf = pxh->px_fields + i;
					hasBlobField = hasBlobField || IsBlobInternalType(pxf->px_ftype);
				}
				if (hasBlobField)
				{
					if (FileExists(blobFilePath))
					{
						openedPXBlobFile = PX_set_blob_file(pxdoc, blobFilePath.c_str()) >= 0;
					}
				}


				if (recordNumber >= 0 && recordNumber < totalNumRecords)
				{
					dataptr = PX_retrieve_record(pxdoc, recordNumber);
				}
				else
				{
					recordNumber = -1;
					dataptr = (pxval_t**)pxdoc->malloc(pxdoc, pxh->px_numfields * sizeof(pxval_t*), "Allocate memory for array of pointers to field values.");
				}



				if (dataptr != NULL)
				{


					//int offset = 0;
					for (int i = 0; i < numFields; i++)
					{
						pxfield_t* pxf = pxh->px_fields + i;


						if (recordNumber >= 0 && recordNumber < totalNumRecords)
						{
						}
						else
						{
							MAKE_PXVAL(pxdoc, dataptr[i]);
							dataptr[i]->type = pxf->px_ftype;
						}

						std::string fieldName = std::string(pxf->px_fname);

						if (rowKVPs.find(fieldName) == rowKVPs.end())
						{
							dataptr[i]->isnull = true;
							continue;
						}

						std::string valueStr = rowKVPs[fieldName];

						bool nonEmptyFieldName = fieldName.length() > 0;

						if (nonEmptyFieldName)
						{
							pxval_t* data = dataptr[i];

							if (!PXLibExt::SetFieldValueString(pxdoc, data, valueStr, pxf->px_flen))//, pxf->px_fdc))
							{
								// throw?
							}
						}




					}

					if (recordNumber >= 0 && recordNumber < totalNumRecords)
					{
						updatedRecordNumber = PX_update_record(pxdoc, dataptr, recordNumber) >= 0;
						if (updatedRecordNumber >= 0)
						{
							ret = true;
							recordNumber = updatedRecordNumber; // Not needed?
						}
					}
					else
					{
						insertedRecordNumber = PX_insert_record(pxdoc, dataptr) >= 0;
						if (insertedRecordNumber >= 0)
						{
							ret = true;
							recordNumber = insertedRecordNumber;
						}
					}

					if (addedPXIndexFile && pindex != NULL && pxdoc != NULL)
					{
						wrotePXIndexFile = PX_write_primary_index(pxdoc, pindex) >= 0;
					}

					// Do we need to do this? Or will pxdoc clean it up itself.
					for (int i = 0; i < numFields; i++)
					{
						pxfield_t* pxf = pxh->px_fields + i;
						bool isStringFieldType = IsStringInternalType(pxf->px_ftype);

						if (dataptr[i] != NULL)
						{
							if (isStringFieldType && dataptr[i]->value.str.val != NULL)
							{
								pxdoc->free(pxdoc, dataptr[i]->value.str.val);
								dataptr[i]->value.str.val = NULL;
							}
							pxdoc->free(pxdoc, dataptr[i]);
						}
						dataptr[i] = NULL;
					}
					pxdoc->free(pxdoc, dataptr); // Do we need to do this? Or will pxdoc clean it up itself.
					dataptr = NULL;
				}





			}
			catch (...)
			{
				if (pxdoc != NULL)
				{
					PX_close(pxdoc);
					PX_delete(pxdoc);
					pxdoc = NULL;
				}
				if (pindex != NULL)
				{
					PX_close(pindex);
					PX_delete(pindex);
					pindex = NULL;
				}
				// blob should auto close
				throw;
			}
		}

		if (pxdoc != NULL)
		{
			PX_close(pxdoc);
			PX_delete(pxdoc);
			pxdoc = NULL;
		}
		if (pindex != NULL)
		{
			PX_close(pindex);
			PX_delete(pindex);
			pindex = NULL;
		}
		// blob should auto close

	}
	catch (...)
	{
		try
		{
			PX_shutdown();
		}
		catch (...)
		{
		}
		throw;
	}

	try
	{
		PX_shutdown();
	}
	catch (...)
	{
		// We don't care??
	}

	return ret;

}

std::vector<std::string> PXLibExt::Split(std::string s, std::string delimiter)
{
	size_t pos_start = 0;
	size_t pos_end;
	size_t delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}