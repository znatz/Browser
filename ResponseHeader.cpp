#pragma hdrstop

#include "ResponseHeader.h"

ResponseHeader::ResponseHeader(char* val) {
FstBuff = val;
HdrLen = UnicodeString(FstBuff).Pos(UnicodeString("\r\n\r\n"))+ 3;

}
