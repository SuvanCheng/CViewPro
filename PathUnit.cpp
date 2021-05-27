#include "stdafx.h"

/*
extern CString GetPathFromFileName(char* lpFileName);
extern CString GetActiveXRegisterPath(CString sCtrlId);
extern CreateDirectoryEx(CString szFilePath);
*/

CString GetPathFromFileName(char* lpFileName){
	CString sPath = "";
	int length = lstrlen(lpFileName);
	for(int i=length-1;i>=0;i--){
		if(lpFileName[i]=='\\' || lpFileName[i]=='/'){
			char* lpBuffer = sPath.GetBufferSetLength(i+1);
			memset(lpBuffer,0,i+1);
			memcpy(lpBuffer,lpFileName,i);
			sPath.ReleaseBuffer();
			break;
		}
	}

	return sPath;
}

void CreateDirectoryEx(CString szFilePath){
	int length = szFilePath.GetLength();
	for(int i=0;i<length;i++){	
		if((char)(szFilePath.GetAt(i))=='\\' || i==(length-1)){
			if((i>0 && (char)(szFilePath.GetAt(i-1))!=':') || i==(length-1)){
				CString szPath;
				if(i==(length-1))
					szPath = szFilePath;
				else
					szPath = szFilePath.Left(i);
				if(szPath.GetLength()>0){
					WIN32_FIND_DATA fd; 
					HANDLE hFind = FindFirstFile(szPath,&fd); 
					if((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){	
						FindClose(hFind);
					}else{
						FindClose(hFind);
						CreateDirectory(szPath,NULL);
					}
				}
			}
		}
	}
}

CString GetActiveXRegisterPath(CString sCtrlId){
	CString sMessage = "";
	CString sPath = "";
	HKEY hKEY;
	CString sSubKey;
	sSubKey.Format("CLSID\\{%s}\\InprocServer32",sCtrlId);
	if(::RegOpenKeyEx(HKEY_CLASSES_ROOT,sSubKey,0,KEY_READ,&hKEY)==ERROR_SUCCESS){
		char sShortPathFile[MAX_PATH];
		memset(sShortPathFile,0,sizeof(sShortPathFile));

		DWORD dwType = REG_SZ;
		DWORD cbData = MAX_PATH;

		if(::RegQueryValueEx(hKEY,NULL,NULL,&dwType,(unsigned char*)sShortPathFile, &cbData)==ERROR_SUCCESS){
			char sFile[MAX_PATH];
			memset(sFile,0,sizeof(sFile));
			GetLongPathName(sShortPathFile,sFile,MAX_PATH);
			sPath = GetPathFromFileName(sFile);
		}
	}
	::RegCloseKey(hKEY);
	return sPath;
}


void MySystemDeleteFile(CString sFile){
	   char szParameters[MAX_PATH + 10];
	   /*
       //设置本进程为实时执行，快速退出。
       SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
       SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
       //通知资源管理器不显示本程序，当然如果程序没有真正的删除，刷新资源管理器后仍会显示出来的。
		SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, _pgmptr, NULL);
      
       //调用cmd传入参数以删除自己
	   */
	   memset(szParameters,0,MAX_PATH + 10);
       sprintf(szParameters, "/c del /q \"%s\"", sFile);
  
		SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = NULL;
        ShExecInfo.lpFile = "cmd";//cmd.c_str();//调用的程序名
        ShExecInfo.lpParameters = szParameters;//par.c_str();//调用程序的命令行参数
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.nShow = SW_HIDE;//窗口状态为隐藏
        ShExecInfo.hInstApp = NULL;
        ShellExecuteEx(&ShExecInfo); //启动新的程序
        WaitForSingleObject(ShExecInfo.hProcess,INFINITE);////等到该进程结束
}

BOOL MyDeleteFile(CString sFile){
	if(DeleteFile(sFile)){
		return TRUE;
	}else{
		MySystemDeleteFile(sFile);
	}
	return TRUE;
}
