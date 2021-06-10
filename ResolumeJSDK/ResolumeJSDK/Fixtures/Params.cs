using System.Xml.Serialization;

namespace ResolumeJSDK.Fixtures
{
    [XmlRoot(ElementName = "Params")]
    public class Params
    {
        [XmlElement(ElementName = "ParamFixturePixels")]
        public ParamFixturePixels ParamFixturePixels { get; set; }

        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}