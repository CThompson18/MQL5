//+------------------------------------------------------------------+
//|                                                     Flow_php.mq4 |
//+------------------------------------------------------------------+
#property copyright ""
#property link      ""

#property indicator_chart_window

#import "wininet.dll"
int InternetAttemptConnect (int x);
int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);

int InternetConnectA( int hInternet, string lpszServerName, int nServerPort,
string lpszUsername, string lpszPassword, int dwService=1, int dwFlags=134217728, int lContext = 1 );

bool FtpPutFileA(int hConnect, string lpszLocalFile, string lpszNewRemoteFile, int dwFlags, int lContext = 1);
int InternetCloseHandle(int hInet);

extern string File_Name="flow_values";
extern string FTP_Path="/4xlounge.com/charts/";
extern string File_Path="D:\\F\\MetaTrader - North Finance\\experts\\files\\";
extern int    Update_Secconds=30;
extern string Server="67.15.97.27";
extern string UserName="riffster";
extern string Password="hummer";


extern int    Periods = 8;
extern bool   invert_strength =  0;

string pairs[17]={"GBPAUD","EURCAD","EURGBP","EURCHF","GBPCHF","CADJPY","NZDJPY","AUDJPY","EURJPY","GBPJPY","USDJPY","USDCAD","USDCHF","NZDUSD","AUDUSD","GBPUSD","EURUSD"};
datetime last_update=0;

int init()
  {
   return(0);
  }

int deinit()
  {

   return(0);
  }
int start()
  {
   string fn=File_Name+".php";
   string fs="";
   FileDelete(fn);
   int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
   fs="<?php\n $e = array();\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);   
   int l=0;
   for (int kk=0;kk<17;kk++){
      for (int i=0;i<30;i++){
         double gr = iCustom(pairs[kk],0,"Heatmap_RSI",Periods,invert_strength,2,i);
         double re = iCustom(pairs[kk],0,"Heatmap_RSI",Periods,invert_strength,7,i);         
         int rez=0;
         if (gr!=0) {rez=1;}else if (re!=0) {rez=2;}
         fs="$e["+l+"]="+rez+";\n";
         FileWriteString(handle,fs,StringLen(fs));
         FileFlush(handle);               
         l++;
      }
   }
   fs="?>\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      
   FileClose(handle);
   
   if (TimeCurrent()>=last_update+Update_Secconds){
      int rw = InternetAttemptConnect(0);
      int hInternetSession = InternetOpenA("Microsoft Internet Explorer", 0, "", "", 0);
      int hURL = InternetConnectA(hInternetSession, Server, 21, UserName, Password, 1, 134217728, 1);
   
      FtpPutFileA(hURL, File_Path+fn, FTP_Path+fn, 2, 1);
      InternetCloseHandle(hURL);
      InternetCloseHandle(hInternetSession);
   
      Print("Send FTP completed at : "+TimeToStr(Time[0])); 
      last_update=TimeCurrent();            
   }

   return(0);
  }

