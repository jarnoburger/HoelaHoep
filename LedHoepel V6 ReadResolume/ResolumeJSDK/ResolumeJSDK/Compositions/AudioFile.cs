using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "AudioFile")]
    public class AudioFile
    {
        [XmlAttribute(AttributeName = "value")]
        public string Value { get; set; }
    }
}