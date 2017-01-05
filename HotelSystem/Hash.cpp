#include "stdafx.h"
#include <wincrypt.h>
#include "Hash.h"
#include "Log.h"
#include <stdexcept>

void csp::CHash::initCSP()
{
	if (!CryptAcquireContext(m_hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		aduit::log.insertNewError(aduit::e_error, "CryptAcquireContext初始化CSP失败", GetLastError());
		throw std::runtime_error("CryptAcquireContext初始化CSP失败");
	}
	if(!CryptCreateHash(*m_hProv,m_nHashAlg,0,0,m_hHash))
	{
		aduit::log.insertNewError(aduit::e_error, "CryptCreateHash初始化失败", GetLastError());
		throw std::runtime_error("CryptCreateHash初始化失败");
	}
}

csp::CHash::CHash(): m_szHashChar("0123456789ABCDEF")
	, m_hProv(new HCRYPTPROV), m_hHash(new HCRYPTHASH), m_nHashAlg(HASHALG_SHA1)
{
	// 初始化CSP
	initCSP();
}

void csp::CHash::setHashAlgorithm(EHashAlg __alg)
{
	m_nHashAlg = __alg;
	if (!CryptCreateHash(*m_hProv, m_nHashAlg, 0, 0, m_hHash))
	{
		aduit::log.insertNewError(aduit::e_error, "CryptCreateHash初始化失败", GetLastError());
		throw std::runtime_error("CryptCreateHash初始化失败");
	}
}

int csp::CHash::getHashAlgorithm() const
{
	return m_nHashAlg;
}

int csp::CHash::getHashLen() const
{
	switch (m_nHashAlg)
	{
	case HASHALG_MD5:
		return HASHLEN_MD5;
	case HASHALG_SHA1:
		return HASHLEN_SHA1;
	case HASHALG_SHA256:
		return HASHLEN_SHA256;
	case HASHALG_SHA384:
		return HASHLEN_SHA384;
	case HASHALG_SHA512:
		return HASHLEN_SHA512;
	default: 
		return 0;
	}
}


bool csp::CHash::calcHash(const char *IN __tar, char *OUT __result) const
{
	size_t nTarLen{ strlen(__tar) };
	BYTE * target = new BYTE[nTarLen];
	memcpy(target, __tar, sizeof target);
	if(!CryptHashData(*m_hHash, target, sizeof(BYTE)*nTarLen, NULL))
	{
		aduit::log.insertNewError(aduit::e_error, "CryptHashData获取Hash失败", GetLastError());
		return false;
	}
	DWORD dwHashLen = getHashLen();
	BYTE * hash = new BYTE[dwHashLen];
	if(!CryptGetHashParam(*m_hHash, HP_HASHVAL, hash, &dwHashLen, 0))
	{
		aduit::log.insertNewError(aduit::e_error, "CryptGetHashParam获取Hash失败", GetLastError());
		return false;
	}
	std::string res;
	for (DWORD i = 0; i < dwHashLen; i++)
		res += char(m_szHashChar[hash[i] >> 4]),
		res += char(m_szHashChar[hash[i] & 0xf]);
	strcpy_s(__result, res.size() + 2, res.c_str());
	return true;
}

csp::CHash::~CHash()
{
	CryptDestroyHash(*m_hHash);
	CryptReleaseContext(*m_hProv, 0);
}
