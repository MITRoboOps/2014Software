using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Collections.ObjectModel;
using System.IO;

using System.Drawing;

namespace spotterGUI
{
    public class Img2
    {
        //private int counter = 0;
        ////const string path = @"C:\RoboFTP";
      
       

        //public Img2(string path){
        //    _path = path;
        //    _directoryInfo = new DirectoryInfo(path);
        //}

        ///// <summary>
        ///// Returns an array of fileInfo
        ///// </summary>
        ///// <returns></returns>
        //private FileInfo[] getFileInfo()
        //{
        //    FileInfo[] fileInfo = directoryInfo.GetFiles("*.jpg");

        //    return fileInfo;
        //}

        //public BitmapImage loadImg(int i)
        //{
        //    BitmapImage image = new BitmapImage();
            
        //    FileInfo[] fis = getFileInfo();
        //    int length = fis.Length;
        //    int index = i % length;//checks that it's not out of bounds
        //    FileInfo fi = (FileInfo) fis.GetValue(index);
        //    string path = fi.FullName.ToString();

        //    var uri = new System.Uri(path);
        //    var converted = uri.AbsoluteUri;

        //    image.BeginInit();
        //    image.UriSource = new Uri(converted);
        //    image.EndInit();
        //    return image;
        //}

        ///// <summary>
        ///// Returns an array of fileInfo
        ///// </summary>
        ///// <returns></returns>
        //private FileInfo[] getFileInfo(DirectoryInfo directoryInfo)
        //{
        //    FileInfo[] fileInfo = directoryInfo.GetFiles("*.jpg");
        //    return fileInfo;
        //}


        ///// <summary>
        ///// loads an image
        ///// </summary>
        ///// <param name="i"></param>
        ///// <param name="directoryInfo"></param>
        ///// <returns></returns>
        //private BitmapImage loadImg(int i, DirectoryInfo directoryInfo)
        //{


        //    FileInfo[] fis = getFileInfo(directoryInfo);
        //    int length = fis.Length;
        //    int index = i % length;//checks that it's not out of bounds
        //    FileInfo fi = (FileInfo)fis.GetValue(index);
        //    string path = fi.FullName.ToString();

        //    var uri = new System.Uri(path);
        //    var converted = uri.AbsoluteUri;

        //    BitmapImage image = new BitmapImage();
        //    image.BeginInit();
        //    image.UriSource = new Uri(converted);
        //    image.EndInit();
        //    return image;
        //}

        //public BitmapImage nextImg()
        //{
        //    counter += 1;
        //    return loadImg(counter, directoryInfo);
        //}

        //public BitmapImage prevImg()
        //{
        //    counter -= 1;
        //    return loadImg(counter, directoryInfo);
        //}

        
    }
}
