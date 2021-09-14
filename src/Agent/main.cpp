#include "stdafx.h"

int _tmain(int nArgc, LPTSTR pszArgv[])
{
    core::CSyncTCPSocket client;
    client.Connect("127.0.0.1", 15000, 5000);

    char szMsg[] = "Hello!!";
    client.Send(szMsg, sizeof(szMsg), 5000, NULL);
    Sleep(1000);
    client.Send(szMsg, sizeof(szMsg), 5000, NULL);

    Sleep(1000);
    client.Send(szMsg, sizeof(szMsg), 5000, NULL);

    Sleep(1000);
    client.Send(szMsg, sizeof(szMsg), 5000, NULL);
    
    client.Close();
    return 0;
}