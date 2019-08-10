#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "common.h"

#include "PlexPX.h"

#include <Windows.h>
#include <string>

#include <dshow.h>

#include "Rijndael.h"
#include "AsicenPropset.h"
#include "CIniFileAccess.h"

#pragma comment(lib, "aes.lib")

struct TunerAndCaptureGuid {
	const WCHAR * const Tuner;
	const WCHAR * const Capture;
};

static constexpr TunerAndCaptureGuid KNOWN_GUIDS_S[] = {
	// BS/CS110用TunerとCaptureのGUID組合せ
	// PX-Q3PE		Tuner 0/1/4/5がそれぞれ1個	計4個
	// PX-W3PE V2	Tuner 0/4がそれぞれ1個		計2個
	// PX-W3PE		Tuner 0/1がそれぞれ1個		計2個
	// PX-W3U3 V2	Tuner 0/2がそれぞれ1個		計2個
	// PX-W3U3		Tuner 0が2個				計2個
	// PX-W3U2		Tuner 0が2個				計2個
	// PX-S3U2		Tuner 0が1個				計1個
	{ L"{8c16b9c1-b2ec-4657-b69f-f1925fcfc174}", L"{49b710c9-9c30-4d27-b664-1a9fa41dc27e}", },		// PX-Q3PE Tuner 0 / PX-W3PE Tuner 0
	{ L"{ead4691e-4ed3-4491-b193-8abdc2895010}", L"{5fdb68db-1b0a-496f-b47a-7db2f8057d50}", },		// PX-Q3PE Tuner 1 / PX-W3PE Tuner 1
	{ L"{8181256c-91d3-440a-87a7-74e193e12ca6}", L"{84503d7b-3f36-4271-9a0e-72e13124b22b}", },		// PX-Q3PE Tuner 4
	{ L"{7d80835e-8f9c-4928-bcab-f0262cbb6bda}", L"{e9e1b74f-04b0-4016-a981-7b21482c9456}", },		// PX-Q3PE Tuner 5
	{ L"{06cd74f9-2ec3-47de-b7c0-fbd1f87bb172}", L"{e367dc1b-bfb5-4698-a036-f49a19a63c55}", },		// PX-W3PE V2 Tuner 0
	//{ L"{a12cea30-b753-44e8-a027-9dc868d68888}", L"{ed5390ab-ead4-4b54-8928-2e4f070a99c9}", },	// PX-W3PE V2 Tuner 1 デバイスとして存在するけど偽物
	{ L"{955b2355-62fc-42bd-a0bb-e75e9451b15d}", L"{5b24d815-b61b-4c85-9ed6-dc2f1772e204}", },		// PX-W3PE V2 Tuner 4
	//{ L"{8b3af864-b968-4b56-a4df-bd518b161dd9}", L"{54c44ae2-eec6-4824-8ebc-8555934cea40}", },	// PX-W3PE V2 Tuner 5 デバイスとして存在するけど偽物
	{ L"{64936de7-6ab9-470a-b0e6-10014f83b75f}", L"{25dcdf37-fb57-4515-8971-ae392196ff27}", },		// PX-S3U2 Tuner 0
	{ L"{f08b8084-075c-4542-9405-c53c16c981ac}", L"{7a172030-e4c3-4903-91d1-d831701ae6b9}", },		// PX-W3U2 Tuner 0
	{ L"{37a61614-c9cd-446f-8214-430b59d769fb}", L"{34a320cb-f54b-47cf-ad49-e66a23a20630}", },		// PX-W3U3 Tuner 0
	{ L"{fc7eee4c-58ab-47bd-a511-1149f49007ea}", L"{5ea23aed-35cb-44d4-8d85-748f36b1ac5b}", },		// PX-W3U3 V2 Tuner 0
	{ L"{ef4af615-7acd-45d2-b7b4-ded5c22a382e}", L"{d572e115-62c7-49ff-956c-84369aada823}", },		// PX-W3U3 V2 Tuner 2
};

