#include "Client.h"

Client::Client(): TCPSocketObj(INVALID_SOCKET), clientAddrLen(sizeof(clientAddr)), resultSet(NULL), resultSetRowLen(NULL), resultSetColLen(NULL){}

Client::Client(SOCKET sock): TCPSocketObj(sock), clientAddrLen(sizeof(clientAddr)), resultSet(NULL), resultSetRowLen(NULL), resultSetColLen(NULL){}

Client::~Client(){

	//close socket
	if(TCPSocketObj!=INVALID_SOCKET)
		closesocket(TCPSocketObj);

	//delete result set
	if(resultSet){
		for(int i=0; i<resultSetRowLen; i++){
			delete[] resultSet[i];
		}
		delete[] resultSet;
		resultSet = NULL;
	}
}

/* Prepare winsock for socket use */
void Client::initialize_winsock(WSADATA& wsaData){

	/* Set winsock2 version */
	if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }

}

/* Clear recv buffer */
void Client::clearRecvBuf(){

	//clear buffer
	memset(recvBuf,'\0', RECV_BUF_LEN);

}

/* Print recv buffer */
void Client::printRecvBuf() const{

	//print
	printf("Received packet from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	printf("Data: %s \n" , recvBuf);

}

/* Get recv buffer */
std::string Client::getRecvBuf() const{
	return std::string(recvBuf);
}

SOCKET Client::getSocket() const{
	return TCPSocketObj;
}

void Client::setSocket(u_short port){

	// set TCP socket
	if((TCPSocketObj = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

	// set address information
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");// ANY
    serverAddr.sin_port = htons( port );

}

/* Bind socket to TCP port */
void Client::bindSocket(){

	// bind socket to addr
	if( bind(TCPSocketObj ,(struct sockaddr *)&serverAddr , sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }

}

/* Listen on TCP port */
void Client::listenSocket(){

	int iResult = listen(TCPSocketObj, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(TCPSocketObj);
        WSACleanup();
    }

}

/* Accept connection with desired blocking-type on TCP port */
Client* Client::acceptSocket(int blockingType){

	Client* returnClientSocket = new Client(accept(TCPSocketObj, NULL, NULL));

	// if accept failed (ignore blocking asnyc accept() error)
    if ((returnClientSocket->getSocket() == INVALID_SOCKET) && (WSAGetLastError()!=WSAEWOULDBLOCK)) {
        printf("accept failed with error: %d\n", WSAGetLastError());
    }else{
		// set blocking type for accepted socket
		setBlockingType(returnClientSocket->getSocket(), blockingType);
	}

	return returnClientSocket;
}

/* Recv packet from assigned TCP port */
bool Client::recvFrom(){

	//clear recv buffer
	clearRecvBuf();

	//recv
	int iResult = recv(TCPSocketObj, recvBuf, RECV_BUF_LEN, 0);

	//handle recv
    if (iResult > 0) {
        printf("Bytes successfully received: %d\n", iResult);

		//print recv
		printRecvBuf();
    }else if(WSAGetLastError() == WSAEWOULDBLOCK){
		//async blocked
	}else if((WSAGetLastError() == WSAECONNRESET) || (WSAGetLastError() == WSAECONNABORTED)){
        printf("Connection closing...\n");
		return false;
	}else{
        printf("recv failed with error: %d\n", WSAGetLastError());
		return false;
    }

	return true;
}

/* Send packet to socket addr */
void Client::sendTo(const char* sendBufParam) const{

	//send
	int iResult = send(TCPSocketObj, sendBufParam, (int)(strlen(sendBufParam)+2), 0);

	if (iResult > 0){
		printf("Successfully sent: %s (%d Bytes)\n", sendBufParam, iResult);
	}else if(WSAGetLastError() == WSAEWOULDBLOCK){
		//async blocked
	}else if(WSAGetLastError() == WSAECONNRESET){
		//ignore since recv() will detect closed connections
	}else{
		printf("send() failed with error code : %d" , WSAGetLastError());
	}

}

/* Set blocking type - Non-blocking(1) or blocking(0) */
bool Client::setBlockingType(SOCKET sock, int type) const{

	//set
	if(type==1){
		u_long mode = 1;
		ioctlsocket(sock, FIONBIO, &mode);
	}else if(type==0){
		u_long mode = 0;
		ioctlsocket(sock, FIONBIO, &mode);
	}else{
		return false;
	}

	//return
	return true;
}

/* Get login name */
std::string Client::getLogin() const{
	return username;
}

/* Set login name */
void Client::setLogin(const char* loginName){
	 username = loginName;
}

/* Check if client has successfully logged into server */
bool Client::isLoggedIn() const{
	bool loggedIn = (username=="") ? (loggedIn=false) : (loggedIn=true);
	return loggedIn;
}

/* Set a SQL query to be executed */
bool Client::SQLExecuteQueryWithResultSet(const char* query){
	
	//delete previous result set data
	if(resultSet){

		//delete result set
		for(int i=0; i<resultSetRowLen; i++){
			delete[] resultSet[i];
		}
		delete[] resultSet;
		resultSet = NULL;

		//reset result set info
		resultSetRowLen = NULL;
		resultSetColLen = NULL;

	}
	
	//execute query with result set
	try{

		//execute query
		cmd.setCommandText(query);
		cmd.Execute();

		//set row and col length
		resultSetColLen = cmd.FieldCount();
		while(cmd.FetchNext()){
			resultSetRowLen++;
		}

		//build result set
		resultSet = new std::string*[resultSetRowLen];
		for(int i=0; i<resultSetRowLen; i++){
			resultSet[i] = new std::string[resultSetColLen];
		}

		//execute query again
		cmd.setCommandText(query);
		cmd.Execute();

		//fill result set
		int tempRowCounter=0;
		while(cmd.FetchNext()){
			for(int i=0; i<cmd.FieldCount(); i++){
				resultSet[tempRowCounter][i] = (const char*)cmd.Field(i+1).asString();
			}
			tempRowCounter++;
		}

		//print result set
		SQLPrintResultSet();

	}catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", (const char*)x.ErrText());
		return false;
    }
	return true;
}

/* Set a SQL query to be executed */
bool Client::SQLExecuteQuery(const char* query){
	
	//execute query
	try{
		cmd.setCommandText(query);
		cmd.Execute();
	}catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", (const char*)x.ErrText());
		return false;
    }
	return true;
}

/* Connect to SQL Server and database */
bool Client::SQLConnect(const char* connectionString, const char* user, const char* pass){

	//attempt connection
	try{
		con.Connect(
            connectionString,
            user,
            pass,
			SA_SQLServer_Client);

		cmd.setConnection(&con);
	}catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", (const char*)x.ErrText());
		return false;
    }
	return true;

}

/* Commit SQL transaction */
bool Client::SQLCommit(){

	//commit SQL transaction since last rollback
	try{
		con.Commit();
	}catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", (const char*)x.ErrText());
		return false;
    }
	return true;

}

