

#include "StdAfx.h"
#include "ServerManager.h"
#include "Server.h"
#include "ServerDlg.h"
#include <Windows.h>
#include<winsock2.h>
#include<WS2tcpip.h>

const int Max = 1000000;

	static SOCKET sArray[100];
	static int iCount; 
	CServerDlg* xm_pDialog;
	string **user = new string* [10];
	int cl_private_1 = 0, cl_private_2 = 0;

ServerManager::ServerManager(CServerDlg* dialog)
{
	m_pDialog = dialog;
	xm_pDialog = dialog;
}


ServerManager::~ServerManager()
{
	
	closesocket(s);
    WSACleanup();
}

void ServerManager::ClearServer()
{
	closesocket(s);
    WSACleanup();

	/*
	for(int i=1;i<=iCount;++i)
	{
		DWORD dwCode;  
        GetExitCodeThread(cpTh[i]->m_hThread, &dwCode);  
        delete cpTh[i];
		//CloseHandle(m_Thread_handle[i]);
	}*/
}

void ServerManager::StartListening(int iPort)
{
	iCount=0;
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
		m_pDialog->ShowServerInfo("Could not create socket");
    }
 
    printf("Socket created.\n");
    //m_pDialog->ShowServerInfo("Socket created. \n"); // TEST SERVER SHOW MESSAGE IN BIALOG BOX

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( iPort );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
		m_pDialog->ShowServerInfo("Bind failed with error code");
        exit(EXIT_FAILURE);
    }
     
    puts("Bind done");
	//m_pDialog->ShowServerInfo("Bind done.\n"); // TEST SERVER SHOW MESSAGE IN BIALOG BOX

    //Listen to incoming connections
    listen(s , 100);
	// char *message;
	 puts("Waiting for incoming connections...");
     m_pDialog->ShowServerInfo("Waiting for incoming connections...\n");
     c = sizeof(struct sockaddr_in);
     
    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
        puts("Connection accepted");
        //m_pDialog->ShowServerInfo("Connection accepted .\n");	// TEST SERVER SHOW MESSAGE IN BIALOG BOX
        //Reply to the client
		socklen_t len;
		struct sockaddr_storage addr;
		char ipstr[INET6_ADDRSTRLEN];
		int port;

		len = sizeof addr;
		getpeername(new_socket, (struct sockaddr*)&addr, &len);

		// deal with IPv4:
		if (addr.ss_family == AF_INET) {
			struct sockaddr_in *s = (struct sockaddr_in *)&addr;
			port = ntohs(s->sin_port);
			inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
		}

		printf("Peer IP address: %s\n", ipstr);
		//m_pDialog->ShowServerInfo("Connected Peer IP address: "+string(ipstr)+"\n");
		CWinThread *cTh = AfxBeginThread( DataThreadFunc, (LPVOID)new_socket );
		++iCount;
		//m_Thread_handle[++iCount] = cTh->m_hThread;
		//cpTh[iCount] = cTh;
		sArray[iCount] = new_socket;
		//message = "Hello Client , I have received your connection.\n";
        //send(new_socket , message , strlen(message) , 0);
		
		//SetStaticVariable(iTempCount, new_socket);

		// create array save username and password
		user[iCount] = new string[2];
    }
     
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
		m_pDialog->ShowServerInfo("Accept failed .\n");
		m_pDialog->ShowServerInfo("Server died .\n");
        return;
    }
}

