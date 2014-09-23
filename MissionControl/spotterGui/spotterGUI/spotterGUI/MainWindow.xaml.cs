using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;


using System.Collections.ObjectModel;
using System.IO;

using System.Drawing;

namespace spotterGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        MainService s = new MainService();
        private int i = 0;
        
        
        //Thread recentThread = null;
        //Thread requestContinuousThread;
        //string lastImage;
        PhotoWatch photoWatch;
        ContinuousRequest contRequest;
        Thread PhotoWatchThread;
        
        //public delegate void LoadNextImage();

        public MainWindow()
        {
            InitializeComponent();
            AddHandler(Keyboard.KeyDownEvent, (KeyEventHandler)HandleKeyDownEvent);
            //Start the server when GUI opens

            if (!MainService.loadSettings())
            {
                Console.WriteLine("Fatal file IO error while trying to load or create base application settings.\n" +
                                  "Troubleshooting tips:\n" +
                                  "-Ensure that the application can read/write to its own directory and sub-directories\n" +
                                  "-Settings/config.xml may be corrupt, try deleting it. The application will recreate defaults\n" +
                                  "\n\nPress any key to exit...");
                Console.ReadKey();
                return;
            }
            s.OnStart();
            Console.WriteLine("Server Started");
            Console.Read();

            //PhotoWatch Thread
            photoWatch = new PhotoWatch();
            photoWatch.Recent_Photo_Detected += new PhotoWatch.RecentPhotoDetectedEventHandler(loadUIImage); //subscriber
       

            //Continous Request Thread
            contRequest = new ContinuousRequest();
            contRequest.requestPhotoThread(); //starts thread


            //Watching for recent photo
            PhotoWatchThread = photoWatch.mostRecentPhotoThread();
            PhotoWatchThread.Start();
            
            

        }
        
        //public Thread mostRecentPhotoThread()
        //{
        //    Thread t = new Thread(new ThreadStart(KeepUpdated));
        //    t.Start();
        //    return t;
        //}
        



        ///// <summary>
        ///// Continuously looks for latest photo. Waits 3 seconds if latest photo is found
        ///// </summary>
        //private void KeepUpdated()
        //{
        //    int sleep_time = 1000;
            
        //    while (true)
        //    {
        //        Log.Instance.Write("recent: " + recent + "\n" +
        //            "last image shown: " + lastImage + "\n" +
        //            "latest_photo: " + MainService.latest_photo + "\n" +
        //            "HAS PHOTO: " + MainService._pc.hasPhoto(MainService.latest_photo) +"\n",
        //            Log.Priority.VERBOSE_DEBUG
        //            );
        //        if (recent && (MainService.status.right == Status.ONLINE))
        //        {
        //            Log.Instance.Write("recent and online", Log.Priority.VERBOSE_DEBUG);
        //            if (lastImage == null || lastImage.Equals(""))
        //            {
        //                Log.Instance.Write("null image found: FIRST CASE", Log.Priority.VERBOSE_DEBUG);

        //                if (MainService._pc.hasPhoto(MainService.latest_photo))
        //                {
        //                    Log.Instance.Write("Last image null; Trying to add: " + MainService.latest_photo, Log.Priority.VERBOSE_DEBUG);
        //                    bool canLoad = MainService._pc.addPhoto(MainService.latest_photo);
        //                    if (canLoad)
        //                    {
        //                        Log.Instance.Write("CANlOAD" , Log.Priority.VERBOSE_DEBUG);
                                
        //                        //Photo currPhoto = MainService._pc._photoList.ElementAt(0);
        //                        //Set image source
        //                        //myImage1.Source = currPhoto.Image;
        //                        //lastImage = currPhoto.Source;

        //                        current_path = MainService.latest_photo;
        //                        myImage1.Source.Dispatcher.BeginInvoke(new LoadNextImage(loadUIImage), null);
                               
                                    

        //                    }

        //                }
        //                #region first image
        //                try
        //                {
        //                    //Photo lastPhoto = new Photo(MainService.latest_photo);
        //                    //myImage1.Source = lastPhoto.Image; //loads last image
        //                    string _path = MainService.latest_photo;
        //                    Uri _source = new Uri(_path);
        //                    myImage1.Source = BitmapFrame.Create(_source);
        //                    lastImage = MainService.latest_photo;
        //                    Log.Instance.Write("First Photo updated", Log.Priority.VERBOSE_DEBUG);
        //                    Thread.Sleep(sleep_time);

        //                }
        //                catch
        //                {
        //                    if (!MainService._pc.hasPhoto(MainService.latest_photo))
        //                    {
        //                        Log.Instance.Write("Last image null; Trying to add: " + MainService.latest_photo, Log.Priority.VERBOSE_DEBUG);
        //                        bool canLoad = MainService._pc.addPhoto(MainService.latest_photo);
        //                        if (canLoad)
        //                        {
        //                            Photo currPhoto = MainService._pc._photoList.ElementAt(MainService._pc.Count - 1);
        //                            //Set image source
        //                            myImage1.Source = currPhoto.Image;
        //                            lastImage = currPhoto.Source;
        //                        }
                                
        //                    }
        //                    else
        //                    {
        //                        //last photo has been found
        //                        Thread.Sleep(sleep_time);
        //                    }
        //                }
        //                #endregion  
        //            }
        //            else if (!lastImage.Equals(MainService.latest_photo))
        //            {
        //                #region Last image loaded and requested not the same
        //                Log.Instance.Write("last image shown: " + lastImage + "last image requested: " + MainService.latest_photo, Log.Priority.VERBOSE_DEBUG);
        //                try
        //                {
        //                    Log.Instance.Write("Trying to load: " + MainService.latest_photo, Log.Priority.VERBOSE_DEBUG);
        //                    // Photo lastPhoto = new Photo(MainService.latest_photo);
        //                    //myImage1.Source = lastPhoto.Image; //loads last image
        //                    string _path = MainService.latest_photo;
        //                    Uri _source = new Uri(_path);
        //                    myImage1.Source = BitmapFrame.Create(_source);
        //                    lastImage = MainService.latest_photo;
        //                    Thread.Sleep(sleep_time);

        //                }
        //                catch
        //                {
        //                    if (!MainService._pc.hasPhoto(MainService.latest_photo))
        //                    {
        //                        Log.Instance.Write("Trying to add: " + MainService.latest_photo, Log.Priority.VERBOSE_DEBUG);
        //                        bool canLoad = MainService._pc.addPhoto(MainService.latest_photo);
        //                        if (canLoad)
        //                        {
        //                            Photo currPhoto = MainService._pc._photoList.ElementAt(MainService._pc.Count - 1);
        //                            //Set image source
        //                            myImage1.Source = currPhoto.Image;
        //                            lastImage = currPhoto.Source;
        //                        }
        //                    }
        //                    else
        //                    {
        //                        //last photo has been found
        //                        Thread.Sleep(sleep_time);
        //                    }
        //                }
        //                #endregion
        //            }

        //        }
        //        else//otherwise sleep another 3 seconds if recent feature not turned on
        //        {
        //            Log.Instance.Write("Recent not turned on", Log.Priority.VERBOSE_DEBUG);
        //            Thread.Sleep(sleep_time);
        //        }

        //    }

       // }

        /// <summary>
        /// start and stop event for Recent watching
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void StartOrStop(object sender, EventArgs e)
        {
            
            if (photoWatch.Recent)
            {
                photoWatch.Recent = false;
                Log.Instance.Write("recent set to false", Log.Priority.INFO);
                startStopButton.Content = "Most Recent";
            }
            else
            {
                photoWatch.Recent= true;
                Log.Instance.Write("recent set to true", Log.Priority.INFO);
                startStopButton.Content = "Not Recent";
                //startStopButton.Dispatcher.BeginInvoke(new LoadNextImage(showRecent), System.Windows.Threading.DispatcherPriority.Background, null);
                //myImage1.Source.Dispatcher.BeginInvoke(new LoadNextImage(showRecent), (object)new object[]{(object) @"C:\RoboFTP\test_1.0.JPG"});
            }
        }





        ///// <summary>
        ///// Request Photo from right camera
        ///// </summary>
        ///// <param name="sender"></param>
        ///// <param name="e"></param>
        //private void RequestPhoto_R(object sender, RoutedEventArgs e)
        //{
        //    s.requestPhoto_R();

        //}

        /// <summary>
        /// requests name to connect to client; then changes the button to force update (looks through all possible sent images)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Send_Name(object sender, RoutedEventArgs e)
        {
            if (spotterGUI.MainService.status.right != Status.ONLINE)
            {
                TCPServer.Send(Messages.NAME, -1);
            }
            else
            {
                backup.Content = "Force Update";
                MainService._pcRight.Update();
            }

        }

        /// <summary>
        /// Continuous request turned on
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            if (continous_request.IsChecked == true)
            {
                Log.Instance.Write("continous is TRUE", Log.Priority.VERBOSE_DEBUG);
               
                contRequest.RequestContinuous = true;
                tab.IsChecked = false;
                //if (requestContinuousThread == null)
                //{
                //    requestContinuousThread = requestPhotoThread();
                //}

            }
        }

        /// <summary>
        /// unchecks continous; turns off continous requests
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void not_continous(object sender, RoutedEventArgs e)
        {
            contRequest.RequestContinuous  = false;
            Log.Instance.Write("check is FALSE", Log.Priority.VERBOSE_DEBUG);
            continous_request.IsChecked = false;

        }

        private void showButton_Click(object sender, RoutedEventArgs e)
        {
            if (!photoWatch.Recent)
            {
                Log.Instance.Write("show button clicked", Log.Priority.VERBOSE_DEBUG);
                // Configure open file dialog box
                Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
                dlg.FileName = "Image"; // Default file name
                dlg.DefaultExt = ".jpg"; // Default file extension
                dlg.Filter = "Images (.jpg)|*.jpg"; // Filter files by extension
                dlg.InitialDirectory = "C:\\RoboFTP";

                // Show open file dialog box
                Nullable<bool> result = dlg.ShowDialog();

                // Process open file dialog box results 
                if (result == true)
                {
                    // Open document 
                    string filename = dlg.FileName;
                    Uri _source = new Uri(filename);
                    BitmapFrame curr_image = BitmapFrame.Create(_source);
                    myImage1.Source = curr_image;
                    photoWatch.Last_Image = filename;
                    updateLabel(filename, name_time_r);
                }
            }
        }

        private void loadUIImage(string path)
        {
           
            myImage1.Dispatcher.Invoke(
                System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        try
                        {
                            string folder = path.Substring(11, 12);
                            Log.Instance.Write("folder: " + folder, Log.Priority.VERBOSE_DEBUG);
                            Uri _source = new Uri(path);

                            if (folder.Equals("0"))
                            {
                                myImage1.Source = BitmapFrame.Create(_source);
                                //photoWatch.Last_Image = MainService.latest_photo_right;
                                updateLabel(path, name_time_r);
                            }
                            else if (folder.Equals("1"))
                            {
                                middle_image.Source = BitmapFrame.Create(_source);
                                //photoWatch.Last_Image = MainService.latest_photo_right;
                                updateLabel(path, name_time_f);
                            }else if(folder.Equals("2"))
                            {
                                left_image.Source = BitmapFrame.Create(_source);
                                //photoWatch.Last_Image = MainService.latest_photo_right;
                                updateLabel(path, name_time_l);
                            }
                            else
                            {
                                Log.Instance.Write("wrong folder!", Log.Priority.INFO);
                            }
                        }
                        catch
                        {
                            Log.Instance.Write("path: " + path + " could not be loaded", Log.Priority.VERBOSE_DEBUG);
                        }
                    }
                    ));
        }

        //public void HandleLoadImage()
        //{
        //    string instring, inbuff;
        //    string inbuff = "";
        //    string[] pathString;
        //    while (true) //change to recent
        //    {
        //        photoWatch.MSGFlag.WaitOne();
        //        while (photoWatch.msgQueue.Count != 0)
        //        {
        //            try
        //            {
        //                lock (photoWatch._QLock)
        //                    inbuff = photoWatch.msgQueue.Dequeue();
        //            }
        //            catch
        //            {
        //                Console.WriteLine("HandleGPSstring Flag Misfire");
        //            }
        //            if (inbuff != null)
        //            {
        //                instring = inbuff;
        //                pathString = instring.Split();
        //                foreach (string item in pathString)
        //                {
        //                    //Console.WriteLine("parsing: " + item);
        //                    loadUIImage(item);
        //                }
        //            }
        //        }
        //    }
        //}

        
        


        /// <summary>
        /// Handles various specific key presses. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void HandleKeyDownEvent(object sender, KeyEventArgs e)
        {
            //Example
            //if (e.Key == Key.Tab && (Keyboard.Modifiers & (ModifierKeys.Control | ModifierKeys.Shift)) == (ModifierKeys.Control | ModifierKeys.Shift))
            //{
            //    MessageBox.Show("CTRL + SHIFT + TAB trapped");
            //}

            //if (e.Key == Key.Tab && (Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control)
            //{
            //    MessageBox.Show("CTRL + TAB trapped");
            //}
            
            
            if (e.Key == Key.Tab && (Keyboard.Modifiers & (ModifierKeys.Shift)) == (ModifierKeys.Shift))
            {
                Log.Instance.Write("q detected; previous picture requests", Log.Priority.VERBOSE_DEBUG);
                //decrement counter 
                i -= 1;
                //make sure counter is not our of range ( 0 < i < length PC list)
                if (i < 0)
                {
                    i = MainService._pcRight.photoList.Count() + i;
                }
                else{
                    i = i % (MainService._pcRight._photoList.Count());
                }

                //Choose Picture
                Photo currPhoto = MainService._pcRight._photoList.ElementAt(i);
                //Set image source
                myImage1.Source = currPhoto.Image;
                photoWatch.Last_Image = currPhoto.Source;
                updateLabel(currPhoto.Source, name_time_r);
            }
            else if (e.Key == Key.Tab)
            {
                Log.Instance.Write("w detected; next picture requests", Log.Priority.VERBOSE_DEBUG);
                //increment counter 
                i += 1;
                MainService._pcRight.Update();
                //make sure counter is not our of range
                if (MainService._pcRight._photoList.Count() > 0) {
                    i = i % (MainService._pcRight._photoList.Count());
                    Photo currPhoto = MainService._pcRight._photoList.ElementAt(i);
                    myImage1.Source = currPhoto.Image;
                    photoWatch.Last_Image = currPhoto.Source;
                    updateLabel(currPhoto.Source, name_time_r);
                }
                
                

            }
                  
            //change recent boolean
            else if (e.Key == Key.R && (Keyboard.Modifiers & (ModifierKeys.Shift)) == (ModifierKeys.Shift))
            {

                if (photoWatch.Recent == true)
                {
                    //Log.Instance.Write("recent set to false", Log.Priority.INFO);
                    RoutedEventArgs newEventArgs = new RoutedEventArgs(Button.ClickEvent);
                    startStopButton.RaiseEvent(newEventArgs);
                }
               
            }
            else if (e.Key == Key.R)
            {
                if (photoWatch.Recent == false)
                {
                    //Log.Instance.Write("recent set to true", Log.Priority.INFO);
                    RoutedEventArgs newEventArgs = new RoutedEventArgs(Button.ClickEvent);
                    startStopButton.RaiseEvent(newEventArgs);
                }
               
                
            }

        }

        /// <summary>
        /// Sends a requests to all cameras. and loads the last image
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RequestPhoto_R(object sender, RoutedEventArgs e)
        {
            contRequest.requestPhoto(0);
            contRequest.requestPhoto(1);
            contRequest.requestPhoto(2);
            //MainService._pc.Update();

            int index = (MainService._pcRight.Count() - 1);
            if (index > 0)
            {
                //Photo currPhoto = MainService._pc.getLastPhoto();
                Photo currPhotoR = MainService._pcRight.getLastPhoto();
                if (currPhotoR != null)
                {
                    myImage1.Source = currPhotoR.Image;
                    updateLabel(currPhotoR.Source, name_time_r);
                }
               
                Photo currPhotoL = MainService._pcLeft.getLastPhoto();
                if (currPhotoR != null)
                {
                    left_image.Source = currPhotoL.Image;
                    updateLabel(currPhotoL.Source, name_time_l);
                }
                
                Photo currPhotoF = MainService._pcFront.getLastPhoto();
                if (currPhotoF != null)
                {
                    middle_image.Source = currPhotoF.Image;
                    updateLabel(currPhotoF.Source, name_time_f);
                }
                
            }
        }

        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            //freqSlider
            double range = freqSlider.Value;
            Log.Instance.Write("Frequency Number: " + range, Log.Priority.VERBOSE_DEBUG);
            int time = (int) (range * 1000);
            time = Math.Max(time, 800);
            continous_request.Content = time.ToString() + "msec";
            contRequest.time_in_milliseconds = time;
        }

        private void updateLabel(string name, Label l)
        {
            try
            {
                
                l.Content = name.Substring(10);
            }catch
            {
                l.Content = name;
            }

            
        }

        private void StartOrStop(object sender, RoutedEventArgs e)
        {

        }



    }
}

