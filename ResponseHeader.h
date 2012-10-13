#ifndef ResponseHeaderH
#define ResponseHeaderH

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>

struct node
{
		UnicodeString fAttrib;
		UnicodeString pAttrib;
		UnicodeString vAttrib;
		node* next ;
};

class ResponseHeader
{

	public:
		// The parameter of the Header
		char* FstBuff;
		int HdrLen;
		// The parameter of header
		UnicodeString cType;
        UnicodeString cDate;
		int cSize;
		int Pos[20];

		ResponseHeader(char*);
		void SetResponseHeader(char*);
		void __fastcall ExamRspHdr();


	public:
		struct node* rootAttrib;
		struct node* list;
		struct node* q;
};

#endif /* #ifndef ResponseHeaderH */
