#include <Windows.h>
#include <iostream>

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation = 2,
    FileBothDirectoryInformation = 3,
    FileBasicInformation = 4,
    FileStandardInformation = 5,
    FileInternalInformation = 6,
    FileEaInformation = 7,
    FileAccessInformation = 8,
    FileNameInformation = 9,
    FileRenameInformation = 10,
    FileLinkInformation = 11,
    FileNamesInformation = 12,
    FileDispositionInformation = 13,
    FilePositionInformation = 14,
    FileFullEaInformation = 15,
    FileModeInformation = 16,
    FileAlignmentInformation = 17,
    FileAllInformation = 18,
    FileAllocationInformation = 19,
    FileEndOfFileInformation = 20,
    FileAlternateNameInformation = 21,
    FileStreamInformation = 22,
    FilePipeInformation = 23,
    FilePipeLocalInformation = 24,
    FilePipeRemoteInformation = 25,
    FileMailslotQueryInformation = 26,
    FileMailslotSetInformation = 27,
    FileCompressionInformation = 28,
    FileObjectIdInformation = 29,
    FileCompletionInformation = 30,
    FileMoveClusterInformation = 31,
    FileQuotaInformation = 32,
    FileReparsePointInformation = 33,
    FileNetworkOpenInformation = 34,
    FileAttributeTagInformation = 35,
    FileTrackingInformation = 36,
    FileIdBothDirectoryInformation = 37,
    FileIdFullDirectoryInformation = 38,
    FileValidDataLengthInformation = 39,
    FileShortNameInformation = 40,
    FileIoCompletionNotificationInformation = 41,
    FileIoStatusBlockRangeInformation = 42,
    FileIoPriorityHintInformation = 43,
    FileSfioReserveInformation = 44,
    FileSfioVolumeInformation = 45,
    FileHardLinkInformation = 46,
    FileProcessIdsUsingFileInformation = 47,
    FileNormalizedNameInformation = 48,
    FileNetworkPhysicalNameInformation = 49,
    FileIdGlobalTxDirectoryInformation = 50,
    FileIsRemoteDeviceInformation = 51,
    FileUnusedInformation = 52,
    FileNumaNodeInformation = 53,
    FileStandardLinkInformation = 54,
    FileRemoteProtocolInformation = 55,
    FileRenameInformationBypassAccessCheck = 56,
    FileLinkInformationBypassAccessCheck = 57,
    FileVolumeNameInformation = 58,
    FileIdInformation = 59,
    FileIdExtdDirectoryInformation = 60,
    FileReplaceCompletionInformation = 61,
    FileHardLinkFullIdInformation = 62,
    FileIdExtdBothDirectoryInformation = 63,
    FileDispositionInformationEx = 64,
    FileRenameInformationEx = 65,
    FileRenameInformationExBypassAccessCheck = 66,
    FileDesiredStorageClassInformation = 67,
    FileStatInformation = 68,
    FileMemoryPartitionInformation = 69,
    FileStatLxInformation = 70,
    FileCaseSensitiveInformation = 71,
    FileLinkInformationEx = 72,
    FileLinkInformationExBypassAccessCheck = 73,
    FileStorageReserveIdInformation = 74,
    FileCaseSensitiveInformationForceAccessCheck = 75,
    FileKnownFolderInformation = 76,
    FileStatBasicInformation = 77,
    FileId64ExtdDirectoryInformation = 78,
    FileId64ExtdBothDirectoryInformation = 79,
    FileIdAllExtdDirectoryInformation = 80,
    FileIdAllExtdBothDirectoryInformation = 81,
    FileMaximumInformation
} FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;

typedef struct _FILE_BASIC_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG         FileAttributes;
} FILE_BASIC_INFORMATION, * PFILE_BASIC_INFORMATION;

typedef NTSTATUS (NTAPI *fnNtSetInformationFile)(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, LONG Length, FILE_INFORMATION_CLASS FileInformationClass);
typedef NTSTATUS (NTAPI *fnNtQueryInformationFile)(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass);

void timePrinter(SYSTEMTIME myTime)
{
    printf("Time of file wYear: %d\n", myTime.wYear);
    printf("Time of file wMonth: %d\n", myTime.wMonth);
    printf("Time of file wDay: %d\n", myTime.wDay);
    printf("Time of file wHour: %d\n", myTime.wHour);
    printf("Time of file wMinute: %d\n", myTime.wMinute);
    printf("Time of file wMilliseconds: %d\n", myTime.wMilliseconds);
    
}