static constexpr TunerAndCaptureGuid KNOWN_GUIDS_T[] = {
	// UHF/CATV用TunerとCaptureのGUID組合せ
	// PX-Q3PE		Tuner 2/3/6/7がそれぞれ1個	計4個
	// PX-W3PE V2	Tuner 2/6がそれぞれ1個		計2個
	// PX-W3PE		Tuner 2/3がそれぞれ1個		計2個
	// PX-W3U3 V2	Tuner 1/3がそれぞれ1個		計2個
	// PX-W3U3		Tuner 1が2個				計2個
	// PX-W3U2		Tuner 1が2個				計2個
	// PX-S3U2		Tuner 1が1個				計1個
	{ L"{b332d1a0-ce5e-4ccd-bbaa-5d27da1937f7}", L"{21915155-d273-4023-9258-258416f48bd9}", },		// PX-Q3PE Tuner 2 / PX-W3PE Tuner 2
	{ L"{1bb610b9-a113-43ef-8707-0d0eb2786d0c}", L"{d3b05569-c1c2-4f8d-b03c-dd7ba04ebc6a}", },		// PX-Q3PE Tuner 3 / PX-W3PE Tuner 3
	{ L"{f7dffb42-19bb-495c-8025-cae755351ddb}", L"{fa8b2a09-53d3-42f7-b9c8-6ce79092f3f6}", },		// PX-Q3PE Tuner 6
	{ L"{4f47ce49-ca06-4482-8fda-aa130873f9fb}", L"{da7b3902-c5f9-4829-a46f-f55922324cd5}", },		// PX-Q3PE Tuner 7
	{ L"{67a09e55-6937-4197-9854-aebabb867cf0}", L"{041ca304-19fc-4885-93a0-657c2e2c83bf}", },		// PX-W3PE V2 Tuner 2
	//{ L"{e9de5b70-97bd-46cf-b3b2-ab7e7d5553af}", L"{5f2d5729-3d78-4210-8dec-82fd21ad5a4d}", },	// PX-W3PE V2 Tuner 3 デバイスとして存在するけど偽物
	{ L"{2ef1f363-01f6-4d40-abf0-bcf60412a91f}", L"{52a7dfeb-de46-4b7f-9196-c1a8ec86d7d7}", },		// PX-W3PE V2 Tuner 6
	//{ L"{a954451c-bb29-4286-862c-977cc622aaca}", L"{e1d5479d-ebba-4689-9253-7ee0fa97bca7}", },	// PX-W3PE V2 Tuner 7 デバイスとして存在するけど偽物
	{ L"{6d693f4b-8575-40c6-9537-4515579d1cd8}", L"{80e65bb1-eed0-4039-bdfc-4c365ce02477}", },		// PX-S3U2 Tuner 1
	{ L"{c2aca3d2-a439-4f7f-b064-75362763edb7}", L"{eb561c45-dcbc-410a-bca2-98a05309eb53}", },		// PX-W3U2 Tuner 1
	{ L"{2e55751d-b8ad-4f98-8de7-3b06c2d4cfe9}", L"{71c25d40-b6bd-46a4-8363-329d28e663ab}", },		// PX-W3U3 Tuner 1
	{ L"{92644c53-ca6c-40b3-89e0-91c8c40ad263}", L"{e2b5be0c-39e8-4d9f-bc14-79ba8805181a}", },		// PX-W3U3 V2 Tuner 1
	{ L"{6430d7ee-7dc7-4978-9481-d9657cbd583f}", L"{974860c1-a2a9-48e8-82d2-5e8c64a39878}", },		// PX-W3U3 V2 Tuner 3
};

static constexpr TunerAndCaptureGuid KNOWN_GUIDS_3[] = {
	// UHF/CATV/BS/CS110用TunerとCaptureのGUID組合せ
	// PX-S3U		Tuner 0が1個				計1個
	{ L"{58d97867-8a41-4f07-97e3-f2819af81a92}", L"{7e65574d-e061-4129-8998-51881804e4a4}", },		// PX-S3U Tuner 0
	//{ L"{cd93ac8b-8e1f-4a3e-98fc-b89c44817ee1}", L"{5e59138b-5322-4f1c-a03c-ce721a11a0a6}", },	// PX-S3U Tuner 1 たぶんこれも偽物
};

FILE *g_fpLog = NULL;

HMODULE CPlexPXSpecials::m_hMySelf = NULL;

