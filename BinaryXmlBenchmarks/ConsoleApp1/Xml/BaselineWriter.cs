using System.Xml;
//[Benchmark]
//public void WriteInt64_2()
//{
//	var ints = MemoryMarshal.Cast<byte, Int64>(data).Slice(0, 1000);
//	foreach (var i in ints)
//		WriteInt64Text2(i);
//}

public class BaselineWriter : XmlStreamNodeWriterBase
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
		if (value >= -128 && value < 128)
		{
			if (value == 0)
			{
				WriteTextNode(XmlBinaryNodeType.ZeroText);
			}
			else if (value == 1)
			{
				WriteTextNode(XmlBinaryNodeType.OneText);
			}
			else
			{
				int offset;
				byte[] buffer = GetTextNodeBuffer(2, out offset);
				buffer[offset + 0] = (byte)XmlBinaryNodeType.Int8Text;
				buffer[offset + 1] = (byte)value;
				Advance(2);
			}
		}
		else if (value >= -32768 && value < 32768)
		{
			int offset;
			byte[] buffer = GetTextNodeBuffer(3, out offset);
			buffer[offset + 0] = (byte)XmlBinaryNodeType.Int16Text;
			buffer[offset + 1] = (byte)value;
			value >>= 8;
			buffer[offset + 2] = (byte)value;
			Advance(3);
		}
		else
		{
			int offset;
			byte[] buffer = GetTextNodeBuffer(5, out offset);
			buffer[offset + 0] = (byte)XmlBinaryNodeType.Int32Text;
			buffer[offset + 1] = (byte)value;
			value >>= 8;
			buffer[offset + 2] = (byte)value;
			value >>= 8;
			buffer[offset + 3] = (byte)value;
			value >>= 8;
			buffer[offset + 4] = (byte)value;
			Advance(5);
		}
	}

	public override void WriteInt64Text(long value)
	{
		if (value >= int.MinValue && value <= int.MaxValue)
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

	private void WriteInt64(long value)
	{
		int offset;
		byte[] buffer = GetBuffer(8, out offset);
		buffer[offset + 0] = (byte)value;
		value >>= 8;
		buffer[offset + 1] = (byte)value;
		value >>= 8;
		buffer[offset + 2] = (byte)value;
		value >>= 8;
		buffer[offset + 3] = (byte)value;
		value >>= 8;
		buffer[offset + 4] = (byte)value;
		value >>= 8;
		buffer[offset + 5] = (byte)value;
		value >>= 8;
		buffer[offset + 6] = (byte)value;
		value >>= 8;
		buffer[offset + 7] = (byte)value;
		Advance(8);
	}
	private void WriteTextNodeWithInt64(XmlBinaryNodeType nodeType, long value)
	{
		int offset;
		byte[] buffer = GetTextNodeBuffer(9, out offset);
		buffer[offset + 0] = (byte)nodeType;
		buffer[offset + 1] = (byte)value;
		value >>= 8;
		buffer[offset + 2] = (byte)value;
		value >>= 8;
		buffer[offset + 3] = (byte)value;
		value >>= 8;
		buffer[offset + 4] = (byte)value;
		value >>= 8;
		buffer[offset + 5] = (byte)value;
		value >>= 8;
		buffer[offset + 6] = (byte)value;
		value >>= 8;
		buffer[offset + 7] = (byte)value;
		value >>= 8;
		buffer[offset + 8] = (byte)value;
		Advance(9);
	}
}
