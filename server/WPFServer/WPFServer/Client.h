#ifndef CLIENT_H
#define CLIENT_H
#endif

#undef UNICODE
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment (lib, "Ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <SQLAPI.h>
#include "PacketNS.h"

using namespace PACKET_INFO;

#define SERVER_PORT 11002
#define RECV_BUF_LEN 1024
#define SEND_BUF_LEN 1024
#define SQL_SERVER_CONNECTION_STRING "tcp:172.16.1.69,1433@WPF"


class Client{

public:

	// constructors
	Client();
	Client(SOCKET);

	// Winsock initialization functions
	static void initialize_winsock(WSADATA& wsaData);

	// clear buffers
	void clearRecvBuf();

	// print recv buf
	void printRecvBuf() const;

	// get recv buf
	std::string getRecvBuf() const;

	// set socket
	void setSocket(u_short);

	// get socket
	SOCKET getSocket() const;

	// bind
	void bindSocket();

	// listen
	void listenSocket();

	// accept
	Client* acceptSocket(int);

	// recv
	bool recvFrom();

	// send
	void sendTo(const char*) const;

	// set blocking type
	bool setBlockingType(SOCKET, int) const;

	// get and set login
	std::string getLogin() const;
	void setLogin(const char*);

	// check if client has successfully logged into server
	bool isLoggedIn() const;

	// connect to SQL Server and database
	bool SQLConnect(const char*, const char*, const char*);

	// execute SQL query
	bool SQLExecuteQuery(const char*);

	// execute SQL query with result set
	bool SQLExecuteQueryWithResultSet(const char*);

	// commit SQL transaction
	bool SQLCommit();

	// disconnect SQL transaction
	bool SQLDisconnect();

	// rollback SQL transaction
	bool SQLRollback();

	// get SQL result set
	std::string** SQLGetResultSet();

	// get result set lengths
	int getResultSetColLen() const;
	int getResultSetRowLen() const;

	// package SQL result set for network transmission
	std::string SQLgetPackagedResultSet() const;

	// print SQL result set
	void SQLPrintResultSet() const;

	~Client();
	
private:

	std::string** resultSet;
	std::string username;
	int resultSetRowLen, resultSetColLen;
	SOCKET TCPSocketObj;
	SAConnection con;
	SACommand cmd;
	struct sockaddr_in serverAddr, clientAddr;
	char recvBuf[RECV_BUF_LEN];
	int clientAddrLen, recv_buf_len;
};