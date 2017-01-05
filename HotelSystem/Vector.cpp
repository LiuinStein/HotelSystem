#include "stdafx.h"
#include "Vector.h"

template <typename T>
void stl::CVector<T>::copyFrom(const T* __From, Rank __lo, Rank __hi)
{
	//����ռ�,ʵ�ʷ���ռ�Ϊ��ʼ����С������
	_elem = new T[_capacity = (__hi - __lo) << 1];
	_size = 0;	//��ʼ����ģ
	while (__lo < __hi)
		_elem[_size++] = __From[__lo++];	//��ֵ
}

template <typename T>
void stl::CVector<T>::expand()
{
	if (_size < _capacity)
		return;							//��δ��Աʱ��������
	if (Default_Capacity > _capacity)
		_capacity = Default_Capacity;	//��֤capacity����Ĭ��ֵ
	T * OldElem = _elem;				//��ʱָ��ָ��ԭ���ݵĵ�ַ
	_elem = new T[_capacity <<= 1];		//�������С����һ��
	for (int i = 0; i < _size; i++)
		_elem[i] = OldElem[i];			//��ԭ�������ݿ�����������
	delete[] OldElem;					//�ͷž�����
}

template <typename T>
void stl::CVector<T>::shrink()
{
	//Ϊ������򷴸����ֽ������ݺ����ݵ����,�ɽ�װ���������õ�Сһ��,����Ϊ0(��ֹ����)
	//������,װ��������25%Ϊ��,С��25%���������,��С����ԭ����1/2
	if (_capacity >> 1 < Default_Capacity)
		return;	//����������Ĭ��capacityС
	if (_size > _capacity >> 2)	//_size>1/4_capacity
		return;	//װ�����Ӵ���25%��������
	T * OldElem = _elem;
	_elem = new T[_capacity >>= 1];	//��������
	for (int i = 0; i < _size; i++)
		_elem[i] = OldElem[i];
	delete[] OldElem;
}

template <typename T>
void stl::CVector<T>::merge(Rank __lo, Rank __mi, Rank __hi)
{
	T * A = _elem + __lo;	//�ϲ��������A[0,__hi-__lo)=_elem[__lo,__hi)
	int lb{ __mi - __lo };	//�е�֮ǰ��Ԫ�ظ���
	T * B = new T[lb];		//ǰ������B[0,lb)=_elem[0,__mi)
	for (Rank i = 0; i < lb; B[i] = A[i++]);//����ǰ������
	int lc{ __hi - __mi };	//��������Ԫ�ظ���
	T * C = _elem + __mi;	//C[0,lc)=_elem[__mi,__hi)
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

template <typename T>
void stl::CVector<T>::mergeSort(Rank __lo, Rank __hi)
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

template <typename T>
int stl::CVector<T>::unique()
{
	if (ordered() == None)
		return unOrderUnique();
	return orderUnique();
}

template <typename T>
void stl::CVector<T>::traverse(void(* visit)(T&))
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}

template <typename T>
void stl::CVector<T>::swap(Rank __a, Rank __b)
{
	/*�����Ľ�������ֻ����������,�������ڸ�����
	//���Ի���ʹ�ñȽ�ͨ�õķ���
	_elem[__a] = _elem[__a] ^ _elem[__b];
	_elem[__b] = _elem[__a] ^ _elem[__b];
	_elem[__a] = _elem[__a] ^ _elem[__b];
	*/
	T tmp = _elem[__a];
	_elem[__a] = _elem[__b];
	_elem[__b] = tmp;
}

template <typename T>
int stl::CVector<T>::orderUnique()
{
	/*
	*������ͬ��Ԫ��,ֱ�ӰѲ���ͬ��Ԫ����ǰ��,�������ͬ��
	*Ԫ�ؾ�����ǰ,��ͬ��Ԫ�ؾ������,ֱ�ӽ�ȡ��βԪ�ؼ���
	*/
	Rank i = 0, j = 0;
	while (++j < _size)
		if (_elem[i] != _elem[j])	//������ͬ��Ԫ��
			_elem[++i] = _elem[j];
	_size = ++i;
	shrink();
	return j - i;
}

template <typename T>
int stl::CVector<T>::unOrderUnique()
{
	int OldSize = _size;	//��¼ԭʼ��С
	Rank i = 0;				//�ӵ�0����ʼ�����
	while (i < _size)
		find(_elem[i], i + 1, _size) == -1 ?
		i++ :		//�Ҳ���
		remove(i);	//�ҵ���ɾ��,���ұ���i����,ֱ���Ҳ���Ϊֹ
	return OldSize - _size;
}

template <typename T>
stl::Rank stl::CVector<T>::unorderFind(const T& __f, Rank __lo, Rank __hi) const
{
	int i = __lo;
	for (; (i < __hi) && (_elem[i] != __f); i++);
	return i;
}

