using System.Collections.Generic;
using System.Xml.Serialization;

namespace ResolumeJSDK.Compositions
{
    [XmlRoot(ElementName = "Params")]
    public class Params
    {
        [XmlElement(ElementName = "Param")]
        public List<Param> Param { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlElement(ElementName = "ParamRange")]
        public List<ParamRange> ParamRange { get; set; }
        [XmlElement(ElementName = "ParamChoice")]
        public ParamChoice ParamChoice { get; set; }
    }
}