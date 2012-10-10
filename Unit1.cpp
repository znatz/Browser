//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
#include "RequestHeader.h"
#include "ResponseHeader.h"
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
TFileStream* FStream;

bool ConnectIt();
void Report(int, UnicodeString);
void RspReport(int, UnicodeString);
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
	Fm->Err->Text="";
	Fm->RqstHdr->Text=="";
	UnicodeString str = Input->Text;

	randomize();
	filename = IntToStr(rand()) +".png";
	FStream= new TFileStream(filename, fmCreate);

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

void RspReport(int iR, UnicodeString ms)
{
	if (iR==0)
	{
		Fm->RspHdrEdt->Text = Fm->RspHdrEdt->Text + ms + " OK " + ENDOFLINE ;
	}
	else
	{
		Fm->RspHdrEdt->Text = Fm->RspHdrEdt->Text + ms + " " + IntToStr(iR) + ENDOFLINE;
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
			msg = "Receiving bytes ...";
			Report(iResult,msg);
			Fm->Output->Text = Fm->Output->Text + UnicodeString(recvbuff).SubString(0,iResult);
			totalRecvBytes += iResult;

				if (totalRecvBytes==iResult)
				{
					  ResponseHeader RspHdr(recvbuff);
					  FStream->Write(&recvbuff[RspHdr.HdrLen],iResult-RspHdr.HdrLen);
						Report(RspHdr.HdrLen,"TotalBytes");
					  // Temporarily used to check whether the response header has been divided correctly.
						for (int i = 0; i < 20; i++) {
							RspReport(1,RspHdr.AttribStr[i]);
							RspReport(1,RspHdr.pAttribStr[i]+"---"+RspHdr.vAttribStr[i]);
							}
				}
				else
				{
					FStream->Write(recvbuff,iResult);
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
// Close connection
	msg = "The End";
	Report(iResult,msg);
	closesocket(iSocket);
	WSACleanup();
	FStream->Free();



//--------------------------

return TRUE;
}
//---------------------------------------------------------------------------

void __fastcall TFm::Button1Click(TObject *Sender)
{
	TImage* Image2 = new TImage(Fm);
	Image2->Parent = Fm;
	Image2->Width = 200;
	Image2->Height= 200;
	Image2->Position->X= 560;
	Image2->Position->Y= Fm->Err->Position->Y;
	Image2->Bitmap->LoadFromFile(filename);
}

//---------------------------------------------------------------------------

