#include "StringUtil.h"
#include "Properties.h"

#include <string>
#include <vector>

#include <SBW/SBW.h>


#ifndef byte
typedef char byte; 
#endif

using namespace std;
using namespace SystemsBiologyWorkbench::Broker;

string StringUtil::HEX = "0123456789ABCDEF";


vector<int> StringUtil::parseRange(string rangeStr)
{
	vector<int> oResult;
	if (empty(rangeStr))
	{
		throw new SBWBrokerStartException("Null range given", "broker/module port-range not set");
	}

	int nPos = (int)rangeStr.find ('-');
	string sLow = rangeStr.substr(0,nPos);
	string sHigh = rangeStr.substr(nPos+1);
	oResult.push_back(Properties::toInt(sLow));
	oResult.push_back(Properties::toInt(sHigh));

	return oResult;
}
string StringUtil::toHex(char data[], int nLength)
{
	stringstream str;
	for (int i = 0; i < nLength; i++)
	{
		byte b = data[i];
		int hi = (b & 0xF0) >> 4;
		int lo = (b & 0x0F);
		str << HEX.at(hi) <<  HEX.at(lo);
	}
	return str.str();
}
string StringUtil::toString(int nInt)
{
	stringstream str; 
	str << nInt;
	return str.str();
}
int StringUtil::toInt(std::string sString)
{
	int nResult = 0;
	stringstream str;
	str << sString;
	str >> nResult;
	return nResult;
}

std::string StringUtil::EMPTY = "";
