#include "stdafx.h"
#include "Log.h"
#include <atlconv.h>
#include <fstream>
#include <Windows.h>

aduit::CLog::CLog(): m_nErrorNum(0), m_wirteto(nullptr)
{
}

void aduit::CLog::insertNewError(int __errlevel, CString& __errinfo, DWORD __lasterror)
{
	insertNewError(__errlevel, __errinfo.GetBuffer(), __lasterror);
	__errinfo.ReleaseBuffer();
}


void aduit::CLog::insertNewError(int __errlevel, const wchar_t * __errinfo, DWORD __lasterror)
{
	USES_CONVERSION;
	char * errinfo = W2A(__errinfo);
	insertNewError(__errlevel, errinfo, __lasterror);
}

void aduit::CLog::insertNewError(int __errlevel, const char* __errinfo, DWORD __lasterror)
{
	const char * errinfo{ !__errinfo ? "未知错误信息" : __errinfo };
	const char * errlevel;
	switch (__errlevel)
	{
	case e_info:
		errlevel = "信息"; break;
	case e_warn:
		errlevel = "警告"; break;
	case e_error:
		errlevel = "错误"; break;
	default:
		errlevel = "未知错误码"; break;
	}
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);
	char time[25]{};
	sprintf_s(time, "%d/%d/%d %d:%d:%d", nowTime.wYear, nowTime.wMonth, nowTime.wDay, nowTime.wHour, nowTime.wMinute, nowTime.wSecond);
	char err[1024]{};
	sprintf_s(err, "%s %5d[%s][%5d]: %s", time, m_nErrorNum++, errlevel, __lasterror, errinfo);
	if (!m_wirteto)
	{
		m_wirteto = new std::ofstream("error.log", std::ios::app);
		*m_wirteto << std::endl << "时间\t\t 编号[等级][系统错误码]: 信息" << std::endl;
	}
	*m_wirteto << err << std::endl;
}

aduit::CLog::~CLog()
{
}
