using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Diagnostics;

namespace spotterGUI
{
    public delegate void EventHandler(object sender, EventArgs args);
    class PhotoWatch 
    {

        #region Class Variables
        private Boolean recent = false; 
        private Boolean requestContinuous = false;
        private string current_pathR;
        private string lastImage;

        public Boolean Recent { get { return recent; } set { recent = value; } }
        public Boolean Request_Continuous { get { return requestContinuous; } set { requestContinuous = value; } }
        public string Current_pathR { get { return current_pathR; } }
        public string Last_Image { get { return lastImage; } set {lastImage = value;} }
        #endregion

        //public class PhotoWatch{};

        //used thread
        public Thread mostRecentPhotoThread()
        {
            Thread t = new Thread(new ThreadStart(showRecent));
            return t;
        }


        public EventWaitHandle MSGFlag = new AutoResetEvent(false);
        public Queue<string> msgQueue = new Queue<string>();
        public readonly object _QLock = new object();

        #region Delegates
        
        public delegate void RecentPhotoDetectedEventHandler(string path);

        public event RecentPhotoDetectedEventHandler Recent_Photo_Detected;

        #endregion 


        //public event EventHandler<photoWatchEventArgs> UpdateRecentPhoto; //event

        //This loop will continutously liste for 
        public virtual void showRecent()
        {
            Log.Instance.Write("show recent entered", Log.Priority.VERBOSE_DEBUG);
            //EventHandler<photoWatchEventArgs> handler = UpdateRecentPhoto; //thread safe copy
            int sleep_time = 200;
            while (true)
            {
                while (recent)
                {
                    Stopwatch sw = new Stopwatch();
                    sw.Start();

                    string lpR = MainService.latest_photo_right;
                    string lpF = MainService.latest_photo_front;
                    string lpL = MainService.latest_photo_left;


                    Log.Instance.Write("recent: " + recent + "\n" +
                        "last image shown: " + lastImage + "\n" +
                        "latest_photo: " + lpR + "\n" +
                        "HAS PHOTO: " + MainService._pcRight.hasPhoto(lpR) + "\n",
                        Log.Priority.VERBOSE_DEBUG
                        );
                    //if (MainService.status.right == Status.ONLINE)
                    //{

                    if (lastImage == null || lastImage.Equals("") || (!lastImage.Equals(lpR)))
                    {
                        bool canLoad = MainService._pcRight.addPhoto(lpR);
                        Log.Instance.Write("CANLOAD: " + canLoad + " path " + lpR, Log.Priority.VERBOSE_DEBUG);
                        if (canLoad)
                        {
                            try
                            {
                                DelegateCurrentPath(MainService._pcRight.getLastPhoto().Source);  
                            }
                            catch
                            {

                            }
                        }
                       
                         
                    }
                    bool canLoad2 = MainService._pcFront.addPhoto(lpF);
                    bool canLoad3 = MainService._pcLeft.addPhoto(lpL);
                    try
                    {
                        DelegateCurrentPath(MainService._pcFront.getLastPhoto().Source);
                    }
                    catch {

                    } try
                    {
                        DelegateCurrentPath(MainService._pcFront.getLastPhoto().Source);
                    }
                    catch
                    {

                    }try
                    {
                        DelegateCurrentPath(MainService._pcLeft.getLastPhoto().Source);
                    }
                    catch
                    {

                    }
                       
                                        
                    sw.Stop();
                    Log.Instance.Write("Time for recent update loop: " + sw.Elapsed, Log.Priority.VERBOSE_DEBUG);

                    Thread.Sleep(sleep_time);
                }
            Thread.Sleep(sleep_time);
            }
            ////}
            //if (recent)
            //{
                
            //    //yImage1.Source.Dispatcher.BeginInvoke(new LoadNextImage(showRecent), null);
            //    //MainWindow.startStopButton.Dispatcher.BeginInvoke(new LoadNextImage(), System.Windows.Threading.DispatcherPriority.Background, null);
            //}

        }

        private void DelegateCurrentPath(string path)
        {
            try
            {
              
                current_pathR = path;
                if (Recent_Photo_Detected != null)
                {
                    Log.Instance.Write("Delegating: " + current_pathR + " to be loaded", Log.Priority.VERBOSE_DEBUG);
                    Recent_Photo_Detected(current_pathR);
                }
     
               
            }
            catch
            {
                Log.Instance.Write("no image to load", Log.Priority.VERBOSE_DEBUG);
            }
        }


    }
}
