
#pragma once

#include "string.h"
#include "string"
#include<winsock2.h>
using namespace std;
class CClientDlg;

class ClientCon
{
public:
	ClientCon(CClientDlg *dlg);
	~ClientCon(void);
	void StartConnect(string sAddress, int iPort, string sUsername, string sPassword);
	void SendData(string sMessage);
	void Logout(string sMessage);
	void SendDataPrivately(string sMessage);
	void FileSend(SOCKET FileSendSocket,char *FilePath);
	void FileReceive(char *FilePath, char* recvbuf, int recvbuflen);

	WSADATA wsa;
    SOCKET s;
	CClientDlg *m_pClient;
	string m_pUser, m_pPassword;

};

