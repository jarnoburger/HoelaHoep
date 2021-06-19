using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "BehaviourDouble")]
    public class BehaviourDouble
    {
        [XmlElement(ElementName = "PhaseSourceTimeline")]
        public PhaseSourceTimeline PhaseSourceTimeline { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}