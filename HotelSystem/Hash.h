#pragma once
#ifndef __HASH_H_
#define __HASH_H_

#include <wincrypt.h>

namespace csp
{
	// Hash �㷨
	enum EHashAlg
	{
		HASHALG_MD5 = CALG_MD5,
		HASHALG_SHA1 = CALG_SHA1,
		HASHALG_SHA256 = CALG_SHA_256,
		HASHALG_SHA384 = CALG_SHA_384,
		HASHALG_SHA512 = CALG_SHA_512
	};

	enum EHashLen
	{
		HASHLEN_MD5 = 16,
		HASHLEN_SHA1 = 20,
		HASHLEN_SHA256 = 32,
		HASHLEN_SHA384 = 48,
		HASHLEN_SHA512 = 64
	};

	class CHash
	{
		const char * m_szHashChar;
		HCRYPTPROV * m_hProv;
		HCRYPTHASH * m_hHash;
		int m_nHashAlg;
		void initCSP();
	public:
		CHash();
		// ����hash�㷨
		void setHashAlgorithm(EHashAlg __alg);
		// ��ȡ��ǰhash�㷨
		int getHashAlgorithm()const;
		// ��ȡhash�ֶγ���
		int getHashLen()const;
		// ����hash
		bool calcHash(const char * IN __tar, char * OUT __result)const;
		~CHash();
	};
}

#endif // !__HASH_H_
