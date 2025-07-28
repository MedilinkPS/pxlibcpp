#ifndef PXLIBEXT_H
#define PXLIBEXT_H

#include <string>
#include <vector>
#include <array>
#include <stdexcept>
#include <sstream>
#include <unordered_map>
#include "paradox.h"
#include <set>

namespace PXLibExtDefs
{
	const int READBUFFER_BYTESSIZE = 16384;

	const std::string FIELDNAME_READ_RECORDNUMBER = "READ_RECORDNUMBER";
	const std::string STRINGFIELD_DELIMITER = "\"";
	const std::string FIELD_NULL = "null";
	const std::string LOGICALFIELD_FALSE = "false";
	const std::string LOGICALFIELD_TRUE = "true";

	const std::string SOLIDUS_UNESCAPED = "/";
	const std::string SOLIDUS_ESCAPED = "\\/";

	const std::string QUOTEMARK_UNESCAPED = "\"";
	const std::string QUOTEMARK_ESCAPED = "\\\"";

	const std::string REVERSESOLIDUS_UNESCAPED = "\\";
	const std::string REVERSESOLIDUS_ESCAPED = "\\\\";

	const std::string COMMA = ",";
	const std::string NEWLINE = "\n";
}

class PXLibExt
{
private: 

	static std::set<int> FIELDTYPES_OUTPUT_STRING;
	static std::set<int> FIELDTYPES_INTERNAL_STRING;
	static std::set<int> FIELDTYPES_INTERNAL_LONG;
	static std::set<int> FIELDTYPES_INTERNAL_DOUBLE;
	static std::set<int> FIELDTYPES_INTERNAL_BLOB;
	static void InitFieldTypeHashes();
	static bool IsStringOutputType(int px_ftype);
	static bool IsStringInternalType(int px_ftype);
	static bool IsDoubleInternalType(int px_ftype);
	static bool IsLongInternalType(int px_ftype);
	static bool IsBlobInternalType(int px_ftype);


	static std::string ToBase64String(const char* input, int len);
	static char ToBase64Char(int v);


	static std::string OutputRecordsVector(const std::vector<std::unordered_map<std::string, std::string>>& recordsVector, bool addWhitespace = true, bool ifNoRecordsOutputEmptyArray = true);

	//static bool GetFieldValueString(pxdoc_t* pxdoc, char px_ftype, int px_flen, int px_fdc, char* readBuffer, int offset, std::string& valueStr);
	static bool GetFieldValueString(pxdoc_t* pxdoc, pxval_t* data, std::string& valueStr);
	static bool SetFieldValueString(pxdoc_t* pxdoc, pxval_t* data, const std::string valueStr, int maxStrFieldLen);//, int bcdDecLen);
	
	static std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace);
	static std::vector<std::string> Split(std::string s, std::string delimiter);
	static bool FileExists(const std::string& filePath);

public:
	static std::string ReadAllJSON(std::string tableFilePath);
	static std::string ReadJSON(std::string tableFilePath, int recordNumberStart = 0, int recordsToReadCount = -1);
	static std::vector<std::unordered_map<std::string, std::string>> Read(std::string tableFilePath, int recordNumberStart = 0, int recordsToReadCount = -1);
	static bool Write(std::string tableFilePath, std::unordered_map<std::string, std::string> rowKVPs, int& recordNumber);
	static std::string GetFieldNames(std::string tableFilePath, std::string delimiter = PXLibExtDefs::COMMA, bool primaryKeyFieldNamesOnly = false);
	static std::string GetPrimaryKeyFieldNames(std::string tableFilePath, std::string delimiter = PXLibExtDefs::COMMA);
};

#endif