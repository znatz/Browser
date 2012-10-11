#pragma hdrstop

#include "ResponseHeader.h"

ResponseHeader::ResponseHeader(char* val) {
	ResponseHeader::SetResponseHeader(val);
}

void ResponseHeader::SetResponseHeader(char* val) {
	FstBuff = val;
	UnicodeString FullStr = UnicodeString(val);
	UnicodeString TempFullStr = FullStr;
	HdrLen = FullStr.Pos(UnicodeString("\r\n\r\n"))+ 3;
	// rootAttrib and list both point to a list. rootAttrib is static. list is used to transvers the list.
	rootAttrib = new node;
	list = new node;
	rootAttrib = list;


	for (int i = 0; i < 20; i++)
	{
		Pos[i]=0;
	}

	int i = 0;
	do
	{
		Pos[i] = FullStr.Pos("\r\n")+ 1;
		// AttribStr is the line of Response Header
		if (Pos[i]!=1) {
			// minus 2 to get rid of \r\n
			list->fAttrib = FullStr.SubString(1,Pos[i]-2);
			// Temppos is the position of ":" inside the line
			int tempPos = list->fAttrib.Pos(":");
			if (tempPos!=0) {
				list->pAttrib = list->fAttrib.SubString(1,tempPos-1);
				list->vAttrib = list->fAttrib.SubString(tempPos+1,list->fAttrib.Length());
			}
			else {
				list->pAttrib = list->vAttrib = list->fAttrib;
			}

			FullStr = FullStr.SubString(Pos[i]+ 1,HdrLen+3);
			if (FullStr.Pos("\r\n")!=FullStr.Pos("\r\n\r\n"))
			{
				struct node* tempNode = new node;
				list->next = tempNode;
				list = tempNode;
			}
		}

		i++;
	}while( Pos[i-1]>=3 );
	list->next=NULL;
	list = rootAttrib;
}

