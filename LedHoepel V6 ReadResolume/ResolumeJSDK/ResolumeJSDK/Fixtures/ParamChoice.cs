using System.Xml.Serialization;

namespace ResolumeJSDK.Fixtures
{
    [XmlRoot(ElementName = "ParamChoice")]
    public class ParamChoice
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }

        [XmlAttribute(AttributeName = "default")]
        public string Default { get; set; }

        [XmlAttribute(AttributeName = "value")]
        public string Value { get; set; }

        [XmlAttribute(AttributeName = "storeChoices")]
        public string StoreChoices { get; set; }
    }
}