#include "Server.h"


/* Route request from client */
void Server::processPacket(Client* it){

	//route packet for processing
	if(LOGIN_REQUEST(it)){
		//authorize client
		authorizeClient(it);
	}else if(EMPLOYEELIST_REQUEST(it)){
		//send employee list from database
		sendEmployeeList(it);
	}

}

/* Send TCP packet of whether or not requested login was found in SQL database */
void Server::authorizeClient(Client* it){
	std::cout << "CAUGHT ATTEMPTED LOGIN! \n";

	//extract username and password
	bool result = false;
	std::string attemptExtractUsername = it->getRecvBuf();
	std::string attemptExtractPassword = it->getRecvBuf();
	std::string attemptExtractAccessLevel = "";

	//extract username and password from packet
	attemptExtractUsername = attemptExtractUsername.substr(attemptExtractUsername.find("Login:")+6, attemptExtractUsername.length());
	attemptExtractUsername = attemptExtractUsername.substr(0, attemptExtractUsername.find('|'));
	attemptExtractPassword = attemptExtractPassword.substr(attemptExtractPassword.find("Password:")+9, attemptExtractPassword.find("|"));
	attemptExtractPassword = attemptExtractPassword.substr(0, attemptExtractPassword.find('|'));

	//connect to SQL Server
	if((it)->SQLConnect(SQL_SERVER_CONNECTION_STRING, "sa", "hbkrko")){
		std::cout << "Successfully connected server->SQL Server \n";
	}else{
		std::cout << "Failed to connect server->SQL Server \n";
	}

	//query logins
	if((it)->SQLExecuteQueryWithResultSet("Select * from Logins")){
		std::cout << "Successful SQL query! \n";
	}else{
		std::cout << "Failed SQL query!  \n";
	}

	//find login in database
	for(int i=0; i<it->getResultSetRowLen(); i++){
		for(int j=0; j<it->getResultSetColLen(); j++){
			if((j==0) && (it->SQLGetResultSet()[i][j]==attemptExtractUsername) && (it->SQLGetResultSet()[i][j+1]==attemptExtractPassword)){
				std::cout << "FOUND LOGIN! \n";
				attemptExtractAccessLevel = it->SQLGetResultSet()[i][j+2];
				result = true;
			}
			std::cout << "|" << it->SQLGetResultSet()[i][j] << "|\n";
		}
		std::cout << "\n";
	}

	//disconnect
	if((it)->SQLDisconnect()){
		std::cout << "Successfully disconnected from SQL Server! \n";
	}else{
		std::cout << "Failed to disconnect from SQL Server!  \n";
	}

	//send authorization decision
	if(result){
		std::string msg = "Accepted ";
		msg = msg.append(attemptExtractAccessLevel);
		msg = msg.append("~");
		it->sendTo(msg.c_str());
	}else{
		it->sendTo("Denied");
	}
}

/* Send TCP packet of whether or not requested login was found in SQL database */
void Server::sendEmployeeList(Client* it){
	
	std::string sendEmployeeListString = "";
	std::string attemptExtractUsername = it->getRecvBuf();

	//extract username from packet
	attemptExtractUsername = attemptExtractUsername.substr(attemptExtractUsername.find("Login:")+6, attemptExtractUsername.length());
	attemptExtractUsername = attemptExtractUsername.substr(0, attemptExtractUsername.find('|'));

	//set login
	it->setLogin(attemptExtractUsername.c_str());

	//connect to SQL Server
	if((it)->SQLConnect(SQL_SERVER_CONNECTION_STRING, "sa", "hbkrko")){
		std::cout << "Successfully connected server->SQL Server \n";
	}else{
		std::cout << "Failed to connect server->SQL Server \n";
	}

	//query logins
	if((it)->SQLExecuteQueryWithResultSet("Select * from EmployeeList_view")){
		std::cout << "Successful SQL query! \n";
	}else{
		std::cout << "Failed SQL query!  \n";
	}

	//form employee list string
	sendEmployeeListString.append("(");
	sendEmployeeListString.append(std::to_string((long double)it->getResultSetRowLen()));
	sendEmployeeListString.append(",");
	sendEmployeeListString.append(std::to_string((long double)it->getResultSetColLen()));
	sendEmployeeListString.append(")");
	for(int i=0; i<it->getResultSetRowLen(); i++){
		for(int j=0; j<it->getResultSetColLen(); j++){
			sendEmployeeListString.append(",");
			sendEmployeeListString.append(it->SQLGetResultSet()[i][j]);
			sendEmployeeListString.append(",");
		}
	}
	sendEmployeeListString.append("~");

	//disconnect
	if((it)->SQLDisconnect()){
		std::cout << "Successfully disconnected from SQL Server! \n";
	}else{
		std::cout << "Failed to disconnect from SQL Server!  \n";
	}

	//send employee list string
	it->sendTo(sendEmployeeListString.c_str());
}