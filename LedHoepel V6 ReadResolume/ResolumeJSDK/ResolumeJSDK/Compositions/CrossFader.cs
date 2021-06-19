using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "CrossFader")]
    public class CrossFader
    {
        [XmlElement(ElementName = "AudioTrack")]
        public AudioTrack AudioTrack { get; set; }
        [XmlElement(ElementName = "VideoTrack")]
        public VideoTrack VideoTrack { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}