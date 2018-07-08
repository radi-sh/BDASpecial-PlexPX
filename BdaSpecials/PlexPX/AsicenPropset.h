#pragma once

/*
//
// Broadcast Driver Architecture で定義されている Property set / Method set
//

static const GUID KSPROPSETID_BdaAutodemodulate = { 0xddf15b12, 0xbd25, 0x11d2,{ 0x9c, 0xa0, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0 } };

enum KSPROPERTY_BDA_AUTODEMODULATE {
    KSPROPERTY_BDA_AUTODEMODULATE_START = 0,			// get only
    KSPROPERTY_BDA_AUTODEMODULATE_STOP					// get only
};

static const GUID KSPROPSETID_BdaFrequencyFilter = { 0x71985f47, 0x1ca1, 0x11d3,{ 0x9c, 0xc8, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0 } };

enum KSPROPERTY_BDA_FREQUENCY_FILTER {
	KSPROPERTY_BDA_RF_TUNER_FREQUENCY = 0,				// get/set
	KSPROPERTY_BDA_RF_TUNER_POLARITY,					// not supported
	KSPROPERTY_BDA_RF_TUNER_RANGE,						// not supported
	KSPROPERTY_BDA_RF_TUNER_TRANSPONDER,				// not supported
	KSPROPERTY_BDA_RF_TUNER_BANDWIDTH,					// get/set
	KSPROPERTY_BDA_RF_TUNER_FREQUENCY_MULTIPLIER,		// get/set
	KSPROPERTY_BDA_RF_TUNER_CAPS,						// not supported
	KSPROPERTY_BDA_RF_TUNER_SCAN_STATUS,				// not supported
	KSPROPERTY_BDA_RF_TUNER_STANDARD,					// not supported
	KSPROPERTY_BDA_RF_TUNER_STANDARD_MODE,				// not supported
};

static const GUID KSPROPSETID_BdaSignalStats = { 0x1347d106, 0xcf3a, 0x428a,{ 0xa5, 0xcb, 0xac, 0xd, 0x9a, 0x2a, 0x43, 0x38 } };

enum KSPROPERTY_BDA_SIGNAL_STATS {
	KSPROPERTY_BDA_SIGNAL_STRENGTH = 0,					// get only
	KSPROPERTY_BDA_SIGNAL_QUALITY,						// get only
	KSPROPERTY_BDA_SIGNAL_PRESENT,						// get only
	KSPROPERTY_BDA_SIGNAL_LOCKED,						// get only
	KSPROPERTY_BDA_SAMPLE_TIME,							// get only
};

static const GUID KSMETHODSETID_BdaChangeSync = { 0xfd0a5af3, 0xb41d, 0x11d2,{ 0x9c, 0x95, 0x0, 0xc0, 0x4f, 0x79, 0x71, 0xe0 } };

enum KSMETHOD_BDA_CHANGE_SYNC {
	KSMETHOD_BDA_START_CHANGES = 0,
	KSMETHOD_BDA_CHECK_CHANGES,
	KSMETHOD_BDA_COMMIT_CHANGES,
	KSMETHOD_BDA_GET_CHANGE_STATE,
};
*/

//
// ASICEN BDA driver 固有の Property set
//

// ASICEN 拡張プロパティセット GUID
static const GUID KSPROPSETID_AsicenExtension = { 0x9e1781e1, 0x9cb1, 0x4407,{ 0xbb, 0xce, 0x54, 0x26, 0xc8, 0xd0, 0x0a, 0x4b } };

