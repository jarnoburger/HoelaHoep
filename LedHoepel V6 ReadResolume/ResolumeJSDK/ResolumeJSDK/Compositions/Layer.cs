using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "Layer")]
    public class Layer
    {
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
        [XmlElement(ElementName = "ClipTransition")]
        public ClipTransition ClipTransition { get; set; }
        [XmlElement(ElementName = "LayerView")]
        public LayerView LayerView { get; set; }
        [XmlElement(ElementName = "AudioTrack")]
        public AudioTrack AudioTrack { get; set; }
        [XmlElement(ElementName = "VideoTrack")]
        public VideoTrack VideoTrack { get; set; }
        [XmlAttribute(AttributeName = "uniqueId")]
        public string UniqueId { get; set; }
        [XmlAttribute(AttributeName = "layerIndex")]
        public string LayerIndex { get; set; }
    }
}