static constexpr BYTE SeedInit[32] = {
	0x61, 0xd8, 0x56, 0x3d, 0xc1, 0x15, 0x46, 0x68, 0xb2, 0xec, 0x6f, 0xa9, 0xed, 0x45, 0x33, 0x81,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
};

static constexpr BYTE SeedM2Dec[16] = {
	0xaa, 0xcb, 0x1e, 0x40, 0x5f, 0x43, 0x42, 0x50, 0x84, 0xfe, 0xa1, 0x1b, 0xc4, 0xb2, 0xb9, 0x1f,
};

static constexpr BYTE SeedDecode[16] = {
	0x3c, 0x51, 0x93, 0x13, 0x9b, 0xa2, 0x41, 0xc9, 0xb3, 0xfe, 0xb5, 0xda, 0xfe, 0xa0, 0x05, 0xe2,
};

static constexpr BYTE DecodeTable[32] = {
	0x40, 0x5b, 0x99, 0xcc, 0x69, 0x54, 0x42, 0xd6, 0xad, 0x0a, 0x95, 0x27, 0x46, 0x92, 0x6b, 0x73,
	0x1c, 0x1a, 0x39, 0xfb, 0x21, 0xd7, 0x4f, 0xe5,	0x8b, 0xec, 0x1f, 0x5f, 0xd3, 0x79, 0x7a, 0x13,
};

static inline void block_xor(BYTE *ret, const BYTE *val1, const BYTE *val2)
{
	for (int i = 0; i < 16; i++)
		ret[i] = val1[i] ^ val2[i];
}

static inline void set_key_encrypt(DWORD *pdwRoundKey, const BYTE *pcbyKey, DWORD dwFlag)
{
	if (dwFlag & 0x80) {
		fake_set_key_encrypt(pdwRoundKey, pcbyKey);
	}
	else {
		Rijndael_set_key_encrypt(pdwRoundKey, pcbyKey);
	}
}

