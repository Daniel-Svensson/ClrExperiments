using System.Xml;

public class SpanWriter : XmlStreamNodeWriterBase
{
	private IXmlDictionary? _dictionary;
	private XmlBinaryWriterSession? _session;
	private bool _inAttribute;
	private bool _inList;
	private bool _wroteAttributeValue;
	private const int maxBytesPerChar = 3;
	private int _textNodeOffset;

	private void WriteNode(XmlBinaryNodeType nodeType)
	{
		WriteByte((byte)nodeType);
		_textNodeOffset = -1;
	}

	private void WroteAttributeValue()
	{
		if (_wroteAttributeValue && !_inList)
			throw System.Runtime.Serialization.DiagnosticUtility.ExceptionUtility.ThrowHelperError(new InvalidOperationException(SR.XmlOnlySingleValue));
		_wroteAttributeValue = true;
	}

	private void WriteTextNode(XmlBinaryNodeType nodeType)
	{
		if (_inAttribute)
			WroteAttributeValue();
		WriteByte((byte)nodeType);
		_textNodeOffset = this.BufferOffset - 1;
	}

	private byte[] GetTextNodeBuffer(int size, out int offset)
	{
		if (_inAttribute)
			WroteAttributeValue();
		byte[] buffer = GetBuffer(size, out offset);
		_textNodeOffset = offset;
		return buffer;
	}

	public override void WriteInt32Text(int value)
	{
		int offset;
		var buffer = GetTextNodeBuffer(5, out offset).AsSpan(offset, 5);

		if (value == (short)value)
		{
			if (value == (sbyte)value)
			{
				if ((uint)value <= 1)
				{
					buffer[0] = (uint)value < 1 ? (byte)XmlBinaryNodeType.ZeroText : (byte)XmlBinaryNodeType.OneText;
					Advance(1);
				}
				else
				{
					buffer[0] = (byte)XmlBinaryNodeType.Int8Text;
					buffer[1] = (byte)value;
					Advance(2);
				}
			}
			else
			{
				buffer[0] = (byte)XmlBinaryNodeType.Int16Text;
				buffer[1] = (byte)value;
				buffer[2] = (byte)(value >> 8);
				Advance(3);
			}
		}
		else
		{
			buffer[0] = (byte)XmlBinaryNodeType.Int32Text;
			buffer[1] = (byte)(value);
			buffer[2] = (byte)(value >> 8);
			buffer[3] = (byte)(value >> 16);
			buffer[4] = (byte)(value >> 24);
			Advance(5);
		}
	}

	public override void WriteInt64Text(long value)
	{
		if (value == (int)value)
		{
			WriteInt32Text((int)value);
		}
		else
		{
			WriteTextNodeWithInt64(XmlBinaryNodeType.Int64Text, value);
		}
	}

	public override void WriteUInt64Text(ulong value)
	{
		if (value <= long.MaxValue)
		{
			WriteInt64Text((long)value);
		}
		else
		{
			WriteTextNodeWithInt64(XmlBinaryNodeType.UInt64Text, (long)value);
		}
	}

	private void WriteTextNodeWithInt64(XmlBinaryNodeType nodeType, long value)
	{
		int offset;
		var span = GetTextNodeBuffer(9, out offset)
			.AsSpan(offset, 9);

		span[0] = (byte)nodeType;
		span[1] = (byte)(value);
		span[2] = (byte)((uint)value >> 8);
		span[3] = (byte)((uint)value >> 16);
		span[4] = (byte)((uint)value >> 24);
		span[5] = (byte)(value >> 32);
		span[6] = (byte)(value >> 40);
		span[7] = (byte)(value >> 48);
		span[8] = (byte)(value >> 56);

		Advance(9);
	}
}
