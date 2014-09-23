using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Diagnostics;
namespace spotterGUI
{
    class ContinuousRequest
    {
        private Boolean _requestContinuous;
        public Boolean RequestContinuous { get { return _requestContinuous; } set { _requestContinuous = value; } }
        private int _time_in_milliseconds;
        public int time_in_milliseconds { get { return time_in_milliseconds; } set { _time_in_milliseconds = value; } }

        public static ContinuousRequest This;
        //public class ContinuousRequest()
        //{
        //    This = this;
        //}

        public Thread requestPhotoThread()
        {

            _time_in_milliseconds = 7000;
            Thread t = new Thread(new ThreadStart(RequestEvery_seconds));
            t.Start();
            return t;
        }

        /// <summary>
        /// sends a request to the BOA Camera (right camera only every 10 seconds)
        /// </summary>
        private void RequestEvery_seconds()
        {
            
            _requestContinuous = true;
            while (true)
            {
                while (_requestContinuous)
                {
                    Stopwatch sw = new Stopwatch();
                    sw.Start();
                    requestPhoto(0);
                    requestPhoto(1);
                    requestPhoto(2);
                    
                    sw.Stop();
                    Log.Instance.Write("request for all photos took: " + sw.Elapsed, Log.Priority.VERBOSE_DEBUG);
                    Thread.Sleep(_time_in_milliseconds);

                }
                
            }
        }

        private int Rcounter = 0;
        private int Fcounter = 0;
        private int Lcounter = 0;
        /// <summary>
        /// This method makes a request to the right Camera. The image name increments. The name starts with an r. 
        /// </summary>
        public void requestPhoto(int clientID)
        {
            Status currStatus = Status.OFFLINE;
            string sendMessage;
            string p;
            if (clientID == 0)
            {
                currStatus = spotterGUI.MainService.status.right;
                sendMessage = "s.r" + Rcounter.ToString() + ".";
                p = @"C:\RoboFTP\0\r" + Rcounter.ToString() + ".0.JPG";
                Log.Instance.Write("requested photo from R", Log.Priority.VERBOSE_DEBUG);
                Rcounter += 1;
                MainService.latest_photo_right = p;
            }
            else if (clientID == 1)
            {
                currStatus = spotterGUI.MainService.status.front;
                sendMessage = "s.f" + Fcounter.ToString() + ".";
                p = @"C:\RoboFTP\1\f" + Fcounter.ToString() + ".0.JPG";
                Log.Instance.Write("requested photo from F", Log.Priority.VERBOSE_DEBUG);
                Fcounter += 1;
                MainService.latest_photo_front = p;
            }
            else if (clientID == 2)
            {
                currStatus = spotterGUI.MainService.status.left;
                sendMessage = "s.l" + Lcounter.ToString() + ".";
                p = @"C:\RoboFTP\2\l" + Lcounter.ToString() + ".0.JPG";
                Log.Instance.Write("requested photo from L", Log.Priority.VERBOSE_DEBUG);
                Lcounter += 1;
                MainService.latest_photo_left = p;
            }else{
                Log.Instance.Write("wrong client ID request", Log.Priority.VERBOSE_DEBUG);
                return;
            }
            
            if (currStatus == Status.ONLINE)
            {
                TCPServer.Send(sendMessage, clientID);
               
                //MainService.latest_photo_right = p;
                //DateTime time = DateTime.Now;
                //MainService._pcRight.time_discovered.Add(p, time);
                //Log.Instance.Write("latest_photo: " + MainService.latest_photo_right, Log.Priority.VERBOSE_DEBUG);
             
            }
            else
                Log.Instance.Write("Camera "   + clientID + " is not connected", Log.Priority.NOTICE);
        
        }

    }
}
