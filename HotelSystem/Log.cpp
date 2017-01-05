#include "stdafx.h"
#include "Log.h"
#include <atlconv.h>
#include <exception>
#include <fstream>

aduit::CLog::CLog(): m_szInfo("信息"), m_szWarn("警告"), m_szError("错误")
        , m_szLastError(nullptr), m_nErrorNum(0), m_wirteto(nullptr)
{
}

const char * aduit::CLog::getLastError() const
{
	return m_szLastError;
}


void aduit::CLog::insertNewError(int __errlevel, const wchar_t * __errinfo, DWORD __lasterror)
{
	USES_CONVERSION;
	char * errinfo = W2A(__errinfo);
	insertNewError(__errlevel, errinfo, __lasterror);
}

void aduit::CLog::insertNewError(int __errlevel, const char* __errinfo, DWORD __lasterror)
{
	const char * errinfo{};
	!__errinfo ? errinfo = "未知错误信息" : errinfo = __errinfo;
	if (m_szLastError)
		delete m_szLastError;
	m_szLastError = new char[strlen(errinfo) + 2];
	strcpy_s(m_szLastError, strlen(errinfo) + 2, errinfo);
	const char * errlevel;
	switch (__errlevel)
	{
	case e_info:
		errlevel = m_szInfo; break;
	case e_warn:
		errlevel = m_szWarn; break;
	case e_error:
		errlevel = m_szError; break;
	default:
		errlevel = "未知错误码"; break;
	}
	char err[1024]{};
	sprintf_s(err, "%5d[%s][%d]: %s\r\n", m_nErrorNum, errlevel, __lasterror, errinfo);
	if (!m_wirteto)
		m_wirteto = new std::ofstream("error.list", std::ios::app);
	*m_wirteto << err << std::endl;
}

aduit::CLog::~CLog()
{
}
