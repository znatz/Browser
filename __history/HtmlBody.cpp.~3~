#pragma hdrstop

#include "HtmlBody.h"

HtmlBody::HtmlBody() {
StrStr = new TStringStream();
}

void HtmlBody::SetBody(char* buff, int startPos,int endPos) {
StrStr->Write(&buff[startPos],endPos);
cBody = StrStr->DataString;
}

