using Microsoft.Maps.MapControl.WPF;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace RoboOpsNavGUI
{
    [Serializable]
    public class SerializablePolyLine
    {
        public struct Pt
        {
            public Pt(double a, double b)
            {
                this.x = a;
                this.y = b;
            }
            public double x, y;
        }
        public List<Pt> points = new List<Pt>();
        public double Thickness;
        public double Opacity;
        //public System.Windows.Media.SolidColorBrush color;
        public string color;

        public SerializablePolyLine()
        {
        }


        public MapPolyline GetPolyLine()
        {
            MapPolyline line = new MapPolyline();
            //line.Stroke = color;
            line.Stroke = new System.Windows.Media.SolidColorBrush((Color)ColorConverter.ConvertFromString(color));
            line.StrokeThickness = 3;
            line.Opacity = 0.5;
            line.Locations = new LocationCollection();

            foreach (Pt point in points)
            {
                line.Locations.Add(new Location(point.x, point.y));
            }

            return line;
        }
    }
}
