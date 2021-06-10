using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "FoldParams")]
    public class FoldParams
    {
        [XmlElement(ElementName = "FoldState")]
        public FoldState FoldState { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}