void getFileTime(LPCWSTR fileCheck)
{
    FILE_BASIC_INFORMATION fileBasic;
    IO_STATUS_BLOCK myIo;

    fnNtQueryInformationFile NtQueryInformationFile = (fnNtQueryInformationFile)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationFile");
    HANDLE hFile = CreateFile(fileCheck, FILE_WRITE_ATTRIBUTES, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    NtQueryInformationFile(hFile, &myIo, &fileBasic, sizeof(fileBasic), FileBasicInformation);

    SYSTEMTIME changeTime;
    SYSTEMTIME createTime;
    SYSTEMTIME lastWrite;
    SYSTEMTIME lastAccess;

    FILETIME ftChange;
    ftChange.dwHighDateTime = fileBasic.ChangeTime.HighPart;
    ftChange.dwLowDateTime = fileBasic.ChangeTime.HighPart;

    FILETIME ftCreate;
    ftCreate.dwHighDateTime = fileBasic.CreationTime.HighPart;
    ftCreate.dwLowDateTime = fileBasic.CreationTime.HighPart;

    FILETIME ftLastWrite;
    ftLastWrite.dwHighDateTime = fileBasic.LastWriteTime.HighPart;
    ftLastWrite.dwLowDateTime = fileBasic.LastWriteTime.HighPart;

    FILETIME ftLastAccess;
    ftLastAccess.dwHighDateTime = fileBasic.LastAccessTime.HighPart;
    ftLastAccess.dwLowDateTime = fileBasic.LastAccessTime.HighPart;

    FileTimeToSystemTime(&ftChange, &changeTime);
    FileTimeToSystemTime(&ftChange, &createTime);
    FileTimeToSystemTime(&ftChange, &lastWrite);
    FileTimeToSystemTime(&ftChange, &lastAccess);

    printf("-------------------------------- Change Time --------------------------------\n");
    timePrinter(changeTime);
    printf("-------------------------------- createTime --------------------------------\n");
    timePrinter(createTime);
    printf("-------------------------------- lastWrite --------------------------------\n");
    timePrinter(lastWrite);
    printf("-------------------------------- lastAccess --------------------------------\n");
    timePrinter(lastAccess);

}

void wompStomp(LPCWSTR stompFile)
{
    int daysToSubtract = 300;

    SYSTEMTIME stTimeNow;
    GetSystemTime(&stTimeNow);
    FILETIME lpFileTime;
    SystemTimeToFileTime(&stTimeNow, &lpFileTime);
    INT64 secondsToSub = daysToSubtract * 24 * 60 * 60;
    (INT64&)lpFileTime -= secondsToSub * 10000000L;
    (INT64&)lpFileTime += 1315113;
    FileTimeToSystemTime(&lpFileTime, &stTimeNow);


    IO_STATUS_BLOCK myIo;

    fnNtSetInformationFile NtSetInformationFile = (fnNtSetInformationFile)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtSetInformationFile");
    HANDLE hFile = CreateFile(stompFile, FILE_WRITE_ATTRIBUTES, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    FILE_BASIC_INFORMATION fileBasic;

    fileBasic.CreationTime.HighPart = lpFileTime.dwHighDateTime;
    fileBasic.CreationTime.LowPart = lpFileTime.dwLowDateTime;

    fileBasic.LastAccessTime.HighPart = lpFileTime.dwHighDateTime;
    fileBasic.LastAccessTime.LowPart = lpFileTime.dwLowDateTime;

    fileBasic.LastWriteTime.HighPart = lpFileTime.dwHighDateTime;
    fileBasic.LastWriteTime.LowPart = lpFileTime.dwLowDateTime;

    fileBasic.ChangeTime.HighPart = lpFileTime.dwHighDateTime;
    fileBasic.ChangeTime.LowPart = lpFileTime.dwLowDateTime;

    fileBasic.FileAttributes = FILE_ATTRIBUTE_NORMAL;

    NTSTATUS fileStatus = NtSetInformationFile(hFile, &myIo, &fileBasic, sizeof(fileBasic), FileBasicInformation);

    CloseHandle(hFile);

}

void switcherooOne(LPCWSTR myfile1, LPCWSTR myfile2)
{
    MoveFile(myfile1, myfile2);
}

void switcherooTwo(LPCWSTR myfile1, LPCWSTR myfile2)
{
    MoveFile(myfile1, myfile2);
}
int main()
{
    printf("============================================================================\n");
    printf("ProjectStomp: Move file to temp, timestomp, backc, updating both FILE_NAME & STANDARD_INFORMATION \n");
    printf("============================================================================\n");
    printf("\n\n\n\n\n\n");
    const wchar_t fileToStomp[] = L"c:\\users\\bobtheapt\\desktop\\example.dll";
    const wchar_t fileSwitcheroo[] = L"c:\\windows\\temp\\example.dll";

    getFileTime(fileToStomp);
    printf("============================================================================\n");
    printf("\n\n\n\n\n\n\n");
    printf("[+] Moving file and timestomping\n");
    switcherooOne(fileToStomp, fileSwitcheroo);

    wompStomp(fileSwitcheroo);
    getFileTime(fileSwitcheroo);
    printf("============================================================================\n");
    printf("\n\n\n\n\n\n");
    printf("[+] moving file and timestomping again\n");

    switcherooTwo(fileSwitcheroo, fileToStomp);
    wompStomp(fileToStomp);
    getFileTime(fileToStomp);
    printf("============================================================================\n");
    printf("Done\n");
    printf("============================================================================\n");

    printf("All Done \n");
}
