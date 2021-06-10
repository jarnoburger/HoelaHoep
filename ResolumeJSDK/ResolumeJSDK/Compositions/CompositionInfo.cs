using System.Collections.Generic;
using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "CompositionInfo")]
    public class CompositionInfo
    {
        [XmlElement(ElementName = "DeckInfo")]
        public List<DeckInfo> DeckInfo { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "description")]
        public string Description { get; set; }
        [XmlAttribute(AttributeName = "width")]
        public string Width { get; set; }
        [XmlAttribute(AttributeName = "height")]
        public string Height { get; set; }
    }
}