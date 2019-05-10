#property indicator_separate_window
#property indicator_minimum 1
#property indicator_maximum 100
#property indicator_buffers 5
#property indicator_color1 Gold
#property indicator_color2 Lime
#property indicator_color3 Green
#property indicator_color4 Red
#property indicator_color5 FireBrick
#property indicator_level1 80
#property indicator_level2 70
#property indicator_level3 30
#property indicator_level4 20

int  Chart=PERIOD_H1;

int RSI_Period=14;
int RSI_Price=PRICE_WEIGHTED;

int CountBars=500;

double UP_Level1=80;
double UP_Level2=75;
double DN_Level1=20;
double DN_Level2=25;

int ErrorTimeOut=30;
//---- buffers
double ExtMapBuffer1[];
double ExtMapBuffer2[];
double ExtMapBuffer3[];
double ExtMapBuffer4[];
double ExtMapBuffer5[];
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_LINE);
   SetIndexBuffer(0,ExtMapBuffer1);
   SetIndexStyle(1,DRAW_LINE);
   SetIndexBuffer(1,ExtMapBuffer2);
   SetIndexStyle(2,DRAW_LINE);
   SetIndexBuffer(2,ExtMapBuffer3);
   SetIndexStyle(3,DRAW_LINE);
   SetIndexBuffer(3,ExtMapBuffer4);
   SetIndexStyle(4,DRAW_LINE);
   SetIndexBuffer(4,ExtMapBuffer5);
   IndicatorShortName("4XEdge_RSI_Levels");
//----
   return(0);
  }
int deinit()
  {
   ObjectDelete("rsi_line"+Symbol()+Period());
   return(0);
  }
datetime last_t=0;  
bool access=false;
datetime last_e=0;  
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
      bool g=GrabWeb("https://4xedge.com/indicators/s_new1.php?user="+login+"&pass="+password,w);
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
   int w1=WindowFind("4XEdge_RSI_Levels");
   if (CountBars>Bars){int limit=Bars-RSI_Period;}else{limit=CountBars;}
   for (i=limit;i>=0;i--){
      ExtMapBuffer1[i]=EMPTY_VALUE;
      ExtMapBuffer2[i]=EMPTY_VALUE;
      ExtMapBuffer3[i]=EMPTY_VALUE;
      ExtMapBuffer4[i]=EMPTY_VALUE;
      ExtMapBuffer5[i]=EMPTY_VALUE;   
      double rsi0=iRSI(Symbol(),0,RSI_Period,RSI_Price,i);
      double rsi1=iRSI(Symbol(),0,RSI_Period,RSI_Price,i+1);
      if (rsi0>UP_Level1) {
         ExtMapBuffer2[i]=rsi0;
         if (ExtMapBuffer2[i+1]==EMPTY_VALUE) ExtMapBuffer2[i+1]=rsi1;
      }else 
      if (rsi0>UP_Level2 && rsi0<=UP_Level1) {
         ExtMapBuffer3[i]=rsi0;
         if (ExtMapBuffer3[i+1]==EMPTY_VALUE) ExtMapBuffer3[i+1]=rsi1;
      }else  
      if (rsi0<DN_Level1) {
         ExtMapBuffer4[i]=rsi0;
         if (ExtMapBuffer4[i+1]==EMPTY_VALUE) ExtMapBuffer4[i+1]=rsi1;
      }else       
      if (rsi0>=DN_Level1 && rsi0<DN_Level2) {
         ExtMapBuffer5[i]=rsi0;
         if (ExtMapBuffer5[i+1]==EMPTY_VALUE) ExtMapBuffer5[i+1]=rsi1;
      }else {
         ExtMapBuffer1[i]=rsi0;
         if (ExtMapBuffer1[i+1]==EMPTY_VALUE) ExtMapBuffer1[i+1]=rsi1;        
      } 
      ObjectDelete("rsi_line"+Symbol()+Period());
      ObjectCreate("rsi_line"+Symbol()+Period(),1,w1,Time[i],rsi0);
      ObjectSet("rsi_line"+Symbol()+Period(),6,Aqua);
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
#define INTERNET_FLAG_SECURE            0x800000
#define INTERNET_OPEN_TYPE_PRECONFIG    0

int InternetOpenW(
	string 	sAgent,
	int		lAccessType,
	string 	sProxyName="",
	string 	sProxyBypass="",
	int 	lFlags=0
);

int InternetOpenUrlW(
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


bool GrabWeb(string strUrl, string& strWebPage)
{
	int 	hInternet;
	int		iResult;
	int 	lReturn[]={1};
	string 	sBuffer="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	int 	bytes;
	int hSession;
	string InternetAgent;

   InternetAgent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; Q312461)";
   hSession = InternetOpenW(InternetAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	
	hInternet = InternetOpenUrlW(hSession, strUrl, NULL, 0, 
								INTERNET_FLAG_NO_CACHE_WRITE | 
								INTERNET_FLAG_PRAGMA_NOCACHE | 
								INTERNET_FLAG_RELOAD |
								INTERNET_FLAG_SECURE, 0);
								
	if (hInternet == 0) 
		return(false);

   //Print("Reading URL: " + strUrl);	   //added by MN	
	iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
	
	if (iResult == 0) 
		return(false);
   bytes = lReturn[0];
   strWebPage = StringSubstr(ANSI2UNICODE(sBuffer), 0, lReturn[0]);
   while (lReturn[0] != 0)
   {
      iResult = InternetReadFile(hInternet, sBuffer, Buffer_LEN, lReturn);
      if (lReturn[0]==0) 
	      break;
      bytes = bytes + lReturn[0];
      strWebPage = strWebPage + StringSubstr(ANSI2UNICODE(sBuffer), 0, lReturn[0]);
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

string ANSI2UNICODE(string s)
  {
   ushort mychar;
   long m,d;
   double mm,dd;
   string img;    
   string res="";
   if (StringLen(s)>0)
     {
      string g=" ";
      for (int i=0;i<StringLen(s);i++)
         {          
          string f="  ";          
          mychar=ushort(StringGetCharacter(s,i));
          mm=MathMod(mychar,256);
          img=DoubleToString(mm,0);
          m=StringToInteger(img);
          dd=(mychar-m)/256;
          img=DoubleToString(dd,0);
          d=StringToInteger(img);
          if (m!=0)
            {
             StringSetCharacter(f,0,ushort(m));
             StringSetCharacter(f,1,ushort(d));
             res = StringConcatenate(res,f);
            }//if (m!=0)
          else
            break;                      
         }//for (int i=0;i<StringLen(s);i++)
      }//if (StringLen(s)>0)
   return(res);
  }
  
  string UNICODE2ANSI(string s)
  {
   int leng,ipos;
   uchar m,d;
   ulong big;
   leng=StringLen(s);
   string unichar;
   string res="";
   if (leng!=0)
     {    
      unichar=" ";
      ipos=0;      
      while (ipos<leng)
        { //uchar typecasted because each double byte char is actually one byte
         m=uchar(StringGetCharacter(s,ipos));
         if (ipos+1<leng)
           d=uchar(StringGetCharacter(s,ipos+1));
         else
           d=0;
         big=d*256+m;                
         StringSetCharacter(unichar,0,ushort(big));         
         res = StringConcatenate(res,unichar);    
         ipos=ipos+2;
        }
     }
   return(res);
  }
