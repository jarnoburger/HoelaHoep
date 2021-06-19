/* 
 Licensed under the Apache License, Version 2.0

 http://www.apache.org/licenses/LICENSE-2.0
 */

using System.Collections.Generic;
using System.Xml.Serialization;
using ResolumeJSDK.Compositions;

namespace ResolumeJSDK
{
    [XmlRoot(ElementName = "Composition")]
	public class Composition
	{
		[XmlElement(ElementName = "versionInfo")]
		public VersionInfo VersionInfo { get; set; }
		[XmlElement(ElementName = "CompositionInfo")]
		public CompositionInfo CompositionInfo { get; set; }
		[XmlElement(ElementName = "Params")]
		public Params Params { get; set; }
		[XmlElement(ElementName = "CompositionView")]
		public CompositionView CompositionView { get; set; }
		[XmlElement(ElementName = "AudioTrack")]
		public AudioTrack AudioTrack { get; set; }
		[XmlElement(ElementName = "VideoTrack")]
		public VideoTrack VideoTrack { get; set; }
		[XmlElement(ElementName = "Layer")]
		public List<Layer> Layer { get; set; }
		[XmlElement(ElementName = "Column")]
		public List<Column> Column { get; set; }
		[XmlElement(ElementName = "CrossFader")]
		public CrossFader CrossFader { get; set; }
		[XmlElement(ElementName = "TempoController")]
		public TempoController TempoController { get; set; }
		[XmlElement(ElementName = "Deck")]
		public List<Deck> Deck { get; set; }
		[XmlAttribute(AttributeName = "name")]
		public string Name { get; set; }
		[XmlAttribute(AttributeName = "uniqueId")]
		public string UniqueId { get; set; }
		[XmlAttribute(AttributeName = "numDecks")]
		public string NumDecks { get; set; }
		[XmlAttribute(AttributeName = "currentDeckIndex")]
		public string CurrentDeckIndex { get; set; }
		[XmlAttribute(AttributeName = "numLayers")]
		public string NumLayers { get; set; }
		[XmlAttribute(AttributeName = "numColumns")]
		public string NumColumns { get; set; }
		[XmlAttribute(AttributeName = "compositionIsRelative")]
		public string CompositionIsRelative { get; set; }
	}

}
