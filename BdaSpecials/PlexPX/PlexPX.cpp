#include <Windows.h>
#include <stdio.h>

#include <string>

#include "PlexPX.h"
#include "aes.h"

#include <iostream>
#include <dshow.h>

#include "common.h"


#pragma comment( lib, "Strmiids.lib" )

using namespace std;

#define PLEX_FUNC_GET_FLAGS	0x0c			// Get Flags
#define PLEX_FUNC_GET_RAND	0x0d			// Get Rand
#define PLEX_FUNC_INIT	0x09				// Init
#define PLEX_FUNC_DECODE	0x0a			// Decode
#define PLEX_FUNC_SET_MULTI2	0x14		// Set M2_Dec
#define PLEX_FUNC_SET_TSID	0x2f			// Set TSID
#define PLEX_FUNC_SET_SID	0x31			// Set SID
#define PLEX_FUNC_GET_SIGNAL_LEVEL	0x3a	// Get Signal Level

static const GUID CLSID_PropSet = { 0x9E1781E1,0x9CB1,0x4407,{ 0xBB,0xCE,0x54,0x26,0xC8,0xD0,0x0A,0x4B } };

static const wstring KNOWN_TUNER_GUIDS_S[] = {
	L"{8c16b9c1-b2ec-4657-b69f-f1925fcfc174}",	// PX-Q3PE Tuner 0 / PX-W3PE Tuner 0
	L"{ead4691e-4ed3-4491-b193-8abdc2895010}",	// PX-Q3PE Tuner 1 / PX-W3PE Tuner 1
	L"{8181256c-91d3-440a-87a7-74e193e12ca6}",	// PX-Q3PE Tuner 4
	L"{7d80835e-8f9c-4928-bcab-f0262cbb6bda}",	// PX-Q3PE Tuner 5
	L"{06cd74f9-2ec3-47de-b7c0-fbd1f87bb172}",	// PX-W3PE V2 Tuner 0
	L"{a12cea30-b753-44e8-a027-9dc868d68888}",	// PX-W3PE V2 Tuner 1
	L"{955b2355-62fc-42bd-a0bb-e75e9451b15d}",	// PX-W3PE V2 Tuner 4
	L"{8b3af864-b968-4b56-a4df-bd518b161dd9}",	// PX-W3PE V2 Tuner 5
	L"{64936de7-6ab9-470a-b0e6-10014f83b75f}",	// PX-S3U2 Tuner 0
	L"{f08b8084-075c-4542-9405-c53c16c981ac}",	// PX-W3U2 Tuner 0
	L"{37a61614-c9cd-446f-8214-430b59d769fb}",	// PX-W3U3 Tuner 0
	L"{fc7eee4c-58ab-47bd-a511-1149f49007ea}",	// PX-W3U3 V2 Tuner 0
	L"{ef4af615-7acd-45d2-b7b4-ded5c22a382e}",	// PX-W3U3 V2 Tuner 2
};

static const wstring KNOWN_TUNER_GUIDS_T[] = {
	L"{b332d1a0-ce5e-4ccd-bbaa-5d27da1937f7}",	// PX-Q3PE Tuner 2 / PX-W3PE Tuner 2
	L"{1bb610b9-a113-43ef-8707-0d0eb2786d0c}",	// PX-Q3PE Tuner 3 / PX-W3PE Tuner 3
	L"{f7dffb42-19bb-495c-8025-cae755351ddb}",	// PX-Q3PE Tuner 6
	L"{4f47ce49-ca06-4482-8fda-aa130873f9fb}",	// PX-Q3PE Tuner 7
	L"{67a09e55-6937-4197-9854-aebabb867cf0}",	// PX-W3PE V2 Tuner 2
	L"{e9de5b70-97bd-46cf-b3b2-ab7e7d5553af}",	// PX-W3PE V2 Tuner 3
	L"{2ef1f363-01f6-4d40-abf0-bcf60412a91f}",	// PX-W3PE V2 Tuner 6
	L"{a954451c-bb29-4286-862c-977cc622aaca}",	// PX-W3PE V2 Tuner 7
	L"{6d693f4b-8575-40c6-9537-4515579d1cd8}",	// PX-S3U2 Tuner 1
	L"{c2aca3d2-a439-4f7f-b064-75362763edb7}",	// PX-W3U2 Tuner 1
	L"{2e55751d-b8ad-4f98-8de7-3b06c2d4cfe9}",	// PX-W3U3 Tuner 1
	L"{92644c53-ca6c-40b3-89e0-91c8c40ad263}",	// PX-W3U3 V2 Tuner 1
	L"{6430d7ee-7dc7-4978-9481-d9657cbd583f}",	// PX-W3U3 V2 Tuner 3
};

