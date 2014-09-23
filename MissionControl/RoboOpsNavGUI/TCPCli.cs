using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections.Generic;
using RoboOpsNavGUI;
using System.Windows.Controls;

// State object for receiving data from remote device.
public class StateObject
{
    // Client socket.
    public Socket workSocket = null;
    // Size of receive buffer.
    public const int BufferSize = 256;
    // Receive buffer.
    public byte[] buffer = new byte[BufferSize];
    // Received data string.
    public StringBuilder sb = new StringBuilder();



 
}

public class TCPCli
{

    
    // ManualResetEvent instances signal completion.
    public EventWaitHandle MSGFlag = new AutoResetEvent(false);
    public Queue<string> msgQueue = new Queue<string>();
    public readonly object _QLock = new object();

    public int status = 0; // 0 = disconnected, 1 = connecting, 2 = connected
    
    private IPAddress ipAddress;
    private int port;

    // The response from the remote device.
    private static String response = String.Empty;

    public bool debug = false;
    Button connButton;
    TextBlock connLabel;

    private Socket client;
    
    public TCPCli(string ip, int p, Button b, TextBlock l)
    {
        connButton = b;
        connLabel = l;
        ipAddress = IPAddress.Parse(ip);
        this.port = p;
    }

    private void ReportToGUI(string BLabel, string Text)
    {
        connLabel.Dispatcher.Invoke(
              System.Windows.Threading.DispatcherPriority.Normal,
              new Action(
                delegate()
                {
                   connLabel.Text = Text;
                   connButton.IsEnabled = true;
                   connButton.Content = BLabel;
                }
            )); 
    }

    public void StartClient()
    {
        // Connect to a remote device.
        if(status != 0)
        {
            Console.WriteLine("Error: Connection attempt on open connection");
            return;
        }
        status = 1;

        try
        {
            Console.Write("Initiating Connection...");
            // Establish the remote endpoint for the socket.
            //IPHostEntry ipHostInfo = Dns.GetHostEntry("192.168.0.1");
            IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

            // Create a TCP/IP socket.
            this.client = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);

            // Connect to the remote endpoint.
            client.BeginConnect(remoteEP,
                new AsyncCallback(ConnectCallback), client);
        }
        catch (Exception e)
        {
            Console.WriteLine("Connect failed.");
            Console.WriteLine(e.ToString());
            status = 0;
            ReportToGUI("Connect", "ERROR");
        }
    }

    public void CloseConnection()
    {
        try
        {
            Console.Write("Disconnecting...");
            status = 0;
            this.client.Shutdown(SocketShutdown.Both);
            this.client.Close();
            client.Dispose();
            client = null;
            Console.Write("Done!\n");

        }
        catch
        { }
        ReportToGUI("Connect", "Disconnected");

    }   
    private void ConnectCallback(IAsyncResult ar)
    {
        try
        {
            // Retrieve the socket from the state object.
            Socket client = (Socket)ar.AsyncState;

            // Complete the connection.
            client.EndConnect(ar);

            Console.WriteLine("Socket connected to {0}",
                client.RemoteEndPoint.ToString());

            Receive(client);

            // Signal that the connection has been made.
            ReportToGUI("Disconnect", "Connected");
            status = 2;
        }
        catch (Exception e)
        {
            Console.WriteLine("Connect failed.");
            Console.WriteLine(e.ToString());
            ReportToGUI("Connect", "ERROR");
            status = 0;
        }
    }

    private void Receive(Socket client)
    {
        try
        {
            // Create the state object.
            StateObject state = new StateObject();
            state.workSocket = client;

            // Begin receiving the data from the remote device.
            client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
                new AsyncCallback(ReceiveCallback), state);
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
            ReportToGUI("Connect", "ERROR");
            status = 0;
        }
    }

    private void ReceiveCallback(IAsyncResult ar)
    {
        try
        {
            // Retrieve the state object and the client socket 
            // from the asynchronous state object.
            StateObject state = (StateObject)ar.AsyncState;
            Socket client = state.workSocket;

            // Read data from the remote device.
            int bytesRead = client.EndReceive(ar);

            if (bytesRead > 0)
            {
                // There might be more data, so store the data received so far.
                response = Encoding.ASCII.GetString(state.buffer, 0, bytesRead);
                //Console.WriteLine(response);

                lock (_QLock)
                    msgQueue.Enqueue(response);
                MSGFlag.Set();
                if (debug)
                    Console.WriteLine("Recv: " + response);
                client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
                    new AsyncCallback(ReceiveCallback), state);
            }
            else
            {
                // Signal that all bytes have been received.
                Console.WriteLine("Disconnect");
                status = 0;
                ReportToGUI("Connect", "ERROR");
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
            ReportToGUI("Connect", "ERROR");
            status = 0;
        }
    }

    private void Send(Socket client, String data)
    {
        // Convert the string data to byte data using ASCII encoding.
        byte[] byteData = Encoding.ASCII.GetBytes(data);

        // Begin sending the data to the remote device.
        client.BeginSend(byteData, 0, byteData.Length, 0,
            new AsyncCallback(SendCallback), client);
    }

    private void SendCallback(IAsyncResult ar)
    {
        try
        {
            // Retrieve the socket from the state object.
            Socket client = (Socket)ar.AsyncState;

            // Complete sending the data to the remote device.
            int bytesSent = client.EndSend(ar);
            Console.WriteLine("Sent {0} bytes to server.", bytesSent);
            
            // Signal that all bytes have been sent.
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
            ReportToGUI("Connect", "ERROR");
            status = 0;
        }
    }
}