#include "process.h"
#include <psapi.h>
//char* procName, DWORD PID, char* SessionName, int Session, double MemUsage

void displayProcess(){
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//int width = 10;
	if(hSnap != INVALID_HANDLE_VALUE){
		PROCESSENTRY32 procEntry;

		procEntry.dwSize = sizeof(PROCESSENTRY32);

		if(Process32First(hSnap, &procEntry)){
			do{
				std::cout  << std::left << std::setw(30) << procEntry.szExeFile << "\t";
				std::cout  << std::right << std::setw(5) << procEntry.th32ProcessID << "\t";

				DWORD sessionID = 0;
				ProcessIdToSessionId(procEntry.th32ProcessID, &sessionID);

				if(sessionID != 0)
					std::cout << std::right << std::setw(12) << "Console";
				else
					std::cout << std::right << std::setw(12) << "Services";
				
				std::cout << "\t";
				std::cout << std::right << std::setw(8) << sessionID << "\t";

				PROCESS_MEMORY_COUNTERS memCounter;

				HANDLE hProcess = 0;

				hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION , FALSE, procEntry.th32ProcessID);
				if(!hProcess)
					std::cout << "Error: " << GetLastError() << " ";
				//try{
					GetProcessMemoryInfo(hProcess,
                	                   &memCounter,
                	                   sizeof( memCounter ));

				std::cout << std::right << std::setw(8) << (int)((memCounter.WorkingSetSize))  ;

				std::cout << "\n";

			}
			while(Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
}



DWORD getProcessID(const char* procName){
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//std::cout << hSnap;
	if (hSnap != INVALID_HANDLE_VALUE) {
		//std::cout << "ok Here" << std::endl;;
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!strcmp(procEntry.szExeFile, procName)) {

					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;

}