using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "TempoController")]
    public class TempoController
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}