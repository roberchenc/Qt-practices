#include "filethread.h"

FileThread::FileThread()
{
    dataClient=0;
    filePath = "";
	ReciveOrSend = 0;
	fileSize = 0;
}

FileThread::FileThread(QString file, int fsize)
{
    filePath = file;
	ReciveOrSend = 0;
	fileSize = fsize;
}

void FileThread::GetSock(SOCKET client)
{
    dataClient = client;
}

void FileThread::SetRecvFlag(int recv)
{
	ReciveOrSend = recv;
}

void FileThread::ReciveFile(QString filePath)
{
	int recvSize=0;
	char buffer[5120];
	FILE *fp = fopen(filePath.toStdString().c_str(), "wb");
	if (fp == NULL)
	{	
		exit(1);
	}

	// 从服务器端接收数据到buffer中   
	memset(buffer, '\0', sizeof(buffer));
	int length = 0;
	while ((length = recv(dataClient, buffer, 4096, 0))>0)
	{
		if (length <= 0)
		{
			break;
		}
		int fileSize_write = fwrite(buffer, sizeof(char), length, fp);
		if (fileSize_write < length)
		{
			break;
		}
		if (length > 0)
		{
			recvSize += length;
			emit SendProgressBarLen(length);
		}
		memset(buffer, '\0', 4096);
		length = 0;
	}
	fclose(fp);
	if(fileSize==recvSize)
		emit ReciveFileSuccess(FILE_RECV);
}

void FileThread::SendFile()
{
	char buffer[4096];
	int sendSize = 0;
	FILE *file = fopen(filePath.toStdString().c_str(), "rb");
	if (NULL == file)
	{
		int ret = GetLastError();
		exit(0);
	}
	else
	{
		memset(buffer, '\0', 4096);
		int length = 0;
		while ((length = fread(buffer, sizeof(char), 4096, file)))
		{
			int ret = GetLastError();
			if (send(dataClient, buffer, length, 0)<=0)
			{
				break;
			}
			if (length > 0)
			{
				emit SendProgressBarLen(length);
				sendSize += length;
			}
			length = 0;
		}
		fclose(file);
	}
	if (fileSize == sendSize)
		emit SendFileSuccess(FILE_SEND);
}

void FileThread::run()
{
    if(ReciveOrSend == FILE_RECV)
	{
		if(filePath != "")
			ReciveFile(filePath);
	}
	else 
	{
		SendFile();
	}
	exit(1);
}
