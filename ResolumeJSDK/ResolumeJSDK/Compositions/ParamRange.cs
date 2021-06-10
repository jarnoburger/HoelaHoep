using System.Collections.Generic;
using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "ParamRange")]
    public class ParamRange
    {
        [XmlAttribute(AttributeName = "storage")]
        public string Storage { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "default")]
        public string Default { get; set; }
        [XmlAttribute(AttributeName = "value")]
        public string Value { get; set; }
        [XmlElement(ElementName = "ValueRange")]
        public List<ValueRange> ValueRange { get; set; }
        [XmlElement(ElementName = "BehaviourDouble")]
        public BehaviourDouble BehaviourDouble { get; set; }
    }
}