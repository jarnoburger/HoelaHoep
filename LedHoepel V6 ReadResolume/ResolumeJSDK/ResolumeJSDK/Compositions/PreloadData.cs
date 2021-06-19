using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "PreloadData")]
    public class PreloadData
    {
        [XmlElement(ElementName = "AudioFile")]
        public AudioFile AudioFile { get; set; }
        [XmlElement(ElementName = "VideoFile")]
        public VideoFile VideoFile { get; set; }
    }
}