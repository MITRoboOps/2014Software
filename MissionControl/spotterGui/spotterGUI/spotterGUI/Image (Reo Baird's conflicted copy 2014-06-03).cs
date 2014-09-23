using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Collections.ObjectModel;
using System.IO;


namespace spotterGUI
{
    /// <summary>
    /// This class describes a single photo - its location, the image and 
    /// the metadata extracted from the image.
    /// </summary>
    public class Photo
    {
        /// <summary>
        /// constructor for photo
        /// </summary>
        /// <param name="path"></param>
        public Photo(string path)
        {
            _path = path;
            
            try
            {
                _source = new Uri(path);
                _image = BitmapFrame.Create(_source);
            }
            catch{
                MainService._pcRight.Remove(this);
            }
        }

        public override string ToString()
        {
            return _source.ToString();          
        }

        private string _path;

        private Uri _source;
        public string Source { get { return _path; } }

        private BitmapFrame _image;
        public BitmapFrame Image { get { return _image; } set { _image = value; } }

    }

    /// <summary>
    /// This class represents a collection of photos in a directory.
    /// </summary>
    public class PhotoCollection : ObservableCollection<Photo>
    {
        public PhotoCollection() { }

        public PhotoCollection(string path) : this(new DirectoryInfo(path)) { }


        //public Dictionary<Photo, > photos = new HashMap<Photo>();
        //Todo create class to hold time and location

        public List<Photo> _photoList = new List<Photo>();
        public Dictionary<string, DateTime> _time_discovered = new Dictionary<string, DateTime>(); 

        public List<Photo> photoList { get { return _photoList; } }
        public Dictionary<string, DateTime> time_discovered {get{return _time_discovered;}}
        /// <summary>
        /// constructor for photoCollection
        /// </summary>
        /// <param name="directory"></param>
        public PhotoCollection(DirectoryInfo directory)
        {
            
            _directory = directory;
            Update();
        }


        public string Path
        {
            set
            {
                _directory = new DirectoryInfo(value);
                Update();
            }
            get { return _directory.FullName; }
        }

        public DirectoryInfo Directory
        {
            set
            {
                _directory = value;
                Update();
            }
            get { return _directory; }
        }
        /// <summary>
        /// Updates the directory. Resets the directory and loads all images.
        /// </summary>
        public void Update()
        {
            Update_helper();
            //if (MainService.number_of_photos < photoList.Count())
            //{
            //    MainService.number_of_photos = photoList.Count();
            //}
            //while (MainService.number_of_photos > photoList.Count())
            //{
            //    Update_helper();
               
            //}
            //Log.Instance.Write("Total photos in Collection: " + MainService.number_of_photos, Log.Priority.VERBOSE_DEBUG);
            return;
        }

        //not done yet
        public void lastHundredPhotos()
        {
            foreach (FileInfo f in _directory.GetFiles("*.jpg"))
            {
                Add(new Photo(f.FullName));//add back in
                photoList.Add(new Photo(f.FullName));
            }
        }


        /// <summary>
        /// Given a filename(path checks if photo is in Directory)
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        public Boolean hasPhoto(string path)
        {

            if (path == null )
            {
                Log.Instance.Write("null photo what??", Log.Priority.VERBOSE_DEBUG);
                return false;
            }

            if (photoList == null || photoList.Count() == 0)
            {return false;}

            foreach (Photo p in photoList)
            {
                if (path.Equals(p.Source))
                {
                    Log.Instance.Write(path + " is founds", Log.Priority.VERBOSE_DEBUG);
                    return true; }
            }
            return false;
        }

        /// <summary>
        /// returns the last photo to be written in the directory. 
        /// </summary>
        /// <returns></returns>
        public Photo getLastPhoto(){
            Log.Instance.Write("Directory Path: " + this.Path, Log.Priority.VERBOSE_DEBUG);
            try
            {  
                var myFile = _directory.GetFiles("*.jpg").OrderByDescending(f => f.LastWriteTime).First();
                string filename = myFile.Name; //this is wrong...
                Log.Instance.Write("last recieved photo is: " + filename, Log.Priority.VERBOSE_DEBUG);
                return new Photo(filename);
            }
            catch
            {
                return null;
            }
               
        }

        ///// <summary>
        ///// some of the most recent photos are still writing. This is the 3rd most recent photo. 
        ///// </summary>
        ///// <returns></returns>
        //public Photo get3rdLastPhoto()
        //{
        //    Array files = _directory.GetFiles("*.jpg")
        //     .OrderByDescending(x => x.LastWriteTime)
        //     .Take(3)
        //     .ToArray();
        //    if (files.Length > 0)
        //    {
        //        var file = files.GetValue(0); //changed to second
        //        string filename = file.
        //        Log.Instance.Write("2nd Last: " + filename, Log.Priority.VERBOSE_DEBUG);
        //        return new Photo(filename);
        //    }else
        //    {
        //        return null;
        //    }
        //}

        private void Update_helper()
        {
            this.Clear(); //reset
            photoList.Clear();
            try
            {

                foreach (FileInfo f in _directory.GetFiles("*.jpg"))
                {
                    Add(new Photo(f.FullName));//add back in
                    photoList.Add(new Photo(f.FullName));
                }
                //Log.Instance.Write("Expected: " + MainService.number_of_photos + "Current: " + photoList.Count(), Log.Priority.VERBOSE_DEBUG);

            }
            catch (DirectoryNotFoundException)
            {
                System.Windows.MessageBox.Show("No Such Directory");
            }
        }

        DirectoryInfo _directory;

        /// <summary>
        /// checks if photo exists, if not tries to add for 2 seconds; 
        /// returns true if photo already exists/ photo succesfully added; else false
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        public bool addPhoto(string path)
        {
            if (hasPhoto(path))
            {
                Log.Instance.Write("already in photo collection", Log.Priority.VERBOSE_DEBUG);
                return true;
            }
            bool stopTrying = false;
            DateTime time = DateTime.Now;
            TimeSpan timeout = new TimeSpan(0,0,0,0,2000); 

            while (!stopTrying)
            {
                try {
                    Add(new Photo(path));
                    photoList.Add(new Photo(path));
                    Log.Instance.Write("Photo found", Log.Priority.VERBOSE_DEBUG);
                    stopTrying = true;
                    return true;
                }
                catch (Exception ex)
                {
                    if (DateTime.Now.Subtract(time).Milliseconds > timeout.TotalMilliseconds)
                        {
                            Log.Instance.Write("Photo not found", Log.Priority.VERBOSE_DEBUG);
                            stopTrying = true;
                            return false;
                            //throw;
                        }
                }
            }
            //shouldn't get here
            return false;
            
           
        }

        

    }
}
