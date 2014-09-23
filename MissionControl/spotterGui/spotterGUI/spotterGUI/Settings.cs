using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace spotterGUI
{
    public struct Settings
    {
        public bool logToConsole;
        public int consoleLogLevel;
        public int serverPort;
    


        /// <summary>
        /// Populate with default values.
        /// </summary>
        public void Default()
        {
           
            this.logToConsole = true;
 
            this.consoleLogLevel = 0;
   
            this.serverPort = 23;
            
        }
}
}
