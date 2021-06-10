using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "Column")]
    public class Column
    {
        [XmlAttribute(AttributeName = "uniqueId")]
        public string UniqueId { get; set; }
        [XmlAttribute(AttributeName = "columnIndex")]
        public string ColumnIndex { get; set; }
    }
}