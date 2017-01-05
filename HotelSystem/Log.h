#pragma once
#ifndef __LOG_H_
#define __LOG_H_

#include <afx.h>
#include <fstream>

// 用于日志审计
namespace aduit
{
	enum 
	{
		e_info = 1,
		e_warn = 2,
		e_error = 3
	};

	// 用来记录日志信息
	class CLog
	{
		const char * m_szInfo;		// 错误码1
		const char * m_szWarn;		// 错误码2
		const char * m_szError;		// 错误码3
		char * m_szLastError;	// 最近一次错误
		UINT m_nErrorNum;		// 错误数
		std::ofstream * m_wirteto;	// 写入文件
	public:
		CLog();
		// 获取最新的错误信息
		const char * getLastError()const;
		// 插入错误
		void insertNewError(int __errlevel, CString & __errinfo, DWORD __lasterror = 0);
		void insertNewError(int __errlevel, const wchar_t * __errinfo, DWORD __lasterror = 0);
		void insertNewError(int __errlevel, const char * __errinfo, DWORD __lasterror = 0);
		~CLog();
	};

	static CLog log;
}

#endif // !__LOG_H_


