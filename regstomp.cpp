#include <Windows.h>
#include <iostream>

#define 	OBJ_CASE_INSENSITIVE   0x00000040
#define debug
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef enum _KEY_INFORMATION_CLASS {
    KeyBasicInformation,
    KeyNodeInformation,
    KeyFullInformation,
    KeyNameInformation,
    KeyCachedInformation,
    KeyFlagsInformation,
    KeyVirtualizationInformation,
    KeyHandleTagsInformation,
    KeyTrustInformation,
    KeyLayerInformation,
    MaxKeyInfoClass
} KEY_INFORMATION_CLASS;

typedef enum _KEY_SET_INFORMATION_CLASS {
    KeyWriteTimeInformation,
    KeyWow64FlagsInformation,
    KeyControlFlagsInformation,
    KeySetVirtualizationInformation,
    KeySetDebugInformation,
    KeySetHandleTagsInformation,
    KeySetLayerInformation,
    MaxKeySetInfoClass
} KEY_SET_INFORMATION_CLASS;

typedef struct _KEY_WRITE_TIME_INFORMATION {
    LARGE_INTEGER LastWriteTime;
} KEY_WRITE_TIME_INFORMATION, * PKEY_WRITE_TIME_INFORMATION;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef NTSTATUS (NTAPI* fnNtQueryKey) ( IN HANDLE KeyHandle, IN KEY_INFORMATION_CLASS KeyInformationClass, OUT PVOID KeyInformation,IN ULONG Length, OUT PULONG ResultLength);
fnNtQueryKey NtQueryKey = (fnNtQueryKey)GetProcAddress(GetModuleHandle(L"NTDLL"), "NtQueryKey");

typedef NTSTATUS (NTAPI* fnNtOpenKey) ( OUT PHANDLE pKeyHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
fnNtOpenKey NtOpenKey = (fnNtOpenKey)GetProcAddress(GetModuleHandle(L"NTDLL"), "NtOpenKey");

typedef NTSTATUS (NTAPI* fnNtSetInformationKey) ( IN HANDLE KeyHandle, IN KEY_SET_INFORMATION_CLASS InformationClass,IN PVOID KeyInformationData, IN ULONG DataLength);
fnNtSetInformationKey NtSetInformationKey = (fnNtSetInformationKey)GetProcAddress(GetModuleHandle(L"NTDLL"), "NtSetInformationKey");

typedef NTSTATUS (NTAPI* fnNtCreateKey) ( OUT PHANDLE pKeyHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN ULONG TitleIndex, IN PUNICODE_STRING Class OPTIONAL, IN ULONG CreateOptions, OUT PULONG Disposition OPTIONAL);
fnNtCreateKey NtCreateKey = (fnNtCreateKey)GetProcAddress(GetModuleHandle(L"NTDLL"), "NtCreateKey");

typedef NTSTATUS (NTAPI* fnNtEnumerateKey) ( IN HANDLE KeyHandle, IN ULONG Index, IN KEY_INFORMATION_CLASS KeyInformationClass, OUT PVOID KeyInformation, IN ULONG Length, OUT PULONG ResultLength);
fnNtEnumerateKey NtEnumerateKey = (fnNtEnumerateKey)GetProcAddress(GetModuleHandle(L"NTDLL"), "NtEnumerateKey");

typedef NTSTATUS (NTAPI* fnNtFlushKey) ( IN HANDLE KeyHandle);
fnNtFlushKey NtFlushKey = (fnNtFlushKey)GetProcAddress(GetModuleHandle(L"NTDLL"), "NtFlushKey");


int main()
{
    //define previous timestamp time & reg key to stomp
    int daysToSubtract = 300;
    wchar_t strSoftwareKey[] = L"\\Registry\\Machine\\SOFTWARE\\7-Zip";

    //timestuff
    SYSTEMTIME stTimeNow;
    GetSystemTime(&stTimeNow);
    FILETIME lpFileTime;
    SystemTimeToFileTime(&stTimeNow, &lpFileTime);
    INT64 secondsToSub = daysToSubtract * 24 * 60 * 60;
    (INT64&)lpFileTime -= secondsToSub * 10000000L;
    FileTimeToSystemTime(&lpFileTime, &stTimeNow);

#ifdef debug
    printf("systemtime is set to year: %d\n", stTimeNow.wYear);
    printf("systemtime is set to month: %d\n", stTimeNow.wMonth);
    printf("systemtime is set to day: %d\n", stTimeNow.wDay);
#endif

    //timestomp start
    HANDLE hKey = NULL;
    UNICODE_STRING uStrSoftwareKey = { sizeof(strSoftwareKey) - sizeof(wchar_t), sizeof(strSoftwareKey), strSoftwareKey };
    
#ifdef debug
    printf("Setting key %ls\n", strSoftwareKey);
#endif 

    OBJECT_ATTRIBUTES objAttrs;
    objAttrs.Attributes = OBJ_CASE_INSENSITIVE;
    objAttrs.Length = sizeof(OBJECT_ATTRIBUTES);
    objAttrs.RootDirectory = NULL;
    objAttrs.ObjectName = &uStrSoftwareKey;
    objAttrs.SecurityDescriptor = NULL;
    objAttrs.SecurityQualityOfService = NULL;

    NTSTATUS ntsOpenKey = NtOpenKey(&hKey, KEY_READ|KEY_WRITE, &objAttrs);
    if (ntsOpenKey != 0)
    {
#ifdef debug
        printf("NtOpenKey status error: %x\n", ntsOpenKey);
#endif
        exit(0);
    }
    KEY_WRITE_TIME_INFORMATION keyWriteInfo;

    LARGE_INTEGER rawtime;
    rawtime.LowPart = lpFileTime.dwLowDateTime;
    rawtime.HighPart = lpFileTime.dwHighDateTime;
    keyWriteInfo.LastWriteTime = rawtime;

    KEY_SET_INFORMATION_CLASS KeysetInfos = KeyWriteTimeInformation;

    NTSTATUS ntsInfoKey = NtSetInformationKey(hKey, KeysetInfos, &keyWriteInfo, sizeof(keyWriteInfo));
    if (ntsInfoKey != 0) {
#ifdef debug
        printf("NtSetInformationKey status: %x\n", ntsInfoKey);
#endif
        exit(0);
    };

    NTSTATUS ntsFlushKey = NtFlushKey(hKey);
    if (ntsFlushKey != 0) {
#ifdef debug
        printf("NtFlushKey status: %x\n", ntsFlushKey);
#endif 
        exit(0);
    };
#ifdef debug
    printf("All Done!");
#endif

    return 0;
}
