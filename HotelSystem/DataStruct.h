#pragma once
#ifndef __DATA_STRUCT_H_
#define __DATA_STRUCT_H_
#include <string>
#include "Vector.h"

namespace data
{
	typedef std::string VARCHAR;
	typedef double DECIMAL;

	struct SRoomType
	{
		int m_nID;
		VARCHAR m_nName;
		DECIMAL m_dPrice;
		int m_nPeopleNum;
	};
	static stl::CVector<SRoomType> g_vecRoomType;

	bool GetRoomType();
}

#endif // !__DATA_STRUCT_H_
