using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "ParamBasedDuration")]
    public class ParamBasedDuration
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
    }
}