using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "LayerView")]
    public class LayerView
    {
        [XmlElement(ElementName = "FoldParams")]
        public FoldParams FoldParams { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}