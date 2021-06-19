using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "PhaseSourceTimeline")]
    public class PhaseSourceTimeline
    {
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
        [XmlElement(ElementName = "ParamBasedDuration")]
        public ParamBasedDuration ParamBasedDuration { get; set; }
        [XmlElement(ElementName = "Beats_double")]
        public Beats_double Beats_double { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "globalSpeedEnabled")]
        public string GlobalSpeedEnabled { get; set; }
    }
}