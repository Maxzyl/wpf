using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace TCPns
{
    static class TCP
    {
        public static Task<bool> ConnectTask(SocketObject client){
            return Task.Factory.StartNew(() =>
            {
                try{
                    client.socket.Connect(client.iep);
                }catch (SocketException ex){
                    Console.WriteLine("Connect() exception triggered: {0}", ex);
                    return false;
                }
                return true;
            });
        }

        public static Task<int> ReceiveTask(SocketObject client)
        {
            return Task.Factory.StartNew(() =>
            {
                int result = -1;
                try{
                    result = client.socket.Receive(client.buffer);
                }catch (SocketException ex){
                    Console.WriteLine("Receive() exception triggered: {0}", ex);
                }
                return result;
            });
        }

        public static async Task ReceiveAllPacketsSync(SocketObject client)
        {
            bool completePacketReceived = false;

            // delete any previously recveived complete packet
            client.packet = "";

            // loop through all partial packets to make complete packet
            while (!completePacketReceived)
            {
                Console.WriteLine("Receiving...");
                
                // recv partial packet
                int recvBytes = await ReceiveTask(client);

                Console.WriteLine("Received partial packet.");

                // check if connection is still alive
                if (recvBytes == -1) { throw new SocketException(); }

                // add partial packet to string
                client.packet += Encoding.ASCII.GetString(client.buffer);

                // check if partial packet made complete packet
                if (client.buffer[recvBytes - 3] == 126) // packet received = buffer + NULL TERMINATOR (0) + NULL TERMINATOR (0)
                {
                    completePacketReceived = true;
                }

                // reset buffer
                client.buffer = new byte[SocketObject.BufferSize];
            }
        }

        public static Task<int> SendTask(SocketObject client, byte[] msg)
        {
            return Task.Factory.StartNew(() =>
            {
                int result = -1;
                try
                {
                    result = client.socket.Send(msg);
                    Console.WriteLine("Sent: {0}", Encoding.ASCII.GetString(msg));
                }
                catch (SocketException ex)
                {
                    Console.WriteLine("Send() exception triggered: {0}", ex);
                }
                return result;
            });
        }

    }

    public class SocketObject
    {
        public IPEndPoint iep;
        public Socket socket = null;
        public const int BufferSize = 32;
        public byte[] buffer = new byte[BufferSize];
        public string packet = "";

        public SocketObject(Socket sock, IPEndPoint IP){
            socket = sock;
            iep = IP;
        }
    }



}
