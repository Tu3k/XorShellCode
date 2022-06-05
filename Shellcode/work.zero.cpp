// 工作主要代码存放

#include "header.h"
#include "api.h"



//内嵌汇编获取Kernel32的地址
__declspec(naked) DWORD getKernel32()
{
    __asm
    {
        mov eax, fs: [30h]
        mov eax, [eax + 0ch]
        mov eax, [eax + 14h]
        mov eax, [eax]
        mov eax, [eax]
        mov eax, [eax + 10h]
        ret
    }
}


//通过kernel32基址获取GetProcAddress的地址
FARPROC _GetProcAddress(HMODULE hModuleBase)
{
    PIMAGE_DOS_HEADER lpDosHeader = (PIMAGE_DOS_HEADER)hModuleBase;
    PIMAGE_NT_HEADERS32 lpNtHeader = (PIMAGE_NT_HEADERS)((DWORD)hModuleBase + lpDosHeader->e_lfanew);
    if (!lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size) {
        return NULL;
    }
    if (!lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress) {
        return NULL;
    }
    PIMAGE_EXPORT_DIRECTORY lpExports = (PIMAGE_EXPORT_DIRECTORY)((DWORD)hModuleBase + (DWORD)lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    PDWORD lpdwFunName = (PDWORD)((DWORD)hModuleBase + (DWORD)lpExports->AddressOfNames);
    PWORD lpword = (PWORD)((DWORD)hModuleBase + (DWORD)lpExports->AddressOfNameOrdinals);
    PDWORD lpdwFunAddr = (PDWORD)((DWORD)hModuleBase + (DWORD)lpExports->AddressOfFunctions);

    DWORD dwLoop = 0;
    FARPROC pRet = NULL;
    for (; dwLoop <= lpExports->NumberOfNames - 1; dwLoop++) {
        char* pFunName = (char*)(lpdwFunName[dwLoop] + (DWORD)hModuleBase);

        if (pFunName[0] == 'G' &&
            pFunName[1] == 'e' &&
            pFunName[2] == 't' &&
            pFunName[3] == 'P' &&
            pFunName[4] == 'r' &&
            pFunName[5] == 'o' &&
            pFunName[6] == 'c' &&
            pFunName[7] == 'A' &&
            pFunName[8] == 'd' &&
            pFunName[9] == 'd' &&
            pFunName[10] == 'r' &&
            pFunName[11] == 'e' &&
            pFunName[12] == 's' &&
            pFunName[13] == 's')
        {
            pRet = (FARPROC)(lpdwFunAddr[lpword[dwLoop]] + (DWORD)hModuleBase);
            break;
        }
    }
    return pRet;
}


// 动态调用函数指针赋值
void InitModule(PFUNCTIONS mDn)
{
    mDn->fnGetProcAddress = (FN_GetProcAddress)_GetProcAddress((HMODULE)getKernel32());

    char szVirtualAlloc[] = { 'V','i','r','t','u','a','l','A','l','l','o','c',0 };
    mDn->fnVirtualAlloc = (FN_VirtualAlloc)mDn->fnGetProcAddress((HMODULE)getKernel32(), szVirtualAlloc);

    char szVirtualFree[] = { 'V','i','r','t','u','a','l','F','r','e','e',0 };
    mDn->fnVirtualFree = (FN_VirtualFree)mDn->fnGetProcAddress((HMODULE)getKernel32(), szVirtualFree);

    // 被弄了
    //char szVirtualProtect[] = { 'V','i','r','t','u','a','l','P','r','o','t','e','c','t', 0 };
    //mDn->fnVirtualProtect = (FN_VirtualProtect)mDn->fnGetProcAddress((HMODULE)getKernel32(), szVirtualProtect);

}