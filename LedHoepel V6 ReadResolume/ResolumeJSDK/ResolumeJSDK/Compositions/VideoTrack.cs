using System.Collections.Generic;
using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "VideoTrack")]
    public class VideoTrack
    {
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
        [XmlElement(ElementName = "RenderPass")]
        public RenderPass RenderPass { get; set; }
        [XmlElement(ElementName = "ChoosableMixer")]
        public List<ChoosableMixer> ChoosableMixer { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlElement(ElementName = "PrimarySource")]
        public PrimarySource PrimarySource { get; set; }
        [XmlAttribute(AttributeName = "manualDuration")]
        public string ManualDuration { get; set; }
    }
}