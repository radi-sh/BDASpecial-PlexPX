#pragma once

#include "IBdaSpecials2.h"

class CPlexPXSpecials : public IBdaSpecials2b5
{
public:
	CPlexPXSpecials(CComPtr<IBaseFilter> pTunerDevice);
	virtual ~CPlexPXSpecials(void);

	const HRESULT InitializeHook(void);

	const HRESULT IsDecodingNeeded(BOOL *pbAns);
	const HRESULT Decode(BYTE *pBuf, DWORD dwSize);
	const HRESULT GetSignalStrength(float *fVal);
	const HRESULT PreLockChannel(TuningParam *pTuningParam);
	const HRESULT PostLockChannel(const TuningParam *pTuningParam);

	virtual void Release(void);

	static HMODULE m_hMySelf;

private:
	CComPtr<IBaseFilter> m_pTunerDevice;
	CComQIPtr<IKsPropertySet> m_pIKsPropertySet;
	DWORD m_dwFlag;
	CRITICAL_SECTION m_CriticalSection;
};
