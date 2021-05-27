#include "stdafx.h"
#include "Tlhelp32.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment(lib, "kernel32.lib")

typedef BOOL (WINAPI*_ChangeWindowMessageFilter)( UINT , DWORD);

DWORD InvokeProgram(CString Program,BOOL IsConsole,WORD wShowWindow,CString &Out){

    SECURITY_ATTRIBUTES saProcess,saThread;

	saProcess.nLength = sizeof(SECURITY_ATTRIBUTES);
    saProcess.lpSecurityDescriptor = NULL;

	if(IsConsole)
		saProcess.bInheritHandle = true;
	else
		saProcess.bInheritHandle = false;

	saThread.nLength = sizeof(SECURITY_ATTRIBUTES);
    saThread.lpSecurityDescriptor = NULL;

	if(IsConsole)
		saThread.bInheritHandle = true; 
	else
		saThread.bInheritHandle = false; 

	HANDLE   hRead,hWrite;
	if(!CreatePipe(&hRead,&hWrite,&saProcess,0)){
		return FALSE;
	}


	PROCESS_INFORMATION pi;


    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	//GetStartupInfo(&si);   
	si.hStdError   =   hWrite;   
	si.hStdOutput   =   hWrite;   
	si.wShowWindow   =   wShowWindow;

	if(IsConsole)
		si.dwFlags   =   STARTF_USESHOWWINDOW   |   STARTF_USESTDHANDLES;
	else
		si.dwFlags = 0;


    si.cb = sizeof(STARTUPINFO);

	
	CString sCurrentDir = "";
	int pos1 = Program.ReverseFind('\\');
	int pos2 = Program.ReverseFind('/');
	if(pos1>pos2){
		sCurrentDir = Program.Left(pos1);
	}else{
		sCurrentDir = Program.Left(pos2);
	}
	
	//char chProgram[MAX_PATH];//MAX_PATH=260,调用扫描仪时，允许扫描仪列表可能超过260
	unsigned long chSize = Program.GetLength()+1;
	char* chProgram = (char*)malloc(chSize);
	memset(chProgram,0,chSize);
	memcpy(chProgram,(char*)(LPCTSTR)Program,Program.GetLength());


	char chCurrentDir[MAX_PATH];
	memset(chCurrentDir,0,MAX_PATH);
	memcpy(chCurrentDir,(char*)(LPCTSTR)sCurrentDir,sCurrentDir.GetLength());

	if(IsConsole){
		//if(!CreateProcess(NULL,(char*)chProgram,&saProcess,&saThread,TRUE,NULL,NULL,NULL,&si,&pi)){ 
		if(!CreateProcess(NULL,(char*)chProgram,&saProcess,&saThread,TRUE,NULL,NULL,NULL,&si,&pi)){ 
			free(chProgram);
			chProgram = NULL;
			return 0;
		}
	}else{
		//CString sMessage;
		//sMessage.Format("%d-%s-%s",si.cb,chProgram,chCurrentDir);

		//使用saProcess参数时有一台XP报内存错误
		if(!CreateProcess(NULL,(char*)chProgram,NULL,NULL,TRUE,NULL,NULL,chCurrentDir,&si,&pi)){ 
			free(chProgram);
			chProgram = NULL;
			return 0;
		}
	}

	if(chProgram)
		free(chProgram);

	if(IsConsole){
		WaitForSingleObject(pi.hProcess,INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(hWrite);


		char   buffer[256]={'\0'};
		DWORD  bytesRead;
		while(true){
			memset(buffer,0,sizeof(buffer));
			bytesRead = 0;
			if(ReadFile(hRead,buffer,sizeof(buffer),&bytesRead,NULL)){
				if(bytesRead>0){
					Out+=buffer;
					Sleep(10);
				}else
					break;
			}
			else
				break; 
		} 
		CloseHandle(hRead);
	}else{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		CloseHandle(hWrite);
		CloseHandle(hRead); 
	}
	
	return pi.dwProcessId;
}

HWND FindProcessWindow(DWORD dwPId,LPCTSTR lpClassName,LPCTSTR lpWindowName)
{
	HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(),GW_CHILD);
	while(::IsWindow(hWndPrevious))
	{ 
		DWORD dwProcessId = 0;
		::GetWindowThreadProcessId(hWndPrevious,&dwProcessId);
		if(dwProcessId==dwPId || dwPId==0){

			BOOL clsFlag = TRUE;

			if(lpClassName){
				CString sClassName="";
				::GetClassName(hWndPrevious,sClassName.GetBuffer(512),512);
				sClassName.ReleaseBuffer();
				if(sClassName==lpClassName){
					clsFlag = TRUE;
				}else{
					clsFlag = FALSE;
				}
				
			}

			BOOL wnFlag = TRUE;
			if(lpWindowName){
				CString sWindowName="";
				::GetWindowText(hWndPrevious,sWindowName.GetBuffer(512),512);
				sWindowName.ReleaseBuffer();

				if(sWindowName==lpWindowName){
					wnFlag = TRUE;
				}else{
					wnFlag = FALSE;
				}
				
			}
			
			if(clsFlag && wnFlag)
			{
				return hWndPrevious;
				//break;
			}
		}
		hWndPrevious = ::GetWindow(hWndPrevious,GW_HWNDNEXT);
	}

	return 0;
}