HMODULE hMySelf;

static inline HRESULT plex_get_flags(IKsPropertySet *pIKsPropertySet, DWORD *pdwFlag)
{
	DWORD dwBytes;
	return pIKsPropertySet->Get(CLSID_PropSet, PLEX_FUNC_GET_FLAGS, NULL, 0, pdwFlag, sizeof(DWORD), &dwBytes);
}

static inline HRESULT plex_init_tuner(IKsPropertySet *pIKsPropertySet, DWORD dwFlag, BOOL m2dec)
{
	HRESULT hr = S_OK;
	DWORD dwBytes;
	const BYTE Seed[16] = { 0x61, 0xd8, 0x56, 0x3d, 0xc1, 0x15, 0x46, 0x68, 0xb2, 0xec, 0x6f, 0xa9, 0xed, 0x45, 0x33, 0x81 };
	const BYTE Data[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 };
	BYTE Key[32], Rand[16];
	CopyMemory(Key, Seed, sizeof(Seed));
	CopyMemory(&Key[16], Data, sizeof(Data));
	if (dwFlag & 0x01) {
		if (SUCCEEDED(hr = pIKsPropertySet->Get(CLSID_PropSet, PLEX_FUNC_GET_RAND, NULL, 0, Rand, sizeof(Rand), &dwBytes))) {
			if (dwFlag & 0x80) {
				aes_gen_key2(Key, Seed, Rand);
			}
			else {
				aes_gen_key(Key, Seed, Rand);
			}
		}
	}

	if (FAILED(hr = pIKsPropertySet->Set(CLSID_PropSet, PLEX_FUNC_INIT, NULL, 0, Key, sizeof(Key)))) {
		return hr;
	}

	if (m2dec) {
		// M2_Dec設定
		const BYTE Seed2[16] = { 0xaa, 0xcb, 0x1e, 0x40, 0x5f, 0x43, 0x42, 0x50, 0x84, 0xfe, 0xa1, 0x1b, 0xc4, 0xb2, 0xb9, 0x1f };
		BYTE Key2[16];
		CopyMemory(Key2, Seed2, sizeof(Seed2));
		if (SUCCEEDED(hr = pIKsPropertySet->Get(CLSID_PropSet, PLEX_FUNC_GET_RAND, NULL, 0, Rand, sizeof(Rand), &dwBytes))) {
			if (dwFlag & 0x80) {
				aes_gen_key2(Key2, Seed2, Rand);
			}
			else {
				aes_gen_key(Key2, Seed2, Rand);
			}
		}

		hr = pIKsPropertySet->Set(CLSID_PropSet, PLEX_FUNC_SET_MULTI2, NULL, 0, Key2, sizeof(Key2));
	}

	return hr;
}

static inline HRESULT plex_decode(IKsPropertySet *pIKsPropertySet, DWORD dwFlag, BYTE *pBuf, DWORD dwSize)
{
	HRESULT hr = S_OK;
	DWORD dwBytes;
	const BYTE Seed[16] = { 0x3c, 0x51, 0x93, 0x13, 0x9b, 0xa2, 0x41, 0xc9, 0xb3, 0xfe, 0xb5, 0xda, 0xfe, 0xa0, 0x05, 0xe2 };

	if (dwFlag & 0x08) {
		// Not support AES decode by myself.
		return E_FAIL;
	}

	if (SUCCEEDED(hr = pIKsPropertySet->Set(CLSID_PropSet, PLEX_FUNC_DECODE, NULL, 0, pBuf, dwSize))) {
		hr = pIKsPropertySet->Get(CLSID_PropSet, PLEX_FUNC_DECODE, NULL, 0, pBuf, dwSize, &dwBytes);
	}

	return hr;
}

static inline HRESULT plex_get_signal_level(IKsPropertySet *pIKsPropertySet, DWORD dwFlag, DWORD *pdwLevel)
{
	HRESULT hr = S_OK;
	DWORD dwBytes;
	BYTE buf[4];
	if (FAILED(hr = pIKsPropertySet->Get(CLSID_PropSet, PLEX_FUNC_GET_SIGNAL_LEVEL, NULL, 0, buf, sizeof(buf), &dwBytes))) {
		return hr;
	}

	*pdwLevel = *(DWORD*)buf;
	return hr;
}

