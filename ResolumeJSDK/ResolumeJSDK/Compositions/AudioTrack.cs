using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "AudioTrack")]
    public class AudioTrack
    {
        [XmlElement(ElementName = "AudioEffectChain")]
        public AudioEffectChain AudioEffectChain { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlElement(ElementName = "PrimarySource")]
        public PrimarySource PrimarySource { get; set; }
    }
}