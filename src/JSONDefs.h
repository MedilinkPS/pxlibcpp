#ifndef JSONDEFS_H
#define JSONDEFS_H

#include <iostream>
#include <string>

namespace JSONDefs
{

	/// <summary>
	/// Whitespace new line
	/// </summary>
	const std::string WSNL = "\n";

	/// <summary>
	/// Whitespace space
	/// </summary>
	const std::string WSSP = " ";


	/// <summary>
	/// Name value separator
	/// </summary>
	const std::string NVS = ":";

	/// <summary>
	/// Element delimiter start
	/// </summary>
	const std::string EDS = "\"";

	/// <summary>
	/// Element delimiter finish
	/// </summary>
	const std::string EDF = "\"";

	/// <summary>
	/// Element object seperator
	/// </summary>
	const std::string EOS = ",";

	/// <summary>
	/// Object delimiter start
	/// </summary>
	const std::string ODS = "{";

	/// <summary>
	/// Object delimiter finish
	/// </summary>
	const std::string ODF = "}";

	/// <summary>
	/// Object array seperator
	/// </summary>
	const std::string OAS = ",";

	/// <summary>
	/// Array delimiter start
	/// </summary>
	const std::string ADS = "[";

	/// <summary>
	/// Array delimiter finish
	/// </summary>
	const std::string ADF = "]";



}

#endif