static inline HRESULT plex_set_tsid(IKsPropertySet *pIKsPropertySet, DWORD dwFlag, DWORD tsid)
{
	BYTE buf[8];
	*(DWORD *)buf = tsid;
	*(DWORD *)&buf[4] = 0;

	return pIKsPropertySet->Set(CLSID_PropSet, PLEX_FUNC_SET_TSID, NULL, 0, buf, sizeof(buf));
}

static inline HRESULT plex_set_sid(IKsPropertySet *pIKsPropertySet, DWORD dwFlag, DWORD sid)
{
	BYTE buf[4];
	*(DWORD *)buf = sid;

	return pIKsPropertySet->Set(CLSID_PropSet, PLEX_FUNC_SET_SID, NULL, 0, buf, sizeof(buf));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	hMySelf = hModule;
    return TRUE;
}

__declspec(dllexport) IBdaSpecials * CreateBdaSpecials(CComPtr<IBaseFilter> pTunerDevice)
{
	return new CPlexPXSpecials(hMySelf, pTunerDevice);
}

__declspec(dllexport) HRESULT CheckAndInitTuner(IBaseFilter *pTunerDevice, const WCHAR *szDisplayName, const WCHAR *szFriendlyName, const WCHAR *szIniFilePath)
{
	HRESULT hr;

	// DebugLogを記録するかどうか
	if (::GetPrivateProfileIntW(L"PLEXPX", L"DebugLog", 0, szIniFilePath)) {
		// INIファイルのファイル名取得
		WCHAR szDebugLogPath[_MAX_PATH + 1];
		::GetModuleFileNameW(hMySelf, szDebugLogPath, _MAX_PATH + 1);
		::wcscpy_s(szDebugLogPath + ::wcslen(szDebugLogPath) - 3, 4, L"log");
		SetDebugLog(szDebugLogPath);
	}

	BOOL bM2_Dec = ::GetPrivateProfileIntW(L"PLEXPX", L"M2_Dec", 0, szIniFilePath);
	BOOL bUseKnownGUID = ::GetPrivateProfileIntW(L"PLEXPX", L"UseKnownGUID", 0, szIniFilePath);
	BOOL bISDBT = ::GetPrivateProfileIntW(L"PLEXPX", L"ISDB-T", 0, szIniFilePath);
	BOOL bISDBS = ::GetPrivateProfileIntW(L"PLEXPX", L"ISDB-S", 0, szIniFilePath);
	wstring displayName = szDisplayName;

	// DisplayNameがGUID一覧と一致しているか比較
	if (bUseKnownGUID) {
		if (bISDBT == bISDBS) {
			OutputDebug(L"Set one of ISDB-S or ISDB-T.\n");
			return E_FAIL;
		}

		if (bISDBS) {
			BOOL found = FALSE;
			for (int i = 0; i < sizeof KNOWN_TUNER_GUIDS_S / sizeof KNOWN_TUNER_GUIDS_S[0]; i++) {
				if (displayName.find(KNOWN_TUNER_GUIDS_S[i]) != wstring::npos) {
					// 見つかった
					found = TRUE;
					break;
				}
			}
			if (!found) {
				OutputDebug(L"Tuner GUID not match.\n");
				return E_FAIL;
			}
		}

		if (bISDBT) {
			BOOL found = FALSE;
			for (int i = 0; i < sizeof KNOWN_TUNER_GUIDS_T / sizeof KNOWN_TUNER_GUIDS_T[0]; i++) {
				if (displayName.find(KNOWN_TUNER_GUIDS_T[i]) != wstring::npos) {
					// 見つかった
					found = TRUE;
					break;
				}
			}
			if (!found) {
				OutputDebug(L"Tuner GUID not match.\n");
				return E_FAIL;
			}
		}
	}

	CComPtr<IKsPropertySet> pIKsPropertySet;

	hr = pTunerDevice->QueryInterface(IID_IKsPropertySet, (LPVOID*)&pIKsPropertySet);
	if (FAILED(hr)) {
		return hr;
	}

	DWORD dwFlag;

	// Flag取得
	hr = plex_get_flags(pIKsPropertySet, &dwFlag);
	if (FAILED(hr)) {
		return hr;
	}

	// Tuner初期化
	hr = plex_init_tuner(pIKsPropertySet, dwFlag, bM2_Dec);
	if (FAILED(hr)) {
		return hr;
	}

	return S_OK;
}

