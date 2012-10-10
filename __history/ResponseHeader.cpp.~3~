#pragma hdrstop

#include "ResponseHeader.h"

ResponseHeader::ResponseHeader(char* val) {
FstBuff = val;
UnicodeString FullStr = UnicodeString(val);
HdrLen = FullStr.Pos(UnicodeString("\r\n\r\n"))+ 3;

	for (int i = 0; i < 20; i++) {
Pos[i]=0;
	}

int i = 0;
do
{
	Pos[i] = FullStr.Pos(UnicodeString("\r\n"))+2;
	FullStr = FullStr.SubString(Pos[i],HdrLen-3);
	i++;
}while( i<20 && Pos[i-1]>=3 );
}
