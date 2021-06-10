using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "ClipView")]
    public class ClipView
    {
        [XmlElement(ElementName = "FoldParams")]
        public FoldParams FoldParams { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}