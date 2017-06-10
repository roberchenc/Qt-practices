#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Kernel32.lib")    // IOCP需要用到的动态链接库

#include <stdio.h>
#include <winsock2.h>
#include <MSWSock.h>
//#include <windows.h>

#include "Server.h"

SOCKET g_cmdListenSocket;
SOCKET g_fileListenSocket;
HANDLE g_stdout_mutex;

LPFN_ACCEPTEX lpfnAcceptEx; //AcceptEx函数指针
LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAddrs;
LPFN_TRANSMITFILE lpfnTransmitFile;

DWORD WINAPI ServerWorkThread(LPVOID IpParam);
int processDataPort(HANDLE CompletionPort, DWORD BytesTransferred, LPPER_IO_DATA lpPerIOData);

int PostAcceptEx(short int port, LPPER_IO_DATA lpAcceptIOData);
SOCKET CreateServerSocket(const char* pszIP, short int cmdPort, short int fileProt, HANDLE completionPort);