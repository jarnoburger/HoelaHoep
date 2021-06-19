using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "RenderPass")]
    public class RenderPass
    {
        [XmlElement(ElementName = "View")]
        public View View { get; set; }
        [XmlAttribute(AttributeName = "storage")]
        public string Storage { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "type")]
        public string Type { get; set; }
        [XmlAttribute(AttributeName = "uniqueId")]
        public string UniqueId { get; set; }
        [XmlAttribute(AttributeName = "baseType")]
        public string BaseType { get; set; }
        [XmlAttribute(AttributeName = "index")]
        public string Index { get; set; }
        [XmlElement(ElementName = "RenderPass")]
        public RenderPass RenderPass { get; set; }
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
    }
}