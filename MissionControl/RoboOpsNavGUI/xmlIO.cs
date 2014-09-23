using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RoboOpsNavGUI
{
    class xmlIO
    {
        #region Properties
        private static string path = "save/";
        #endregion
        #region Methods
        /// <summary>
        /// Load a settings file.
        /// </summary>
        /// <param name="configType">type of the configuration struct</param>
        /// <param name="filename">config filename</param>
        /// <returns>struct of type configtype</returns>
        public static object loadSettings(Type configType, string filename)
        {
            object config;
            System.IO.StreamReader file = null;
            try
            {
                if (!System.IO.File.Exists(path + filename))
                {
                    Console.WriteLine("file not found");
                    return null;
                }
                System.Xml.Serialization.XmlSerializer reader =
                        new System.Xml.Serialization.XmlSerializer(configType);
                file = new System.IO.StreamReader(path + filename);
                config = reader.Deserialize(file);
                return config;
            }
            catch (Exception e)
            {
                Console.WriteLine("xml err: " + e.ToString());
                return null;
            }
            finally
            {
                if (file != null)
                    file.Close();
            }
        }
        /// <summary>
        /// Create a new config file in the event a config file is missing
        /// </summary>
        /// <param name="config">The config object (defaul config) to write</param>
        /// <param name="configType">The config struct type</param>
        /// <param name="filename">config filename</param>
        public static void createSettings(object config, Type configType, string filename)
        {
            System.Xml.Serialization.XmlSerializer writer;
            System.IO.StreamWriter file = null;
            
            try
            {
                writer =
                    new System.Xml.Serialization.XmlSerializer(configType);
                if (!System.IO.Directory.Exists(path))
                    System.IO.Directory.CreateDirectory(path);
                file = new System.IO.StreamWriter(path + filename);
                writer.Serialize(file, config);
            }
            catch (Exception e)
            {
                Console.WriteLine("xml write error: " + e.ToString());
            }
            finally
            {
                if (file != null)
                    file.Close();
            }
            return;
        }
        #endregion
    }
}
