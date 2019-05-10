//+------------------------------------------------------------------+
//|                                                 www.4xlounge.com |
//+------------------------------------------------------------------+
#property copyright "Dan Campbell"
#property link      "www.4xlounge.com"
//----
#property indicator_separate_window

/*#import "kernel32.dll"
bool MoveFileA (string fromfile, string tofile);
*/
//---- input parameters
#import "wininet.dll"
int InternetAttemptConnect (int x);
int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);

int InternetConnectA( int hInternet, string lpszServerName, int nServerPort,
string lpszUsername, string lpszPassword, int dwService=1, int dwFlags=0, int lContext = 1 );

bool FtpPutFileA(int hConnect, string lpszLocalFile, string lpszNewRemoteFile, int dwFlags, int lContext = 1);
int InternetCloseHandle(int hInet);

extern string File_Name="trendmap2";
extern string File_Path="c:\\Program Files\\4FIBO Group2\\experts\\files\\";
extern int    Update_Secconds=10;
extern string FTP_Path="/4xlounge.com/data/P4206/";

extern string Server="10.10.10.1";
string UserName="riffster@pro.4xlounge.com";
extern string Password="zamk1209zamk";

extern int TradeBar=0;


int TimeFrames[3] = {5,30,60};
double rsi_level[3] = {50,50,50};
int rsi_period[3] = {7,7,7};
double red_values[3] = {-40,-40,-45};
double green_values[3] = {40,40,45};

string currency[17]={"EURUSD","GBPUSD","AUDUSD","NZDUSD","USDCHF","USDCAD","USDJPY","GBPJPY","EURJPY","AUDJPY","NZDJPY","CADJPY","GBPCHF","EURCHF","EURGBP","EURCAD","GBPAUD"};

int ErrorTimeOut=30;
datetime last_update=0;

//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+
int init()
  {
//---- indicators
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custor indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
//---- 
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
  int start() 
  {
     if (TimeCurrent()>=last_update+Update_Secconds){
   string fn=File_Name+".php";
   string fs="";
   FileDelete(fn);
   int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
   fs="<?php\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);    
   fs="$danx = array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);    
   for (int ti=0;ti<17;ti++){
      fs="$danx[\'"+currency[ti]+"\'] = array();\n";
      FileWriteString(handle,fs,StringLen(fs));
      FileFlush(handle);    
      for (int val=0;val<3;val++) {         
         double v_red = iCustom(currency[ti],TimeFrames[val],"DanX",10,rsi_level[val],rsi_period[val],0,TradeBar);
         double v_green = iCustom(currency[ti],TimeFrames[val],"DanX",10,rsi_level[val],rsi_period[val],1,TradeBar);
         int rez =0;
         if (v_red<=red_values[val] && v_red!=EMPTY_VALUE) rez=1; //red image
         if (v_green>=green_values[val] && v_green!=EMPTY_VALUE) rez=2; //red image
         fs="$danx[\'"+currency[ti]+"\']["+val+"]="+rez+";\n"; 
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);    
      }
   }
   fs="?>\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      
   FileClose(handle);


      int rw = InternetAttemptConnect(0);
      int hInternetSession = InternetOpenA("Microsoft Internet Explorer", 0, "", "", 0);
      int hURL = InternetConnectA(hInternetSession, Server, 21, UserName, Password, 1, 0, 1);
   
      FtpPutFileA(hURL, File_Path+fn, FTP_Path+fn, 2, 1);
      InternetCloseHandle(hURL);
      InternetCloseHandle(hInternetSession);
   
      Print("Send FTP completed at : "+TimeToStr(Time[0])); 
      /*FileDelete(File_Name+".php");
      MoveFileA(File_Path+fn, File_Path+File_Name+".php");*/
      last_update=TimeCurrent();            
   }
   return(0);
  }
//+------------------------------------------------------------------+