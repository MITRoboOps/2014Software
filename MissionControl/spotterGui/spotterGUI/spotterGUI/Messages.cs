using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace spotterGUI
{
   /// <summary>
   /// The Message class contains the set of standard string messages. 
   /// These messages are sent to the client over TCP/IP. 
   /// No actions are performed. Mostly used for User 
   /// </summary>
    public class Messages
    {
        #region Root level messages

        ////Message of the Day
        //public const string MOTD = "CONNECTED\r" +
        //                    "Hello Mission Control! This is the MIT ROBO-OPS Spotter Server. \r" +
        //                    "type help for a list of commands. (then press enter)";

        //Help message
        public const string HELP = "\r\rMIT ROBO-OPS Spotter GUI \r" +
                               "Author: Dion Low\r" +
                               "Contact: dionlow@mit.edu\r\r" +
                               "Command Systax: command.subcommand=arg1,arg2\r" +
                               "For help on a specific command, type command.help (for example: sys.help)\r" +
                               "Root level commands: \r" +
                               "    sys - Spotter server system commands. \r" +
                               "    cam - BOA camera commands. Most commands require direct control.\r" +
                               "    pt - DSC Pan and tilt commands. Most commands require direct control.\r" + 
                               "    status - Display system status.\r" +
                               "    help - This help dialog\r";

        public const string BADCMD = "BADCMD\r";
        public const string NAME = "n.n.";

        /// <summary>
        /// SEND returns a string for requesting a picture to be made
        /// </summary>
        /// <param name="img_name">the name to </param>
        /// <returns> string representation of of send message to boa camera</returns>
        public string Send(string img_name)
        {
           string sendMsg = "send." + 
               img_name + 
               ".";
           return sendMsg;
         }
        #endregion

    }
}
