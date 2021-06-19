using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "Beats_double")]
    public class Beats_double
    {
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "mode")]
        public string Mode { get; set; }
        [XmlAttribute(AttributeName = "numDetectedBeats")]
        public string NumDetectedBeats { get; set; }
        [XmlAttribute(AttributeName = "numManualBeats")]
        public string NumManualBeats { get; set; }
        [XmlAttribute(AttributeName = "detectedTempo")]
        public string DetectedTempo { get; set; }
        [XmlAttribute(AttributeName = "manualTempo")]
        public string ManualTempo { get; set; }
        [XmlAttribute(AttributeName = "detected")]
        public string Detected { get; set; }
    }
}