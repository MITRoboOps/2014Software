using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace RoboOpsSpotterGUI
{
    class LoadLatestImg
    {
        public Thread thread;

        private int camID;
        private Location location;
        private string imgPath;

        public LoadLatestImg(int camid, Location loc, string path)
        {
            if (Globals.live[camid] == false)
                return;
            camID = camid;
            location = loc;
            imgPath = path;
            thread = new Thread(new ParameterizedThreadStart(this.run));

            thread.Start();  
        }
        private static bool IsFileLocked(string file)
        {
            FileStream stream = null;

            try
            {
                stream = File.Open(file, FileMode.Open);
            }
            catch (IOException)
            {
                return true;
            }
            finally
            {
                if (stream != null)
                    stream.Close();
            }

            //file is not locked
            return false;
        }
        public static BitmapImage loadImg(string path, bool wait)
        {
            try
            {
                int i = 0;
                while (IsFileLocked(path))
                {
                    i++;
                    Thread.Sleep(75);
                    if (i == 100 || wait == false)
                        return null;
                }
                BitmapImage myRetVal = null;
                if (path != null)
                {
                    BitmapImage image = new BitmapImage();
                    using (FileStream stream = File.OpenRead(path))
                    {
                        image.BeginInit();
                        image.CacheOption = BitmapCacheOption.OnLoad;
                        image.StreamSource = stream;
                        image.EndInit();
                    }
                    myRetVal = image;
                }
                return myRetVal;
            }
            catch
            {
                return null;
            }
        }
  
        void run(object num) {  
            //Console.WriteLine("Waiting for image: " + imgPath);
            
            Stopwatch timeElapsed = Stopwatch.StartNew(); 
            do {  
                Thread.Sleep(40);
                if (timeElapsed.ElapsedMilliseconds >= Globals.imageWaitTimeout)
                {
                    Console.WriteLine("Error retrieving " + imgPath + ". Thread timed out.");
                    return;
                }
                if(File.Exists(imgPath))
                {
                    Console.WriteLine("Found image " + imgPath + ". Latency: " + timeElapsed.ElapsedMilliseconds);
                    //Photo photo = new Photo(imgPath, location);
                    int i = 0;
                    while (IsFileLocked(imgPath))
                    {
                        i++;
                        Thread.Sleep(40);
                        if (i == 100)
                            return;
                    }
                    MainWindow.mainWind.Dispatcher.Invoke(
                      System.Windows.Threading.DispatcherPriority.Normal,
                      new Action(
                        delegate()
                        {
                            BitmapImage img = loadImg(imgPath, true);
                            if (img == null)
                                return;
                            string[] c = imgPath.Split(new char[]{'.', '/'});
                            switch(camID)
                            {
                                case 0:
                                    MainWindow.mainWind.imgRight.Source = img;
                                    MainWindow.mainWind.disp[0] = Convert.ToInt32(c[c.Length - 3]);
                                    MainWindow.mainWind.tCurrFileRight.Text = MainWindow.mainWind.disp[0].ToString();
                                    break;
                                case 1:
                                    MainWindow.mainWind.imgCenter.Source = img;
                                    MainWindow.mainWind.disp[1] = Convert.ToInt32(c[c.Length - 3]);
                                    MainWindow.mainWind.tCurrFileCenter.Text = MainWindow.mainWind.disp[1].ToString();
                                    break;
                                case 2:
                                    MainWindow.mainWind.imgLeft.Source = img;
                                    MainWindow.mainWind.disp[2] = Convert.ToInt32(c[c.Length - 3]);
                                    MainWindow.mainWind.tCurrFileLeft.Text = MainWindow.mainWind.disp[2].ToString();
                                    break;
                            }
                        }
                    ));
                    return;
                }
                
            } while(true);  
        } 
    }
}
