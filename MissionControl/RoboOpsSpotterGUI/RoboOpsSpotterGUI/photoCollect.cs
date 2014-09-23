/*using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace RoboOpsSpotterGUI
{
    /// <summary>
    /// This class describes a single photo - its location, the image and 
    /// the metadata extracted from the image.
    /// </summary>
    public class Photo
    {
        private string _path;

        private Uri _source;
        public string Source { get { return _path; } }
        public Location location;

        private BitmapFrame _image;
        public BitmapFrame Image { get { return _image; } set { _image = value; } }

        private bool IsFileLocked(string file)
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

        /// <summary>
        /// constructor for photo
        /// </summary>
        /// <param name="path"></param>
        public Photo(string path, Location loc, bool wait)
        {
            location = loc;
            int i = 0;
            while(IsFileLocked(path))
            {
                i++;
                Thread.Sleep(100);
                if(i==100 || !wait)
                    return;
            }
            _path = path;
            try
            {
                _source = new Uri(path);
               _image = BitmapFrame.Create(_source);
            }
            catch
            {
                Console.WriteLine("Could not create BigmapFrame from file: " + path);
            }
        }

        public override string ToString()
        {
            return _source.ToString();
        }

    }

    /// <summary>
    /// This class represents a collection of photos in a directory.
    /// </summary>
    public class PhotoCollection : ObservableCollection<Photo>
    {

        public List<Photo> _photoList = new List<Photo>();
        public Dictionary<string, DateTime> _time_discovered = new Dictionary<string, DateTime>();

        /// <summary>
        /// constructor for photoCollection
        /// </summary>
        /// <param name="directory"></param>
        public PhotoCollection()
        {
        }

    }
}
*/