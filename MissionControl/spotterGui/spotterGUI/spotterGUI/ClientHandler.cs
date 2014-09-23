using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Net.Sockets;
using System.Threading;


//! \file ClientCommands.cs Contains client-handling porition of ClientHandler class.

/*  
 * Contact: Reo - reobaird@gmail.com
 * Revision: 1
 * Date: March 21th
 * 
 * Description: The core portion of the client handler class. 
 * 
 * */

namespace spotterGUI
{
    /// <summary>
    /// ClientHandler consists of two parts. Part 1 is contained in ClientHandler.cs
    /// and handles client connections. Part two is contained in ClientCommands.cs and
    /// handles parsing commands. The client handler processes client commands and 
    /// allows two-way communication between the scheduler/devices and the client.
    /// 
    /// StartThread is the entry point. Each client handler exists in its own thread. 
    /// The class and thread instances are stored in a list in the TCP Server class.
    /// 
    /// Command parsing functions via a recursive hash-table lookup system. 
    /// Commands are "tiered" using deliminators. For example, the command:
    /// cam.takephoto results in two hashtable lookups. 'cam' is looked up 
    /// in the root hash table, then 'takephotoat' is looked up in the cam hashtable.
    /// 
    /// All incoming client data is passed to `ProcessRootCommand()` which then 
    /// calls the appropriate function call, mapped in the hash table. 
    /// 
    /// All response messages are contained in Messages.cs.
    /// 
    /// Full command structure and useage is available under 
    /// "Command Structure" in the user guide.
    /// 
    /// A tutorial on how to add a new command is available in the
    /// programmer's guide.
    /// </summary>
    partial class ClientHandler //'Commands' section of class in commands.cs
    {

        public readonly object _TCPLock = new object();
        public readonly object _QLock = new object();
        public EventWaitHandle _actionFlag = new AutoResetEvent(false);
        public Queue<ActionItem> actionQueue = new Queue<ActionItem>();

        private int activeCamSys = 0; //placeholder for multiple cameras later on.

        public volatile Socket Socket; //WARNING: NOT threadsafe. Access only in async recieve callback.
        public byte[] Buffer; //WARNING: NOT threadsafe. Access only in async recieve callback.

        public volatile string msgBuffer;
        private int clientID;

        /// <summary>
        /// Start the client worker thread and return its handle. Entry point for creating a new client handler.
        /// </summary>
        /// <returns>Handle of the created thread</returns>
        public Thread StartClientHandler(int cid)
        {
            this.clientID = cid;
            Thread t = new Thread(new ThreadStart(ClientMain));
            t.Start();
            return t;
        }


        /// <summary>
        /// Main loop for client handling. 
        /// </summary>
        private void ClientMain()
        {
            Log.Instance.Write("New ClientHandler thread started.", Log.Priority.DEBUG);
            if (this.rootCmds == null)
                this.InitRootCmds();
 
            //lock (this._TCPLock)
                //TCPServer.Send(Messages.MOTD, this.Socket);
            //Main loop
            while (true)
            {
                 _actionFlag.WaitOne();
                ActionItem action = null;
                try
                {
                    lock (_QLock)
                        action = this.actionQueue.Dequeue();
                }
                catch
                {
                    Log.Instance.Write("Client " + this.clientID + " action flag misfire.", Log.Priority.WARN);
                }
                string[] s = null;
                lock (_TCPLock)
                {
                    while (this.msgBuffer != null )
                    {
                     
                        s = this.msgBuffer.Split(new char[] { '.' }, 2);
                        
                        
                        if (s != null && s[0] != null && !s.Equals("") && s[0] != "")
                        {
                            Log.Instance.Write("Client " + this.clientID + " sent cmd: " + s[0], Log.Priority.VERBOSE_DEBUG);
                            string[] args = s[0].Split(new char[] { '.'}); //delimiters // could use {'.', '='} 
                           
                            this.ProcessRootCommand(args); //alter for multiple clients
                        }
                        try
                        {
                            this.msgBuffer = s[1];
                        }
                        catch
                        {
                            this.msgBuffer = null;
                        }
                    }
                }
                lock (_QLock)
                    if (this.actionQueue.Count != 0)
                        _actionFlag.Set();

            } /*endwhile*/
        }
    }
}