static inline void decrypt_buf(BYTE *pbyBuf, DWORD dwSize, DWORD *pdwRoundKey)
{
	for (int i = 0; i < (int)dwSize; i += 188, pbyBuf += 188) {
		for (int j = 4; j < 164; j += 32) {
			Rijndael_decrypt(pdwRoundKey, &pbyBuf[j], &pbyBuf[j]);
		}
		for (int j = 4; j < 188; j++) {
			pbyBuf[j] ^= DecodeTable[(j - 4) & 0x1f];
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
		::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		// モジュールハンドル保存
		CPlexPXSpecials::m_hMySelf = hModule;
		break;

	case DLL_PROCESS_DETACH:
		// デバッグログファイルのクローズ
		CloseDebugLog();
		break;
	}
    return TRUE;
}

__declspec(dllexport) IBdaSpecials * CreateBdaSpecials(CComPtr<IBaseFilter> pTunerDevice)
{
	return new CPlexPXSpecials(pTunerDevice);
}

__declspec(dllexport) HRESULT CheckAndInitTuner(IBaseFilter *pTunerDevice, const WCHAR *szDisplayName, const WCHAR *szFriendlyName, const WCHAR *szIniFilePath)
{
	HRESULT hr;

	CIniFileAccess IniFileAccess(szIniFilePath);
	IniFileAccess.SetSectionName(L"PLEXPX");

	// DebugLogを記録するかどうか
	if (IniFileAccess.ReadKeyB(L"DebugLog", FALSE)) {
		// DebugLogのファイル名取得
		SetDebugLog(common::GetModuleName(CPlexPXSpecials::m_hMySelf) + L"log");
	}

	BOOL bM2_Dec = IniFileAccess.ReadKeyB(L"M2_Dec", FALSE);
	BOOL bUseKnownGUID = IniFileAccess.ReadKeyB(L"UseKnownGUID", FALSE);
	BOOL bISDBT = IniFileAccess.ReadKeyB(L"ISDB-T", FALSE);
	BOOL bISDBS = IniFileAccess.ReadKeyB(L"ISDB-S", FALSE);
	std::wstring displayName = szDisplayName;

	// DisplayNameがGUID一覧と一致しているか比較
	if (bUseKnownGUID) {
		if (!bISDBT && !bISDBS) {
			OutputDebug(L"Set ISDB-S or/and ISDB-T.\n");
			return E_FAIL;
		}

		else if (bISDBS && !bISDBT) {
			BOOL found = FALSE;
			for (int i = 0; i < sizeof KNOWN_GUIDS_S / sizeof KNOWN_GUIDS_S[0]; i++) {
				if (displayName.find(KNOWN_GUIDS_S[i].Tuner) != std::wstring::npos) {
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

		else if (bISDBT && !bISDBS) {
			BOOL found = FALSE;
			for (int i = 0; i < sizeof KNOWN_GUIDS_T / sizeof KNOWN_GUIDS_T[0]; i++) {
				if (displayName.find(KNOWN_GUIDS_T[i].Tuner) != std::wstring::npos) {
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

		else {
			BOOL found = FALSE;
			for (int i = 0; i < sizeof KNOWN_GUIDS_3 / sizeof KNOWN_GUIDS_3[0]; i++) {
				if (displayName.find(KNOWN_GUIDS_3[i].Tuner) != std::wstring::npos) {
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

	CComQIPtr<IKsPropertySet> pIKsPropertySet(pTunerDevice);
	if (!pIKsPropertySet) {
		return E_FAIL;
	}

	// Flag取得
	DWORD dwFlag;
	if (FAILED(hr = asicen_GetFlags(pIKsPropertySet, &dwFlag))) {
		return hr;
	}

	DWORD rk[44];
	BYTE KeyInit[32], Rand[16], buf[16];
	memcpy(KeyInit, SeedInit, sizeof(SeedInit));
	if (dwFlag & 0x01) {
		if (SUCCEEDED(hr = asicen_GetRand(pIKsPropertySet, Rand, sizeof(Rand)))) {
			block_xor(buf, SeedInit, Rand);
			set_key_encrypt(rk, buf, dwFlag);
			memcpy(KeyInit, &rk[40], 16);
		}
	}

	if (FAILED(hr = asicen_Init(pIKsPropertySet, KeyInit, sizeof(KeyInit)))) {
		return hr;
	}

	if (bM2_Dec) {
		// M2_Dec設定
		BYTE KeyM2Dec[16];
		memcpy(KeyM2Dec, SeedM2Dec, sizeof(SeedM2Dec));
		if (SUCCEEDED(hr = asicen_GetRand(pIKsPropertySet, Rand, sizeof(Rand)))) {
			block_xor(buf, SeedM2Dec, Rand);
			set_key_encrypt(rk, buf, dwFlag);
			memcpy(KeyM2Dec, &rk[40], 16);
		}

		hr = asicen_SetMulti2(pIKsPropertySet, KeyM2Dec, sizeof(KeyM2Dec));
	}

	return S_OK;
}

__declspec(dllexport) HRESULT CheckCapture(const WCHAR *szTunerDisplayName, const WCHAR *szTunerFriendlyName,
	const WCHAR *szCaptureDisplayName, const WCHAR *szCaptureFriendlyName, const WCHAR *szIniFilePath)
{
	CIniFileAccess IniFileAccess(szIniFilePath);
	IniFileAccess.SetSectionName(L"PLEXPX");

	BOOL bUseKnownGUID = IniFileAccess.ReadKeyB(L"UseKnownGUID", FALSE);
	BOOL bISDBT = IniFileAccess.ReadKeyB(L"ISDB-T", FALSE);
	BOOL bISDBS = IniFileAccess.ReadKeyB(L"ISDB-S", FALSE);

	if (!bUseKnownGUID) {
		return S_OK;
	}

	std::wstring tunerDisplayName = szTunerDisplayName;
	std::wstring captureDisplayName = szCaptureDisplayName;

	// チューナーデバイスとキャプチャーデバイスの組合せが正しいか確認
	if (bISDBS && !bISDBT) {
		for (int i = 0; i < sizeof KNOWN_GUIDS_S / sizeof KNOWN_GUIDS_S[0]; i++) {
			if (tunerDisplayName.find(KNOWN_GUIDS_S[i].Tuner) != std::wstring::npos && captureDisplayName.find(KNOWN_GUIDS_S[i].Capture) != std::wstring::npos) {
				return S_OK;
			}
		}
	}
	else if (bISDBT && !bISDBS) {
		for (int i = 0; i < sizeof KNOWN_GUIDS_T / sizeof KNOWN_GUIDS_T[0]; i++) {
			if (tunerDisplayName.find(KNOWN_GUIDS_T[i].Tuner) != std::wstring::npos && captureDisplayName.find(KNOWN_GUIDS_T[i].Capture) != std::wstring::npos) {
				return S_OK;
			}
		}
	}
	else {
		for (int i = 0; i < sizeof KNOWN_GUIDS_3 / sizeof KNOWN_GUIDS_3[0]; i++) {
			if (tunerDisplayName.find(KNOWN_GUIDS_3[i].Tuner) != std::wstring::npos && captureDisplayName.find(KNOWN_GUIDS_3[i].Capture) != std::wstring::npos) {
				return S_OK;
			}
		}
	}
	OutputDebug(L"Capture GUID not match.\n");

	return E_FAIL;
}

CPlexPXSpecials::CPlexPXSpecials(CComPtr<IBaseFilter> pTunerDevice)
	: m_pTunerDevice(pTunerDevice),
	  m_pIKsPropertySet(m_pTunerDevice),
	  m_dwFlag(0)
{
	::InitializeCriticalSection(&m_CriticalSection);

	HRESULT hr;

	::EnterCriticalSection(&m_CriticalSection);
	hr = asicen_GetFlags(m_pIKsPropertySet, &m_dwFlag);
	::LeaveCriticalSection(&m_CriticalSection);

	return;
}

CPlexPXSpecials::~CPlexPXSpecials()
{
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

	HRESULT hr = S_OK;
	DWORD rk[44];
	BYTE Rand[16];
	BOOL bNeedDecrypt = FALSE;

	::EnterCriticalSection(&m_CriticalSection);
	if (m_dwFlag & 0x08) {
		if (SUCCEEDED(hr = asicen_GetRand(m_pIKsPropertySet, Rand, sizeof(Rand)))) {
			BYTE buf[16];
			block_xor(buf, SeedDecode, Rand);
			Rijndael_set_key_decrypt(rk, buf);
			bNeedDecrypt = TRUE;
		}
	}

	if (SUCCEEDED(hr = asicen_Decode(m_pIKsPropertySet, pBuf, dwSize)) && bNeedDecrypt) {
		decrypt_buf(pBuf, dwSize, rk);
	}
	::LeaveCriticalSection(&m_CriticalSection);

	return hr;
}

const HRESULT CPlexPXSpecials::GetSignalStrength(float *fVal)
{
	HRESULT hr;
	DWORD level;

	::EnterCriticalSection(&m_CriticalSection);
	hr = asicen_GetSignalLevel(m_pIKsPropertySet, &level);
	::LeaveCriticalSection(&m_CriticalSection);
	if (FAILED(hr)) {
		return hr;
	}

	if (fVal) {
		*fVal = (float)level / 100.0F;
	}

	return S_OK;
}

const HRESULT CPlexPXSpecials::PreLockChannel(TuningParam *pTuningParam)
{
	// Bandwidthは固定値6を設定しておかないとチューニングに失敗する
	pTuningParam->Modulation.BandWidth = 6;

	return S_OK;
}

const HRESULT CPlexPXSpecials::PostLockChannel(const TuningParam *pTuningParam)
{
	HRESULT hr = S_OK;

	if (pTuningParam->TSID != 0 && pTuningParam->TSID != -1) {
		::EnterCriticalSection(&m_CriticalSection);
		hr = asicen_SetTSID(m_pIKsPropertySet, pTuningParam->TSID);
		::LeaveCriticalSection(&m_CriticalSection);
		if (FAILED(hr)) {
			return hr;
		}
	}

	if (pTuningParam->SID != 0 && pTuningParam->SID != -1) {
		::EnterCriticalSection(&m_CriticalSection);
		hr = asicen_SetSID(m_pIKsPropertySet, (DWORD)pTuningParam->SID);
		::LeaveCriticalSection(&m_CriticalSection);
	}

	return hr;
}

void CPlexPXSpecials::Release(void)
{
	delete this;
}
