//+------------------------------------------------------------------+
//|                                             Power_Bars_part1.mq4 |
//+------------------------------------------------------------------+
#property copyright "4xedge"
#property link      "http://www.4xedge.com"

#property indicator_chart_window
#property indicator_buffers 4
#property indicator_color1 Aqua
#property indicator_color2 CLR_NONE
#property indicator_color3 Red
#property indicator_color4 Lime

//---- buffers

int  Chart=PERIOD_M30;
string address="";


int  TMA2_Periods=25;
int  TMA2_Shift=12;
int  TMA2_ApplyTo=0;
int  CountBars=1000;

int RISK=14;
color Text_Color=Yellow;
int Font_Size=18;
bool Show_Text=true;

double TMA2_Buffer[];
double val1[];
double val2[];



int    weights[];
double divisor;
int ErrorTimeOut=30;

int init()
  {
//---- indicators
   SetIndexStyle(0,DRAW_LINE,STYLE_SOLID,3);
   SetIndexBuffer(0,TMA2_Buffer);
   SetIndexStyle(1,DRAW_NONE);
   SetIndexStyle(2,DRAW_ARROW,EMPTY,4);
   SetIndexBuffer(2,val1);
   SetIndexArrow(2,234);
   SetIndexStyle(3,DRAW_ARROW,EMPTY,4);  
   SetIndexBuffer(3,val2);
   SetIndexArrow(3,233);
   
   
//----   
   SetIndexShift(0,TMA2_Shift);
    
   return(0);
  }
int deinit()
  {
   for (int pos=Bars;pos>=0;pos--){
      ObjectDelete("tag25"+Symbol()+Period()+DoubleToStr(RISK,0)+DoubleToStr(pos,0));
   }    
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

    TMA(TMA2_Buffer,TMA2_Periods,TMA2_ApplyTo);
    Trend();
   return(0);
  }

void TMA(double &r[],int Periods,int ApplyTo){
  ArrayResize(weights,Periods);
  double d_Periods = Periods;
  int to=MathCeil(d_Periods/2.0);

  for(int i=0; i<to; i++)
  {
    weights[i]            = i+1;
    weights[Periods-1-i]  = i+1;
  }

  divisor = 0.0;
  for(int j=0; j<Periods; j++)
    divisor += weights[j];

   int l=0;
   if (CountBars>Bars) {l=Bars;}else{l=CountBars;}
   for(i=l-1; i>=0; i--){
    double tma_val = 0.0;
    for(j=0; j<Periods; j++)
    {
      double price = getPrice(ApplyTo, i+Periods-j);
      tma_val += price*weights[j];
    }
      r[i] = tma_val/divisor;
   }
}


double getPrice(int priceType, int index)
{
  double price = 0.0;
  
  switch(priceType)
  {
    case PRICE_OPEN    : price = Open[index];
                         break;
  
    case PRICE_HIGH    : price = High[index];
                         break;
  
    case PRICE_LOW     : price = Low[index];
                         break;
  
    case PRICE_MEDIAN  : price = (High[index]+Low[index])/2.0;
                         break;
  
    case PRICE_TYPICAL : price = (High[index]+Low[index]+Close[index])/3.0;
                         break;

    case PRICE_WEIGHTED: price = (High[index]+Low[index]+2*Close[index])/4.0;
                         break;

    
    case PRICE_CLOSE   : 
    default            : price = Close[index];
                         break;

  }
  
  return(price);
}

void Trend()
  {
   if (CountBars>=1000) CountBars=950;
   SetIndexDrawBegin(6,Bars-CountBars+11+1);
   SetIndexDrawBegin(7,Bars-CountBars+11+1);  
   int i,shift,counted_bars=IndicatorCounted();
   int Counter,i1,value10,value11;
   double value1,x1,x2;
   double value2,value3;
   double TrueCount,Range,AvgRange,MRO1,MRO2;
   double Table_value2[1000];
   
   value10=3+RISK*2;
   x1=67+RISK;
   x2=33-RISK;
   value11=value10;
//----
   if(Bars<=11+1) return(0);
//---- initial zero
   if(counted_bars<11+1)
   {
      for(i=1;i<=0;i++) val1[CountBars-i]=0.0;
      for(i=1;i<=0;i++) val2[CountBars-i]=0.0;
   }
//----
   shift=CountBars-11-1;
   while(shift>=0)
     {
     
   Counter=shift;
	Range=0.0;
	AvgRange=0.0;
	for (Counter=shift; Counter<=shift+9; Counter++) AvgRange=AvgRange+MathAbs(High[Counter]-Low[Counter]);
		
	Range=AvgRange/10;
	Counter=shift;
	TrueCount=0;
	while (Counter<shift+9 && TrueCount<1)
		{if (MathAbs(Open[Counter]-Close[Counter+1])>=Range*2.0) TrueCount=TrueCount+1;
		Counter=Counter+1;
		}
	if (TrueCount>=1) {MRO1=Counter;} else {MRO1=-1;}
	Counter=shift;
	TrueCount=0;
	while (Counter<shift+6 && TrueCount<1)
		{if (MathAbs(Close[Counter+3]-Close[Counter])>=Range*4.6) TrueCount=TrueCount+1;
		Counter=Counter+1;
		}
	if (TrueCount>=1) {MRO2=Counter;} else {MRO2=-1;}
	if (MRO1>-1) {value11=3;} else {value11=value10;}
	if (MRO2>-1) {value11=4;} else {value11=value10;}
	value2=100-MathAbs(iWPR(NULL,0,value11,shift)); // PercentR(value11=9)
	Table_value2[shift]=value2;
	val1[shift]=0;
	val2[shift]=0;
	value3=0;
	if (value2<x2)
		{i1=1;
		while (Table_value2[shift+i1]>=x2 && Table_value2[shift+i1]<=x1){i1++;}
		if (Table_value2[shift+i1]>x1) 
			{
			value3=High[shift]+Range*0.5;
			val1[shift]=value3;
			} 
		}
	if (value2>x1)
		{i1=1;
		while (Table_value2[shift+i1]>=x2 && Table_value2[shift+i1]<=x1){i1++;}
		if (Table_value2[shift+i1]<x2) 
			{
			value3=Low[shift]-Range*0.5;
			val2[shift]=value3;
			}
		}
      if (val1[shift+1]!=0 || val2[shift+1]!=0 && Show_Text){
         ObjectDelete("tag25"+Symbol()+Period()+DoubleToStr(RISK,0)+DoubleToStr(shift,0));
         ObjectCreate("tag25"+Symbol()+Period()+DoubleToStr(RISK,0)+DoubleToStr(shift,0),21,0,Time[shift],Low[shift]-5*Point);       
         ObjectSetText("tag25"+Symbol()+Period()+DoubleToStr(RISK,0)+DoubleToStr(shift,0),DoubleToStr(Open[shift],Digits),Font_Size,"Arial",Text_Color);
      }
      
      shift--;
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
