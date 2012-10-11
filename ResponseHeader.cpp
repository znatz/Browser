#pragma hdrstop

#include "ResponseHeader.h"

ResponseHeader::ResponseHeader(char* val) {
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
		AttribStr[i]= FullStr.SubString(1,Pos[i]);
		list->fAttrib = FullStr.SubString(1,Pos[i]);
		// Temppos is the position of ":" inside the line
		int tempPos = AttribStr[i].Pos(":");
		// AttribStr = pAttribStr + ":" + vAttribStr
		pAttribStr[i]=AttribStr[i].SubString(0,tempPos-1);
		list->pAttrib = AttribStr[i].SubString(0,tempPos-1);

		vAttribStr[i]=AttribStr[i].SubString(tempPos+1,Pos[i]);
		rootAttrib->vAttrib = AttribStr[i].SubString(tempPos+1,Pos[i]);

		struct node* tempNode = new node;
		list->next = tempNode;
		list = tempNode;

		FullStr = FullStr.SubString(Pos[i],HdrLen+3);
		i++;



	}while( i<20 && Pos[i-1]>=3 );
}
