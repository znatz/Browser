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
		char* FstBuff;
		int HdrLen;

		ResponseHeader(char*);
		void SetResponseHeader(char*);
		int Pos[20];

	public:
		struct node* rootAttrib;
		struct node* list;
		struct node* q;
};

#endif /* #ifndef ResponseHeaderH */
