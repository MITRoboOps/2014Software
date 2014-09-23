using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
//! \file TCPServer.cs The TCP server class. Accepts client connections, and creates client handlers.

/* 
 * Contact: Dion - dionlow@mit.edu
 * Revision: 
 * Date: May 26, 2014
 * 
 * Changelog:
 * 
 * 
 * */

namespace spotterGUI
{
    /// <summary>
    /// Description: This is the core TCP/IP server class. 
    /// </summary>
    class TCPServer
    {

        #region Class Variables
        public static readonly object _TCPStaticMethodLock = new object();

        private static TCPServer _tcpServerInst;
        private static int curr_client_attempt = -1; 
        public static TCPServer tcpServerInst
        {
            get
            {
                if (_tcpServerInst == null)
                {
                    _tcpServerInst = new TCPServer();
                    Log.Instance.Write("TCP/IP Server class loaded.", Log.Priority.VERBOSE_DEBUG);
                }
                return _tcpServerInst;
            }
            private set
            {

            }
        }

        private TcpListener tcpListner;
        private Thread listenThread;

        private IPEndPoint IP;
        private static Socket serverSocket;
        private static object serverLock = new object();
        #endregion

        private class ClientObject
        {
            
            public ClientHandler clientInstance;
            public Thread clientThread;
            public int clientID;
        }

        private static List<ClientObject> clientList = new List<ClientObject>();
        
    

        //#region Methods
        /// <summary>
        /// Initialize and start the DSC TCP/IP Server. TCPServer Constuctor
        /// </summary>
        public TCPServer(IPAddress ip, int port) { 
            this.tcpListner = new TcpListener(ip, port);
            this.listenThread = new Thread(new ThreadStart(ListenForClients));
            this.listenThread.Start();

        }

        /// <summary>
        /// Starts a a tcpListener. This thread will listen for client connections. 
        /// </summary>
        private void ListenForClients()
        {
         
            tcpListner.Start();
            Console.WriteLine("Starting TCP/IP Server");
            Console.WriteLine("Local End point is :" + tcpListner.LocalEndpoint);
            Console.WriteLine("Waiting for Connection...");

            while (true)
            {
                //waits for a client to connect to the server
                TcpClient client = this.tcpListner.AcceptTcpClient();
                Console.WriteLine("Connection Accepted from: " + client.Client.LocalEndPoint);

                //thread to handle communication w/ connected client
                Thread clientThread = new Thread(new ParameterizedThreadStart(HandleClientComm));
                clientThread.Start(client);
            }
         

        }

        /// <summary>
        /// This function reads data from the client
        /// </summary>
        /// <param name="client"></param>
        private void HandleClientComm(object client)
        {
            TcpClient tcpClient = (TcpClient)client;
            NetworkStream clientStream = tcpClient.GetStream();

            byte[] byteMessage = new byte[4096]; //TODO: number of bytes adjustments?
            int bytesRead;
              while (true){
                bytesRead = 0;

                try{
                  //blocks until a client sends a message
                  bytesRead = clientStream.Read(byteMessage, 0, 4096);
                }catch{
                  //a socket error has occured
                  break;
                }
                  if (bytesRead == 0){
                  //the client has disconnected from the server
                  break;
                }

                //message has successfully been received
                ASCIIEncoding encoder = new ASCIIEncoding();
                String newMessage = encoder.GetString(byteMessage, 0, bytesRead);

                //converst bytes into string and prints to console
                System.Diagnostics.Debug.WriteLine(newMessage); 
                



                  //TODO: Do Something with newMessage!
                  //Use messages from the Messages.cs class 
                  //newMessage Protocol: 16 characters max including "."
                  // Delimiters: "."
                  // Commands: "name." boa camera returns which camera it is (right, left, front)
                  //           "send.[img_name]." requests images from the boa, may input any image name
                  //


                //Sending Msg back to client
                //NetworkStream clientStream = tcpClient.GetStream();
                //ASCIIEncoding encoder = new ASCIIEncoding();
                //byte[] buffer = encoder.GetBytes("Hello Client!");

                //clientStream.Write(buffer, 0, buffer.Length);
                //clientStream.Flush();
              }

              tcpClient.Close();


        }

