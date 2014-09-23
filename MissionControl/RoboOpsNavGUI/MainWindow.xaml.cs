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
using Microsoft.Maps.MapControl.WPF;
using Microsoft.Maps.MapControl.WPF.Design;
using System.Threading;

namespace RoboOpsNavGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static MainWindow MainWind;        
        Location locJSCCentered = new Location(29.564855, -95.081505);
        NMEAParse GPS = new NMEAParse();
        bool quit = false;
        bool viewStatic = false;

        Thread GPSParser;

        //Track
        MapPolyline GPSTrack = new MapPolyline();
        MapPolyline HeadingLine = new MapPolyline();
        MapPolygon polyArrow = new MapPolygon();
        Location locCurrent = new Location(0, 0);
        double headCurrent = 0; 

        //Draw Poly
        List<MapPolyline> PolyLineArray = new List<MapPolyline>();
        MapPolyline PolyLineCurrent = new MapPolyline();

        TCPCli tcpCli, statusCli;

        public MainWindow()
        {
            MainWind = this;
            InitializeComponent();

            GPS.PositionReceived += new NMEAParse.PositionReceivedEventHandler(GPS_PositionReceived);
            GPS.SatellitesInViewReceived += new NMEAParse.SatellitesInViewReceivedEventHandler(GPS_SatellitesInViewReceived);
            GPS.SatellitesUsed += new NMEAParse.SatellitesUsedReceivedEventHandler(GPS_SatellitesUsed);
            GPS.SpeedReceived += new NMEAParse.SpeedReceivedEventHandler(GPS_SpeedReceived);
            GPS.BearingReceived += new NMEAParse.BearingReceivedEventHandler(GPS_BearingReceived);
            GPS.FixLost += new NMEAParse.FixLostEventHandler(GPS_FixLost);
            GPS.FixObtained += new NMEAParse.FixObtainedEventHandler(GPS_FixObtained);
            GPS.HDOPReceived += new NMEAParse.HDOPReceivedEventHandler(GPS_HDOPReceived);
            GPS.EllipsoidHeightReceived += new NMEAParse.EllipsoidHeightReceivedEventHandler(GPS_EllipsoidHeightReceived);

            SerializablePolyLine pl = new SerializablePolyLine();
            pl = (SerializablePolyLine)xmlIO.loadSettings(typeof(SerializablePolyLine), "lasttrack.xml");

            if (pl != null)
                GPSTrack = pl.GetPolyLine();
            else
            {
                GPSTrack.Stroke = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Blue);
                GPSTrack.StrokeThickness = 2;
                GPSTrack.Opacity = 0.7;
                GPSTrack.Locations = new LocationCollection();
            }
            Map.Children.Add(GPSTrack);

            HeadingLine.Stroke = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Red);
            HeadingLine.StrokeThickness = 3;
            HeadingLine.Opacity = 0.5;
            HeadingLine.Locations = new LocationCollection();
            HeadingLine.Locations.Add(new Location(0,0));
            HeadingLine.Locations.Add(new Location(0,0));
            Map.Children.Add(HeadingLine);


            tcpCli = new TCPCli("192.168.0.1", 60660, MainWind.bConnectGPS, MainWind.tGPSConnStatus);
            tcpCli.StartClient();

            statusCli = new TCPCli("192.168.0.10", 9940, MainWind.bConnectStatus, MainWind.tStatusConnStatus);
            statusCli.StartClient();
            statusCli.debug = true;

            GPSParser = new Thread(new ThreadStart(HandleGPSstring));
            GPSParser.Start();

            loadPolyFromFile();
        }
      
        public void HandleGPSstring()
        {
            string instring, inbuff;
            string[] gpsString;
            inbuff = "";
            instring = "";
            while(!quit)
            {
                tcpCli.MSGFlag.WaitOne();
                while (tcpCli.msgQueue.Count != 0)
                {
                    try
                    {
                        lock (tcpCli._QLock)
                            inbuff = tcpCli.msgQueue.Dequeue();
                    }
                    catch
                    {
                        Console.WriteLine("HandleGPSstring Flag Misfire");
                    }
                    if (inbuff != null)
                    {
                       // if (inbuff.StartsWith("$"))
                        //{
                            instring = inbuff;
                       // }
                        //else
                       // {
                            //instring += inbuff;
                        //}
                        gpsString = instring.Split();
                        foreach (string item in gpsString)
                        {
                            //Console.WriteLine("parsing: " + item);
                            GPS.Parse(item);
                        }
                    }
                }
            }
        }

        private void bDropPin_Click(object sender, RoutedEventArgs e)
        {
            Pushpin pin = new Pushpin();
            pin.Location = locCurrent;
            Map.Children.Add(pin);
        }

        private void bViewStatic_Click(object sender, RoutedEventArgs e)
        {    
            viewStatic = true;
        }
        private void bViewFollow_Click(object sender, RoutedEventArgs e)
        {
            viewStatic = false;
        }
        private void bViewReset_Click(object sender, RoutedEventArgs e)
        {
            Map.SetView(locJSCCentered, 20);
        }

        private void savePoly()
        {
            PolyLineArray.Add(PolyLineCurrent);
            PolyLineCurrent = new MapPolyline();
            PolyLineCurrent.Stroke = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Goldenrod);
            PolyLineCurrent.StrokeThickness = 2;
            PolyLineCurrent.Opacity = 1;
            PolyLineCurrent.Locations = new LocationCollection();
            Map.Children.Add(PolyLineCurrent);
        }
        private void savePolyTofile()
        {
            List<SerializablePolyLine> pl = new List<SerializablePolyLine>();
            foreach(MapPolyline element in PolyLineArray)
            {
                SerializablePolyLine line = new SerializablePolyLine();
                line.color = ((SolidColorBrush)element.Stroke).Color.ToString();
                line.Opacity = element.Opacity;
                line.Thickness = element.StrokeThickness;
                foreach(Location l in element.Locations)
                {
                    Console.WriteLine("adding point");
                    line.points.Add(new SerializablePolyLine.Pt(l.Latitude, l.Longitude));
                }
                pl.Add(line);
            }
            xmlIO.createSettings(pl, typeof( List<SerializablePolyLine>), "polyline.xml");

        }
        private void loadPolyFromFile()
        {
            List<SerializablePolyLine> pl = new List<SerializablePolyLine>();
            pl = (List<SerializablePolyLine>)xmlIO.loadSettings(typeof(List<SerializablePolyLine>), "polyline.xml");
            PolyLineArray = new List<MapPolyline>();
            if(pl != null)
                foreach(SerializablePolyLine line in pl)
                {
                    PolyLineArray.Add(line.GetPolyLine());
                    Map.Children.Add(PolyLineArray.Last());
                }
            
            PolyLineCurrent.Stroke = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Gold);
            PolyLineCurrent.StrokeThickness = 2;
            PolyLineCurrent.Opacity = 1;
            PolyLineCurrent.Locations = new LocationCollection();
            Map.Children.Add(PolyLineCurrent);

        }

        private void LoadLastTrack_Click(object sender, RoutedEventArgs e)
        {
            Map.Children.Remove(GPSTrack);
            GPSTrack = new MapPolyline();
            GPSTrack.Stroke = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.Blue);
            GPSTrack.StrokeThickness = 2;
            GPSTrack.Opacity = 0.7;
            GPSTrack.Locations = new LocationCollection();
            Map.Children.Add(GPSTrack);
            
        }

        private void togPinOnClick_Click(object sender, RoutedEventArgs e)
        {
            if (togDrawPoly.IsChecked == true)
            {
                togDrawPoly.IsChecked = false;
                savePoly();
            }
        }
        private void togDrawPoly_Click(object sender, RoutedEventArgs e)
        {
            if (togPinOnClick.IsChecked == true)
                togPinOnClick.IsChecked = false;

            if (togDrawPoly.IsChecked == false)
                savePoly();

        }

        private void Map_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (togPinOnClick.IsChecked == true)
            {

                // Disables the default mouse double-click action.
                e.Handled = true;

                //Get the mouse click coordinates
                Point mousePosition = e.GetPosition(this);
                mousePosition.X -= 200;
                //Convert the mouse coordinates to a locatoin on the map
                Location pinLocation = Map.ViewportPointToLocation(mousePosition);

                // The pushpin to add to the map.
                Pushpin pin = new Pushpin();
                pin.Location = pinLocation;

                // Adds the pushpin to the map.
                Map.Children.Add(pin);
            }
            else if(togDrawPoly.IsChecked == true)
            {
                e.Handled = true;

                //Get the mouse click coordinates
                Point mousePosition = e.GetPosition(this);
                mousePosition.X -= 200;
                //Convert the mouse coordinates to a locatoin on the map
                Location l = Map.ViewportPointToLocation(mousePosition);

                // The pushpin to add to the map.
                PolyLineCurrent.Locations.Add(l);
            }
        }
        private void Map_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (togPinOnClick.IsChecked == true)
            {

                // Disables the default mouse double-click action.
                e.Handled = true;

                // Determin the location to place the pushpin at on the map.

                //Get the mouse click coordinates
                Point mousePosition = e.GetPosition(this);
                mousePosition.X -= 200;
                //Convert the mouse coordinates to a locatoin on the map
                Location mouseLoc = Map.ViewportPointToLocation(mousePosition);
                try
                {
                    foreach (UIElement element in Map.Children)
                    {
                        if (element.GetType() == typeof(Pushpin))
                        {
                            Pushpin pin = (Pushpin)element;
                            if (Math.Abs(pin.Location.Latitude - mouseLoc.Latitude) < 0.000015 && Math.Abs(pin.Location.Longitude - mouseLoc.Longitude) < 0.000015)
                            {
                                Map.Children.Remove(pin);
                            }  
                        }
                    }
                }
                catch { }
            }
            else if (togDrawPoly.IsChecked == true)
            {
                e.Handled = true;
                if (PolyLineCurrent.Locations.Last() != null)
                    PolyLineCurrent.Locations.Remove(PolyLineCurrent.Locations.Last());
            }
        }

        #region GPS data
        private void GPS_PositionReceived(string Lat, string Lon)
        {
            tLat.Dispatcher.Invoke(
              System.Windows.Threading.DispatcherPriority.Normal,
              new Action(
                delegate()
                {
                    tLat.Text = Lat;
                }
            ));
            tLat.Dispatcher.Invoke(
              System.Windows.Threading.DispatcherPriority.Normal,
              new Action(
                delegate()
                {
                    tLon.Text = Lon;
                }
            ));

            string[] temp = new string[3];
            try
            {
                temp = Lat.Split(new char[] { '"', '°' });
                float lat, lon;
                lat = float.Parse(temp[0]) + float.Parse(temp[1]) / 60;
                if (temp[2] == "S")
                    lat *= -1;

                temp = Lon.Split(new char[] { '"', '°' });
                lon = float.Parse(temp[0]) + float.Parse(temp[1]) / 60;
                if (temp[2] == "W")
                    lon *= -1;

                this.locCurrent = new Location(lat, lon);
            }
            catch(Exception e)
            {
                Console.WriteLine("Exception in GPS_PositionReceived: " + e.ToString());
            }

            if(!viewStatic) //Recenter view
            {
                 
                 string[] temp1 = new string[3];
               

                    GPSTrack.Dispatcher.Invoke(
                     System.Windows.Threading.DispatcherPriority.Normal,
                     new Action(
                       delegate()
                       {

                           GPSTrack.Locations.Add(this.locCurrent);
                       }
                   ));
                    
                    Map.Dispatcher.Invoke(
                      System.Windows.Threading.DispatcherPriority.Normal,
                      new Action(
                        delegate()
                        {

                            Map.SetView(this.locCurrent, Map.ZoomLevel);
                        }
                    ));
              
            }

            HeadingLine.Dispatcher.Invoke(
                     System.Windows.Threading.DispatcherPriority.Normal,
                     new Action(
                       delegate()
                       {
                           double lat, lon;
                           lat = locCurrent.Latitude + 0.0003 * Math.Cos((Math.PI / 180) * this.headCurrent);
                           lon = locCurrent.Longitude + 0.0003 * Math.Sin((Math.PI / 180) * this.headCurrent);
                           HeadingLine.Locations.RemoveAt(1);
                           HeadingLine.Locations.RemoveAt(0);
                           HeadingLine.Locations.Add(locCurrent);
                           Location headingLoc = new Location();
                           HeadingLine.Locations.Add(new Location(lat, lon));

                       }
                   ));
            
            //this.tLat.Text = Lat;
            //this.tLon.Text = Lon;
            /*
            //convert to OS grid
            if (OSGconv.ParseNMEA(Lat, Lon, ellipHeight))
            {
                //display decimal values of lat and lon
                textBox1.Text = Convert.ToString(OSGconv.deciLat);
                textBox2.Text = Convert.ToString(OSGconv.deciLon);
            }
             * */
        }
        private void GPS_SatellitesInViewReceived(int SatInView)
        {
            //textBoxSatInView.Text = Convert.ToString(SatInView);
        }
        private void GPS_SatellitesUsed(int SatInUse)
        {
            //textBoxSatInUse.Text = Convert.ToString(SatInUse);
        }
        private void GPS_SpeedReceived(double Speed)
        {
            tSpeed.Dispatcher.Invoke(
              System.Windows.Threading.DispatcherPriority.Normal,
              new Action(
                delegate()
                {
                    tSpeed.Text = Speed.ToString() +"[MPH]";
                }
            ));
        }
        private void GPS_BearingReceived(double Bearing)
        {
            this.headCurrent = Bearing;
            tHeading.Dispatcher.Invoke(
              System.Windows.Threading.DispatcherPriority.Normal,
              new Action(
                delegate()
                {
                    tHeading.Text = Bearing.ToString() + "°";
                }
            ));

            HeadingLine.Dispatcher.Invoke(
                     System.Windows.Threading.DispatcherPriority.Normal,
                     new Action(
                       delegate()
                       {
                           double lat, lon;
                           lat = locCurrent.Latitude + 0.0005*Math.Cos((Math.PI/180)*Bearing);
                           lon = locCurrent.Longitude + 0.0005 * Math.Sin((Math.PI / 180)*Bearing);
                           HeadingLine.Locations.RemoveAt(1);
                           HeadingLine.Locations.RemoveAt(0);
                           HeadingLine.Locations.Add(locCurrent);
                           Location headingLoc = new Location();
                           HeadingLine.Locations.Add(new Location(lat, lon));
                       }
                   ));




        }

        void GPS_FixLost()
        {
           //textBoxFix.Text = "Lost";
        }
        void GPS_FixObtained()
        {
            //textBoxFix.Text = "Obtained";
        }
        void GPS_HDOPReceived(double value)
        {
            //textBoxHDOP.Text = Convert.ToString(value);
        }
        void GPS_EllipsoidHeightReceived(double value)
        {
            //ellipHeight = value;
            //textBoxEllipsoidHeight.Text = Convert.ToString(ellipHeight);
        }

        #endregion

        private void bDisconnect_Click(object sender, RoutedEventArgs e)
        {
            tcpCli.CloseConnection();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            savePolyTofile();

            //Save Track
            SerializablePolyLine line = new SerializablePolyLine();
            line.color = ((SolidColorBrush)GPSTrack.Stroke).Color.ToString();
            line.Opacity = .3;
            line.Thickness = GPSTrack.StrokeThickness;
            foreach (Location l in GPSTrack.Locations)
            {
                line.points.Add(new SerializablePolyLine.Pt(l.Latitude, l.Longitude));
            }
            xmlIO.createSettings(line, typeof(SerializablePolyLine), "lasttrack.xml");

            quit = true;
            tcpCli.MSGFlag.Set();
        }

        private void bConnectGPS_Click(object sender, RoutedEventArgs e)
        {
            bConnectGPS.IsEnabled = false;
            if (tcpCli.status == 0)
                tcpCli.StartClient();
            else
                tcpCli.CloseConnection();
        }

        private void bConnectStatus_Click(object sender, RoutedEventArgs e)
        {
            bConnectStatus.IsEnabled = false;
            if (statusCli.status == 0)
                statusCli.StartClient();
            else
                statusCli.CloseConnection();
        }




    }
}
