#pragma once

#include "IBdaSpecials2.h"

class CPlexPXSpecials : public IBdaSpecials2a
{
public:
	CPlexPXSpecials(HMODULE hMySelf, CComPtr<IBaseFilter> pTunerDevice);
	virtual ~CPlexPXSpecials(void);

	const HRESULT InitializeHook(void);
	const HRESULT Set22KHz(bool bActive);
	const HRESULT FinalizeHook(void);

	const HRESULT GetSignalState(int *pnStrength, int *pnQuality, int *pnLock);
	const HRESULT LockChannel(BYTE bySatellite, BOOL bHorizontal, unsigned long ulFrequency, BOOL bDvbS2);

	const HRESULT SetLNBPower(bool bActive);

	const HRESULT Set22KHz(long nTone);
	const HRESULT LockChannel(const TuningParam *pTuningParm);

	const HRESULT ReadIniFile(WCHAR *szIniFilePath);
	const HRESULT IsDecodingNeeded(BOOL *pbAns);
	const HRESULT Decode(BYTE *pBuf, DWORD dwSize);
	const HRESULT GetSignalStrength(float *fVal);
	const HRESULT PreTuneRequest(const TuningParam *pTuningParm, ITuneRequest *pITuneRequest);
	const HRESULT PostLockChannel(const TuningParam *pTuningParm);

	virtual void Release(void);

private:
	HMODULE m_hMySelf;
	CComPtr<IBaseFilter> m_pTunerDevice;
	IKsPropertySet *m_pIKsPropertySet;
	DWORD m_dwFlag;
	CRITICAL_SECTION m_CriticalSection;
};
