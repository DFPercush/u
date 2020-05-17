#ifdef _MSC_VER
#pragma warning(disable:0000)
#endif
#include <stdio.h>
#include <Windows.h>

int CopyChar(unsigned long codePoint)
{
	if (!OpenClipboard(NULL))
	{
		fprintf(stderr, "u: Could not open clipboard!\n");
		return 1;
	}
	if (!EmptyClipboard())
	{
		CloseClipboard();
		return 2;
	}
	auto hMem = GlobalAlloc(GMEM_MOVEABLE, 8);
	if (!hMem)
	{
		fprintf(stderr, "u: Memory allocation failure.\n");
		CloseClipboard();
		return 3;
	}

	unsigned short* pText = GlobalLock(hMem);
	if (codePoint <= 0xD7FF)
	{
		pText[0] = codePoint;
		pText[1] = 0;
	}
	else
	{
		pText[0] = (((codePoint >> 10) & 0x3FF) | 0xD800);
		pText[1] = ((codePoint & 0x3FF) | 0xDC00);
		pText[2] = 0;
	}
	GlobalUnlock(hMem);

	HANDLE hNewClipboardData = SetClipboardData(CF_UNICODETEXT, hMem);

	CloseClipboard();
	return 0;
}

unsigned long GetCurrentContents()
{
	// Returns code point of first character of text on clipboard
	auto hClip = OpenClipboard(NULL);
	if (!hClip) { return 0; }
	auto hRawData = GetClipboardData(CF_UNICODETEXT);
	if (!hRawData) { return 0; }
	unsigned short* pText = GlobalLock(hRawData);
	if (pText[0] & 0xC000)
	{

	}
	GlobalUnlock(hRawData);
	CloseClipboard();
}

int main(int argc, char** argv)
{
	unsigned long cp = 0;
	if (argc == 2)
	{
		sscanf(argv[1], "%x", &cp);
		return CopyChar(cp);
	}
	else
	{
		fprintf(stderr, "u - Copy a unicode text character to the clipboard.\n");
		fprintf(stderr, "To use:\n");
		fprintf(stderr, "	u [code point in hex]\n");
		fprintf(stderr, "(do not include the 'U+' - just the hex number after that.)\n");
		fprintf(stderr, "Example: u 263A\n");
		fprintf(stderr, "\n");
	}
}

