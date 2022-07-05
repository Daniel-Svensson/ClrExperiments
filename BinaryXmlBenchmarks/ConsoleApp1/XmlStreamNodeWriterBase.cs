using System.Xml;
/// <summary>
///  Implement abstract members
/// </summary>
public abstract class XmlStreamNodeWriterBase : XmlStreamNodeWriter
	{
		public override void WriteDeclaration()
	{
		throw new NotImplementedException();
	}

	public override void WriteComment(string text)
	{
		throw new NotImplementedException();
	}

	public override void WriteCData(string text)
	{
		throw new NotImplementedException();
	}

	public override void WriteStartElement(string? prefix, string localName)
	{
		throw new NotImplementedException();
	}

	public override void WriteStartElement(string? prefix, XmlDictionaryString localName)
	{
		throw new NotImplementedException();
	}

	public override void WriteEndStartElement(bool isEmpty)
	{
		throw new NotImplementedException();
	}

	public override void WriteEndElement(string? prefix, string localName)
	{
		throw new NotImplementedException();
	}

	public override void WriteXmlnsAttribute(string? prefix, string ns)
	{
		throw new NotImplementedException();
	}

	public override void WriteXmlnsAttribute(string? prefix, XmlDictionaryString ns)
	{
		throw new NotImplementedException();
	}

	public override void WriteStartAttribute(string prefix, string localName)
	{
		throw new NotImplementedException();
	}

	public override void WriteStartAttribute(string prefix, XmlDictionaryString localName)
	{
		throw new NotImplementedException();
	}

	public override void WriteEndAttribute()
	{
		throw new NotImplementedException();
	}
	public override void WriteCharEntity(int ch)
	{
		throw new NotImplementedException();
	}

	public override void WriteEscapedText(string value)
	{
		throw new NotImplementedException();
	}

	public override void WriteEscapedText(XmlDictionaryString value)
	{
		throw new NotImplementedException();
	}

	public override void WriteEscapedText(char[] chars, int offset, int count)
	{
		throw new NotImplementedException();
	}

	public override void WriteEscapedText(byte[] buffer, int offset, int count)
	{
		throw new NotImplementedException();
	}

	public override void WriteText(string value)
	{
		throw new NotImplementedException();
	}

	public override void WriteText(XmlDictionaryString value)
	{
		throw new NotImplementedException();
	}

	public override void WriteText(char[] chars, int offset, int count)
	{
		throw new NotImplementedException();
	}

	public override void WriteText(byte[] buffer, int offset, int count)
	{
		throw new NotImplementedException();
	}

	public override void WriteBoolText(bool value)
	{
		throw new NotImplementedException();
	}

	public override void WriteFloatText(float value)
	{
		throw new NotImplementedException();
	}

	public override void WriteDoubleText(double value)
	{
		throw new NotImplementedException();
	}

	public override void WriteDecimalText(decimal value)
	{
		throw new NotImplementedException();
	}

	public override void WriteDateTimeText(DateTime value)
	{
		throw new NotImplementedException();
	}

	public override void WriteUniqueIdText(UniqueId value)
	{
		throw new NotImplementedException();
	}

	public override void WriteTimeSpanText(TimeSpan value)
	{
		throw new NotImplementedException();
	}

	public override void WriteGuidText(Guid value)
	{
		throw new NotImplementedException();
	}

	public override void WriteStartListText()
	{
		throw new NotImplementedException();
	}

	public override void WriteListSeparator()
	{
		throw new NotImplementedException();
	}

	public override void WriteEndListText()
	{
		throw new NotImplementedException();
	}

	public override void WriteBase64Text(byte[] trailBuffer, int trailCount, byte[] buffer, int offset, int count)
	{
		throw new NotImplementedException();
	}

	public override void WriteQualifiedName(string prefix, XmlDictionaryString localName)
	{
		throw new NotImplementedException();
	}
}
