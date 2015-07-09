#include <iostream>
#include <fstream>
#include <string>
#include "crypt.h"
#include "Server.h"
#include <vector>

int main(){

	//variables
    WSADATA wsaData;
	Client serverSocket;
	std::vector<Client*> clientSockets;




	//seed
	srand(1);


	// winsock
	Client::initialize_winsock(wsaData);

	// set socket address
	serverSocket.setSocket((u_short)SERVER_PORT);

	// bind socket
	serverSocket.bindSocket();

	// listen socket
	serverSocket.listenSocket();

	// set non-blocking socket
	serverSocket.setBlockingType(serverSocket.getSocket(), 1);

	printf("Waiting for clients...\n");

	// infinite loop
	while(true){

		// accept connections
		Client* tempClient = serverSocket.acceptSocket(1);

		if(tempClient->getSocket()!=INVALID_SOCKET){

			printf("Added client! \n");
			clientSockets.push_back(tempClient);

			std::string str = "Connected clients: "+std::to_string((long double)clientSockets.size())+"\n";
			printf(str.c_str());

		}else{
			delete tempClient;
		}

		// loop clients
		for (std::vector<Client*>::iterator it = clientSockets.begin(); it != clientSockets.end();){
			
			////connect to SQL Server
			//if((*it)->SQLConnect(SQL_SERVER_CONNECTION_STRING, "sa", "hbkrko")){
			//	std::cout << "Successfully connected server->SQL Server \n";
			//}else{
			//	std::cout << "Failed to connect server->SQL Server \n";
			//}

			////query 0
			//if((*it)->SQLExecuteQuery("CREATE TABLE Persons (ID int, Name varchar(255))")){
			//	std::cout << "Successful SQL query! \n";
			//}else{
			//	std::cout << "Failed SQL query!  \n";
			//}

			////query 0
			//if((*it)->SQLExecuteQuery("Delete From Persons")){
			//	std::cout << "Successful SQL query! \n";
			//}else{
			//	std::cout << "Failed SQL query!  \n";
			//}

			////query 1
			//if((*it)->SQLExecuteQuery("Insert into Persons(ID, Name) values (1245, 'PeeWee Herman')")){
			//	std::cout << "Successful SQL query! \n";
			//}else{
			//	std::cout << "Failed SQL query!  \n";
			//}

			////query 2 with result set
			//if((*it)->SQLExecuteQueryWithResultSet("Select * from Persons")){
			//	std::cout << "Successful SQL query! \n";
			//}else{
			//	std::cout << "Failed SQL query!  \n";
			//}

			//(*it)->sendTo(std::string((*it)->SQLgetPackagedResultSet()).c_str());
			//printf("Sending SQL result set! \n");

			////commit
			//if((*it)->SQLCommit()){
			//	std::cout << "Successful SQL commit! \n";
			//}else{
			//	std::cout << "Failed SQL commit!  \n";
			//}

			////disconnect
			//if((*it)->SQLDisconnect()){
			//	std::cout << "Successfully disconnected from SQL Server! \n";
			//}else{
			//	std::cout << "Failed to disconnect from SQL Server!  \n";
			//}

			// recv, and delete client if disconnected
			if(!(*it)->recvFrom()){
				it = clientSockets.erase(it);
				std::string str = "Connected clients: "+std::to_string((long double)clientSockets.size())+"\n";
				printf(str.c_str());
			}else{
				// process packet
				Server::processPacket((*it));

				// progress pointer
				++it;
			}

		}

		Sleep(500);

	}



	//cleanup
    WSACleanup();



	char* salt = new char[SALT_LENGTH];
	char* hash = new char[HASH_LENGTH];
	char* password = "password123";
	char* attemptedPassword = "password123";

	//crypt
	Crypt(hash, password, salt);
	Print("Crypt",hash,64);

	//decrypt
	bool answer = false;
	answer = Decrypt(attemptedPassword, salt, hash);
	std::cout << answer << std::endl;

	//delete
	delete salt;
	delete hash;





	system("pause");
	return 0;
}