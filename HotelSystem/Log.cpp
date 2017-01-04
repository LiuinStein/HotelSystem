#include "stdafx.h"
#include "Log.h"
#include <atlconv.h>

aduit::CLog::CLog(): m_szInfo(_T("信息")), m_szWarn(_T("警告")), m_szError(_T("错误"))
    , m_szLastError(nullptr), m_nErrorNum(0)
	, m_wirteto(_T("error.list"), CFile::typeText | CFile::modeCreate | CFile::modeWrite)
{
}

const wchar_t * aduit::CLog::getLastError() const
{
	return m_szLastError;
}


void aduit::CLog::insertNewError(int __errlevel, const wchar_t * __errinfo)
{
	const wchar_t * errinfo{};
	if(!__errinfo)
		errinfo = _T("未知错误信息");
	if (m_szLastError)
		delete m_szLastError;
	m_szLastError = new wchar_t[sizeof errinfo + 2];
	wcscpy_s(m_szLastError, sizeof errinfo + 2, errinfo);
	const wchar_t * errlevel;
	switch (__errlevel)
	{
	case e_info:
		errlevel = m_szInfo; break;
	case e_warn:
		errlevel = m_szWarn; break;
	case e_error:
		errlevel = m_szError; break;
	default:
		errlevel = _T("未知错误码"); break;
	}
	CString err;
	err.Format(_T("%5d[%s]: %s\r\n"), m_nErrorNum, errlevel, errinfo);
	m_wirteto.SeekToEnd();
	m_wirteto.WriteString(err);
}

void aduit::CLog::insertNewError(int __errlevel, const char* __errinfo)
{
	USES_CONVERSION;
	wchar_t * errinfo = A2W(__errinfo);
	insertNewError(__errlevel, errinfo);
}

aduit::CLog::~CLog()
{
}