template <typename T>
stl::Rank stl::CVector<T>::binSearch(const T& __f, Rank __lo, Rank __hi) const
{
	while (1 < __hi - __lo)
	{
		Rank mid = (__hi + __lo) >> 1;
		//������������[__lo,mid)��[mid,__hi)
		(__f < _elem[mid]) ? __hi = mid : __lo = mid;
		//������ҳɹ�����㷨Ҳ������ǰ��ֹ
	}	//����ʱ__hi=__lo+1,���������ʣһ��Ԫ��__lo
	return __f == _elem[__lo] ? __lo : -1;
	//���Ҳ���ܱ�֤�ж������Ԫ��ʱ�ķ���ֵ��������С����һ��
	//ͨ����ȫ����ͬԪ�����еĹ۲켴�ɷ���
}

template <typename T>
stl::Rank stl::CVector<T>::fibSearch(const T& __f, Rank __lo, Rank __hi) const
{
	Fib fib(__hi - __lo);	//����쳲���������
	while (__lo < __hi)
	{
		while (__hi - __lo < fib.getFib())
			fib.prev();	//����fib�е�쳲���������Ϊ����
						//__hi-__lo����С쳲�������
		Rank mid = Rank(__lo + fib.getFib() - 1);
		//�����㷨�ı���Ϊ�����ֲ��ҷ��Ĳ����е�
		//��Ϊ�˻ƽ�ָ��
		if (__f < _elem[mid])
			__hi = mid;
		else if (_elem[mid] < __f)
			__lo = mid + 1;
		else
			return mid;
	}
	return -1;
	//ͬ�����Ҳ���ܱ�֤���ж������Ԫ��ʱ,���ص��ȵ�������һ��
}

template <typename T>
stl::CVector<T>::CVector()
{
	_elem = new T[_capacity = Default_Capacity];
	_size = 0;
}

template <typename T>
stl::CVector<T>::CVector(int __c, int __s, T __i)
{
	_elem = new T[_capacity = __c]; //�����ڴ沢��ֵ
	for (_size = 0; _size < __s; _elem[_size++] = __i);
	//һ�д��������Ϊ_size��ֵ��Ϊ���鸳Ĭ��ֵ
}

template <typename T>
stl::CVector<T>::CVector(const T* __i, Rank __hi, Rank __lo)
{
	copyFrom(__i, 0, __hi);
}

template <typename T>
stl::CVector<T>::CVector(const CVector<T>& __i)
{
	copyFrom(__i._elem, 0, __i._size);
}

template <typename T>
stl::CVector<T>::CVector(const CVector<T>& __i, Rank __lo, Rank __hi)
{
	copyFrom(__i._elem, __lo, __hi);
}

template <typename T>
stl::CVector<T>::~CVector()
{
	delete[] _elem;
}

template <typename T>
stl::Rank stl::CVector<T>::size() const
{
	return _size;
}

template <typename T>
bool stl::CVector<T>::empty() const
{
	return !_size;
}

template <typename T>
stl::Order stl::CVector<T>::ordered() const
{
	int az{},	//������
		za{},	//������
		aa{};	//�����
	for (int i = 1; i < _size; i++)
		if (_elem[i - 1] > _elem[i])
			za++;
		else if (_elem[i - 1] < _elem[i])
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

template <typename T>
stl::Rank stl::CVector<T>::find(const T& __f, Rank __lo, Rank __hi) const
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

template <typename T>
T& stl::CVector<T>::operator[](Rank __r) const
{
	return _elem[__r];
}

template <typename T>
stl::CVector<T>& stl::CVector<T>::operator=(CVector<T> const& __t)
{
	if (_size)	//copyFrom������Ϊ��������ռ�,�������_elem�������ݵĻ�,
		delete[] _elem;	//ɾ��_elem
	copyFrom(__t._elem, 0, __t._size);
	return *this;
}

template <typename T>
T stl::CVector<T>::remove(Rank __d)
{
	T tmp = _elem[__d];
	remove(__d, __d + 1);	//�൱�ڴ���__d,__d+1�����
	return tmp;
}

template <typename T>
int stl::CVector<T>::remove(Rank __lo, Rank __hi)
{
	while (__hi < _size)
		_elem[__lo++] = _elem[__hi++];
	_size = __lo;
	shrink();			//���б�Ҫ,����
	return __hi - __lo;
}

template <typename T>
int stl::CVector<T>::clear()
{
	return remove(0, _size);
}

template <typename T>
stl::Rank stl::CVector<T>::insert(Rank __inp, const T& __e)
{
	expand();	//���б�Ҫ,����
	for (int i = _size; i > __inp; i--)
		_elem[i] = _elem[i - 1];
	_elem[__inp] = __e;
	_size++;
	return __inp;
}

template <typename T>
void stl::CVector<T>::push_front(const T& __e)
{
	insert(0, __e);
}

template <typename T>
void stl::CVector<T>::push_back(const T& __e)
{
	insert(_size, __e);
}

