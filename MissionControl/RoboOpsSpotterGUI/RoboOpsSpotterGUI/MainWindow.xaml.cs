using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RoboOpsSpotterGUI
{

    public struct Location
    {
        public int lat, lon;
    };

    public static class Globals
    {
        public const int imageWaitTimeout = 10000;
        //public static PhotoCollection[] photoCollect = new PhotoCollection[3];//This needs a lock
        public static bool[] live = new bool[3];
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public static MainWindow mainWind;

        Location locCurrent;

        private TCPCli[] tcpCli = new TCPCli[3]; // Socket connections
        private int[] index = new int[3];
        public int[] disp = new int[3];
        private int[] interval = new int[3];

        private static System.Timers.Timer _PhotoReqTimerLeft, _PhotoReqTimerCenter, _PhotoReqTimerRight; // From System.Timers

        public MainWindow()
        {
            InitializeComponent();
            mainWind = this;

            index[0] = 0; // Right
            index[1] = 0; // Center
            index[2] = 0; // Left

            disp[0] = 0; // Right
            disp[1] = 0; // Center
            disp[2] = 0; // Left

            interval[0] = 2000; // Right
            interval[1] = 2000; // Center
            interval[2] = 2000; // Left

            tcpCli[0] = new TCPCli("192.168.0.50", 1234, bConnectRight, tConnStatusRight);
            tcpCli[1] = new TCPCli("192.168.0.51", 1234, bConnectCenter, tConnStatusCenter);
            tcpCli[2] = new TCPCli("192.168.0.52", 1234, bConnectLeft, tConnStatusLeft);

            Globals.live[0] = true;
            Globals.live[1] = true;
            Globals.live[2] = true;

            /*Globals.photoCollect[0] = new PhotoCollection();
            Globals.photoCollect[1] = new PhotoCollection();
            Globals.photoCollect[2] = new PhotoCollection();
            */
            //this.imgRight.SizeMode = PictureBoxSizeMode.Zoom; 

            _PhotoReqTimerRight = new System.Timers.Timer(interval[0]);
            _PhotoReqTimerRight.Elapsed += new ElapsedEventHandler(_PhotoReqTimerRight_Elapsed);
            _PhotoReqTimerRight.AutoReset = true;
            _PhotoReqTimerRight.Enabled = true;

            _PhotoReqTimerCenter = new System.Timers.Timer(interval[1]);
            _PhotoReqTimerCenter.Elapsed += new ElapsedEventHandler(_PhotoReqTimerCenter_Elapsed);
            _PhotoReqTimerCenter.AutoReset = true;
            _PhotoReqTimerCenter.Enabled = true;

            _PhotoReqTimerLeft = new System.Timers.Timer(interval[2]);
            _PhotoReqTimerLeft.Elapsed += new ElapsedEventHandler(_PhotoReqTimerLeft_Elapsed);
            _PhotoReqTimerLeft.AutoReset = true;
            _PhotoReqTimerLeft.Enabled = true;

            AddHandler(Keyboard.KeyDownEvent, (KeyEventHandler)HandleKeyDownEvent);

        }

        private void bConnectRight_Click(object sender, RoutedEventArgs e)
        {
            tConnStatusRight.Text = "Connecting...";
            bConnectRight.IsEnabled = false;
            if (tcpCli[0].status == 0)
                tcpCli[0].StartClient();
            else
                tcpCli[0].CloseConnection();
        }
        private void bConnectCenter_Click(object sender, RoutedEventArgs e)
        {
            tConnStatusCenter.Text = "Connecting...";
            bConnectCenter.IsEnabled = false;
            if (tcpCli[1].status == 0)
                tcpCli[1].StartClient();
            else
                tcpCli[1].CloseConnection();
        }
        private void bConnectLeft_Click(object sender, RoutedEventArgs e)
        {
            tConnStatusLeft.Text = "Connecting...";
            bConnectLeft.IsEnabled = false;
            if (tcpCli[2].status == 0)
                tcpCli[2].StartClient();
            else
                tcpCli[2].CloseConnection();
        }
    
        private void _PhotoReqTimerLeft_Elapsed(object sender, ElapsedEventArgs e)
        {
            reqPhoto(2);
        }
        private void _PhotoReqTimerCenter_Elapsed(object sender, ElapsedEventArgs e)
        {
            reqPhoto(1);
        }
        private void _PhotoReqTimerRight_Elapsed(object sender, ElapsedEventArgs e)
        {
            reqPhoto(0);
        }

        private void reqPhoto(int camID)
        {
            if (tcpCli[camID].status == 2)
            {
                tcpCli[camID].Send("s." + index[camID] + ".");
                LoadLatestImg loadimg = new LoadLatestImg(camID, locCurrent, "c:/RoboFTP/" + camID + "/" + index[camID] + ".0.jpg");
                index[camID]++;
                Console.WriteLine("index[" + camID + "] = " + index[camID]);
            }
                //Console.WriteLine("Could not request photo of name: " + index[camID] + ".0.jpg from Camera: " + camID + " ERROR: not connected");
        }

        private void sFreqRight_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            try{
                interval[0] = (int)sFreqRight.Value;
                tFreqRight.Text = "Capture Rate: " + interval[0].ToString();
                _PhotoReqTimerRight.Interval = interval[0];
            }
            catch { }
        }
        private void sFreqCenter_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            try{
                interval[1] = (int)sFreqCenter.Value;
                tFreqCenter.Text = "Capture Rate: " + interval[1].ToString();
                _PhotoReqTimerCenter.Interval = interval[1];

            }
            catch { }
        }
        private void sFreqLeft_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            try
            {
                interval[2] = (int)sFreqLeft.Value;
                tFreqLeft.Text = "Capture Rate: " + interval[2].ToString();
                _PhotoReqTimerLeft.Interval = interval[2];

            }
            catch { }

        }

        private void HandleKeyDownEvent(object sender, KeyEventArgs e)
        {
            string filename;
            string[] c;
            int i;
            switch(e.Key)
            {
                case Key.A: //Left cam Back
                    i = 2;
                    do
                    {
                        disp[i]--;
                        filename = "C:/RoboFTP/"+i+"/" + disp[i] + ".0.jpg";
                    } while ((!File.Exists(filename) && disp[i] >= 0));
                    Console.WriteLine("BAK Attempting to open:" + filename);
                    if (!File.Exists(filename))
                        return;
                   // if (disp[i] > index[i])
                    //    index[i] = disp[i];
                    imgLeft.Source = LoadLatestImg.loadImg(filename, false);
                    c = filename.Split(new char[] { '.', '/' });
                    disp[i] = Convert.ToInt32(c[c.Length - 3]);
                    MainWindow.mainWind.tCurrFileLeft.Text = disp[i].ToString();
                    Globals.live[i] = false;
                    tLiveLeft.Text = "REVIEW";
                    break;
                case Key.S: //Left cam live
                    i = 2;
                    Globals.live[i] = true;
                    tLiveLeft.Text = "LIVE";
                    break;
                case Key.D: //Left cam Forward
                    i = 2;
                    do
                    {
                        disp[i]++;
                        filename = "C:/RoboFTP/" + i + "/" + disp[i] + ".0.jpg";
                    } while ((!File.Exists(filename) && disp[i] <= index[i]));
                    Console.WriteLine("FWD Attempting to open:" + filename);
                    if (!File.Exists(filename))
                        return;
                    imgLeft.Source = LoadLatestImg.loadImg(filename, false);
                    c = filename.Split(new char[] { '.', '/' });
                    disp[i] = Convert.ToInt32(c[c.Length - 3]);
                    tCurrFileLeft.Text = disp[i].ToString();
                    Globals.live[i] = false;
                    tLiveLeft.Text = "REVIEW";
                    break;
                #region center
                case Key.J: //Center cam Back
                    i = 1;
                    do
                    {
                        disp[i]--;
                        filename = "C:/RoboFTP/" + i + "/" + disp[i] + ".0.jpg";
                    } while ((!File.Exists(filename) && disp[i] >= 0));
                    Console.WriteLine("BAK Attempting to open:" + filename);
                    if (!File.Exists(filename))
                        return;
                    imgCenter.Source = LoadLatestImg.loadImg(filename, false);
                    c = filename.Split(new char[] { '.', '/' });
                    disp[i] = Convert.ToInt32(c[c.Length - 3]);
                    tCurrFileCenter.Text = disp[i].ToString();
                    tLiveCenter.Text = "REVIEW";
                    Globals.live[i] = false;
                    break;
                case Key.K: //Center cam live
                    i = 1;
                    tLiveCenter.Text = "LIVE";
                    Globals.live[i] = true;
                    break;
                case Key.L: //Center cam Forward
                    i = 1;
                    do
                    {
                        disp[i]++;
                        filename = "C:/RoboFTP/" + i + "/" + disp[i] + ".0.jpg";
                    } while ((!File.Exists(filename) && disp[i] <= index[i]));
                    Console.WriteLine("FWD Attempting to open:" + filename);
                    if (!File.Exists(filename))
                        return;
                    imgCenter.Source = LoadLatestImg.loadImg(filename, false);
                    c = filename.Split(new char[] { '.', '/' });
                    disp[i] = Convert.ToInt32(c[c.Length - 3]);
                    tCurrFileCenter.Text = disp[i].ToString();
                    Globals.live[i] = false;
                    tLiveCenter.Text = "REVIEW";
                    break;
                #endregion
                #region Right
                case Key.NumPad4: //Right cam Back
                    i = 0;
                    do
                    {
                        disp[i]--;
                        filename = "C:/RoboFTP/" + i + "/" + disp[i] + ".0.jpg";
                    } while ((!File.Exists(filename) && disp[i] >= 0));
                    Console.WriteLine("BAK Attempting to open:" + filename);
                    if (!File.Exists(filename))
                        return;
                    imgRight.Source = LoadLatestImg.loadImg(filename, false);
                    c = filename.Split(new char[] { '.', '/' });
                    disp[i] = Convert.ToInt32(c[c.Length - 3]);
                    tCurrFileRight.Text = disp[i].ToString();
                    tLiveRight.Text = "REVIEW";
                    Globals.live[i] = false;
                    break;
                case Key.NumPad5: //Right cam live
                    i = 0;
                    tLiveRight.Text = "LIVE";
                    Globals.live[i] = true;
                    break;
                case Key.NumPad6: //Right cam Forward
                    i = 0;
                    do
                    {
                        disp[i]++;
                        filename = "C:/RoboFTP/" + i + "/" + disp[i] + ".0.jpg";
                    } while ((!File.Exists(filename) && disp[i] <= index[i]));
                    Console.WriteLine("FWD Attempting to open:" + filename);
                    if (!File.Exists(filename))
                        return;
                    imgRight.Source = LoadLatestImg.loadImg(filename, false);
                    c = filename.Split(new char[] { '.', '/' });
                    disp[i] = Convert.ToInt32(c[c.Length - 3]);
                    tCurrFileRight.Text = disp[i].ToString();
                    Globals.live[i] = false;
                    tLiveRight.Text = "REVIEW";
                    break;
                #endregion
            }
        }

    }

}
