// 函数定义臃肿代码存放

#pragma once
#include "header.h"

typedef FARPROC(WINAPI* FN_GetProcAddress)(
    _In_ HMODULE hModule,
    _In_ LPCSTR lpProcName
    );

typedef LPVOID(WINAPI* FN_VirtualAlloc)(
    _In_opt_ LPVOID lpAddress,
    _In_     SIZE_T dwSize,
    _In_     DWORD flAllocationType,
    _In_     DWORD flProtect
    );

typedef BOOL(WINAPI* FN_VirtualFree)(
    _Pre_notnull_ _When_(dwFreeType == MEM_DECOMMIT, _Post_invalid_) _When_(dwFreeType == MEM_RELEASE, _Post_ptr_invalid_) LPVOID lpAddress,
    _In_ SIZE_T dwSize,
    _In_ DWORD dwFreeType
    );

typedef BOOL(WINAPI* FN_VirtualProtect)(
    _In_  LPVOID lpAddress,
    _In_  SIZE_T dwSize,
    _In_  DWORD flNewProtect,
    _Out_ PDWORD lpflOldProtect
    );

typedef struct _FUNCTIONS {
    FN_GetProcAddress fnGetProcAddress;
    FN_VirtualAlloc fnVirtualAlloc;
    FN_VirtualFree fnVirtualFree;
    FN_VirtualProtect fnVirtualProtect;
}FUNCTIONS, *PFUNCTIONS;