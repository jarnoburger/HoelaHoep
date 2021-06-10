using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "Clip")]
    public class Clip
    {
        [XmlElement(ElementName = "PreloadData")]
        public PreloadData PreloadData { get; set; }
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
        [XmlElement(ElementName = "Transport")]
        public Transport Transport { get; set; }
        [XmlElement(ElementName = "ClipView")]
        public ClipView ClipView { get; set; }
        [XmlElement(ElementName = "AudioTrack")]
        public AudioTrack AudioTrack { get; set; }
        [XmlElement(ElementName = "VideoTrack")]
        public VideoTrack VideoTrack { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "uniqueId")]
        public string UniqueId { get; set; }
        [XmlAttribute(AttributeName = "layerIndex")]
        public string LayerIndex { get; set; }
        [XmlAttribute(AttributeName = "columnIndex")]
        public string ColumnIndex { get; set; }
    }
}