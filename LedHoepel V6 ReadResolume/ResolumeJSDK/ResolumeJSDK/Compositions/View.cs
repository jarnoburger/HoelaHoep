using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "View")]
    public class View
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "bCanBeDisabled")]
        public string BCanBeDisabled { get; set; }
        [XmlAttribute(AttributeName = "bCanBeRemoved")]
        public string BCanBeRemoved { get; set; }
    }
}