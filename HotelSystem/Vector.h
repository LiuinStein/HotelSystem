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
		CFib(int n) //初始化为不小于n的最小Fibonacci项
		{
			m_nNow = 1, m_nAdd = 0;
			while (m_nNow < n)
				next();
		}

		long long next()
		{
			//1...fib(k)=fib(k-1)+fib(k-2)
			//2...fib(k-2)=fib(k-1)-fib(k-3)
			m_nNow += m_nAdd; //Add 相当于1中的fib(k-2)
							  //计算下一个Add的值,现在知道了fib(k)
							  //并且现在在Add中存放着fib(k-2)
			m_nAdd = m_nNow - m_nAdd; //fib(k-1)=fib(k)-fib(k-2)
			return m_nNow;
		}

		long long prev()
		{
			//当前Now中存放着fib(k),Add中存放着fib(k-1)
			//我要让Now变为fib(k-1),而Add变为fib(k-2)
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
		void copyFrom(const T * __From, Rank __lo, Rank __hi)
		{
			//分配空间,实际分配空间为初始化大小的两倍
			m_tElem = new T[m_nCapacity = (__hi - __lo) << 1];
			m_nSize = 0;	//初始化规模
			while (__lo < __hi)
				m_tElem[m_nSize++] = __From[__lo++];	//赋值
		}

		//内存管理类函数	
		//空间不足时,扩容
		void expand()
		{
			if (m_nSize < m_nCapacity)
				return;							//尚未满员时不必扩容
			if (c_nDefaultCapacity > m_nCapacity)
				m_nCapacity = c_nDefaultCapacity;	//保证capacity大于默认值
			T * OldElem = m_tElem;				//临时指针指向原数据的地址
			m_tElem = new T[m_nCapacity <<= 1];		//将数组大小扩大一倍
			for (int i = 0; i < m_nSize; i++)
				m_tElem[i] = OldElem[i];			//将原数组内容拷贝到新数组
			delete[] OldElem;					//释放旧数组
		}

		//装填因子过小时,压缩
		void shrink()
		{
			//为避免程序反复出现交替缩容和扩容的情况,可将装填因子设置的小一点,或者为0(禁止缩容)
			//在这里,装填因子以25%为界,小于25%则进行缩容,大小缩至原来的1/2
			if (m_nCapacity >> 1 < c_nDefaultCapacity)
				return;	//不能缩到比默认capacity小
			if (m_nSize > m_nCapacity >> 2)	//_size>1/4_capacity
				return;	//装填因子大于25%不必缩容
			T * OldElem = m_tElem;
			m_tElem = new T[m_nCapacity >>= 1];	//容量减半
			for (int i = 0; i < m_nSize; i++)
				m_tElem[i] = OldElem[i];
			delete[] OldElem;
		}

		//算法类
		//排序类函数
		//归并算法
		void merge(Rank __lo, Rank __mi, Rank __hi)
		{
			T * A = m_tElem + __lo;	//合并后的向量A[0,__hi-__lo)=m_tElem[__lo,__hi)
			int lb{ __mi - __lo };	//中点之前的元素个数
			T * B = new T[lb];		//前子向量B[0,lb)=m_tElem[0,__mi)
			for (Rank i = 0; i < lb; B[i] = A[i++]);//复制前子向量
			int lc{ __hi - __mi };	//后子向量元素个数
			T * C = m_tElem + __mi;	//C[0,lc)=m_tElem[__mi,__hi)
									//至此,前子向量和后子向量就被拿出来了
									//其中A为指向前子向量第一个元素的指针
									//B为前子向量的备份,C为指向后子向量第一个元素的指针
									//例如,排序状态1 5 4 8,前子向量1 5后子向量4 8
									//我下面要干的活就是排成1 4 5 8,也就是把4 8插入1 5
			for (int i{}, j{}, k{}; (j < lb) || (k < lc);)
			{
				//在以下的过程中我会重写A,A的前子向量我以备份至B
				//重写的过程相当于把B[j]和C[k]中不符合Compare的放到A末尾
				//也就是说当Compare相当于<功能时,把大的放到A的末尾实现升序排列
				if ((j < lb) &&	//保证B[j++]不会越界,因为B为复制的前子向量,所以B的长度为lb
					(
						!(k < lc) ||	//这里的设计也是防越界的设计
										//而且比较巧妙,首先让我们来考虑一个问题,
										//我要想让C[k]越界,那么k必需k>=lc
										//当k>=lc时!(k<lc)为真,所以在逻辑或表达式比中
										//第一个为真时后面的Compare(B[j], C[k])不会被计算
										//所以!(k<lc)||的设计是防止越界
						(Compare(B[j], C[k]))//到这里之前的前两个条件
											 //!(k < lc)||和(j < lb)&&都是为了保证不越界设计的
											 //那么到这里就引发了重点
											 //也就是整个表达式的核心意思
											 //在保证不下标越界的情况下,如果B[j]<C[k]
											 //注:=>1.如果前子序列的第一个小于后子序列的第一个
											 //其中归并算法中的子序列都是已经排好序的
											 //也就是前子序列最小的那一个小于后子序列最小的那一个
											 //在我要升序排列的情况下,我要把前子序列的第一个放在第一位
											 //也就是将拷贝好的B[0]放入A[0]
											 //在第二步中B[j]表示的是前子序列当前操作的那一个
											 //对于1 5,4 8的实例来说也就是5
											 //B[j]的实际意义也就是当前对A操作结尾的那一个
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
						!(j < lb) ||	//跟上面讲的原理一样前两个是为了保证不越界
						(Compare(C[k], B[j]))
						//整个表达式的核心意思,由此转为
						//在保证不越界的情况下,如果C[k]<B[j]的话
						//在第一步中,也就是判断后子序列的第1个
						//和前子序列的第2个的大小,如果说后子序列的
						//第一个小于前子序列的最后一个,也就是在升序的状态下
						//后子序列的第一个应当插入前子序列的中间
						//也就是将C[0]赋值给A[1]
						))
					A[i++] = C[k++];
				//=>1:
				//A_1 5 4 8,B[1 5],C_4 8,i==j==1,k==0;
				//(0<2)&&(!(1<2)||(4<5))==true
				//A[1++]=C[0++]=>
				//A_1 4 4 8,B[1 5],C_4 8,i==2,j==1,k==1;

				//=>2:
				//A_1 4 5 8,B[1 5],C_4 8,i==3,j==2,k==1;
				//(1<2)&&(!(2<2)||(不会计算))
			}
		}

		//数值交换函数,交换秩为__a和__b的元素
		void swap(Rank __a, Rank __b)
		{
			/*这样的交换方法只适用于整型,不适用于浮点型
			//所以还是使用比较通用的方法
			m_tElem[__a] = m_tElem[__a] ^ m_tElem[__b];
			m_tElem[__b] = m_tElem[__a] ^ m_tElem[__b];
			m_tElem[__a] = m_tElem[__a] ^ m_tElem[__b];
			*/
			T tmp = m_tElem[__a];
			m_tElem[__a] = m_tElem[__b];
			m_tElem[__b] = tmp;
		}

		//去重器函数,返回被删除的元素的数目
		//有序去重器
		int orderUnique()
		{
			/*
			*跳过相同的元素,直接把不相同的元素往前移,到最后不相同的
			*元素均在最前,相同的元素均在最后,直接截取结尾元素即可
			*/
			Rank i = 0, j = 0;
			while (++j < m_nSize)
				if (m_tElem[i] != m_tElem[j])	//跳过相同的元素
					m_tElem[++i] = m_tElem[j];
			m_nSize = ++i;
			shrink();
			return j - i;
		}

		//无序去重器
		int unOrderUnique()
		{
			int OldSize = m_nSize;	//记录原始大小
			Rank i = 0;				//从第0个开始向后找
			while (i < m_nSize)
				find(m_tElem[i], i + 1, m_nSize) == -1 ?
				i++ :		//找不到
				remove(i);	//找到了删除,并且保持i不变,直到找不到为止
			return OldSize - m_nSize;
		}

		//查找类函数
		//无序查找,查找区间[__lo,__hi),找不到返回_hi+1
		//找到返回从左向右第一个元素的秩
		Rank unorderFind(const T & __f, Rank __lo, Rank __hi)const
		{
			int i = __lo;
			for (; (i < __hi) && (m_tElem[i] != __f); i++);
			return i;
		}

		//有序查找
		//二分查找算法
		Rank binSearch(const T & __f, Rank __lo, Rank __hi)const
		{
			while (1 < __hi - __lo)
			{
				Rank mid = (__hi + __lo) >> 1;
				//划分两个区间[__lo,mid)和[mid,__hi)
				(__f < m_tElem[mid]) ? __hi = mid : __lo = mid;
				//即便查找成功这个算法也不能提前终止
			}	//出口时__hi=__lo+1,查找区间仅剩一个元素__lo
			return __f == m_tElem[__lo] ? __lo : -1;
			//这个也不能保证有多个命中元素时的返回值的秩是最小的那一个
			//通过对全部相同元素序列的观察即可发现
		}

		//斐波那契查找算法
		Rank fibSearch(const T & __f, Rank __lo, Rank __hi)const
		{
			CFib fib(__hi - __lo);	//创建斐波那契数列
			while (__lo < __hi)
			{
				while (__hi - __lo < fib.getFib())
					fib.prev();	//保持fib中的斐波那契数列为大于
								//__hi-__lo的最小斐波那契数
				Rank mid = Rank(__lo + fib.getFib() - 1);
				//查找算法的本质为将二分查找法的查找中点
				//变为了黄金分割点
				if (__f < m_tElem[mid])
					__hi = mid;
				else if (m_tElem[mid] < __f)
					__lo = mid + 1;
				else
					return mid;
			}
			return -1;
			//同样这个也不能保证当有多个命中元素时,返回的秩到底是哪一个
		}

	public:
		//基础接口
		//构造函数
		/*
		*默认构造函数
		*/
		CVector()
		{
			m_tElem = new T[m_nCapacity = c_nDefaultCapacity];
			m_nSize = 0;
		}

		/*
		*__c:初始_capacity,默认为Default_Capacity
		*__s:初始_size,默认为0
		*__i:元素初始值,默认为0
		*/
		CVector(int __c, int __s, T __i)
		{
			m_tElem = new T[m_nCapacity = __c]; //分配内存并赋值
			for (m_nSize = 0; m_nSize < __s; m_tElem[m_nSize++] = __i);
			//一行代码完成了为_size赋值和为数组赋默认值
		}

		/*
		*通过现有数组初始化向量[__lo,__hi)
		*/
		CVector(const T* __i, Rank __hi, Rank __lo = 0)
		{
			copyFrom(__i, 0, __hi);
		}

		/*
		*使用已有向量的全部元素初始化向量
		*/
		CVector(const CVector<T>& __i)
		{
			copyFrom(__i.m_tElem, 0, __i.m_nSize);
		}

		/*
		*用已有向量初始化现有向量
		*使用已有向量的[__lo,__hi)内的元素
		*/
		CVector(const CVector<T>& __i, Rank __lo, Rank __hi)
		{
			copyFrom(__i.m_tElem, __lo, __hi);
		}

		//析构函数
		~CVector()
		{
			delete[] m_tElem;
		}

		//只读访问接口
		Rank size() const //返回向量规模
		{
			return m_nSize;
		}

		bool empty() const //空向量返回true
		{
			return !m_nSize;
		}

		Order ordered()const					//判断排序顺序
		{
			int az{},	//升序数
				za{},	//降序数
				aa{};	//相等数
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
		
		//在区间[__lo,__hi)中查找元素__f,找不到返回-1
		Rank find(const T & __f, Rank __lo, Rank __hi)const
		{
			if (ordered() != None)
				return
				//fibSearch(__f, __lo, __hi);
				binSearch(__f, __lo, __hi);
			else
			{
				Rank res = UnorderFind(__f, __lo, __hi);
				//UnorderFind函数在查找失败的情况下返回的是__hi
				//所以在此处需要一个判断
				return res == __hi ? -1 : res;
			}
		}

		//可写访问接口
		//重载下标访问运算符便于像数组一样访问元素
		T& operator[](Rank __r) const
		{
			return m_tElem[__r];
		}

		//重载赋值运算符,便于直接赋值,返回一个引用,便于连续赋值的写法
		CVector<T> & operator=(CVector<T> const & __t)
		{
			if (m_nSize)	//copyFrom函数会为向量分配空间,所以如果_elem中有内容的话,
				delete[] m_tElem;	//删除_elem
			copyFrom(__t.m_tElem, 0, __t.m_nSize);
			return *this;
		}

		//删除秩为__d的元素,返回值为当前删除的这个元素
		T remove(Rank __d)
		{
			T tmp = m_tElem[__d];
			remove(__d, __d + 1);	//相当于处理__d,__d+1的情况
			return tmp;
		}

		//删除区间[__lo,__hi)范围内的元素,返回被删除元素的数目
		int remove(Rank __lo, Rank __hi)
		{
			while (__hi < m_nSize)
				m_tElem[__lo++] = m_tElem[__hi++];
			m_nSize = __lo;
			shrink();			//如有必要,缩容
			return __hi - __lo;
		}

		//清空向量
		int clear()
		{
			return remove(0, m_nSize);
		}

		//插入元素,在秩为__in的地方插入数据__e,返回插入位置的秩
		Rank insert(Rank __inp, const T & __e)
		{
			expand();	//如有必要,扩容
			for (int i = m_nSize; i > __inp; i--)
				m_tElem[i] = m_tElem[i - 1];
			m_tElem[__inp] = __e;
			m_nSize++;
			return __inp;
		}

		//前后位置快速插入
		void push_front(const T& __e)
		{
			insert(0, __e);
		}

		void push_back(const T& __e)
		{
			insert(m_nSize, __e);
		}

		//归并排序
		void mergeSort(Rank __lo, Rank __hi)
		{
			//与冒泡排序通过反复调用单趟扫描交换类似,归并排序
			//就是反复调用二路归并来实现的,所谓的二路归并就是将
			//两个有序序列合并成一个有序序列
			if (__hi - __lo < 2)
				return;		//忽略单元素区间
			int mid{ (__lo + __hi) >> 1 };	//取中点
											//对中点两边分别排序
			mergeSort(__lo, mid);
			mergeSort(mid, __hi);
			merge(__lo, mid, __hi);	//进行归并
		}

		//去重器
		int unique()
		{
			if (ordered() == None)
				return unOrderUnique();
			return orderUnique();
		}

		//遍历接口
		//根据函数指针来遍历向量内部数据
		void traverse(void(*visit)(T &))
		{
			for (int i = 0; i < m_nSize; i++)
				visit(m_tElem[i]);
		}

	};


}

#endif // !__VECTOR_H_
