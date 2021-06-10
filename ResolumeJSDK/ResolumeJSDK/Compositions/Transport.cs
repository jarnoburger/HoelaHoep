using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "Transport")]
    public class Transport
    {
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}