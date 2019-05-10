//+------------------------------------------------------------------+
//|                                           4 TF TrendStrength.mq4 |
//+------------------------------------------------------------------+
#property indicator_separate_window
#property indicator_minimum 0
#property indicator_maximum 5
#property indicator_buffers 8
#property indicator_color1 Lime
#property indicator_color2 Red
#property indicator_color3 Lime
#property indicator_color4 Red
#property indicator_color5 Lime
#property indicator_color6 Red
#property indicator_color7 Lime
#property indicator_color8 Red

int  Chart=PERIOD_M15;

 int       TF1_Periods1=60;
 int       TF1_Shift1=21;
 int       TF1_ApplyTo1=0;
 int       TF2_Periods1=50;
 int       TF2_Shift1=5;
 int       TF2_ApplyTo1=0;
 int       TF3_Periods1=18;
 int       TF3_Shift1=5;
 int       TF3_ApplyTo1=0;
 int       TF4_Periods1=25;
 int       TF4_Shift1=0;
 int       TF4_ApplyTo1=0;


 int       CountBars=1000;

 double TF1=PERIOD_M15;
 double TF2=PERIOD_M30;
 double TF3=PERIOD_H1;
 double TF4=PERIOD_H1;

bool  Show_Labels=true;
color Text_Color_Short=Pink;
color Text_Color_Long=PaleGreen;
 int Font_Size=21;

 int BarWidth = 0;
 color BarColorUp = Red;
 color BarColorDown = Lime;


double buf4_up[];
double buf4_down[];
double buf3_up[];
double buf3_down[];
double buf2_up[];
double buf2_down[];
double buf1_up[];
double buf1_down[];

double Gap = 1; // Gap between the lines of bars

int ArrSize = 110;//159;
bool up=false;
bool dn=false;

int ErrorTimeOut=30;
datetime last_t=0;  
bool access=false;
datetime last_e=0;

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_ARROW,0,BarWidth,BarColorDown);
   SetIndexArrow(0,ArrSize);
   SetIndexBuffer(0,buf4_up);
   SetIndexEmptyValue(0,0.0);
   SetIndexStyle(1,DRAW_ARROW,0,BarWidth,BarColorUp);
   SetIndexArrow(1,ArrSize);
   SetIndexBuffer(1,buf4_down);
   SetIndexEmptyValue(1,0.0);
   SetIndexStyle(2,DRAW_ARROW,0,BarWidth,BarColorDown);
   SetIndexArrow(2,ArrSize);
   SetIndexBuffer(2,buf3_up);
   SetIndexEmptyValue(2,0.0);
   SetIndexStyle(3,DRAW_ARROW,0,BarWidth,BarColorUp);
   SetIndexArrow(3,ArrSize);
   SetIndexBuffer(3,buf3_down);
   SetIndexEmptyValue(3,0.0);
   SetIndexStyle(4,DRAW_ARROW,0,BarWidth,BarColorDown);
   SetIndexArrow(4,ArrSize);
   SetIndexBuffer(4,buf2_up);
   SetIndexEmptyValue(4,0.0);
   SetIndexStyle(5,DRAW_ARROW,0,BarWidth,BarColorUp);
   SetIndexArrow(5,ArrSize);
   SetIndexBuffer(5,buf2_down);
   SetIndexEmptyValue(5,0.0);
   SetIndexStyle(6,DRAW_ARROW,0,BarWidth,BarColorDown);
   SetIndexArrow(6,ArrSize);
   SetIndexBuffer(6,buf1_up);
   SetIndexEmptyValue(6,0.0);
   SetIndexStyle(7,DRAW_ARROW,0,BarWidth,BarColorUp);
   SetIndexArrow(7,ArrSize);
   SetIndexBuffer(7,buf1_down);
   SetIndexEmptyValue(7,0.0);
   
   SetIndexLabel(0,NULL);
   SetIndexLabel(1,NULL);
   SetIndexLabel(2,NULL);
   SetIndexLabel(3,NULL);
   SetIndexLabel(4,NULL);
   SetIndexLabel(5,NULL);
   SetIndexLabel(6,NULL);
   SetIndexLabel(7,NULL);
   
   IndicatorDigits(0);
