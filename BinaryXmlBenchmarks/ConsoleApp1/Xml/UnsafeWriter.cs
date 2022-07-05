using System.Runtime.CompilerServices;
using System.Xml;
using System.Runtime.InteropServices;

public class UnsafeWriter : XmlStreamNodeWriterBase
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
		if (value == (sbyte)value)
		{
			if ((byte)value <= 1)
			{
				if ((byte)value < 1)
				{
					WriteTextNode(XmlBinaryNodeType.ZeroText);
				}
				else
				{
					WriteTextNode(XmlBinaryNodeType.OneText);
				}
			}
			else
			{
				int offset;
				ref byte buffer = ref Unsafe.Add(ref MemoryMarshal.GetArrayDataReference(GetTextNodeBuffer(2, out offset)), offset);
				buffer = (byte)XmlBinaryNodeType.Int8Text;
				Unsafe.Add(ref buffer, 1) = (byte)value;
				Advance(2);
			}
		}
		else if (value == (short)value)
		{
			int offset;
			ref byte buffer = ref Unsafe.Add(ref MemoryMarshal.GetArrayDataReference(GetTextNodeBuffer(3, out offset)), offset);
			buffer = (byte)XmlBinaryNodeType.Int16Text;
			Unsafe.WriteUnaligned(ref Unsafe.Add(ref buffer, 1), (short)value);
			Advance(3);
		}
		else
		{
			int offset;
			ref byte buffer = ref Unsafe.Add(ref MemoryMarshal.GetArrayDataReference(GetTextNodeBuffer(5, out offset)), offset);
			buffer = (byte)XmlBinaryNodeType.Int32Text;
			Unsafe.WriteUnaligned(ref Unsafe.Add(ref buffer, 1), value);
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
			WriteTextNodeWithInt64(XmlBinaryNodeType.UInt64Text, unchecked((long)value));
		}
	}

	private void WriteTextNodeWithInt64(XmlBinaryNodeType nodeType, long value)
	{
		ref byte buffer = ref Unsafe.Add(ref MemoryMarshal.GetArrayDataReference(GetTextNodeBuffer(9, out int offset)), offset);
		buffer = (byte)XmlBinaryNodeType.Int32Text;
		Unsafe.WriteUnaligned(ref Unsafe.Add(ref buffer, 1), value);
		Advance(9);
	}
}
