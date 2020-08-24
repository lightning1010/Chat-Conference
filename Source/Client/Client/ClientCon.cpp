

#include "StdAfx.h"
#include "ClientCon.h"
#include<stdio.h>

#include <string.h>
#include "ClientDlg.h"
#include <fstream>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library

ClientCon::ClientCon(CClientDlg *dlg)
{
	m_pClient = dlg;
}


ClientCon::~ClientCon(void)
{
	closesocket(s);
	WSACleanup();
}

void ClientCon::StartConnect(string sAddress, int iPort, string sUsername, string sPassword)
{
    struct sockaddr_in server;
    char *message , server_reply[2000];
    int recv_size;
	m_pUser = sUsername;
	m_pPassword = sPassword;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
     
	server.sin_addr.s_addr = inet_addr(sAddress.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( iPort );
 
    //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return;
    }
     
    puts("Connected");
	//m_pClient->ShowServerInfo("\n Connected \n. "); // TEST
	//m_pClient->ShowServerInfo("\n TEST CLIENT SHOW MESSAGE ON DIALOG BOX \n "); // TEST
    //Send some data
    //message = "GET / HTTP/1.1\r\n\r\n";
	//message = "PassWord";

    puts("Data Send\n");
	// Send User Name
	if( send(s , m_pUser.c_str(), m_pUser.size() , 0) < 0){
			//printf("Send failed \n");
			m_pClient->ShowServerInfo("\n Send failed ! \n. ");
	}
	else{
			//m_pClient->ShowServerInfo("\n Sended User name successfully ! \n. ");
	}
	// Waiting a second
	Sleep(100);


	// Send Password
	if( send(s , m_pPassword.c_str(), m_pPassword.size() , 0) < 0){
			//printf("Send failed \n");
			m_pClient->ShowServerInfo("\n Send failed ! \n. ");
	}
	else{
			//m_pClient->ShowServerInfo("\n Sended Password successfully ! \n. ");
	}

    //Receive a reply from the server
    while((recv_size = recv(s , server_reply , 2000 , 0)) != SOCKET_ERROR)
    { 
		puts("Reply received\n");
		server_reply[recv_size] = '\0';
		// seperate recieve string
		string	xtemp_server = string(server_reply);	//true
		size_t pos = xtemp_server.find("/");
		string	xprivate  = xtemp_server.substr(0, pos);

		if ( (xprivate.compare("xprivate")) == 0 )
		{
			string xtemp_data  = xtemp_server.substr(pos+1);
			string xprivate_data = "\n" + xtemp_data + "\n" ;
			
			m_pClient->ShowPrivateInfo(xprivate_data);
		}
		else
		{
			//Add a NULL terminating character to make it a proper string before printing
			server_reply[recv_size] = '\0';
			puts(server_reply);

			string sTempMsg ="\n"+string(server_reply)+"\n";
			m_pClient->ShowServerInfo(sTempMsg);
		}
    }

	// 
    
}

void ClientCon::SendData(string sMessage)
{
	string sTemp = m_pUser +">>"+ sMessage+"\n";

	if( send(s , sTemp.c_str(), sTemp.size() , 0) < 0)
    {
        puts("Send failed");
        return;
    }
}

void ClientCon::SendDataPrivately(string sMessage)
{
	if( send(s , sMessage.c_str(), sMessage.size() , 0) < 0)
    {
        puts("Send failed");
        return;
    }
}

void ClientCon::Logout(string sMessage)
{
	string sTemp = m_pUser + sMessage ;

	if( send(s , sTemp.c_str(), sTemp.size() , 0) < 0)
    {
        puts("Send failed");
        return;
    }
}

void ClientCon::FileSend(SOCKET FileSendSocket,char *FilePath)
{
	streampos filesize = 0;
    ifstream in(FilePath,ios::binary);
    ZeroMemory( &sendbuf, sendbuflen);

    if(in.is_open())
    {
        while(1)
        {
            in.read(sendbuf,sendbuflen);
            if(in.eof())
            {
                cout << "End of File sending from Client" <<  endl; 
                in.close();
                break;
            }
            else
            {
                send(FileSendSocket,sendbuf,sendbuflen,0);
                ZeroMemory( &sendbuf, sendbuflen);
            }
        }
    }
}