DWORD GetProcessID(CString szProcessName){
	HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(snapshot,&pe);
	//char ch[] = "UsbExplore.exe";
	CString szExeFile;
	CString szProcess = szProcessName;
	szProcess.MakeLower();

	do
	{
		szExeFile = pe.szExeFile;
		szExeFile.MakeLower();

		if(szExeFile==szProcess){
			return pe.th32ProcessID;
			DWORD dwVersion = GetVersion();
			if(dwVersion < 0x80000000){
			}else{//9x OS
			}

		}
	}while(Process32Next(snapshot,&pe));

	return 0;
}


typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;


BOOL IsWow64(){
	BOOL bIsWow64 = FALSE;
	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function  
	//and GetProcAddress to get a pointer to the function if available.      
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
	if(NULL != fnIsWow64Process){        
		if(!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)){
			return FALSE;
		}
	}
	return bIsWow64; 
}

typedef UINT (WINAPI* GETSYSTEMWOW64DIRECTORY)(LPTSTR, UINT);

BOOL GetSystemWow64Path(char* sPath,int len)//ERROR?
{
	memset(sPath,0,len);
	GETSYSTEMWOW64DIRECTORY getSystemWow64Directory;
	HMODULE hKernel32;
	TCHAR Wow64Directory[MAX_PATH];
	hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
	if(hKernel32 == NULL){
		// // This shouldn't happen, but if we can't get 
		// kernel32's module handle then assume we are 
		//on x86. We won't ever install 32-bit drivers 
		// on 64-bit machines, we just want to catch it 
		// up front to give users a better error message. 
		return FALSE; 
	}
	getSystemWow64Directory = (GETSYSTEMWOW64DIRECTORY)GetProcAddress(hKernel32, "GetSystemWow64DirectoryW");
	if(getSystemWow64Directory == NULL){
		// // This most likely means we are running
		// on Windows 2000, which didn't have this API 
		// and didn't have a 64-bit counterpart. // 
		return FALSE;
	}
	if((getSystemWow64Directory(Wow64Directory, sizeof(Wow64Directory)/sizeof(TCHAR)) == 0) && (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)){
		return FALSE;
	}
	memcpy(sPath,Wow64Directory,len);
	return TRUE;
}



//typedef BOOL (__stdcall *ISWOW64PROCESS)(HANDLE hProcess, PBOOL Wow64Process);
typedef BOOL (__stdcall *WOW64ENABLEWOW64FSREDIRECTION)(BOOL Wow64FsEnableRedirection);
/*
PGET_SYSTEM_WOW64_DIRECTORY_A pGetSystemWow64DirectoryA;
ISWOW64PROCESS pIsWow64Process;
*/
WOW64ENABLEWOW64FSREDIRECTION pWow64EnableWow64FsRedirection = 0;

void ResolveWow64References(void)
{
       HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
       if (hKernel32)
       {
              //pGetSystemWow64DirectoryA = (PGET_SYSTEM_WOW64_DIRECTORY_A) GetProcAddress(hKernel32, GET_SYSTEM_WOW64_DIRECTORY_NAME_A_A);
              //pIsWow64Process = (ISWOW64PROCESS) GetProcAddress(hKernel32, "IsWow64Process");
              pWow64EnableWow64FsRedirection = (WOW64ENABLEWOW64FSREDIRECTION) GetProcAddress(hKernel32, "Wow64EnableWow64FsRedirection");
		if(pWow64EnableWow64FsRedirection)
			  pWow64EnableWow64FsRedirection(FALSE);
	   }
}


//BOOL GetWindowsSystemDirectory(char* sPath,int len){
	/*
	if(IsWow64()){
		memset(sPath,0,len);
		char windir[MAX_PATH];
		::GetWindowsDirectory(windir,MAX_PATH);
		sprintf(sPath,"%s\\%s",windir,"SysWOW64");
		//ResolveWow64References();

		//sprintf(sPath,"%s","c:\\");

		return TRUE;
	}else{
		::GetSystemDirectory(sPath,len);
		return TRUE;
	}*/
//			::GetSystemDirectory(sPath,len);
//		return TRUE;

//}


BOOL ChangeWindowMessage(UINT uMessageID, BOOL bAllow)//Vista/win7/win8
{
	BOOL bResult=FALSE;
	HMODULE hUserMod=NULL;
	//vista and later
	hUserMod=LoadLibrary("user32.dll");
	if(NULL==hUserMod){
		return FALSE;
	}
	_ChangeWindowMessageFilter pChangeWindowMessageFilter=(_ChangeWindowMessageFilter)GetProcAddress(hUserMod,"ChangeWindowMessageFilter");
	if(NULL==pChangeWindowMessageFilter){
		FreeLibrary(hUserMod);
		return FALSE;
	}
	bResult=pChangeWindowMessageFilter(uMessageID, bAllow?1:2);//MSGFLT_ADD: 1, MSGFLT_REMOVE: 2
	if(NULL!=hUserMod){
		FreeLibrary(hUserMod);
	}
	return bResult;
}