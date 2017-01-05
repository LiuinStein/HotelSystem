#pragma once
#ifndef __VECTOR_H_
#define __VECTOR_H_

namespace stl
{
	typedef int Rank;	//秩
	const int c_nDefaultCapacity = 10;	//初始预留空间
	enum Order
	{
		A_Z,	//升序排列
		Z_A,	//降序排列
		A_A,	//全相同
		None	//未排序
	};

	class CFib
	{
		long long m_nNow,		//当前项
			m_nAdd;		//加法因子
	public:
		CFib(int n); //初始化为不小于n的最小Fibonacci项
		long long next();
		long long prev();
		long long getFib() const;
	};

	template<typename T>
	class CVector	//向量类模板
	{
	protected:

		//内部数据区
		Rank m_nSize;		//当前规模,最大秩-1
		int m_nCapacity;	//当前容量
		T * m_tElem;		//数据区，采用动态数组来管理数据

		//内部函数区
		//数值操作类函数
		//将数组__From在区间[__lo,__hi)里面的数据复制到向量
		void copyFrom(const T * __From, Rank __lo, Rank __hi);

		//内存管理类函数	
		//空间不足时,扩容
		void expand();
		//装填因子过小时,压缩
		void shrink();

		//算法类
		//排序类函数
		//归并算法
		void merge(Rank __lo, Rank __mi, Rank __hi);

		//数值交换函数,交换秩为__a和__b的元素
		void swap(Rank __a, Rank __b);

		//去重器函数,返回被删除的元素的数目
		int orderUnique();		//有序去重器
		int unOrderUnique();	//无序去重器

		//查找类函数
		//无序查找,查找区间[__lo,__hi),找不到返回_hi+1
		//找到返回从左向右第一个元素的秩
		Rank unorderFind(const T & __f, Rank __lo, Rank __hi)const;
		//有序查找
		//二分查找算法
		Rank binSearch(const T & __f, Rank __lo, Rank __hi)const;
		//斐波那契查找算法
		Rank fibSearch(const T & __f, Rank __lo, Rank __hi)const;

	public:
		//基础接口
		//构造函数
		/*
		*默认构造函数
		*/
		CVector();

		/*
		*__c:初始_capacity,默认为Default_Capacity
		*__s:初始_size,默认为0
		*__i:元素初始值,默认为0
		*/
		CVector(int __c, int __s, T __i);

		/*
		*通过现有数组初始化向量[__lo,__hi)
		*/
		CVector(const T* __i, Rank __hi, Rank __lo = 0);

		/*
		*使用已有向量的全部元素初始化向量
		*/
		CVector(const CVector<T>& __i);

		/*
		*用已有向量初始化现有向量
		*使用已有向量的[__lo,__hi)内的元素
		*/
		CVector(const CVector<T>& __i, Rank __lo, Rank __hi);
		//析构函数
		~CVector();

		//只读访问接口
		Rank size() const; //返回向量规模
		bool empty() const; //空向量返回true
		Order ordered()const;					//判断排序顺序
												//在区间[__lo,__hi)中查找元素__f,找不到返回-1
		Rank find(const T & __f, Rank __lo, Rank __hi)const;

		//可写访问接口
		//重载下标访问运算符便于像数组一样访问元素
		T& operator[](Rank __r) const;
		//重载赋值运算符,便于直接赋值,返回一个引用,便于连续赋值的写法
		CVector<T> & operator=(CVector<T> const & __t);
		//删除秩为__d的元素,返回值为当前删除的这个元素
		T remove(Rank __d);
		//删除区间[__lo,__hi)范围内的元素,返回被删除元素的数目
		int remove(Rank __lo, Rank __hi);
		//清空向量
		int clear();
		//插入元素,在秩为__in的地方插入数据__e,返回插入位置的秩
		Rank insert(Rank __inp, const T & __e);
		//前后位置快速插入
		void push_front(const T& __e);
		void push_back(const T& __e);
		//归并排序
		void mergeSort(Rank __lo, Rank __hi);
		//去重器
		int unique();

		//遍历接口
		//根据函数指针来遍历向量内部数据
		void traverse(void(*visit)(T &));
	};


}

#endif // !__VECTOR_H_
