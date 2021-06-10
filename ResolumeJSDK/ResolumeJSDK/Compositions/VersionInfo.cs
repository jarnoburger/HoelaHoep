using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "versionInfo")]
    public class VersionInfo
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "majorVersion")]
        public string MajorVersion { get; set; }
        [XmlAttribute(AttributeName = "minorVersion")]
        public string MinorVersion { get; set; }
        [XmlAttribute(AttributeName = "microVersion")]
        public string MicroVersion { get; set; }
        [XmlAttribute(AttributeName = "revision")]
        public string Revision { get; set; }
    }
}