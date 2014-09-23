using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Text;
using System.Diagnostics;
//! \file Log.cs Contains the log class, which logs to windows event log and console.

/* Message logging class
 * 
 * Contact: Reo - reobaird@gmail.com
 * Revision: 3
 * Date: Apr 16th
 * 
 * Changelog:
 * Rev2. Apr 16th: Improved perfomance. Decreased complexity. Implemented settings file.
 * Rev1. Mar 19th: Created.
 * */

namespace spotterGUI
{
    /// <summary>
    /// * Description: This class handles logging. The class posts to the 
    /// console window and/or the DSC windows event log, accessable in 
    /// Start -> Control Panel -> Administrative Tools -> Event Viewer. 
    /// 
    /// Logging options can be set in the main configuration file, "settings/config.xml".
    ///
    /// Event Levels:
    /// 0,1,2,3,4: Information
    /// 5: SuccessAudit
    /// 6: FailureAudit
    /// 7,8,9: Warning
    /// 10: Error
    /// </summary>
    class Log
    {
        #region Properties
        private static readonly object _logLock = new object();

        public enum Priority
        {
            VERBOSE_DEBUG = 0, DEBUG = 1, INFO = 2, NOTICE = 3,
            P_NOTICE = 4, SUCCESS_AUDIT = 5, FAILURE_AUDIT = 6,
            WARN = 8, P_WARN = 9, ERROR = 10
        };

        private System.Diagnostics.EventLog eLog;
        private static Log instance = null;

        /// <summary>
        /// Access the Log instance
        /// </summary>
        public static Log Instance
        {
            get
            {
                // If the instance is null then create one and init the Queue
                if (instance == null)
                {
                    lock (_logLock)
                    {

                        //Create an instance
                        instance = new Log();
                    }
                }
                return instance;
            }
        }
        #endregion
        #region Methods
        /// <summary>
        /// Private constructor to prevent instance creation
        /// </summary>
        private Log() { }

        /// <summary>
        /// Write a log entry.
        /// </summary>
        /// <param name="message">The message to write to the log</param>
        /// <param name="priority">Message priority</param>
        public void Write(string message, Log.Priority priority)
        {
            this.Write(message, (int)priority);
        }
        /// <summary>
        /// Write a log entry.
        /// </summary>
        /// <param name="message">The message to write to the log</param>
        /// <param name="priority">Message priority</param>
        public void Write(string message, int priority)
        {
            lock (_logLock)
            {
                LogItem logEntry = new LogItem(message, priority);
                if (MainService.settings.logToConsole == true && priority >= MainService.settings.consoleLogLevel)
                    Console.WriteLine(string.Format("{1}\t{2} (Priority: {0})", logEntry.Priority, logEntry.LogTime, logEntry.Message));
                /*
                 *  * Debug Levels:
                 * 0,1,2,3,4: Information
                 * 5: SuccessAudit
                 * 6: FailureAudit
                 * 7,8,9: Warning
                 * 10: Error
                 */

                
            }
        }
        #endregion
    }

    /// <summary>
    /// Log entry item object.
    /// </summary>
    public class LogItem
    {
        public string Message { get; set; }
        public int Priority { get; set; }
        public string LogTime { get; set; }
        public string LogDate { get; set; }

        /// <summary>
        /// Construct a new LogItem
        /// </summary>
        /// <param name="message">The Message</param>
        /// <param name="priority">Priority level (0-10). See Log.Priority enum.</param>
        public LogItem(string message, int priority)
        {
            Message = message;
            Priority = priority;
            LogDate = DateTime.Now.ToString("yyyy-MM-dd");
            LogTime = DateTime.Now.ToString("hh:mm:ss tt");
        }
    }
}
