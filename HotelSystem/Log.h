#pragma once
#ifndef __LOG_H_
#define __LOG_H_

#include <afx.h>

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
		const wchar_t * m_szInfo;		// ������1
		const wchar_t * m_szWarn;		// ������2
		const wchar_t * m_szError;		// ������3
		wchar_t * m_szLastError;	// ���һ�δ���
		UINT m_nErrorNum;		// ������
		CStdioFile m_wirteto;	// д���ļ�
	public:
		CLog();
		// ��ȡ���µĴ�����Ϣ
		const wchar_t * getLastError()const;
		// �������
		void insertNewError(int __errlevel, const wchar_t * __errinfo);
		void insertNewError(int __errlevel, const char * __errinfo);
		~CLog();
	};

	CLog log;
}

#endif // !__LOG_H_


