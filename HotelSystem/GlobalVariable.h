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
// 所有的物品信息,定义于DataStruct.cpp
extern stl::CVector<data::SItem> g_vecAllItem;
// typeid对房间类型名映射关系
extern std::map<int, data::VARCHAR> g_mapRoomType;
// 房间类型名对typeid映射
extern std::map<data::VARCHAR, int> g_mapRoomID;

#endif // !__GLOBAL_VARIABLE_H_
