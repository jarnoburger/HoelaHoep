using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "PrimarySource")]
    public class PrimarySource
    {
        [XmlElement(ElementName = "AudioFileSource")]
        public AudioFileSource AudioFileSource { get; set; }
        [XmlElement(ElementName = "VideoSource")]
        public VideoSource VideoSource { get; set; }
    }
}