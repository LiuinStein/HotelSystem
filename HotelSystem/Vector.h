#pragma once
#ifndef __VECTOR_H_
#define __VECTOR_H_

namespace stl
{
	typedef int Rank;	//��
	const int c_nDefaultCapacity = 10;	//��ʼԤ���ռ�
	enum Order
	{
		A_Z,	//��������
		Z_A,	//��������
		A_A,	//ȫ��ͬ
		None	//δ����
	};

	class CFib
	{
		long long m_nNow,		//��ǰ��
			m_nAdd;		//�ӷ�����
	public:
		CFib(int n); //��ʼ��Ϊ��С��n����СFibonacci��
		long long next();
		long long prev();
		long long getFib() const;
	};

	template<typename T>
	class CVector	//������ģ��
	{
	protected:

		//�ڲ�������
		Rank m_nSize;		//��ǰ��ģ,�����-1
		int m_nCapacity;	//��ǰ����
		T * m_tElem;		//�����������ö�̬��������������

		//�ڲ�������
		//��ֵ�����ຯ��
		//������__From������[__lo,__hi)��������ݸ��Ƶ�����
		void copyFrom(const T * __From, Rank __lo, Rank __hi);

		//�ڴ�����ຯ��	
		//�ռ䲻��ʱ,����
		void expand();
		//װ�����ӹ�Сʱ,ѹ��
		void shrink();

		//�㷨��
		//�����ຯ��
		//�鲢�㷨
		void merge(Rank __lo, Rank __mi, Rank __hi);

		//��ֵ��������,������Ϊ__a��__b��Ԫ��
		void swap(Rank __a, Rank __b);

		//ȥ��������,���ر�ɾ����Ԫ�ص���Ŀ
		int orderUnique();		//����ȥ����
		int unOrderUnique();	//����ȥ����

		//�����ຯ��
		//�������,��������[__lo,__hi),�Ҳ�������_hi+1
		//�ҵ����ش������ҵ�һ��Ԫ�ص���
		Rank unorderFind(const T & __f, Rank __lo, Rank __hi)const;
		//�������
		//���ֲ����㷨
		Rank binSearch(const T & __f, Rank __lo, Rank __hi)const;
		//쳲����������㷨
		Rank fibSearch(const T & __f, Rank __lo, Rank __hi)const;

	public:
		//�����ӿ�
		//���캯��
		/*
		*Ĭ�Ϲ��캯��
		*/
		CVector();

		/*
		*__c:��ʼ_capacity,Ĭ��ΪDefault_Capacity
		*__s:��ʼ_size,Ĭ��Ϊ0
		*__i:Ԫ�س�ʼֵ,Ĭ��Ϊ0
		*/
		CVector(int __c, int __s, T __i);

		/*
		*ͨ�����������ʼ������[__lo,__hi)
		*/
		CVector(const T* __i, Rank __hi, Rank __lo = 0);

		/*
		*ʹ������������ȫ��Ԫ�س�ʼ������
		*/
		CVector(const CVector<T>& __i);

		/*
		*������������ʼ����������
		*ʹ������������[__lo,__hi)�ڵ�Ԫ��
		*/
		CVector(const CVector<T>& __i, Rank __lo, Rank __hi);
		//��������
		~CVector();

		//ֻ�����ʽӿ�
		Rank size() const; //����������ģ
		bool empty() const; //����������true
		Order ordered()const;					//�ж�����˳��
												//������[__lo,__hi)�в���Ԫ��__f,�Ҳ�������-1
		Rank find(const T & __f, Rank __lo, Rank __hi)const;

		//��д���ʽӿ�
		//�����±�������������������һ������Ԫ��
		T& operator[](Rank __r) const;
		//���ظ�ֵ�����,����ֱ�Ӹ�ֵ,����һ������,����������ֵ��д��
		CVector<T> & operator=(CVector<T> const & __t);
		//ɾ����Ϊ__d��Ԫ��,����ֵΪ��ǰɾ�������Ԫ��
		T remove(Rank __d);
		//ɾ������[__lo,__hi)��Χ�ڵ�Ԫ��,���ر�ɾ��Ԫ�ص���Ŀ
		int remove(Rank __lo, Rank __hi);
		//�������
		int clear();
		//����Ԫ��,����Ϊ__in�ĵط���������__e,���ز���λ�õ���
		Rank insert(Rank __inp, const T & __e);
		//ǰ��λ�ÿ��ٲ���
		void push_front(const T& __e);
		void push_back(const T& __e);
		//�鲢����
		void mergeSort(Rank __lo, Rank __hi);
		//ȥ����
		int unique();

		//�����ӿ�
		//���ݺ���ָ�������������ڲ�����
		void traverse(void(*visit)(T &));
	};


}

#endif // !__VECTOR_H_
