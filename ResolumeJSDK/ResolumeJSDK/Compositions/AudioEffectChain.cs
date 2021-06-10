using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "AudioEffectChain")]
    public class AudioEffectChain
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}