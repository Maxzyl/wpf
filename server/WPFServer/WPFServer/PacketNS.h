#ifndef PACKETNS_H
#define PACKETNS_H
#endif

namespace PACKET_INFO
{
	/*
	Recv: "Login:{username}| Password:{password}|"
	Send: "{Accepted/Denied} {if Accepted, Admin/Manager/Employee}~"
	*/
	#define LOGIN_REQUEST(client) ((client)->getRecvBuf().length()>6) && ((client)->getRecvBuf()[0]=='L') && ((client)->getRecvBuf()[1]=='o') && ((client)->getRecvBuf()[2]=='g') && ((client)->getRecvBuf()[3]=='i') && ((client)->getRecvBuf()[4]=='n') && ((client)->getRecvBuf()[5]==':')
	/*
	Recv: "EmployeeList: Login:{username}|"
	Send: "({rowLength},{colLength}),{row1col1},{row1col2},{row2col1},{row2col2}"
	*/
	#define EMPLOYEELIST_REQUEST(client) ((client)->getRecvBuf().length()>13) && ((client)->getRecvBuf()[0]=='E') && ((client)->getRecvBuf()[8]=='L') && ((client)->getRecvBuf()[12]==':')
}