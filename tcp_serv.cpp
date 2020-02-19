// Module Name: callback.cpp
//
// Description:
//
//    This sample illustrates how to develop a simple echo server Winsock
//    application using the Overlapped I/O model with callback routines.
//    This sample is implemented as a console-style application and simply prints
//    messages when connections are established and removed from the server.
//    The application listens for TCP connections on port 5150 and accepts them
//    as they arrive. When this application receives data from a client, it
//    simply echos (this is why we call it an echo server) the data back in
//    it's original form until the client closes the connection.
//
// Compile:
//
//    cl -o callback callback.cpp ws2_32.lib
//
// Command Line Options:
//
//    callback.exe
//
//    Note: There are no command line options for this sample.

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "session.h"
#include <qDebug>
#include <fstream>

extern struct sessionVariables globals;

#define PORT globals.port
#define DATA_BUFSIZE 65000000

int total_data_read;
ofstream myfile;
SOCKET AcceptSocket;

typedef struct _SOCKET_INFORMATION {
    OVERLAPPED Overlapped;
    SOCKET Socket;
    CHAR Buffer[DATA_BUFSIZE];
    WSABUF DataBuf;
    DWORD BytesSEND;
    DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

void CALLBACK WorkerRoutine(DWORD Error, DWORD BytesTransferred,LPWSAOVERLAPPED Overlapped, DWORD InFlags);
DWORD WINAPI WorkerThread(LPVOID lpParameter);

void tcp_serv()
{

    WSADATA wsaData;
    SOCKET ListenSocket;
    SOCKADDR_IN InternetAddr;
    INT Ret;
    HANDLE ThreadHandle;
    DWORD ThreadId;
    WSAEVENT AcceptEvent;

    myfile.open ("tcp_server.txt", std::ofstream::trunc);

    if ((Ret = WSAStartup(0x0202,&wsaData)) != 0)
    {
       qDebug("WSAStartup failed with error %d\n", Ret);
       WSACleanup();
       return;
    }

    if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
      printf("Failed to get a socket %d\n", WSAGetLastError());
      return;
    }

    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(PORT);

    if (bind(ListenSocket, (PSOCKADDR) &InternetAddr,
        sizeof(InternetAddr)) == SOCKET_ERROR)
    {
        printf("bind() failed with error %d\n", WSAGetLastError());
        return;
    }

    if (listen(ListenSocket, 5))
    {
        printf("listen() failed with error %d\n", WSAGetLastError());
        return;
    }

    if ((AcceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
        return;
    }

    // Create a worker thread to service completed I/O requests.

    if ((ThreadHandle = CreateThread(NULL, 0, WorkerThread, (LPVOID) AcceptEvent, 0, &ThreadId)) == NULL)
    {
        printf("CreateThread failed with error %d\n", GetLastError());
        return;
    }


      AcceptSocket = accept(ListenSocket, NULL, NULL);

      if (WSASetEvent(AcceptEvent) == FALSE)
      {
        printf("WSASetEvent failed with error %d\n", WSAGetLastError());
        return;
      }


}

DWORD WINAPI WorkerThread(LPVOID lpParameter)
{
    DWORD Flags;
    LPSOCKET_INFORMATION SocketInfo;
    WSAEVENT EventArray[1];
    DWORD Index;
    DWORD RecvBytes;
    // Save the accept event in the event array.

    EventArray[0] = (WSAEVENT) lpParameter;

    while(TRUE)
    {
      // Wait for accept() to signal an event and also process WorkerRoutine() returns.

      while(TRUE)
      {
         Index = WSAWaitForMultipleEvents(1, EventArray, FALSE, WSA_INFINITE, TRUE);

         if (Index == WSA_WAIT_FAILED)
         {
            qDebug("WSAWaitForMultipleEvents failed with error %d\n", WSAGetLastError());
            return FALSE;
         }

         if (Index != WAIT_IO_COMPLETION)
         {
            // An accept() call event is ready - break the wait loop
            break;
         }
      }

      WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

      // Create a socket information structure to associate with the accepted socket.

      if ((SocketInfo = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR,
         sizeof(SOCKET_INFORMATION))) == NULL)
      {
         printf("GlobalAlloc() failed with error %d\n", GetLastError());
         return FALSE;
      }

      // Fill in the details of our accepted socket.

      SocketInfo->Socket = AcceptSocket;
      ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));
      SocketInfo->BytesSEND = 0;
      SocketInfo->BytesRECV = 0;
      SocketInfo->DataBuf.len = DATA_BUFSIZE;
      SocketInfo->DataBuf.buf = SocketInfo->Buffer;

      Flags = 0;
      if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf),
                  1, &RecvBytes, &Flags, &(SocketInfo->Overlapped), WorkerRoutine) == SOCKET_ERROR)
      {
         if (WSAGetLastError() != WSA_IO_PENDING)
         {
            printf("WSARecv() failed with error %d\n", WSAGetLastError());
            return FALSE;
         }
      }



    }

    return TRUE;
}

void CALLBACK WorkerRoutine(DWORD Error, DWORD BytesTransferred,
   LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
   DWORD SendBytes, RecvBytes;
   DWORD Flags;

   // Reference the WSAOVERLAPPED structure as a SOCKET_INFORMATION structure
   LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION) Overlapped;
   myfile << SI->Buffer;
   if (Error != 0)
   {
     printf("I/O operation failed with error %d\n", Error);
   }

   if (BytesTransferred == 0)
   {
      printf("Closing socket %d\n", SI->Socket);
   }

   if (Error != 0 || BytesTransferred == 0)
   {
      closesocket(SI->Socket);
      GlobalFree(SI);
      return;
   }

   // Check to see if the BytesRECV field equals zero. If this is so, then
   // this means a WSARecv call just completed so update the BytesRECV field
   // with the BytesTransferred value from the completed WSARecv() call.

   if (SI->BytesRECV == 0)
   {
      SI->BytesRECV = BytesTransferred;
      SI->BytesSEND = 0;

   }
   else
   {
      SI->BytesSEND += BytesTransferred;
   }


   total_data_read += SI->BytesRECV;

   globals.app->set_total_data_trasmitted(total_data_read);

      SI->BytesRECV = 0;

      // Now that there are no more bytes to send post another WSARecv() request.

      Flags = 0;
      ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));

      SI->DataBuf.len = DATA_BUFSIZE;
      SI->DataBuf.buf = SI->Buffer;

      if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags,
         &(SI->Overlapped), WorkerRoutine) == SOCKET_ERROR)
      {
         if (WSAGetLastError() != WSA_IO_PENDING )
         {
            printf("WSARecv() failed with error %d\n", WSAGetLastError());
            return;
         }
      }


  myfile << SI->Buffer;


}
