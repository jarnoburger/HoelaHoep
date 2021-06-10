using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "AudioFileSource")]
    public class AudioFileSource
    {
        [XmlAttribute(AttributeName = "FileName")]
        public string FileName { get; set; }
    }
}