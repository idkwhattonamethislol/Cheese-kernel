typedef unsigned long long size_t;
typedef struct{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int width;
	unsigned int height;
	unsigned int PixelsPerScanLine;
}FrameBuffer;

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
}PSF1_HEADER;

typedef struct{
	PSF1_HEADER* psf1Header;
	void* glyphBuffer;
}PSF1_FONT;

typedef struct {
	unsigned int X;
	unsigned int Y;
}POINT;

void drawChar(FrameBuffer* frameBuffer, PSF1_FONT* psf1_font, unsigned int colour, char chr, unsigned int xOff, unsigned int yOff)
{
	unsigned int* pixPtr = (unsigned int*)frameBuffer->BaseAddress;
	char* fontPtr = psf1_font->glyphBuffer + (chr * psf1_font->psf1Header->charsize);
	for(unsigned long y=yOff; y<yOff+9; y++)
	{
		for(unsigned long x=xOff; x<xOff+8; x++)
		{
			if((*fontPtr & (0b10000000>>(x-xOff)))>0)
			{
				*(unsigned int*)(pixPtr+x+(y*frameBuffer->PixelsPerScanLine))=colour;

			}
		}
		fontPtr++;
	}	
}

POINT CursorPosition;
void Print(FrameBuffer* frameBuffer, PSF1_FONT* psf1_font, unsigned int colour, char* str)
{
	char* chr = str;
	while(*chr!=0)
	{
		drawChar(frameBuffer, psf1_font, colour, *chr, CursorPosition.X, CursorPosition.Y);
		CursorPosition.X+=8;
		if(CursorPosition.X+8>frameBuffer->width)
		{
			CursorPosition.X=0;
			CursorPosition.Y+=16;
		}
		chr++;

	}
}



void _start(FrameBuffer* framebuffer, PSF1_FONT* psf1Font)
{
	CursorPosition.X=0;
	CursorPosition.Y=0;
	
	while(CursorPosition.Y!=12000)
	{
		Print(framebuffer,psf1Font, 0xff00ff,".");
	}
	
	if (CursorPosition.X>framebuffer->width)
	{
		CursorPosition.X=0;
		CursorPosition.Y+=16;
	}
    return;
    
}
