//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#include <winsock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
//#include <stdio.h>
#define DEFAULT_PORT "80"
#define ENDOFLINE StringOfChar('\n',2)
#pragma comment (lib, "ws2_32.lib")
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
//---------------------------------------------------------------------------
int  iResult;
UnicodeString msg;
bool ConnectIt();
void Report(int, UnicodeString);
UnicodeString filename = "TempFile.bmp";
TFileStream* FStream= new TFileStream(filename, fmCreate);
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
ConnectIt();
/*
	TFm *Cosmos = new TFm(this);
	Cosmos->ShowModal();

	delete Cosmos;
	Cosmos = NULL;
*/
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
// Setup request header
const char* sendbuff;
if (Fm->RqstHdr->Text=="")
{
UnicodeString requst = "GET /Sample.bmp HTTP/1.1\r\n" ;
requst = requst + "Host:" + Fm->Input->Text  + "\r\n";
requst = requst + "Connection: close\r\n\r\n";
sendbuff = AnsiString(requst).c_str();
}
else
{
sendbuff = AnsiString(Fm->RqstHdr->Text).c_str();
};
// Send request header
iResult = send(iSocket, sendbuff, 512,0);
msg = "Bytes sent";
Report(iResult,msg);
// Receive data
char* recvbuff = new char[512];
int totalRecvBytes = 0;
do
{
	iResult = recv(iSocket, recvbuff, 512, 0);
	if (iResult>0)
	{
		msg = "Receiving bytes ...";
		Report(iResult,msg);
		Fm->Output->Text = Fm->Output->Text + UnicodeString(recvbuff).SubString(0,iResult);
		totalRecvBytes += iResult;

			if (totalRecvBytes==iResult)
			{
			// TODO the counts of bytes 274 of header needed to be calculated from other methods!
				  FStream->Write(&recvbuff[274],iResult-274);

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

return TRUE;
}
//---------------------------------------------------------------------------

void __fastcall TFm::Button1Click(TObject *Sender)
{

	// The following codes work!!!
	TImage* Image2 = new TImage(Fm);
	Image2->Parent = Fm;
	Image2->Position->X=650;
	Image2->Position->Y=200;
	Image2->Bitmap->LoadFromFile(filename);
	// TODO the filename has to be randomized

}

//---------------------------------------------------------------------------