// ASICEN 拡張プロパティID
enum KSPROPERTY_ASICEN_EXTENSION {
	KSPROPERTY_ASICEN_EX_FUNC_0 = 0,					// get only
	KSPROPERTY_ASICEN_EX_FUNC_1,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_2,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_3,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_4,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_5,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_6,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_7,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_8,						// set only
	KSPROPERTY_ASICEN_EX_INIT,							// set only
	KSPROPERTY_ASICEN_EX_DECODE,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_11,						// get only
	KSPROPERTY_ASICEN_EX_GET_FLAGS,						// get only
	KSPROPERTY_ASICEN_EX_GET_RAND,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_14,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_15,						// set only
	KSPROPERTY_ASICEN_EX_SEND_CARD,						// set only
	KSPROPERTY_ASICEN_EX_RECEIVE_CARD,					// get only
	KSPROPERTY_ASICEN_EX_FUNC_18,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_19,						// set only
	KSPROPERTY_ASICEN_EX_SET_MULTI2,					// set only
	KSPROPERTY_ASICEN_EX_FUNC_21,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_22,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_23,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_24,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_25,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_26,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_27,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_28,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_29,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_30,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_31,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_32,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_33,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_34,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_35,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_36,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_37,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_38,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_39,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_40,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_41,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_42,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_43,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_44,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_45,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_46,						// get only
	KSPROPERTY_ASICEN_EX_SET_TSID,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_48,						// get only
	KSPROPERTY_ASICEN_EX_SET_SID,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_50,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_51,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_52,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_53,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_54,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_55,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_56,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_57,						// get only
	KSPROPERTY_ASICEN_EX_GET_SIGNAL_LEVEL,				// get only
	KSPROPERTY_ASICEN_EX_FUNC_59,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_60,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_61,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_62,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_63,						// get/set
	KSPROPERTY_ASICEN_EX_FUNC_64,						// set only
	KSPROPERTY_ASICEN_EX_FUNC_65,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_66,						// get only
	KSPROPERTY_ASICEN_EX_FUNC_67,						// get only
	KSPROPERTY_ASICEN_EX_SET_LNB_POWER,					// set only
	KSPROPERTY_ASICEN_EX_GET_LNB_POWER,					// get only
};

//
// ASICEN 拡張プロパティセット用関数
//
static inline HRESULT asicen_GetFlags(IKsPropertySet *pIKsPropertySet, DWORD *pdwFlags)
{
	DWORD dwBytes;
	return pIKsPropertySet->Get(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_GET_FLAGS, NULL, 0, pdwFlags, sizeof(DWORD), &dwBytes);
}

static inline HRESULT asicen_GetRand(IKsPropertySet *pIKsPropertySet, BYTE *pbyRand, DWORD dwSize)
{
	DWORD dwBytes;
	return pIKsPropertySet->Get(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_GET_RAND, NULL, 0, pbyRand, dwSize, &dwBytes);
}

static inline HRESULT asicen_Init(IKsPropertySet *pIKsPropertySet, const BYTE *pbyKey, DWORD dwSize)
{
	return pIKsPropertySet->Set(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_INIT, NULL, 0, (LPVOID)pbyKey, dwSize);
}

static inline HRESULT asicen_SetMulti2(IKsPropertySet *pIKsPropertySet, BYTE *pbyKey, DWORD dwSize)
{
	return pIKsPropertySet->Set(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_SET_MULTI2, NULL, 0, pbyKey, dwSize);
}

static inline HRESULT asicen_Decode(IKsPropertySet *pIKsPropertySet, BYTE *pbyBuf, DWORD dwSize)
{
	HRESULT hr;
	DWORD dwBytes;
	if (FAILED(hr = pIKsPropertySet->Set(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_DECODE, NULL, 0, pbyBuf, dwSize)))
		return hr;
	return pIKsPropertySet->Get(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_DECODE, NULL, 0, pbyBuf, dwSize, &dwBytes);
}

static inline HRESULT asicen_GetSignalLevel(IKsPropertySet *pIKsPropertySet, DWORD *pdwLevel)
{
	DWORD dwBytes;
	return pIKsPropertySet->Get(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_GET_SIGNAL_LEVEL, NULL, 0, pdwLevel, sizeof(*pdwLevel), &dwBytes);
}

static inline HRESULT asicen_SetTSID(IKsPropertySet *pIKsPropertySet, QWORD qwTSID)
{
	return pIKsPropertySet->Set(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_SET_TSID, NULL, 0, &qwTSID, sizeof(qwTSID));
}

static inline HRESULT asicen_SetSID(IKsPropertySet *pIKsPropertySet, DWORD dwSID)
{
	return pIKsPropertySet->Set(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_SET_SID, NULL, 0, &dwSID, sizeof(dwSID));
}

static inline HRESULT asicen_SendCard(IKsPropertySet *pIKsPropertySet, const BYTE *pbyBuf, DWORD dwSize)
{
	return pIKsPropertySet->Set(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_SEND_CARD, NULL, 0, (LPVOID)pbyBuf, dwSize);
}

static inline HRESULT asicen_ReceiveCard(IKsPropertySet *pIKsPropertySet, BYTE *pbyBuf, DWORD *pdwSize)
{
	return pIKsPropertySet->Get(KSPROPSETID_AsicenExtension, KSPROPERTY_ASICEN_EX_RECEIVE_CARD, NULL, 0, pbyBuf, *pdwSize, pdwSize);
}
