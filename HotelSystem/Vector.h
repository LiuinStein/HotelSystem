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
		CFib(int n) //��ʼ��Ϊ��С��n����СFibonacci��
		{
			m_nNow = 1, m_nAdd = 0;
			while (m_nNow < n)
				next();
		}

		long long next()
		{
			//1...fib(k)=fib(k-1)+fib(k-2)
			//2...fib(k-2)=fib(k-1)-fib(k-3)
			m_nNow += m_nAdd; //Add �൱��1�е�fib(k-2)
							  //������һ��Add��ֵ,����֪����fib(k)
							  //����������Add�д����fib(k-2)
			m_nAdd = m_nNow - m_nAdd; //fib(k-1)=fib(k)-fib(k-2)
			return m_nNow;
		}

		long long prev()
		{
			//��ǰNow�д����fib(k),Add�д����fib(k-1)
			//��Ҫ��Now��Ϊfib(k-1),��Add��Ϊfib(k-2)
			m_nAdd = m_nNow - m_nAdd; //fib(k-2)=fib(k)-fib(k-1)
			m_nNow -= m_nAdd; //fib(k-1)=fib(k)-fib(k-2)
			return m_nNow;
		}

		long long getFib() const
		{
			return m_nNow;
		}

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
		void copyFrom(const T * __From, Rank __lo, Rank __hi)
		{
			//����ռ�,ʵ�ʷ���ռ�Ϊ��ʼ����С������
			m_tElem = new T[m_nCapacity = (__hi - __lo) << 1];
			m_nSize = 0;	//��ʼ����ģ
			while (__lo < __hi)
				m_tElem[m_nSize++] = __From[__lo++];	//��ֵ
		}

		//�ڴ�����ຯ��	
		//�ռ䲻��ʱ,����
		void expand()
		{
			if (m_nSize < m_nCapacity)
				return;							//��δ��Աʱ��������
			if (c_nDefaultCapacity > m_nCapacity)
				m_nCapacity = c_nDefaultCapacity;	//��֤capacity����Ĭ��ֵ
			T * OldElem = m_tElem;				//��ʱָ��ָ��ԭ���ݵĵ�ַ
			m_tElem = new T[m_nCapacity <<= 1];		//�������С����һ��
			for (int i = 0; i < m_nSize; i++)
				m_tElem[i] = OldElem[i];			//��ԭ�������ݿ�����������
			delete[] OldElem;					//�ͷž�����
		}

		//װ�����ӹ�Сʱ,ѹ��
		void shrink()
		{
			//Ϊ������򷴸����ֽ������ݺ����ݵ����,�ɽ�װ���������õ�Сһ��,����Ϊ0(��ֹ����)
			//������,װ��������25%Ϊ��,С��25%���������,��С����ԭ����1/2
			if (m_nCapacity >> 1 < c_nDefaultCapacity)
				return;	//����������Ĭ��capacityС
			if (m_nSize > m_nCapacity >> 2)	//_size>1/4_capacity
				return;	//װ�����Ӵ���25%��������
			T * OldElem = m_tElem;
			m_tElem = new T[m_nCapacity >>= 1];	//��������
			for (int i = 0; i < m_nSize; i++)
				m_tElem[i] = OldElem[i];
			delete[] OldElem;
		}

		//�㷨��
		//�����ຯ��
		//�鲢�㷨
		void merge(Rank __lo, Rank __mi, Rank __hi)
		{
			T * A = m_tElem + __lo;	//�ϲ��������A[0,__hi-__lo)=m_tElem[__lo,__hi)
			int lb{ __mi - __lo };	//�е�֮ǰ��Ԫ�ظ���
			T * B = new T[lb];		//ǰ������B[0,lb)=m_tElem[0,__mi)
			for (Rank i = 0; i < lb; B[i] = A[i++]);//����ǰ������
			int lc{ __hi - __mi };	//��������Ԫ�ظ���
			T * C = m_tElem + __mi;	//C[0,lc)=m_tElem[__mi,__hi)
									//����,ǰ�������ͺ��������ͱ��ó�����
									//����AΪָ��ǰ��������һ��Ԫ�ص�ָ��
									//BΪǰ�������ı���,CΪָ�����������һ��Ԫ�ص�ָ��
									//����,����״̬1 5 4 8,ǰ������1 5��������4 8
									//������Ҫ�ɵĻ�����ų�1 4 5 8,Ҳ���ǰ�4 8����1 5
			for (int i{}, j{}, k{}; (j < lb) || (k < lc);)
			{
				//�����µĹ������һ���дA,A��ǰ���������Ա�����B
				//��д�Ĺ����൱�ڰ�B[j]��C[k]�в�����Compare�ķŵ�Aĩβ
				//Ҳ����˵��Compare�൱��<����ʱ,�Ѵ�ķŵ�A��ĩβʵ����������
				if ((j < lb) &&	//��֤B[j++]����Խ��,��ΪBΪ���Ƶ�ǰ������,����B�ĳ���Ϊlb
					(
						!(k < lc) ||	//��������Ҳ�Ƿ�Խ������
										//���ұȽ�����,����������������һ������,
										//��Ҫ����C[k]Խ��,��ôk����k>=lc
										//��k>=lcʱ!(k<lc)Ϊ��,�������߼�����ʽ����
										//��һ��Ϊ��ʱ�����Compare(B[j], C[k])���ᱻ����
										//����!(k<lc)||������Ƿ�ֹԽ��
						(Compare(B[j], C[k]))//������֮ǰ��ǰ��������
											 //!(k < lc)||��(j < lb)&&����Ϊ�˱�֤��Խ����Ƶ�
											 //��ô��������������ص�
											 //Ҳ�����������ʽ�ĺ�����˼
											 //�ڱ�֤���±�Խ��������,���B[j]<C[k]
											 //ע:=>1.���ǰ�����еĵ�һ��С�ں������еĵ�һ��
											 //���й鲢�㷨�е������ж����Ѿ��ź����
											 //Ҳ����ǰ��������С����һ��С�ں���������С����һ��
											 //����Ҫ�������е������,��Ҫ��ǰ�����еĵ�һ�����ڵ�һλ
											 //Ҳ���ǽ������õ�B[0]����A[0]
											 //�ڵڶ�����B[j]��ʾ����ǰ�����е�ǰ��������һ��
											 //����1 5,4 8��ʵ����˵Ҳ����5
											 //B[j]��ʵ������Ҳ���ǵ�ǰ��A������β����һ��
						))
					A[i++] = B[j++];
				//=>1:
				//1 5,4 8->A_1 5 4 8,B[1 5],C_4 8 , i,j,k==0
				//0<2&&!(0<2)||(1<4)==true,
				//A_1 5 4 8,B[1 5],C_4 8,i==j==1,k==0;

				//=>2:
				//A_1 4 4 8,B[1 5],C_4 8,i==2,j==1,k==1;
				//(1<2)&&(!(1<2)||(5<8))==true
				//A_1 4 5 8,B[1 5],C_4 8,i==3,j==2,k==1;

				if ((k < lc) &&
					(
						!(j < lb) ||	//�����潲��ԭ��һ��ǰ������Ϊ�˱�֤��Խ��
						(Compare(C[k], B[j]))
						//�������ʽ�ĺ�����˼,�ɴ�תΪ
						//�ڱ�֤��Խ��������,���C[k]<B[j]�Ļ�
						//�ڵ�һ����,Ҳ�����жϺ������еĵ�1��
						//��ǰ�����еĵ�2���Ĵ�С,���˵�������е�
						//��һ��С��ǰ�����е����һ��,Ҳ�����������״̬��
						//�������еĵ�һ��Ӧ������ǰ�����е��м�
						//Ҳ���ǽ�C[0]��ֵ��A[1]
						))
					A[i++] = C[k++];
				//=>1:
				//A_1 5 4 8,B[1 5],C_4 8,i==j==1,k==0;
				//(0<2)&&(!(1<2)||(4<5))==true
				//A[1++]=C[0++]=>
				//A_1 4 4 8,B[1 5],C_4 8,i==2,j==1,k==1;

				//=>2:
				//A_1 4 5 8,B[1 5],C_4 8,i==3,j==2,k==1;
				//(1<2)&&(!(2<2)||(�������))
			}
		}

		//��ֵ��������,������Ϊ__a��__b��Ԫ��
		void swap(Rank __a, Rank __b)
		{
			/*�����Ľ�������ֻ����������,�������ڸ�����
			//���Ի���ʹ�ñȽ�ͨ�õķ���
			m_tElem[__a] = m_tElem[__a] ^ m_tElem[__b];
			m_tElem[__b] = m_tElem[__a] ^ m_tElem[__b];
			m_tElem[__a] = m_tElem[__a] ^ m_tElem[__b];
			*/
			T tmp = m_tElem[__a];
			m_tElem[__a] = m_tElem[__b];
			m_tElem[__b] = tmp;
		}

		//ȥ��������,���ر�ɾ����Ԫ�ص���Ŀ
		//����ȥ����
		int orderUnique()
		{
			/*
			*������ͬ��Ԫ��,ֱ�ӰѲ���ͬ��Ԫ����ǰ��,�������ͬ��
			*Ԫ�ؾ�����ǰ,��ͬ��Ԫ�ؾ������,ֱ�ӽ�ȡ��βԪ�ؼ���
			*/
			Rank i = 0, j = 0;
			while (++j < m_nSize)
				if (m_tElem[i] != m_tElem[j])	//������ͬ��Ԫ��
					m_tElem[++i] = m_tElem[j];
			m_nSize = ++i;
			shrink();
			return j - i;
		}

		//����ȥ����
		int unOrderUnique()
		{
			int OldSize = m_nSize;	//��¼ԭʼ��С
			Rank i = 0;				//�ӵ�0����ʼ�����
			while (i < m_nSize)
				find(m_tElem[i], i + 1, m_nSize) == -1 ?
				i++ :		//�Ҳ���
				remove(i);	//�ҵ���ɾ��,���ұ���i����,ֱ���Ҳ���Ϊֹ
			return OldSize - m_nSize;
		}

		//�����ຯ��
		//�������,��������[__lo,__hi),�Ҳ�������_hi+1
		//�ҵ����ش������ҵ�һ��Ԫ�ص���
		Rank unorderFind(const T & __f, Rank __lo, Rank __hi)const
		{
			int i = __lo;
			for (; (i < __hi) && (m_tElem[i] != __f); i++);
			return i;
		}

		//�������
		//���ֲ����㷨
		Rank binSearch(const T & __f, Rank __lo, Rank __hi)const
		{
			while (1 < __hi - __lo)
			{
				Rank mid = (__hi + __lo) >> 1;
				//������������[__lo,mid)��[mid,__hi)
				(__f < m_tElem[mid]) ? __hi = mid : __lo = mid;
				//������ҳɹ�����㷨Ҳ������ǰ��ֹ
			}	//����ʱ__hi=__lo+1,���������ʣһ��Ԫ��__lo
			return __f == m_tElem[__lo] ? __lo : -1;
			//���Ҳ���ܱ�֤�ж������Ԫ��ʱ�ķ���ֵ��������С����һ��
			//ͨ����ȫ����ͬԪ�����еĹ۲켴�ɷ���
		}

		//쳲����������㷨
		Rank fibSearch(const T & __f, Rank __lo, Rank __hi)const
		{
			CFib fib(__hi - __lo);	//����쳲���������
			while (__lo < __hi)
			{
				while (__hi - __lo < fib.getFib())
					fib.prev();	//����fib�е�쳲���������Ϊ����
								//__hi-__lo����С쳲�������
				Rank mid = Rank(__lo + fib.getFib() - 1);
				//�����㷨�ı���Ϊ�����ֲ��ҷ��Ĳ����е�
				//��Ϊ�˻ƽ�ָ��
				if (__f < m_tElem[mid])
					__hi = mid;
				else if (m_tElem[mid] < __f)
					__lo = mid + 1;
				else
					return mid;
			}
			return -1;
			//ͬ�����Ҳ���ܱ�֤���ж������Ԫ��ʱ,���ص��ȵ�������һ��
		}

	public:
		//�����ӿ�
		//���캯��
		/*
		*Ĭ�Ϲ��캯��
		*/
		CVector()
		{
			m_tElem = new T[m_nCapacity = c_nDefaultCapacity];
			m_nSize = 0;
		}

		/*
		*__c:��ʼ_capacity,Ĭ��ΪDefault_Capacity
		*__s:��ʼ_size,Ĭ��Ϊ0
		*__i:Ԫ�س�ʼֵ,Ĭ��Ϊ0
		*/
		CVector(int __c, int __s, T __i)
		{
			m_tElem = new T[m_nCapacity = __c]; //�����ڴ沢��ֵ
			for (m_nSize = 0; m_nSize < __s; m_tElem[m_nSize++] = __i);
			//һ�д��������Ϊ_size��ֵ��Ϊ���鸳Ĭ��ֵ
		}

		/*
		*ͨ�����������ʼ������[__lo,__hi)
		*/
		CVector(const T* __i, Rank __hi, Rank __lo = 0)
		{
			copyFrom(__i, 0, __hi);
		}

		/*
		*ʹ������������ȫ��Ԫ�س�ʼ������
		*/
		CVector(const CVector<T>& __i)
		{
			copyFrom(__i.m_tElem, 0, __i.m_nSize);
		}

		/*
		*������������ʼ����������
		*ʹ������������[__lo,__hi)�ڵ�Ԫ��
		*/
		CVector(const CVector<T>& __i, Rank __lo, Rank __hi)
		{
			copyFrom(__i.m_tElem, __lo, __hi);
		}

		//��������
		~CVector()
		{
			delete[] m_tElem;
		}

		//ֻ�����ʽӿ�
		Rank size() const //����������ģ
		{
			return m_nSize;
		}

		bool empty() const //����������true
		{
			return !m_nSize;
		}

		Order ordered()const					//�ж�����˳��
		{
			int az{},	//������
				za{},	//������
				aa{};	//�����
			for (int i = 1; i < m_nSize; i++)
				if (m_tElem[i - 1] > m_tElem[i])
					za++;
				else if (m_tElem[i - 1] < m_tElem[i])
					az++;
				else
					aa++;
			if (az != 0 && za == 0)
				return A_Z;
			if (za != 0 && az == 0)
				return Z_A;
			if (aa != 0 && az == 0 && za == 0)
				return A_A;
			return None;
		}
		
		//������[__lo,__hi)�в���Ԫ��__f,�Ҳ�������-1
		Rank find(const T & __f, Rank __lo, Rank __hi)const
		{
			if (ordered() != None)
				return
				//fibSearch(__f, __lo, __hi);
				binSearch(__f, __lo, __hi);
			else
			{
				Rank res = UnorderFind(__f, __lo, __hi);
				//UnorderFind�����ڲ���ʧ�ܵ�����·��ص���__hi
				//�����ڴ˴���Ҫһ���ж�
				return res == __hi ? -1 : res;
			}
		}

		//��д���ʽӿ�
		//�����±�������������������һ������Ԫ��
		T& operator[](Rank __r) const
		{
			return m_tElem[__r];
		}

		//���ظ�ֵ�����,����ֱ�Ӹ�ֵ,����һ������,����������ֵ��д��
		CVector<T> & operator=(CVector<T> const & __t)
		{
			if (m_nSize)	//copyFrom������Ϊ��������ռ�,�������_elem�������ݵĻ�,
				delete[] m_tElem;	//ɾ��_elem
			copyFrom(__t.m_tElem, 0, __t.m_nSize);
			return *this;
		}

		//ɾ����Ϊ__d��Ԫ��,����ֵΪ��ǰɾ�������Ԫ��
		T remove(Rank __d)
		{
			T tmp = m_tElem[__d];
			remove(__d, __d + 1);	//�൱�ڴ���__d,__d+1�����
			return tmp;
		}

		//ɾ������[__lo,__hi)��Χ�ڵ�Ԫ��,���ر�ɾ��Ԫ�ص���Ŀ
		int remove(Rank __lo, Rank __hi)
		{
			while (__hi < m_nSize)
				m_tElem[__lo++] = m_tElem[__hi++];
			m_nSize = __lo;
			shrink();			//���б�Ҫ,����
			return __hi - __lo;
		}

		//�������
		int clear()
		{
			return remove(0, m_nSize);
		}

		//����Ԫ��,����Ϊ__in�ĵط���������__e,���ز���λ�õ���
		Rank insert(Rank __inp, const T & __e)
		{
			expand();	//���б�Ҫ,����
			for (int i = m_nSize; i > __inp; i--)
				m_tElem[i] = m_tElem[i - 1];
			m_tElem[__inp] = __e;
			m_nSize++;
			return __inp;
		}

		//ǰ��λ�ÿ��ٲ���
		void push_front(const T& __e)
		{
			insert(0, __e);
		}

		void push_back(const T& __e)
		{
			insert(m_nSize, __e);
		}

		//�鲢����
		void mergeSort(Rank __lo, Rank __hi)
		{
			//��ð������ͨ���������õ���ɨ�轻������,�鲢����
			//���Ƿ������ö�·�鲢��ʵ�ֵ�,��ν�Ķ�·�鲢���ǽ�
			//�����������кϲ���һ����������
			if (__hi - __lo < 2)
				return;		//���Ե�Ԫ������
			int mid{ (__lo + __hi) >> 1 };	//ȡ�е�
											//���е����߷ֱ�����
			mergeSort(__lo, mid);
			mergeSort(mid, __hi);
			merge(__lo, mid, __hi);	//���й鲢
		}

		//ȥ����
		int unique()
		{
			if (ordered() == None)
				return unOrderUnique();
			return orderUnique();
		}

		//�����ӿ�
		//���ݺ���ָ�������������ڲ�����
		void traverse(void(*visit)(T &))
		{
			for (int i = 0; i < m_nSize; i++)
				visit(m_tElem[i]);
		}

	};


}

#endif // !__VECTOR_H_