CPlexPXSpecials::CPlexPXSpecials(HMODULE hMySelf, CComPtr<IBaseFilter> pTunerDevice)
	: m_hMySelf(hMySelf),
	  m_pTunerDevice(pTunerDevice),
	  m_pIKsPropertySet(NULL),
	  m_dwFlag(0)
{
	::InitializeCriticalSection(&m_CriticalSection);

	HRESULT hr;

	hr = m_pTunerDevice->QueryInterface(IID_IKsPropertySet, (LPVOID*)&m_pIKsPropertySet);

	::EnterCriticalSection(&m_CriticalSection);
	hr = plex_get_flags(m_pIKsPropertySet, &m_dwFlag);
	::LeaveCriticalSection(&m_CriticalSection);

	return;
}

CPlexPXSpecials::~CPlexPXSpecials()
{
	m_hMySelf = NULL;

	SAFE_RELEASE(m_pIKsPropertySet);

	if (m_pTunerDevice) {
		m_pTunerDevice.Release();
		m_pTunerDevice = NULL; 
	}

	::DeleteCriticalSection(&m_CriticalSection);

	return;
}

const HRESULT CPlexPXSpecials::InitializeHook(void)
{
	if (m_pTunerDevice == NULL) {
		return E_POINTER;
	}

	return S_OK;
}

const HRESULT CPlexPXSpecials::Set22KHz(bool bActive)
{
	return E_NOINTERFACE;
}

const HRESULT CPlexPXSpecials::Set22KHz(long nTone)
{
	return E_NOINTERFACE;
}

const HRESULT CPlexPXSpecials::FinalizeHook(void)
{
	return S_OK;
}

const HRESULT CPlexPXSpecials::GetSignalState(int *pnStrength, int *pnQuality, int *pnLock)
{
	return E_NOINTERFACE;
}

const HRESULT CPlexPXSpecials::LockChannel(BYTE bySatellite, BOOL bHorizontal, unsigned long ulFrequency, BOOL bDvbS2)
{
	return E_NOINTERFACE;
}

const HRESULT CPlexPXSpecials::LockChannel(const TuningParam *pTuningParm)
{
	return E_NOINTERFACE;
}

const HRESULT CPlexPXSpecials::SetLNBPower(bool bActive)
{
	return E_NOINTERFACE;
}

const HRESULT CPlexPXSpecials::ReadIniFile(WCHAR *szIniFilePath)
{
	return S_OK;
}

const HRESULT CPlexPXSpecials::IsDecodingNeeded(BOOL *pbAns)
{
	if (pbAns)
		*pbAns = TRUE;

	return S_OK;
}

const HRESULT CPlexPXSpecials::Decode(BYTE *pBuf, DWORD dwSize)
{
	if (!m_pIKsPropertySet)
		return E_FAIL;

	if (dwSize > (188 * 1024)) {
		return E_FAIL;
	}

	::EnterCriticalSection(&m_CriticalSection);
	HRESULT hr = plex_decode(m_pIKsPropertySet, m_dwFlag, pBuf, dwSize);
	::LeaveCriticalSection(&m_CriticalSection);

	return hr;
}

const HRESULT CPlexPXSpecials::GetSignalStrength(float *fVal)
{
	HRESULT hr;
	DWORD level;

	::EnterCriticalSection(&m_CriticalSection);
	hr = plex_get_signal_level(m_pIKsPropertySet, m_dwFlag, &level);
	::LeaveCriticalSection(&m_CriticalSection);
	if (FAILED(hr)) {
		return hr;
	}

	if (fVal) {
		*fVal = (float)level / 100.0F;
	}

	return S_OK;
}

const HRESULT CPlexPXSpecials::PreTuneRequest(const TuningParam *pTuningParm, ITuneRequest *pITuneRequest)
{
	return S_OK;
}

const HRESULT CPlexPXSpecials::PostLockChannel(const TuningParam *pTuningParm)
{
	HRESULT hr = S_OK;

	if (pTuningParm->TSID != 0 && pTuningParm->TSID != -1) {
		::EnterCriticalSection(&m_CriticalSection);
		hr = plex_set_tsid(m_pIKsPropertySet, m_dwFlag, (DWORD)pTuningParm->TSID);
		::LeaveCriticalSection(&m_CriticalSection);
		if (FAILED(hr)) {
			return hr;
		}
	}

	if (pTuningParm->SID != 0 && pTuningParm->SID != -1) {
		::EnterCriticalSection(&m_CriticalSection);
		hr = plex_set_sid(m_pIKsPropertySet, m_dwFlag, (DWORD)pTuningParm->SID);
		::LeaveCriticalSection(&m_CriticalSection);
	}

	return hr;
}

void CPlexPXSpecials::Release(void)
{
	delete this;
}