UINT __cdecl ServerManager::DataThreadFunc(LPVOID pParam)
{
	SOCKET pYourSocket = reinterpret_cast<SOCKET>(pParam);
    //UINT retCode = pYourClass->ThreadFunc();
	//SendReceiveData(pYourClass);
	
	char server_reply[2000], cl_password[100], cl_name[100];
    int recv_size, cl_password_size, cl_name_size;

	// Recieve User name from clien
	cl_name_size = recv(pYourSocket , cl_name , 100 , 0);
	while (cl_name_size > 0)
	{
		if (cl_name_size  < 0){
			//puts("Recieve failed \n");
			xm_pDialog->ShowServerInfo("Recieve failed \n");
		}
		else{
			//printf("cl_password_size = %d \n", cl_password_size);
			cl_name[cl_name_size] = '\0';
			//xm_pDialog->ShowServerInfo("client_name : "+ string(cl_name) + "\n");	// TEST SERVER SHOW MESSAGE IN BIALOG BOX
			// save username and password into string array to compare
			user[iCount][0] = string(cl_name);
		}
		cl_name_size-=Max;
	}
	
	// Recieve Password from clien
	cl_password_size = recv(pYourSocket , cl_password , 100 , 0);
	while (cl_password_size > 0)
	{
		if (cl_password_size  < 0){
			xm_pDialog->ShowServerInfo("Recieve failed \n");
		}
		else{
			cl_password[cl_password_size] = '\0';
			//xm_pDialog->ShowServerInfo("client_password : "+ string(cl_password) + "\n");	// TEST SERVER SHOW MESSAGE IN BIALOG BOX
			// save username and password into string array to compare
			user[iCount][1] = string(cl_password);
		}
		cl_password_size-=Max;
	}


	if (iCount > 1)
	{
		// check user name
		for (int j = iCount; j > 1 ; j--)
		{
			if ( (user[iCount][0].compare(user[j-1][0])) != 0 )
			{
				printf("ok");
			}
			else
			{
				xm_pDialog->ShowServerInfo("Username is duplicate. \n");
				user[iCount][1] = "Notok" ;
				char *message2;
				message2 = "Username is duplicate. Please change your Username !\n";
				send(pYourSocket , message2 , strlen(message2) , 0);
				break;
				//return 0;
			}
		}
	}

	if ( (user[iCount][1].compare("Notok")) != 0 )
	{
		xm_pDialog->ShowServerInfo("Connection accepted .\n");	// TEST SERVER SHOW MESSAGE IN BIALOG BOX
		xm_pDialog->ShowServerInfo("client_name : "+ string(cl_name) + "\n");	// TEST SERVER SHOW MESSAGE IN BIALOG BOX
		//xm_pDialog->ShowServerInfo("client_password : "+ string(cl_password) + "\n");	// TEST SERVER SHOW MESSAGE IN BIALOG BOX
		// Accept after check user name
		char *message;
		message = "Welcome to Matrix chat room.\n";
		send(pYourSocket , message , strlen(message) , 0);

		// Data transmission between clients
		while((recv_size = recv(pYourSocket , server_reply , 2000 , 0)) != SOCKET_ERROR)
		{
			// seperate recieve string
			string xtemp_client = string(server_reply);	//true
			size_t pos = xtemp_client.find("/");
			string xprivate  = xtemp_client.substr(0, pos);

			server_reply[recv_size] = '\0';
			xm_pDialog->ShowServerInfo("Message Received: "+ string(server_reply) + "\n");	// TEST SERVER SHOW MESSAGE IN BIALOG BOX

			//xm_pDialog->ShowServerInfo("Message Received: "+ xprivate + "\n");

			if ( (xprivate.compare("xprivate")) == 0 )
			{
				if (cl_private_1 == 0 && cl_private_2 == 0)
				{
					// Find Thread number
					size_t pos1 = xtemp_client.find("//");
					size_t pos3 = xtemp_client.find("///");
					string xtemp2 = xtemp_client.substr (pos1+2, pos3-(pos1+2));	// username( is talked privately )
					string xtemp1 = xtemp_client.substr(pos+1, pos1-(1+pos)); // username( who want to talk privately)
					
					//xm_pDialog->ShowServerInfo("Message Received: "+ xtemp2 + "\n");
					//xm_pDialog->ShowServerInfo("Message Received: "+ xtemp1 + "\n");

					for (int k = 1; k <= iCount; k++)
					{
						if ( (xtemp2.compare(user[k][0])) == 0  )
						{
							printf("%d", k);	// iCount which is talked privately
							cl_private_2 = k;
						}
					}

					for (int l = 1; l <= iCount; l++)
					{
						if ( (xtemp1.compare(user[l][0])) == 0 )
						{
							printf("%d", l);	// username( who want to talk privately)
							cl_private_1 = l;
						}
					}
					// to_string(cl_private_1)
					xm_pDialog->ShowServerInfo("Message Received: "+ user[cl_private_1][0] + " want to talk privately with " + user[cl_private_2][0] + "\n");

				}
				else
				{
					// private chat start.
					// send data to client 2
					if( send(sArray[cl_private_2] , server_reply, recv_size , 0) < 0)
					{
						puts("Send failed");
						xm_pDialog->ShowServerInfo("Send privately failed \n");
						//return -1;
					}
					if( send(sArray[cl_private_1] , server_reply, recv_size , 0) < 0)
					{
						puts("Send failed");
						xm_pDialog->ShowServerInfo("Send privately failed \n");
						//return -1;
					}
				}
				
			}
			else
			{
				for(int i = 1;i<=iCount; i++)
				{
					if( send(sArray[i] , server_reply, recv_size , 0) < 0)
					{
						puts("Send failed");
						//return -1;
					}
				}
			}

		}
	}
	else
	{
		iCount--;
	}

    return 0;
}

UINT ServerManager::SendReceiveData(SOCKET cSocket)
{
	return 0;
}

void ServerManager::SetStaticVariable(int iC, SOCKET cS)
{
	iCount = iC;
	sArray[iCount] = cS;
}

void ServerManager::CloseConnect()
{
	string xmessage = "The server has closed the connection ! \n";
	for(int i = 1;i<=iCount; i++)
	{
		if( send(sArray[i] , xmessage.c_str(), xmessage.size() , 0) < 0)
		{
			puts("Send failed");
			return;
		}
	}
}