/* Rollback SQL transaction */
bool Client::SQLRollback(){

	// rollback SQL transaction
	try
    {
        // on error rollback changes
        con.Rollback();
    }
    catch(SAException &)
    {
		return false;
    }
	return true;

}

/* Get result set */
std::string** Client::SQLGetResultSet(){
	return resultSet;
}

/* Get result set column length */
int Client::getResultSetColLen() const{
	return resultSetColLen;
}

/* Get result set row length */
int Client::getResultSetRowLen() const{
	return resultSetRowLen;
}

/* Disconnect SQL transaction */
bool Client::SQLDisconnect(){

	//commit SQL transaction since last rollback
	try{
		con.Disconnect();
	}catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message
        printf("%s\n", (const char*)x.ErrText());
		return false;
    }
	return true;

}

/* Package SQL result set for network transmission */
std::string Client::SQLgetPackagedResultSet() const{

	std::string tempStr="";

	//add header
	tempStr+="RS:";

	//add result set size information
	tempStr+="(";
	tempStr+=std::to_string((long double)resultSetRowLen);
	tempStr+=",";
	tempStr+=std::to_string((long double)resultSetColLen);
	tempStr+=")";

	//add
	for(int i=0; i<resultSetRowLen; i++){
		tempStr+=',';
		for(int j=0; j<resultSetColLen; j++){
			tempStr+=" |";
			tempStr+=resultSet[i][j];
			tempStr+="| ";
		}
	}

	//add footer
	tempStr+="~";

	return tempStr;

}

/* Print result set to console */
void Client::SQLPrintResultSet() const{

	if(resultSet){

		//print result set
		std::cout << "\n";
		for(int i=0; i<resultSetRowLen; i++){
			for(int j=0; j<resultSetColLen; j++){
				std::cout << "|" << resultSet[i][j] << "|";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

}