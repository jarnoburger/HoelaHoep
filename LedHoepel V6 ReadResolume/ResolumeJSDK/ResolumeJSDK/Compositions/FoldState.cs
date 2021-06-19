using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "FoldState")]
    public class FoldState
    {
        [XmlAttribute(AttributeName = "component")]
        public string Component { get; set; }
        [XmlAttribute(AttributeName = "folded")]
        public string Folded { get; set; }
    }
}