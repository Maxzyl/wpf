#ifndef SERVER_H
#define SERVER_H
#endif

#include "Client.h"

class Server{

public:

	/* Route request from client */
	static void processPacket(Client*);

private:

	/* Determine whether or not requested login is in SQL database */
	static void authorizeClient(Client*);

	/* Return employee list from SQL database */
	static void sendEmployeeList(Client*);
};