using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "DeckInfo")]
    public class DeckInfo
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "id")]
        public string Id { get; set; }
        [XmlAttribute(AttributeName = "closed")]
        public string Closed { get; set; }
    }
}