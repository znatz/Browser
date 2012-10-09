#ifndef RequestHeaderH
#define RequestHeaderH

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>

class RequestHeader {

public:
	RequestHeader(UnicodeString);
	UnicodeString UStrReqHdr;
	const char* HdrToSent;
};

#endif /* #ifndef RequestHeaderH */
