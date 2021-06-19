using System.Xml.Serialization;

namespace ResolumeJSDK.Fixtures
{
    [XmlRoot(ElementName = "ParamRange")]
    public class ParamRange
    {
        [XmlElement(ElementName = "ValueRange")]
        public ValueRange ValueRange { get; set; }

        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }

        [XmlAttribute(AttributeName = "default")]
        public string Default { get; set; }

        [XmlAttribute(AttributeName = "value")]
        public string Value { get; set; }
    }
}