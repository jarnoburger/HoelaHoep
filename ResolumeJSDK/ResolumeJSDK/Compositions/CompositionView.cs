using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "CompositionView")]
    public class CompositionView
    {
        [XmlElement(ElementName = "FoldParams")]
        public FoldParams FoldParams { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}