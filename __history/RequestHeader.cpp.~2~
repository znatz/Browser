#pragma hdrstop

#include "RequestHeader.h"

RequestHeader::RequestHeader(UnicodeString val) {
if (val=="")
{
	UStrReqHdr= "GET /Sample.bmp HTTP/1.1\r\nHost:localhost\r\nConnection: close\r\n\r\n";
}
else
{
UStrReqHdr = val;
}
HdrToSent = AnsiString(UStrReqHdr).c_str();
}