        /// <summary>
        /// Initialize and start the DSC TCP/IP Server.
        /// </summary>
        private TCPServer() { }

        /// <summary>
        /// Start the TCP server
        /// </summary>
        /// <param name="ip">IP to bind on</param>
        /// <param name="port">Port to bind on</param>
        public void StartTCPServer(IPAddress ip, int port)
        {
            
            string s = "Starting TCP/IP Server... with IP: " + ip.ToString() + " port" + port.ToString();
            Log.Instance.Write(s, Log.Priority.VERBOSE_DEBUG);
            lock (MainService._statusLock)
                MainService.status.TCPServer = Status.STARTING;
            this.IP = new IPEndPoint(ip, port);
            Thread t = new Thread(new ThreadStart(Start));
            t.Start();
            
        }
        /// <summary>
        /// Stop the TCP/IP server
        /// </summary>
        public void StopTCPServer()
        {

            CloseAllClientConns();
            try
            {
                serverSocket.Close();
                serverSocket.Dispose();
                serverSocket = null;

            }
            catch
            {
                Log.Instance.Write("Unable to stop TCP/IP Server.", Log.Priority.ERROR);
            }
            Log.Instance.Write("TCP/IP Server stopped.", Log.Priority.NOTICE);
        }
        /// <summary>
        /// Main loop for TCP/IP Server. Listens for, and handles, incomming connections.
        /// </summary>
        private void Start()
        {

            try //Lets start listening for clients
            {
                serverSocket = new Socket( SocketType.Stream, ProtocolType.Tcp);
                serverSocket.Blocking = false;
                lock (_TCPStaticMethodLock)
                    serverSocket.Bind(IP);
                serverSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Linger, new LingerOption(true, 2));
                serverSocket.Listen((int)SocketOptionName.MaxConnections);

                //i is number of "new connection" workers. Set low as low number of clients expected.
                //Higher numbers will incur overhead but allow for more concurrent client connection requests.
                for (int i = 0; i < 3; i++) 
                    serverSocket.BeginAccept(
                        new AsyncCallback(NewConnCallback), serverSocket);
                Log.Instance.Write("TCP/IP Server started. " +
                               "Listening on: " + serverSocket.LocalEndPoint.ToString(), Log.Priority.NOTICE);
            }
            catch (Exception e)
            {
                Log.Instance.Write("TCP/IP Server Thread failed to start with exception: " +
                                    e.ToString(), Log.Priority.ERROR);
            }
            
