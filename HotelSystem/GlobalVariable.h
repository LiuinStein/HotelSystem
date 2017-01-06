#pragma once
#ifndef __GLOBAL_VARIABLE_H_
#define __GLOBAL_VARIABLE_H_
#include "mysql_conn.h"
#include "Log.h"
#include "DataStruct.h"

// mysql访问,定义于mysql_conn.cpp
extern db::CMysql g_mysql;	
// 日志记录,定义于Log.cpp
extern aduit::CLog g_log;
// 房间类型,定义于DataStruct.cpp
extern stl::CVector<data::SRoomType> g_vecRoomType;
// 所有的房间信息,定义于DataStruct.cpp
extern stl::CVector<data::SRoom> g_vecAllRoom;
// 获取所有可用房间,定义于DataStruct.cpp
extern stl::CVector<data::SRoom> g_vecAvailableRoom;

#endif // !__GLOBAL_VARIABLE_H_
