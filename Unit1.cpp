//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "HtmlBody.h"
//---------------------------------------------------------------------------
#include <winsock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
//---------------------------------------------------------------------------
#define DEFAULT_PORT "80"
#define BUFFLEN 1024
#define ENDOFLINE StringOfChar('\n',2)
#pragma comment (lib, "ws2_32.lib")
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
//---------------------------------------------------------------------------
int  iResult;
UnicodeString msg;
UnicodeString filename;
ResponseHeader RspHdr("");
HtmlBody Content;

bool ConnectIt();
void Report(int, UnicodeString);

//---------------------------------------------------------------------------
TFm *Fm;
//---------------------------------------------------------------------------
__fastcall TFm::TFm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFm::SbClick(TObject *Sender)
{
	Fm->Err->Text		=	"";
	Fm->Output->Text	=	"";
	Fm->BodyEdt->Text	=	"";
	Fm->Err->Text		=	"";
	//Fm->RspHdrEdt->Text	=	"";
	UnicodeString str 	= 	Input->Text;

	randomize();


	ConnectIt();
}

//---------------------------------------------------------------------------
void Report(int iR, UnicodeString ms)
{
	if (iR==0)
	{
		Fm->Err->Text=Fm->Err->Text + ms + " OK " + ENDOFLINE ;
	}
	else
	{
		Fm->Err->Text = Fm->Err->Text + ms + " " + IntToStr(iR) + ENDOFLINE;
	}
}


//---------------------------------------------------------------------------
bool ConnectIt()
{
// Initiate WSAData
	WSADATA wsadata;
	iResult = WSAStartup(0x0202,&wsadata);
	msg = "WSAStartup";
	Report(iResult,msg);
// Initiate addrinfo
	struct addrinfo *result,hints;
	ZeroMemory(&hints,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;
// Retrieve addrinfo
	const char* IPAddress = AnsiString(Fm->Input->Text).c_str();
	iResult = getaddrinfo(IPAddress,DEFAULT_PORT,&hints,&result);
	msg = "Address info";
	Report(iResult,msg);
// Retrieve socket
	int iSocket = socket (result->ai_family,result->ai_socktype,result->ai_protocol);
	msg = "Socket initiated";
	Report(iSocket,msg);
// Connect to server
	iResult = connect(iSocket, result->ai_addr, (int)result->ai_addrlen);
	msg = "Connection";
	Report(iResult,msg);
// Free up addrinfo
	freeaddrinfo(result);
// Send request header
	RequestHeader RqHdr(Fm->RqstHdr->Text);
	iResult = send(iSocket, RqHdr.HdrToSent, BUFFLEN,0);
	msg = "Bytes sent";
	Report(iResult,msg);
// Receive data
	char* recvbuff = new char[BUFFLEN];
	int totalRecvBytes = 0;
	do
	{
		iResult = recv(iSocket, recvbuff, BUFFLEN, 0);
		if (iResult>0)
		{
			Fm->Output->Text = Fm->Output->Text + UnicodeString(recvbuff).SubString(0,iResult);
			totalRecvBytes += iResult;

				if (totalRecvBytes==iResult)
				{
					  // RspHdr get the buffer, calculate the length, create the header
					  RspHdr.SetResponseHeader(recvbuff);
					  // Content get the buffer, create the first part of content
					  // Starting from the end of the header.
					  Content.SetBody(recvbuff,RspHdr.HdrLen,iResult-RspHdr.HdrLen);

					  Report(RspHdr.cSize," The lenth of the content is");
					  Report(1, " The date is " + RspHdr.cDate);
					  Report(RspHdr.HdrLen,"Total Bytes of header");
				}
				else
				{
					   Content.SetBody(recvbuff,0,iResult);
				}

		}
		else if (iResult==0)
		{
			msg = "Receiving bytes ...";
			Report(iResult,msg);
			closesocket(iSocket);
			WSACleanup();
			msg = "Total bytes received.";
			Report(totalRecvBytes,msg);
		} ;

	} while (iResult > 0);

	if (iResult<0)
	{
		msg = "Error";
		Report(iResult,msg);
		closesocket(iSocket);
		WSACleanup();
	};

// Show the body in the BodyEdt from TStr.
Fm->BodyEdt->Text=Content.cBody;
// Save the content or not according to the type of the content
if (RspHdr.cType!=".html")
{
	filename = IntToStr(rand()) + RspHdr.cType;
	Content.SaveToFile(filename);
}
// !!!!!!!!!!!! TODO: Do sth. with the content  !!!!!!!!!!

// Close connection
	msg = "The End";
	Report(iResult,msg);
	closesocket(iSocket);
	WSACleanup();
 //--------------------------

return TRUE;
}
//---------------------------------------------------------------------------

void __fastcall TFm::Button1Click(TObject *Sender)
{
	if (RspHdr.cType!=".html") {
	TImage* Image2 = new TImage(Fm);
	Image2->Parent = Fm;
	Image2->Width = 200;
	Image2->Height= 200;
	Image2->Position->X= 560;
	Image2->Position->Y= Fm->Err->Position->Y;
	Image2->Bitmap->LoadFromFile(filename);
	}
	else {Report(0, "Not an image file.");  }
}

//---------------------------------------------------------------------------