            lock (MainService._statusLock)
                MainService.status.TCPServer = Status.ONLINE;
        }
        /// <summary>
        /// End a client connection
        /// </summary>
        /// <param name="ci">ClientObject to close</param>
        private static void CloseClientConn(ClientObject ci)
        {
            lock (_TCPStaticMethodLock)
            {
//DirectControl.ReleaseDirectControl(ci.clientID);
                try
                {
                    Log.Instance.Write("Closing connection to " + ci.clientInstance.Socket.RemoteEndPoint.ToString(), Log.Priority.DEBUG);
                    ci.clientInstance.Socket.Shutdown(SocketShutdown.Both);
                }
                catch
                {
                    Log.Instance.Write("Closing connection.", Log.Priority.DEBUG);
                }

                ci.clientInstance.Socket.Close();
                clientList.Remove(ci);
                if( ci.clientThread != null)
                    ci.clientThread.Abort(); //Temporary. Fix this.

            }
        }
        /// <summary>
        /// End all client connections
        /// </summary>
        private static void CloseAllClientConns()
        {

            lock (_TCPStaticMethodLock)
            {
                //DirectControl.ReleaseDirectControl(-1);
                foreach (ClientObject conn in clientList.ToList())
                    CloseClientConn(conn);

            }

        }
        /// <summary>
        /// Get socket object of client with specified ID
        /// </summary>
        /// <param name="id">client id</param>
        /// <returns>client's socket object</returns>
        private static Socket GetClientSocket(int id)
        {
            foreach (ClientObject ci in clientList)
            {
                if (ci.clientID == id)
                    return ci.clientInstance.Socket;
            }
            return null;
        }
        /// <summary>
        /// Get socket object of client with specified ID
        /// </summary>
        /// <param name="id">client id</param>
        /// <returns>client's socket object</returns>
        private static ClientObject GetClientObject(int id)
        {
            foreach (ClientObject ci in clientList)
            {
                if (ci.clientID == id)
                    return ci;
            }
            return null;
        }
        /// <summary>
        /// Send a message to client with known id
        /// </summary>
        /// <param name="msg">message string to send</param>
        /// <param name="id">client id</param>
        /// <returns>Success/failure</returns>
        public static bool Send(string msg, int id)
        {
            Socket sock = GetClientSocket(id);
            if (sock == null)
                return false;
            else
                Log.Instance.Write("message sending :" + msg, Log.Priority.DEBUG);
                return Send(msg, sock);
            
        }
        /// <summary>
        /// Send a message to client with known socket object
        /// </summary>
        /// <param name="msg">message string to send</param>
        /// <param name="sock">client socket object</param>
        /// <returns>Success/failure</returns>
        public static bool Send(string msg, Socket sock)
        {
            try
            {
                Byte[] bytes = Encoding.UTF8.GetBytes(msg);
                sock.Send(bytes, bytes.Length, SocketFlags.None);
            }
             catch (Exception)
            {
                return false;
            }
            return true;
        }
        /// <summary>
        /// Broadcast a message to all connected clients
        /// </summary>
        /// <param name="msg">msg in string format</param>
        public static void BroadcastToClients(string msg)
        {
            Log.Instance.Write("Broadcasting: " + msg, Log.Priority.DEBUG);
            lock (_TCPStaticMethodLock)
            {

                foreach (ClientObject conn in clientList.ToList())
                {
                    try
                    {
                        Byte[] bytes = Encoding.UTF8.GetBytes(msg);
                        conn.clientInstance.Socket.Send(bytes, bytes.Length, SocketFlags.None);
                    }
                    catch (ObjectDisposedException)
                    {
                        return;
                    }
                    catch (SocketException exc)
                    {
                        CloseClientConn(conn);
                        Log.Instance.Write("Socket exception during broadcast: " + exc.ToString(), Log.Priority.WARN);
                    }
                    catch (Exception exc) //TCP server has crashed
                    {
                        if (conn.clientInstance.Socket.Connected)
                            CloseClientConn(conn);
                        Log.Instance.Write("Fatal exception during broadcast: " + exc.ToString(), Log.Priority.ERROR);
                    }
                }
            }
        }
        /// <summary>
        /// Called every time a new client connection is initiated. This method sets up the client handler for each client.
        /// Each client recieves its own worker thread, which uses synchronous sockets.
        /// </summary>
        private static void NewConnCallback(IAsyncResult result)
        {
            ClientObject currClient = new ClientObject();
            currClient.clientInstance = new ClientHandler();

            try
            {
                // Accept connection
                Socket s = (Socket)result.AsyncState;
                currClient.clientInstance.Socket = s.EndAccept(result);
                currClient.clientInstance.Socket.Blocking = false;
                currClient.clientInstance.Buffer = new byte[255];
                lock (_TCPStaticMethodLock) clientList.Add(currClient);

                Log.Instance.Write("New client connection: " + currClient.clientInstance.Socket.RemoteEndPoint.ToString(), Log.Priority.INFO);

                // Fire up the handler thread for this client
                currClient.clientID = curr_client_attempt; 
                curr_client_attempt -= 1; 
                Log.Instance.Write("Starting handler for client ID: " + currClient.clientID, Log.Priority.VERBOSE_DEBUG);
                currClient.clientThread = currClient.clientInstance.StartClientHandler(currClient.clientID);

          
                
                // Begin receiving data
                currClient.clientInstance.Socket.BeginReceive(currClient.clientInstance.Buffer, 0,
                    currClient.clientInstance.Buffer.Length, SocketFlags.None,
                    new AsyncCallback(ReceiveCallback), currClient);
                
                // Ready to accept another connection
                serverSocket.BeginAccept(new AsyncCallback(NewConnCallback),
                    result.AsyncState);

                //Handshake Protocol 
                Send(Messages.NAME, currClient.clientID); 

                //Test Msg
                Thread.Sleep(2000);
                Send("s.atest_1.", currClient.clientID);

            }
            catch (SocketException exc)
            {
                CloseClientConn(currClient);
                Log.Instance.Write("Socket exception during client accept: " + exc.ToString(), Log.Priority.WARN);
                try //Can we continue?
                {
                    serverSocket.BeginAccept(new AsyncCallback(NewConnCallback),
                        result.AsyncState);
                    Log.Instance.Write("Previous socket exception non-fatal.", Log.Priority.SUCCESS_AUDIT);
                }
                catch //Nope, TCP server has crashed
                {
                    NewFatalException("Previous socket exception fatal.");
                }
            }
            catch (Exception exc) //TCP server has crashed
            {
                try
                {
                    if (currClient.clientInstance.Socket.Connected)
                        CloseClientConn(currClient);
                    NewFatalException("Fatal exception during client accept: " + exc.ToString());
                }
                catch
                {
                }
            }
        }
        /// <summary>
        /// Handles incoming client messages and adds to appropriate
        /// message queue.
        /// </summary>
        /// <param name="result"></param>
        private static void ReceiveCallback(IAsyncResult result)
        {
            ClientObject currClient = (ClientObject)result.AsyncState;;
            try
            {
                int bytesRead = currClient.clientInstance.Socket.EndReceive(result);
                if (0 != bytesRead)
                {
                    string s;
                    lock (_TCPStaticMethodLock) s = Encoding.UTF8.GetString(currClient.clientInstance.Buffer, 0, bytesRead);

                    
                    lock (currClient.clientInstance._QLock)
                    {
                        //Max queue length of 5
                        if (currClient.clientInstance.actionQueue.Count <= 10)
                        {
                            lock (currClient.clientInstance._TCPLock)
                                currClient.clientInstance.msgBuffer += s;
                            currClient.clientInstance._actionFlag.Set();
                            currClient.clientInstance.actionQueue.Enqueue(
                                new ActionItem(ActionType.CLIENTMSG));
                        }
                    }

                    currClient.clientInstance.Socket.BeginReceive(currClient.clientInstance.Buffer, 0,
                        currClient.clientInstance.Buffer.Length, SocketFlags.None,
                        new AsyncCallback(ReceiveCallback), currClient);
                }
                else CloseClientConn(currClient);
            }
            catch (ObjectDisposedException)
            {
                return;
            }
            catch (SocketException exc)
            {
                CloseClientConn(currClient);
                Log.Instance.Write("Socket exception during recieve: " + exc.ToString(), Log.Priority.WARN);
            }
            catch (Exception exc) //TCP server has crashed
            {
                CloseClientConn(currClient);
                NewFatalException("Fatal exception during recieve: " + exc.ToString());
            }
        }
        /// <summary>
        /// Raise an error flag for watchdog.
        /// After this function is called, the TCPServer will be
        /// forcibly restarted by the watchdog.
        /// </summary>
        /// <param name="s">error message</param>
        private static void NewFatalException(string s)
        {
            lock (MainService._statusLock)
                MainService.status.TCPServer = Status.ERROR;

            Log.Instance.Write("TCP Server error. " + s, Log.Priority.ERROR);
        }
 
    }
}

