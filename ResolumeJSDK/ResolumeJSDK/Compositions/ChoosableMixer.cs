using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "ChoosableMixer")]
    public class ChoosableMixer
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
        [XmlElement(ElementName = "RenderPass")]
        public RenderPass RenderPass { get; set; }
    }
}