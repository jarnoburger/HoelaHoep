/* 
 Licensed under the Apache License, Version 2.0

 http://www.apache.org/licenses/LICENSE-2.0
 */

using System;
using System.Xml.Serialization;
using ResolumeJSDK.Fixtures;
using Params = ResolumeJSDK.Compositions.Params;

namespace ResolumeJSDK
{
    [XmlRoot(ElementName = "Fixture")]
	public class Fixture
	{
		[XmlElement(ElementName = "Params")]
		public Params Params { get; set; }

		[XmlAttribute(AttributeName = "name")]
		public string Name { get; set; }

		[XmlAttribute(AttributeName = "uuid")]
		public string Uuid { get; set; }

		[XmlAttribute(AttributeName = "fixtureName")]
		public string FixtureName { get; set; }
	}

}
