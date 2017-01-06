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
		UINT m_nErrorNum;		// 错误数
		std::ofstream * m_wirteto;	// 写入文件
	public:
		CLog();
		// 插入错误
		void insertNewError(int __errlevel, CString & __errinfo, DWORD __lasterror = 0);
		void insertNewError(int __errlevel, const wchar_t * __errinfo, DWORD __lasterror = 0);
		void insertNewError(int __errlevel, const char * __errinfo, DWORD __lasterror = 0);
		~CLog();
	};

	static CLog log;
}

#endif // !__LOG_H_


