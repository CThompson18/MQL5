//+------------------------------------------------------------------+
//|                                               X_Factor_Part1.mq4 |
//+------------------------------------------------------------------+
#property copyright "4xLounge"
#property link      "http://www.4xlounge.com"

#property indicator_chart_window
#property indicator_buffers 3
#property indicator_color1 C'0,91,0'
#property indicator_color2 C'155,30,30'
#property indicator_color3 DarkBlue


int  Chart=PERIOD_M30;
string address="";


//Input Params
string PivotRangeStart = "00:30";
string PivotRangeEnd = "21:30";
bool DisplayPreviousHighLow = true;
bool Color_Space_High_Low=true;
color High_Low_Space_Color=C'107,107,107';
int  High_Low_Line_Widht=4;
int Count_Bars=300;


int  TMA_Periods=121;
int  TMA_Shift=5;
int  TMA_ApplyTo=0;

int  CountBars=1000;

double Buffer4[];
double Buffer5[];
double TMA_Buffer[];

double pivots[50];

double pivotRangeHigh;
double pivotRangeLow;

int openBar;
int    weights[];
double divisor;

int ErrorTimeOut=30;
    
int init()
{
   
   SetIndexStyle(0,DRAW_LINE, STYLE_SOLID, High_Low_Line_Widht);
   SetIndexBuffer(0,Buffer4);
   SetIndexLabel(0,"Previous Day High");
   
   SetIndexStyle(1,DRAW_LINE, STYLE_SOLID, High_Low_Line_Widht);
   SetIndexBuffer(1,Buffer5);
   SetIndexLabel(1,"Previous Day Low");

   SetIndexStyle(2,DRAW_LINE,STYLE_SOLID,4);
   SetIndexBuffer(2,TMA_Buffer);   
   
   SetIndexShift(2,TMA_Shift);
   return(0);
}
datetime last_e=0;
int deinit()
{
   for (int i=Bars;i>=0;i--){
      ObjectDelete("HLSC"+Symbol()+Period()+DoubleToStr(i,1));
   }
   return(0);
}
datetime last_t=0;  
bool access=false;
int start()
{   
    /*int handle=FileOpen("4xlounge.ini",FILE_BIN|FILE_READ);
    if (handle<0) {
      if (TimeCurrent()-last_e>ErrorTimeOut) {       
         Alert("Can not read login setings from 4xlounge.ini");
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
      bool g=GrabWeb(decript("^kli4*+tuv.5zosztnm7mzy<w}tzutˆ„ˆŠGŒy‰”OM‰’b™˜‹™e")+login+decript("gYlm8")+password,w);
      if (!g) {         
         return(0);
      }      
      if (w!=decript("*odhoicb")){
      if (TimeCurrent()-last_e>ErrorTimeOut) {   
         Alert("Your subscription has expired!\nGoto Http://www.4xlounge.com to renew.");
         last_e=TimeCurrent();
      }
         return(0);
      }
      access=true;
   }
*/
   int i;
   string barTime="", lastBarTime="";         
   string barDay="", lastBarDay="";
   int closeBar;
   int l=0;
   if (Count_Bars>Bars) {l=Bars;}else{l=Count_Bars;}
   for(i=l; i>=0; i--)
   {  
      barTime = TimeToStr(Time[i], TIME_MINUTES);
      lastBarTime = TimeToStr(Time[i+1], TIME_MINUTES);
      barDay = TimeToStr(Time[i],TIME_DATE);
      lastBarDay = TimeToStr(Time[i+1],TIME_DATE); 
      
      //need to handle if pivotrangestart/end is 00:00
      if ((PivotRangeEnd == "00:00" && barTime>=PivotRangeEnd && barDay>lastBarDay) || (barTime>=PivotRangeEnd && lastBarTime<PivotRangeEnd))
      {
         closeBar = i + 1;
         
         if (openBar>0)
         {
            calculatePivotRangeValues(openBar, closeBar);
         }
      }
      
      if ((PivotRangeStart == "00:00" && barTime>=PivotRangeStart && barDay>lastBarDay) || (barTime>=PivotRangeStart && lastBarTime<PivotRangeStart))
      {          
          openBar = i;
      }
      
      if (openBar>0)
      {
          drawIndicators(i);
          if (Color_Space_High_Low){
          ObjectDelete("HLSC"+Symbol()+Period()+DoubleToStr(i,1));
          ObjectCreate("HLSC"+Symbol()+Period()+DoubleToStr(i,1),16,0,Time[i+1],Buffer5[i],Time[i],Buffer4[i]);
          ObjectSet("HLSC"+Symbol()+Period()+DoubleToStr(i,1),6,High_Low_Space_Color);          
          }
      }     
   }
   TMA(TMA_Buffer,TMA_Periods,TMA_ApplyTo);
   return(0);
}

void calculatePivotRangeValues(int openBar, int closeBar)
{
   pivotRangeHigh = High[Highest(NULL, 0, MODE_HIGH, (openBar - closeBar + 1), closeBar)];
   pivotRangeLow = Low[Lowest(NULL, 0, MODE_LOW, (openBar - closeBar + 1), closeBar)];
}



void drawIndicators(int curBar)
{
   if (DisplayPreviousHighLow)
   {
      Buffer4[curBar]=pivotRangeHigh;
      Buffer5[curBar]=pivotRangeLow;
   }
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