#pragma once
#ifndef __GLOBAL_VARIABLE_H_
#define __GLOBAL_VARIABLE_H_
#include "mysql_conn.h"
#include "Log.h"
#include "DataStruct.h"

// mysql����,������mysql_conn.cpp
extern db::CMysql g_mysql;	
// ��־��¼,������Log.cpp
extern aduit::CLog g_log;
// ��������,������DataStruct.cpp
extern stl::CVector<data::SRoomType> g_vecRoomType;
// ���еķ�����Ϣ,������DataStruct.cpp
extern stl::CVector<data::SRoom> g_vecAllRoom;
// ��ȡ���п��÷���,������DataStruct.cpp
extern stl::CVector<data::SRoom> g_vecAvailableRoom;
// ���е���Ʒ��Ϣ,������DataStruct.cpp
extern stl::CVector<data::SItem> g_vecAllItem;
// typeid�Է���������ӳ���ϵ
extern std::map<int, data::VARCHAR> g_mapRoomType;
// ������������typeidӳ��
extern std::map<data::VARCHAR, int> g_mapRoomID;

#endif // !__GLOBAL_VARIABLE_H_
