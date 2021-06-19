using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "VideoFormatReaderSource")]
    public class VideoFormatReaderSource
    {
        [XmlAttribute(AttributeName = "fileName")]
        public string FileName { get; set; }
        [XmlAttribute(AttributeName = "LoadingThread")]
        public string LoadingThread { get; set; }
    }
}