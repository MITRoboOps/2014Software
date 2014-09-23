using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Timers;
using System.Reflection;

//! \file MainService.cs Contains the main service entry point

/* 
 * Contact: Reo - reobaird@gmail.com
 * Revision: 2
 * Date: June 19th
 * 
 * Changelog:
 * 
 * June 19: Revamped watchdog functionality
 */

namespace spotterGUI
{
    /// <summary>
    /// Description: This class runs as the core service thread. 
    /// OnStart() is the main service entry point. The main purpose 
    /// of this thread, after start, is to monitor the health of 
    /// the DSC system and restart buggy/hanging threads.
    /// </summary>
    class MainService 
    {
        public static MainService This;

        public static Settings settings;
       // private static System.Timers.Timer _WatchDogTimer; // From System.Timers
      
       
        //Constructing 3 separate directories
        public static PhotoCollection _pcRight = new PhotoCollection(@"C:\RoboFTP\0");
        public PhotoCollection pcRight { get { return _pcRight; } }
        public static PhotoCollection _pcFront = new PhotoCollection(@"C:\RoboFTP\1");
        public PhotoCollection pcFront { get { return _pcFront; } }
        public static PhotoCollection _pcLeft = new PhotoCollection(@"C:\RoboFTP\2");
        public PhotoCollection pcLeft { get { return _pcLeft; } }
        
        
        public static string latest_photo_right;
        public static string latest_photo_front;
        public static string latest_photo_left;

        public static readonly object _statusLock = new object();
        /// <summary>
        /// This struct contains the status of each system and device.
        /// </summary>
        public struct status
        {
            public static Status system = Status.STARTING;
            public static Status TCPServer = Status.OFFLINE;
            public static Status left = Status.OFFLINE; //never set back to offline after turned on... fix me
            public static Status right = Status.OFFLINE;
            public static Status front = Status.OFFLINE;

        }



        /// <summary>
        /// Instance the service.
        /// </summary>
        public MainService()
        {
            //Set service information
            //ServiceName = "Spotter Server";
            This = this;
            //number_of_photos = _pcRight.photoList.Count();
            latest_photo_right = @"C:\RoboFTP\0\atest_1.0.JPG";
            latest_photo_front = @"C:\RoboFTP\1\atest_1.0.JPG";
            latest_photo_left = @"C:\RoboFTP\2\atest_1.0.JPG";
        }
        
        /// <summary>
        /// Restart the entire system. This function must be spawned
        /// in its own thread.
        /// </summary>
        public void Restart()
        {
            OnStop();
            Log.Instance.Write("Restarting system in 5 seconds...", Log.Priority.NOTICE);
            GC.Collect();
            Thread.Sleep(5000);
            OnStart();
        }

        /// <summary>
        /// Main service entry point. This will initialize the Spotter Server
        /// </summary>
        public void OnStart()
        {
            Log.Instance.Write("Initializing Spotter Server...", Log.Priority.NOTICE);
            try {
                TCPServer _tcpServer = TCPServer.tcpServerInst;
                _tcpServer.StartTCPServer(System.Net.IPAddress.Any,
                    MainService.settings.serverPort);
            }
            catch{
                Log.Instance.Write("Failed to initialize server...", Log.Priority.DEBUG);

            }
            MainService.status.system = Status.ONLINE;
            // TODO: add startup stuff

        }

        public void OnStop()
        {
            Log.Instance.Write("Shutting down Spotter Server...", Log.Priority.NOTICE);
            TCPServer.tcpServerInst.StopTCPServer();
            Log.Instance.Write("Server Terminated", Log.Priority.NOTICE);
        }

        /// <summary>
        /// Load the application settings from Settings/config.xml.
        /// If no settings are found, defaults will be created.
        /// 
        /// If this function returns false, this represents a critical
        /// error and the application cannot continue.
        /// </summary>
        /// <returns>success/failure</returns>
        public static bool loadSettings()
        {
            //ClientPermissions.Client.RecreateDefaults();
            System.IO.StreamReader file = null;
            try
            {
                if (!System.IO.File.Exists("Settings/config.xml"))
                {
                    Log.Instance.Write("No config.xml found.", Log.Priority.FAILURE_AUDIT);
                    createSettings();
                    return true;
                }
                System.Xml.Serialization.XmlSerializer reader =
                        new System.Xml.Serialization.XmlSerializer(typeof(Settings));
                file = new System.IO.StreamReader("Settings/config.xml");
                settings = (Settings)reader.Deserialize(file);
            }
            catch (Exception e)
            {
                Log.Instance.Write("DSC Server unable to load config.xml. " +
                    "Additionally, a default config file could not be created. Exception: " +
                    e.ToString(), Log.Priority.ERROR);
                return false;
            }
            finally
            {
                if (file != null)
                    file.Close();
            }
            return true;

        }
        /// <summary>
        /// Create a set of application settings (ie: first run)
        /// </summary>
        public static void createSettings()
        {
            System.Xml.Serialization.XmlSerializer writer;
            System.IO.StreamWriter file = null;
            Log.Instance.Write("Creating config file...", Log.Priority.VERBOSE_DEBUG);
            try
            {
                // settings = new Settings();
                settings.Default();
                writer =
                    new System.Xml.Serialization.XmlSerializer(typeof(Settings));
                if (!System.IO.Directory.Exists("Settings/"))
                    System.IO.Directory.CreateDirectory("Settings/");
                file = new System.IO.StreamWriter("Settings/config.xml");
                writer.Serialize(file, settings);
            }
            catch (Exception e)
            {
                Log.Instance.Write("File IO error writing config.xml. " + e.ToString(), Log.Priority.ERROR);
            }
            finally
            {
                if (file != null)
                    file.Close();
            }
            return;
        }



  
        //    // TODO: add shutdown stuff

        //static void _WatchDog_Elapsed(object sender, ElapsedEventArgs e)
        //{
        //    if (status.relays == Status.ERROR)
        //    {
        //        Log.Instance.Write("Watchdog is restarting Phidget.", Log.Priority.NOTICE);
        //        cameraSystems[0].digInst.Stop();
        //        cameraSystems[0].digInst.Start();
        //    }
        //    if (status.camera == Status.ERROR)
        //    {
        //        Log.Instance.Write("Watchdog is restarting Camera.", Log.Priority.NOTICE);
        //        cameraSystems[0].camInst.Stop();
        //        cameraSystems[0].camInst.Start();
        //    }
        //    if (status.pt == Status.ERROR)
        //    {
        //        Log.Instance.Write("Watchdog is restarting PT.", Log.Priority.NOTICE);
        //        cameraSystems[0].ptInst.Stop();
        //        cameraSystems[0].ptInst.Start();
        //    }
        //}

    }
}
