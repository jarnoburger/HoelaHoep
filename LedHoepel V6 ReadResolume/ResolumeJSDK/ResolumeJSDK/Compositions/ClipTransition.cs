using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "ClipTransition")]
    public class ClipTransition
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}