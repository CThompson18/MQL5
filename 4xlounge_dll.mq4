//+------------------------------------------------------------------+
//|                                                 ScreenUpload.mq4 |
//+------------------------------------------------------------------+

#import "wininet.dll"
int InternetAttemptConnect (int x);
int InternetOpenA(string sAgent, int lAccessType, string sProxyName = "", string sProxyBypass = "", int lFlags = 0);

int InternetConnectA( int hInternet, string lpszServerName, int nServerPort,
string lpszUsername, string lpszPassword, int dwService=1, int dwFlags=134217728, int lContext = 1 );

bool FtpPutFileA(int hConnect, string lpszLocalFile, string lpszNewRemoteFile, int dwFlags, int lContext = 1);
int InternetCloseHandle(int hInet);
#import


/*#import "kernel32.dll"
bool MoveFileA (string fromfile, string tofile);
*/
extern string File_Name="meterx.gif";
extern string File_Path="C:\\Program Files\\5FIBO Group\\experts\\files\\";
extern int    X_Width=1000;  //the dimensions of the chart
extern int    Y_Width=750;
extern string FTP_Path="/4xlounge.com/data/P6138/";
extern int    Update_Secconds=10;
extern string Server_address="10.10.10.1";
string User="riffster@pro.4xlounge.com";
extern string Password="zamk1209zamk";


datetime expire=D'2020.10.01 01:01:01';
datetime last_update=0;
int init(){
   start();
   return(0);
}
int deinit()
  {
   return(0);
  }
int start()
  {
   if (TimeCurrent()>expire) {
      Alert("This indicator has expired.");
      return(0);
   }
   int i=0;   
   if (TimeCurrent()>=last_update+ Update_Secconds){
      if (!WindowScreenShot(File_Name,X_Width,Y_Width)){
         i=GetLastError();
         if (i!=1 && i!=0) Print("Window screenshot error :"+error(i)+" at "+Symbol()+" "+Period());                              
      }
      /*FileDelete(File_Name);
      MoveFileA(File_Path+"tmp_"+File_Name, File_Path+File_Name);*/
      last_update=TimeCurrent();
   
//   Print("Screenshot made at : "+TimeToStr(Time[0])); 
   
   int rw = InternetAttemptConnect(0);
   int hInternetSession = InternetOpenA("Microsoft Internet Explorer", 0, "", "", 0);
   int hURL = InternetConnectA(hInternetSession, Server_address, 21, User, Password, 1, 0, 1);
   
   FtpPutFileA(hURL, File_Path+File_Name, FTP_Path+File_Name, 2, 1);
   InternetCloseHandle(hURL);
   InternetCloseHandle(hInternetSession);
   
   Print("Send FTP completed at : "+TimeToStr(Time[0])); 
   
   }
   return(0);
  }

string error(int err){
switch (err){
 case 0 :return("No error returned.");
 case 1 :return("No error returned, but the result is unknown. ");
 case 2 :return("Common error. ");
 case 3 :return("Invalid trade parameters. ");
 case 4 :return("Trade server is busy. ");
 case 5 :return("Old version of the client terminal. ");
 case 6 :return("No connection with trade server. ");
 case 7 :return("Not enough rights. ");
 case 8 :return("Too frequent requests."); 
 case 9 :return("Malfunctional trade operation. ");
 case 64 :return("Account disabled. ");
 case 65 :return("Invalid account. ");
 case 128 :return("Trade timeout. ");
 case 129 :return("Invalid price. ");
 case 130 :return("Invalid stops. ");
 case 131 :return("Invalid trade volume. ");
 case 132 :return("Market is closed. ");
 case 133 :return("Trade is disabled. ");
 case 134 :return("Not enough money. ");
 case 135 :return("Price changed. ");
 case 136 :return("Off quotes. ");
 case 137 :return("Broker is busy."); 
 case 138 :return("Requote. ");
 case 139 :return("Order is locked. ");
 case 140 :return("Long positions only allowed. ");
 case 141 :return("Too many requests. ");
 case 145 :return("Modification denied because order too close to market. ");
 case 146 :return("Trade context is busy. ");
 case 147 :return("Expirations are denied by broker. ");
 case 148 :return("The amount of open and pending orders has reached the limit set by the broker. ");
 case 4000 :return("No error. ");
 case 4001 :return("Wrong function pointer. ");
 case 4002 :return("Array index is out of range. ");
 case 4003 :return("No memory for function call stack. ");
 case 4004 :return("Recursive stack overflow. ");
 case 4005 :return("Not enough stack for parameter. ");
 case 4006 :return("No memory for parameter string. ");
 case 4007 :return("No memory for temp string. ");
 case 4008 :return("Not initialized string. ");
 case 4009 :return("Not initialized string in array. ");
 case 4010 :return("No memory for array string. ");
 case 4011 :return("Too long string. ");
 case 4012 :return("Remainder from zero divide. ");
 case 4013 :return("Zero divide. ");
 case 4014 :return("Unknown command. ");
 case 4015 :return("Wrong jump (never generated error). ");
 case 4016 :return("Not initialized array. ");
 case 4017 :return("DLL calls are not allowed. ");
 case 4018 :return("Cannot load library. ");
 case 4019 :return("Cannot call function. ");
 case 4020 :return("Expert function calls are not allowed. ");
 case 4021 :return("Not enough memory for temp string returned from function. ");
 case 4022 :return("System is busy (never generated error). ");
 case 4050 :return("Invalid function parameters count. ");
 case 4051 :return("Invalid function parameter value. ");
 case 4052 :return("String function internal error. ");
 case 4053 :return("Some array error. ");
 case 4054 :return("Incorrect series array using. ");
 case 4055 :return("Custom indicator error. ");
 case 4056 :return("Arrays are incompatible. ");
 case 4057 :return("Global variables processing error. ");
 case 4058 :return("Global variable not found. ");
 case 4059 :return("Function is not allowed in testing mode. ");
 case 4060 :return("Function is not confirmed. ");
 case 4061 :return("Send mail error. ");
 case 4062 :return("String parameter expected. ");
 case 4063 :return("Integer parameter expected. ");
 case 4064 :return("Double parameter expected. ");
 case 4065 :return("Array as parameter expected. ");
 case 4066 :return("Requested history data in updating state. ");
 case 4067 :return("Some error in trading function. ");
 case 4099 :return("End of file. ");
 case 4100 :return("Some file error."); 
 case 4101 :return("Wrong file name. ");
 case 4102 :return("Too many opened files. ");
 case 4103 :return("Cannot open file. ");
 case 4104 :return("Incompatible access to a file. ");
 case 4105 :return("No order selected. ");
 case 4106 :return("Unknown symbol. ");
 case 4107 :return("Invalid price. ");
 case 4108 :return("Invalid ticket. ");
 case 4109 :return("Trade is not allowed. ");
 case 4110 :return("Longs are not allowed. ");
 case 4111 :return("Shorts are not allowed. ");
 case 4200 :return("Object exists already. ");
 case 4201 :return("Unknown object property. ");
 case 4202 :return("Object does not exist. ");
 case 4203 :return("Unknown object type. ");
 case 4204 :return("No object name. ");
 case 4205 :return("Object coordinates error. ");
 case 4206 :return("No specified subwindow. ");
 case 4207 :return("Some error in object function. ");
 default: return("Unknown error:"+err);
}
}