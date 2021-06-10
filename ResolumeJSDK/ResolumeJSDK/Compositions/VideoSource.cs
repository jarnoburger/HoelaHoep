using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "VideoSource")]
    public class VideoSource
    {
        [XmlElement(ElementName = "RenderPass")]
        public RenderPass RenderPass { get; set; }
        [XmlElement(ElementName = "VideoFormatReaderSource")]
        public VideoFormatReaderSource VideoFormatReaderSource { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "width")]
        public string Width { get; set; }
        [XmlAttribute(AttributeName = "height")]
        public string Height { get; set; }
        [XmlAttribute(AttributeName = "type")]
        public string Type { get; set; }
    }
}