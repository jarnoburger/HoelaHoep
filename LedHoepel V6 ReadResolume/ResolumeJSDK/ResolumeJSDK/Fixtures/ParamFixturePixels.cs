using System.Collections.Generic;
using System.Xml.Serialization;

namespace ResolumeJSDK.Fixtures
{
    [XmlRoot(ElementName = "ParamFixturePixels")]
    public class ParamFixturePixels
    {
        [XmlElement(ElementName = "ParamRange")]
        public List<ParamRange> ParamRange { get; set; }

        [XmlElement(ElementName = "ParamChoice")]
        public List<ParamChoice> ParamChoice { get; set; }

        [XmlAttribute(AttributeName = "storage")]
        public string Storage { get; set; }

        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}