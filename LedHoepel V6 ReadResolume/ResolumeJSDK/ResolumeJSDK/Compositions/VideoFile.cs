using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "VideoFile")]
    public class VideoFile
    {
        [XmlAttribute(AttributeName = "value")]
        public string Value { get; set; }
    }
}