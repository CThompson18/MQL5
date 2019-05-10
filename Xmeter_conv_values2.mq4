#property copyright ""
#property link      ""

#property indicator_chart_window
/*#import "kernel32.dll"
bool MoveFileA (string fromfile, string tofile);*/
#import "wininet.dll"
int InternetAttemptConnect (int x);
int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);

int InternetConnectA( int hInternet, string lpszServerName, int nServerPort,
string lpszUsername, string lpszPassword, int dwService=1, int dwFlags=134217728, int lContext = 1 );

bool FtpPutFileA(int hConnect, string lpszLocalFile, string lpszNewRemoteFile, int dwFlags, int lContext = 1);
int InternetCloseHandle(int hInet);

extern string File_Name="xmeter_conv_values";
extern string File_Path="C:\\Program Files\\NewFibo2\\experts\\files\\";
extern int    Update_Secconds=10;
extern string FTP_Path="/data/";

extern string Server="10.10.10.2";
string UserName="riffster@4xedge.com";
extern string Password="pf4377dxzaQ~";

extern int       Periods=121;
extern int       Shift=0;
extern int       ApplyTo=0;

extern int       Atr_Period=50;
extern int       Count_Bars=101;

string pairs[17]={"GBPAUD","EURCAD","EURGBP","EURCHF","GBPCHF","CADJPY","NZDJPY","AUDJPY","EURJPY","GBPJPY","USDJPY","USDCAD","USDCHF","NZDUSD","AUDUSD","GBPUSD","EURUSD"};
int tf[7] = {1,5,15,30,60,240,1440};
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
     if (TimeCurrent()>=last_update+Update_Secconds){
   string fn=File_Name+".php";
   string fs="";
   FileDelete(fn);
   int handle=FileOpen(fn,FILE_WRITE|FILE_BIN);
   fs="<?php\n $x=array();\n $t=array();\n";
   FileWriteString(handle,fs,StringLen(fs));
//   FileFlush(handle);   
   fs = "";
   for (int kk=0;kk<17;kk++){
      fs=fs+"$x[\""+pairs[kk]+"\"]=array();\n";
      fs=fs+"$t[\""+pairs[kk]+"\"]=array();\n";
      for (int ll=0;ll<7;ll++){
         fs=fs+"$x[\""+pairs[kk]+"\"]["+ll+"]=array();\n";
         fs=fs+"$t[\""+pairs[kk]+"\"]["+ll+"]=array();\n";
         for (int yy=0;yy<50;yy++){
            double xm0=iCustom(pairs[kk],tf[ll],"X-Meter_conv",Periods,Shift,ApplyTo,Atr_Period,Count_Bars,0,yy);
            double xm1=iCustom(pairs[kk],tf[ll],"X-Meter_conv",Periods,Shift,ApplyTo,Atr_Period,Count_Bars,1,yy);
            if (xm0!=0) double val = xm0;
            else val=xm1;
            fs=fs+"$x[\""+pairs[kk]+"\"]["+ll+"]["+yy+"]="+DoubleToStr(val,MarketInfo(pairs[kk],MODE_DIGITS))+";\n";
            fs=fs+"$t[\""+pairs[kk]+"\"]["+ll+"]["+yy+"]="+iTime(pairs[kk],tf[ll],yy)+";\n";
            FileWriteString(handle,fs,StringLen(fs));
  //          FileFlush(handle);
            fs="";
         }
      }
   }
   fs=fs+"?>\n";
   FileWriteString(handle,fs,StringLen(fs));
   FileFlush(handle);      
   FileClose(handle);
   
      int rw = InternetAttemptConnect(0);
      int hInternetSession = InternetOpenA("Microsoft Internet Explorer", 0, "", "", 0);
      int hURL = InternetConnectA(hInternetSession, Server, 21, UserName, Password, 1, 0, 1);
   
      bool x = FtpPutFileA(hURL, File_Path+fn, FTP_Path+fn, 2, 1);
      InternetCloseHandle(hURL);
      InternetCloseHandle(hInternetSession);
   
      if (!x) Print("FAILED Send FTP completed at : "+TimeToStr(TimeCurrent()));
      /*FileDelete(File_Name+".php");
      MoveFileA(File_Path+fn, File_Path+File_Name+".php");*/
      last_update=TimeCurrent();            
   }

   return(0);
  }

