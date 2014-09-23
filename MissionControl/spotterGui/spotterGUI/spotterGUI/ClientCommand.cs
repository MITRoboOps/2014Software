using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Net.Sockets;
using System.Threading;

using System.Security.Cryptography;
//! \file ClientCommands.cs Contains command-parsing porition of ClientHandler class.

/* 
 * Contact: Reo - reobaird@gmail.com
 * Revision: 2
 * Date: June 6th
 * 
 * Changelog:
 * 
 * June 6: Added usr branch for presets and moved preset logic out of cam module.
 * 
 * */
namespace spotterGUI
{
    //Summary written in clienthandler.cs
    partial class ClientHandler
    {
        // any command function must match this interface (same return type and arguments)
        delegate void CommandDelegate(string[] args);
        PhotoCollection _pc;
      
        private Hashtable rootCmds;
        /// <summary>
        /// Initialize root-level commands hashtable. This function is called the first time
        /// a client sends a command.
        /// </summary>
        private void InitRootCmds()
        {
            Log.Instance.Write("Initializing root cmds hashtable", Log.Priority.VERBOSE_DEBUG);
            this.rootCmds = new Hashtable();
            this.rootCmds.Add("n=r", new CommandDelegate(this.Name_right)); //changed to single letter protocol
            this.rootCmds.Add("n=l", new CommandDelegate(this.Name_left));
            this.rootCmds.Add("n=f", new CommandDelegate(this.Name_front));
            this.rootCmds.Add("err", new CommandDelegate(this.No));
            this.rootCmds.Add("sent", new CommandDelegate(this.Sent));

            //this.rootCmds.Add("sys", new CommandDelegate(this.Root_SysCmdFunc));
            //this.rootCmds.Add("cam", new CommandDelegate(this.Root_CamCmdFunc));
            //this.rootCmds.Add("help", new CommandDelegate(this.Root_HelpCmdFunc));
            //this.rootCmds.Add("status", new CommandDelegate(this.Root_StatusFunc));
        }
        /// <summary>
        /// Entry point for command parsing.
        /// </summary>
        /// <param name="args">args</param>
        private void ProcessRootCommand(string[] args)
        {
            Log.Instance.Write("Command Processed: " + args[0], Log.Priority.VERBOSE_DEBUG);
            if (rootCmds.ContainsKey(args[0]))
                try
                {
                    ((CommandDelegate)rootCmds[args[0]])(args);
                }
                catch (Exception e)
                {
                    lock (this._TCPLock)
                        TCPServer.Send(Messages.BADCMD, this.Socket);
                    Log.Instance.Write("Command Exception: " + e.ToString(), Log.Priority.ERROR);
                }

            else
            {
                lock (this._TCPLock)
                    TCPServer.Send(Messages.BADCMD, this.Socket);


            }
        }

        private void Name_right(string[] args)
        {
            Log.Instance.Write("Right Camera is connected", Log.Priority.INFO);
            MainService.status.right = Status.ONLINE;
            this.clientID = 0; 
        }
       
        private void Name_left(string[] args)
        {
            Log.Instance.Write("Left Camera is connected", Log.Priority.INFO);
            MainService.status.left = Status.ONLINE;
            this.clientID = 2;
        }
        private void Name_front(string[] args)
        {
            Log.Instance.Write("Right Camera is connected", Log.Priority.INFO);
            MainService.status.front = Status.ONLINE;
            this.clientID = 1;
        }
        private void No(string[] args)
        {
            Log.Instance.Write("ClientID: " + clientID +"Boa Failed to Send Image", Log.Priority.INFO);

        }
        /// <summary>
        /// Logs that and image was sent by the boa camera; photoCollection updated.
        /// </summary>
        /// <param name="args"></param>
        private void Sent(string[] args)
        {
            Log.Instance.Write("BOA sent an FTP image", Log.Priority.INFO);
            //fix me
            //MainService.number_of_photos += 1;
            if (this.clientID == 0)
                 MainService._pcRight.addPhoto(MainService.latest_photo_right);
            if (this.clientID == 1)   
                MainService._pcFront.addPhoto(MainService.latest_photo_front);
            if (this.clientID == 2)
                MainService._pcLeft.addPhoto(MainService.latest_photo_left);
      
        }




    }
}
