//+------------------------------------------------------------------+
//|                                                   FiboPiv_v1.mq4 |
//|                                                          Kalenzo |
//|                                      bartlomiej.gorski@gmail.com |
//+------------------------------------------------------------------+
#property copyright "Kalenzo"
#property link      "bartlomiej.gorski@gmail.com"

#property indicator_chart_window
//+------------------------------------------------------------------+
//| Custom indicator initialization function                         |
//+------------------------------------------------------------------+

#include <mysql.mqh>
int mysql;

extern string R3_Message="R3 hit ";
extern string R2_Message="R2 hit ";
extern string R1_Message="R1 hit ";

extern string Pivot_Message="Pivot hit ";

extern string S3_Message="S3 hit ";
extern string S2_Message="S2 hit ";
extern string S1_Message="S1 hit ";



extern bool Send_To_Free=true;
extern string Free_Chart_Address="http://www.google.com/";
extern bool Send_To_Paid=true;
extern string Paid_Chart_Address="http://www.google.com/";
extern int MinutesTimeOut=15;
int init()
  {
//---- indicators
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator deinitialization function                       |
//+------------------------------------------------------------------+
int deinit()
  {
//----
   ObjectDelete("S1");
   ObjectDelete("S2");
   ObjectDelete("S3");
   ObjectDelete("R1");
   ObjectDelete("R2");
   ObjectDelete("R3");
   ObjectDelete("PIVIOT");
   ObjectDelete("Support 1");
   ObjectDelete("Support 2");
   ObjectDelete("Support 3");
   ObjectDelete("Piviot level");
   ObjectDelete("Resistance 1");
   ObjectDelete("Resistance 2");
   ObjectDelete("Resistance 3");
   Comment(" ");
//----
   return(0);
  }
//+------------------------------------------------------------------+
//| Custom indicator iteration function                              |
//+------------------------------------------------------------------+
double last_bid=0;

datetime lowr3=0;
datetime lowr2=0;
datetime lowr1=0;
datetime lows3=0;
datetime lows2=0;
datetime lows1=0;
datetime lowp=0;
int start()

  {

//----
double rates[1][6],yesterday_close,yesterday_high,yesterday_low;
ArrayCopyRates(rates, Symbol(), PERIOD_D1);

if(DayOfWeek() == 1)
{
   if(TimeDayOfWeek(iTime(Symbol(),PERIOD_D1,1)) == 5)
   {
       yesterday_close = rates[1][4];
       yesterday_high = rates[1][3];
       yesterday_low = rates[1][2];
   }
   else
   {
      for(int d = 5;d>=0;d--)
      {
         if(TimeDayOfWeek(iTime(Symbol(),PERIOD_D1,d)) == 5)
         {
             yesterday_close = rates[d][4];
             yesterday_high = rates[d][3];
             yesterday_low = rates[d][2];
         }
         
      }  
   }
}
else
{
    yesterday_close = rates[1][4];
    yesterday_high = rates[1][3];
    yesterday_low = rates[1][2];
}


//---- Calculate Pivots

Comment("\nYesterday quotations:\nH ",yesterday_high,"\nL ",yesterday_low, "\nC ",yesterday_close);
double R = yesterday_high - yesterday_low;//range
double p = (yesterday_high + yesterday_low + yesterday_close)/3;// Standard Pivot
double r3 = p + (R * 1.000);
double r2 = p + (R * 0.618);
double r1 = p + (R * 0.382);
double s1 = p - (R * 0.382);
double s2 = p - (R * 0.618);
double s3 = p - (R * 1.000);

drawLine(r3,"R3", LightGray,0);
drawLabel("61.8",r3,LightGray);
drawLine(r2,"R2", LightGray,0);
drawLabel("50.0",r2,LightGray);
drawLine(r1,"R1", LightGray,0);
drawLabel("38.2",r1,LightGray);

drawLine(p,"PIVIOT",Gold,1);
drawLabel("Piviot level",p,Gold);

drawLine(s1,"S1",LightGray,0);
drawLabel("-38.2",s1,LightGray);
drawLine(s2,"S2",LightGray,0);
drawLabel("-50.0",s2,LightGray);
drawLine(s3,"S3",LightGray,0);
drawLabel("-61.8",s3,LightGray);


      if (ObjectFind("R3")!=-1){
         double t=ObjectGetValueByShift("R3",0);         
         if (((last_bid>t && Bid<=t) || (last_bid>t && Bid<=t)) && TimeCurrent()-lowr3>=MinutesTimeOut*60){
            if (Send_To_Paid) send_mysql_message(R3_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Paid_Chart_Address,false);                     
            if (Send_To_Free) send_mysql_message(R3_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Free_Chart_Address,true);                     
            lowr3=TimeCurrent();
         }
      }
      if (ObjectFind("R2")!=-1){
         t=ObjectGetValueByShift("R2",0);         
         if (((last_bid>t && Bid<=t) || (last_bid>t && Bid<=t)) && TimeCurrent()-lowr2>=MinutesTimeOut*60){
            if (Send_To_Paid) send_mysql_message(R2_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Paid_Chart_Address,false);                  
            if (Send_To_Free) send_mysql_message(R2_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Free_Chart_Address,true);                     
            lowr2=TimeCurrent();
         }
      }
      if (ObjectFind("R1")!=-1){
         t=ObjectGetValueByShift("R1",0);         
         if (((last_bid>t && Bid<=t) || (last_bid>t && Bid<=t)) && TimeCurrent()-lowr1>=MinutesTimeOut*60){
            if (Send_To_Paid) send_mysql_message(R1_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Paid_Chart_Address,false);                  
            if (Send_To_Free) send_mysql_message(R1_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Free_Chart_Address,true);                     
            lowr1=TimeCurrent();
         }
      }
      if (ObjectFind("PIVIOT")!=-1){
         t=ObjectGetValueByShift("PIVIOT",0);         
         if (((last_bid>t && Bid<=t) || (last_bid>t && Bid<=t)) && TimeCurrent()-lowp>=MinutesTimeOut*60){
            if (Send_To_Paid) send_mysql_message(Pivot_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Paid_Chart_Address,false);                  
            if (Send_To_Free) send_mysql_message(Pivot_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Free_Chart_Address,true);                     
            lowp=TimeCurrent();
         }
      }
      if (ObjectFind("S1")!=-1){
         t=ObjectGetValueByShift("S1",0);         
         if (((last_bid>t && Bid<=t) || (last_bid>t && Bid<=t)) && TimeCurrent()-lows1>=MinutesTimeOut*60){
            if (Send_To_Paid) send_mysql_message(S1_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Paid_Chart_Address,false);                  
            if (Send_To_Free) send_mysql_message(S1_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Free_Chart_Address,true);                     
            lows1=TimeCurrent();
         }
      }
      if (ObjectFind("S2")!=-1){
         t=ObjectGetValueByShift("S2",0);         
         if (((last_bid>t && Bid<=t) || (last_bid>t && Bid<=t)) && TimeCurrent()-lows2>=MinutesTimeOut*60){
            if (Send_To_Paid) send_mysql_message(S2_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Paid_Chart_Address,false);                  
            if (Send_To_Free) send_mysql_message(S2_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Free_Chart_Address,true);                     
            lows2=TimeCurrent();
         }
      }
      if (ObjectFind("S3")!=-1){
         t=ObjectGetValueByShift("S3",0);         
         if (((last_bid>t && Bid<=t) || (last_bid>t && Bid<=t)) && TimeCurrent()-lows3>=MinutesTimeOut*60){
            if (Send_To_Paid) send_mysql_message(S3_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Paid_Chart_Address,false);                  
            if (Send_To_Free) send_mysql_message(S3_Message+" "+Symbol()+" "+tf()+" "+TimeToStr(TimeCurrent()),Free_Chart_Address,true);                     
            lows1=TimeCurrent();
         }
      }


//----
 last_bid=Bid; 
   return(0);
  }
//+------------------------------------------------------------------+
void drawLabel(string name,double lvl,color Color)
{
    if(ObjectFind(name) != 0)
    {
        ObjectCreate(name, OBJ_TEXT, 0, Time[10], lvl);
        ObjectSetText(name, name, 10, "Arial", Yellow);
        ObjectSet(name, OBJPROP_COLOR, Color);
    }
    else
    {
        ObjectMove(name, 0, Time[10], lvl);
    }
}


void drawLine(double lvl,string name, color Col,int type)
{
         if(ObjectFind(name) != 0)
         {
            ObjectCreate(name, OBJ_HLINE, 0, Time[0], lvl,Time[0],lvl);
            
            if(type == 1)
            ObjectSet(name, OBJPROP_STYLE, STYLE_SOLID);
            else
            ObjectSet(name, OBJPROP_STYLE, STYLE_DOT);
            
            ObjectSet(name, OBJPROP_COLOR, Col);
            ObjectSet(name,OBJPROP_WIDTH,1);
            
         }
         else
         {
            ObjectDelete(name);
            ObjectCreate(name, OBJ_HLINE, 0, Time[0], lvl,Time[0],lvl);
            
            if(type == 1)
            ObjectSet(name, OBJPROP_STYLE, STYLE_SOLID);
            else
            ObjectSet(name, OBJPROP_STYLE, STYLE_DOT);
            
            ObjectSet(name, OBJPROP_COLOR, Col);        
            ObjectSet(name,OBJPROP_WIDTH,1);
          
         }
}
void send_mysql_message(string m,string Chart_Address,bool free){   
   string host="76.162.254.133";
   string user="riffste_admin1";
   string password="hummer";
   if (!free){
      string DB="riffste_Signal4x";
   }else{DB="riffste_FreeSignal";}
   int clientflag=0;
   int port=3306;
   string socket="";
   int res=mysql_real_connect(mysql,host,user,password,DB,port,socket,clientflag);
   int err=GetLastError();
   //Print (res);
   //Print (socket);
   if (res==mysql) Print("connected");
   else Alert("error=",mysql," ",mysql_errno(mysql)," "); 
         string query="INSERT INTO Signal ( id , d , t , message,type,url) VALUES ( NULL, \'"+Year()+"-"+Month()+"-"+Day()+"\', \'"+Hour()+":"+Minute()+":"+Seconds()+"\',\'"+m+"\',1, \'"+Chart_Address+"\')";
         int length=StringLen(query);
         mysql_real_query(res,query,length);
         int myerr=mysql_errno(res);
         if (myerr>0)Print("error=",myerr);
}

string tf(){
   if (Period()==1) return ("M1");
   if (Period()==5) return ("M5");
   if (Period()==15) return ("M15");
   if (Period()==30) return ("M30");
   if (Period()==60) return ("H1");
   if (Period()==240) return ("H4");
   if (Period()==1440) return ("D1");
   if (Period()==PERIOD_W1) return ("W1");
   if (Period()==PERIOD_MN1) return ("MN1");
}

