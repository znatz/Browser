#ifndef HtmlBodyH
#define HtmlBodyH

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>

class HtmlBody {

public:
	HtmlBody();
	void SetBody(char*, int, int);
	void SaveToFile(UnicodeString fn);
	UnicodeString cBody;
	int bodySize;
private:
	TStringStream* StrStr;
};
#endif /* #ifndef Class1H */
