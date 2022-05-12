#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string.h>
#include <iomanip>
#include <math.h>
#include <psapi.h>
#include <winerror.h>



DWORD getProcessID(const char* procName);

void displayProcess();

void label();

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);