using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TCPns;

namespace SharedDataNS{

    public enum AccessLevel { Admin, Manager, Employee };

    public class SharedData
    {
        public string _name { get; set; }
        public AccessLevel _accesslevel { get; set; }
        public SocketObject _clientObject { get; set; }

        public SharedData(string login, AccessLevel accessLevel, SocketObject sock)
        {
            _name = login;
            _accesslevel = accessLevel;
            _clientObject = sock;
        }
    }

};
