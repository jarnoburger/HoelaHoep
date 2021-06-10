using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "Deck")]
    public class Deck
    {
        [XmlElement(ElementName = "Params")]
        public Params Params { get; set; }
        [XmlElement(ElementName = "Clip")]
        public Clip Clip { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "uniqueId")]
        public string UniqueId { get; set; }
        [XmlAttribute(AttributeName = "closed")]
        public string Closed { get; set; }
        [XmlAttribute(AttributeName = "numLayers")]
        public string NumLayers { get; set; }
        [XmlAttribute(AttributeName = "numColumns")]
        public string NumColumns { get; set; }
        [XmlAttribute(AttributeName = "deckIndex")]
        public string DeckIndex { get; set; }
    }
}