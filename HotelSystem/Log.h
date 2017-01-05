#pragma once
#ifndef __LOG_H_
#define __LOG_H_

#include <afx.h>
#include <fstream>

// ������־���
namespace aduit
{
	enum 
	{
		e_info = 1,
		e_warn = 2,
		e_error = 3
	};

	// ������¼��־��Ϣ
	class CLog
	{
		const char * m_szInfo;		// ������1
		const char * m_szWarn;		// ������2
		const char * m_szError;		// ������3
		char * m_szLastError;	// ���һ�δ���
		UINT m_nErrorNum;		// ������
		std::ofstream * m_wirteto;	// д���ļ�
	public:
		CLog();
		// ��ȡ���µĴ�����Ϣ
		const char * getLastError()const;
		// �������
		void insertNewError(int __errlevel, CString & __errinfo, DWORD __lasterror = 0);
		void insertNewError(int __errlevel, const wchar_t * __errinfo, DWORD __lasterror = 0);
		void insertNewError(int __errlevel, const char * __errinfo, DWORD __lasterror = 0);
		~CLog();
	};

	static CLog log;
}

#endif // !__LOG_H_