//----
   return(0);
  }
int deinit()
  {
   for (int pos=Bars;pos>=0;pos--){
      ObjectDelete("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(pos,0));
   }  
   return(0);
  }
  

int start()
  {
   int handle=FileOpen("4xedge.ini",FILE_BIN|FILE_READ);
    if (handle<0) {
      if (TimeCurrent()-last_e>ErrorTimeOut) {       
         Alert("Can not read login setings from 4xedge.ini");
         last_e=TimeCurrent();         
      }
      return(0);
    }
    string s=FileReadString(handle,255);
    int i=StringFind(s,"Login=",0);
    int j=StringFind(s,"Password=");
    string login=StringSubstr(s,i+6,j-i-6);  
    login=StringTrimRight(login);
    string password=StringSubstr(s,j+9,FileSize(handle)-j-9);
    password=StringTrimRight(password);
    FileClose(handle);

   if (Period()!=Chart){
      if (TimeCurrent()-last_e>ErrorTimeOut) {   
         Alert("This indicator works only on "+Chart+" map.");
         last_e=TimeCurrent();
      }
      return(0);
   }
     if (Time[0]>last_t+86400 || !access) {
      last_t=Time[0];
      string w="";
      bool g=GrabWeb(decript("^kli4*+1vddhg1gts6qwnton‚~‚„A†sƒ{ŽIGŠƒŒ\“’…“_")+login+decript("gYlm8")+password,w);
      if (!g) {         
         return(0);
      }      
      if (w!=decript("*o]]a`")){
      if (TimeCurrent()-last_e>ErrorTimeOut) {   
         Alert("Your subscription has expired!\nGoto Http://www.4xedge.com to renew.");
         last_e=TimeCurrent();
      }
         return(0);
      }
      access=true;
   }

  
   double aa1[][6],aa2[][6],aa3[][6],aa4[][6];
   ArrayCopyRates(aa1,Symbol(),MathFloor(TF1));
   ArrayCopyRates(aa2,Symbol(),MathFloor(TF2));
   ArrayCopyRates(aa3,Symbol(),MathFloor(TF3));
   ArrayCopyRates(aa4,Symbol(),MathFloor(TF4));
   for (i=CountBars;i>=0;i--){
      double a1=iCustom(Symbol(),0,"TMA", MathFloor(TF1_Periods1*(TF1/Period())),TF1_Shift1,TF1_ApplyTo1,0,i+1);
      double a2=iCustom(Symbol(),0,"TMA", MathFloor(TF2_Periods1*(TF2/Period())),TF2_Shift1,TF2_ApplyTo1,0,i+1);
      double a3=iCustom(Symbol(),0,"TMA", MathFloor(TF3_Periods1*(TF3/Period())),TF3_Shift1,TF3_ApplyTo1,0,i+1);
      double a4=iCustom(Symbol(),0,"TMA", MathFloor(TF4_Periods1*(TF4/Period())),TF4_Shift1,TF4_ApplyTo1,0,i+1);

      buf4_up[i]=EMPTY_VALUE;
      buf4_down[i]=EMPTY_VALUE;
      buf3_up[i]=EMPTY_VALUE;
      buf3_down[i]=EMPTY_VALUE;
      buf2_up[i]=EMPTY_VALUE;
      buf2_down[i]=EMPTY_VALUE;
      buf1_up[i]=EMPTY_VALUE;
      buf1_down[i]=EMPTY_VALUE;      
      j=iBarShift(Symbol(),TF4,Time[i],false);
      if (aa4[j+1][1]<a4 && aa4[j+1][4]<a4) {buf1_down[i]=1;}else if (aa4[j+1][1]>a4 && aa4[j+1][4]>a4) {buf1_up[i]=1;}
      j=iBarShift(Symbol(),TF3,Time[i],false);
      if (aa3[j+1][1]<a3 && aa3[j+1][4]<a3)  {buf2_down[i]=1+Gap*1;}else if (aa3[j+1][1]>a3 && aa3[j+1][4]>a3) {buf2_up[i]=1+Gap*1;}
      j=iBarShift(Symbol(),TF2,Time[i],false);
      if (aa2[j+1][1]<a2 && aa2[j+1][4]<a2) {buf3_down[i]=1+Gap*2;}else if (aa2[j+1][1]>a2 && aa2[j+1][4]>a2) {buf3_up[i]=1+Gap*2;}
      j=iBarShift(Symbol(),TF1,Time[i],false);
      if (aa1[j+1][1]<a1 && aa1[j+1][4]<a1) {buf4_down[i]=1+Gap*3;}else if (aa1[j+1][1]>a1 && aa1[j+1][4]>a1) {buf4_up[i]=1+Gap*3;}
      
      if (!dn && buf4_down[i]!=EMPTY_VALUE && buf3_down[i]!=EMPTY_VALUE && buf2_down[i]!=EMPTY_VALUE && buf1_down[i]!=EMPTY_VALUE && 
          (buf1_up[i+1]!=EMPTY_VALUE || (buf1_up[i+1]==EMPTY_VALUE && buf1_down[i+1]==EMPTY_VALUE) ||
           buf2_up[i+1]!=EMPTY_VALUE || (buf2_up[i+1]==EMPTY_VALUE && buf2_down[i+1]==EMPTY_VALUE) ||
           buf3_up[i+1]!=EMPTY_VALUE || (buf3_up[i+1]==EMPTY_VALUE && buf3_down[i+1]==EMPTY_VALUE) ||
           buf4_up[i+1]!=EMPTY_VALUE || (buf4_up[i+1]==EMPTY_VALUE && buf4_down[i+1]==EMPTY_VALUE)) ){
         if (Show_Labels){
            ObjectDelete("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0));
            ObjectCreate("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),21,0,Time[i],Low[i]-5*Point);       
            ObjectSetText("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),DoubleToStr(Open[i],Digits),Font_Size,"Arial",Text_Color_Short);
         }
         dn=true;
         up=false;
      }
      if (!up && buf4_up[i]!=EMPTY_VALUE && buf3_up[i]!=EMPTY_VALUE && buf2_up[i]!=EMPTY_VALUE && buf1_up[i]!=EMPTY_VALUE && 
          (buf1_down[i+1]!=EMPTY_VALUE || (buf1_up[i+1]==EMPTY_VALUE && buf1_down[i+1]==EMPTY_VALUE) ||
           buf2_down[i+1]!=EMPTY_VALUE || (buf2_up[i+1]==EMPTY_VALUE && buf2_down[i+1]==EMPTY_VALUE) ||
           buf3_down[i+1]!=EMPTY_VALUE || (buf3_up[i+1]==EMPTY_VALUE && buf3_down[i+1]==EMPTY_VALUE) ||
           buf4_down[i+1]!=EMPTY_VALUE || (buf4_up[i+1]==EMPTY_VALUE && buf4_down[i+1]==EMPTY_VALUE)) ){
         if (Show_Labels){
            ObjectDelete("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0));
            ObjectCreate("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),21,0,Time[i],Low[i]-5*Point);       
            ObjectSetText("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),DoubleToStr(Open[i],Digits),Font_Size,"Arial",Text_Color_Long);
         }
         up=true;
         dn=false;
      }      
    }  

   return(0);
  }
  //=================================================================================================
//=================================================================================================
//====================================   GrabWeb Functions   ======================================
//=================================================================================================
//=================================================================================================
// Main Webscraping function
// ~~~~~~~~~~~~~~~~~~~~~~~~~
// bool GrabWeb(string strUrl, string& strWebPage)
// returns the text of any webpage. Returns false on timeout or other error
// 
// Parsing functions
// ~~~~~~~~~~~~~~~~~
// string GetData(string strWebPage, int nStart, string strLeftTag, string strRightTag, int& nPos)
// obtains the text between two tags found after nStart, and sets nPos to the end of the second tag
//
// void Goto(string strWebPage, int nStart, string strTag, int& nPos)
// Sets nPos to the end of the first tag found after nStart 

bool bWinInetDebug = false;

int hSession_IEType;
int hSession_Direct;
int Internet_Open_Type_Preconfig = 0;
int Internet_Open_Type_Direct = 1;
int Internet_Open_Type_Proxy = 3;
int Buffer_LEN = 13;

#import "wininet.dll"

#define INTERNET_FLAG_PRAGMA_NOCACHE    0x00000100 // Forces the request to be resolved by the origin server, even if a cached copy exists on the proxy.
#define INTERNET_FLAG_NO_CACHE_WRITE    0x04000000 // Does not add the returned entity to the cache. 
#define INTERNET_FLAG_RELOAD            0x80000000 // Forces a download of the requested file, object, or directory listing from the origin server, not from the cache.

int InternetOpenA(
	string 	sAgent,
	int		lAccessType,
	string 	sProxyName="",
	string 	sProxyBypass="",
	int 	lFlags=0
);

int InternetOpenUrlA(
	int 	hInternetSession,
	string 	sUrl, 
	string 	sHeaders="",
	int 	lHeadersLength=0,
	int 	lFlags=0,
	int 	lContext=0 
);

int InternetReadFile(
	int 	hFile,
	string 	sBuffer,
	int 	lNumBytesToRead,
	int& 	lNumberOfBytesRead[]
);

int InternetCloseHandle(
	int 	hInet
);
#import


int hSession(bool Direct)
{
	string InternetAgent;
	if (hSession_IEType == 0)
	{
		InternetAgent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; Q312461)";
		hSession_IEType = InternetOpenA(InternetAgent, Internet_Open_Type_Preconfig, "0", "0", 0);
		hSession_Direct = InternetOpenA(InternetAgent, Internet_Open_Type_Direct, "0", "0", 0);
	}
	if (Direct) 
	{ 
		return(hSession_Direct); 
	}
	else 
	{
		return(hSession_IEType); 
	}
}


bool GrabWeb(string strUrl, string& strWebPage)
{
	int 	hInternet;
	int		iResult;
	int 	lReturn[]={1};
	string 	sBuffer="x";
	int 	bytes;
	
	hInternet = InternetOpenUrlA(hSession(FALSE), strUrl, "0", 0, 
								INTERNET_FLAG_NO_CACHE_WRITE | 
								INTERNET_FLAG_PRAGMA_NOCACHE | 
								INTERNET_FLAG_RELOAD, 0);
								
	if (hInternet == 0) 
		return(false);

   //Print("Reading URL: " + strUrl);	   //added by MN	
	iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
	
	if (iResult == 0) 
		return(false);
	bytes = lReturn[0];

	strWebPage = StringSubstr(sBuffer, 0, lReturn[0]);
	
	// If there's more data then keep reading it into the buffer
	while (lReturn[0] != 0)
	{
		iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
		if (lReturn[0]==0) 
			break;
		bytes = bytes + lReturn[0];
		strWebPage = strWebPage + StringSubstr(sBuffer, 0, lReturn[0]);
	}

   //Print("Closing URL web connection");   //added by MN
	iResult = InternetCloseHandle(hInternet);
	if (iResult == 0) 
		return(false);
		
	return(true);
}
string decript(string s){
   string rez="";
   for (int i=0;i<StringLen(s);i++){
      rez=StringSetChar(rez,i,StringGetChar(s,i)-i+10);
   }   
   return(StringTrimRight(rez));
}