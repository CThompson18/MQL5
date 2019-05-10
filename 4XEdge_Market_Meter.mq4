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


extern int       TF1_Periods1=60;
extern int       TF1_Shift1=21;
extern int       TF1_ApplyTo1=0;
extern int       TF2_Periods1=50;
extern int       TF2_Shift1=5;
extern int       TF2_ApplyTo1=0;
extern int       TF3_Periods1=18;
extern int       TF3_Shift1=5;
extern int       TF3_ApplyTo1=0;
extern int       TF4_Periods1=25;
extern int       TF4_Shift1=0;
extern int       TF4_ApplyTo1=0;


extern int       CountBars=1000;

extern double TF1=PERIOD_M15;
extern double TF2=PERIOD_M30;
extern double TF3=PERIOD_H1;
extern double TF4=PERIOD_H1;

extern color Text_Color_Short=Pink;
extern color Text_Color_Long=PaleGreen;
extern int Font_Size=21;

extern int BarWidth = 0;
extern color BarColorUp = Red;
extern color BarColorDown = Lime;


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
   double aa1[][6],aa2[][6],aa3[][6],aa4[][6];
   ArrayCopyRates(aa1,Symbol(),MathFloor(TF1));
   ArrayCopyRates(aa2,Symbol(),MathFloor(TF2));
   ArrayCopyRates(aa3,Symbol(),MathFloor(TF3));
   ArrayCopyRates(aa4,Symbol(),MathFloor(TF4));
   for (int i=CountBars;i>=0;i--){
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
      int j=iBarShift(Symbol(),TF4,Time[i],false);
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
         ObjectDelete("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0));
         ObjectCreate("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),21,0,Time[i],Low[i]-5*Point);       
         ObjectSetText("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),DoubleToStr(Open[i],Digits),Font_Size,"Arial",Text_Color_Short);
         dn=true;
         up=false;
      }
      if (!up && buf4_up[i]!=EMPTY_VALUE && buf3_up[i]!=EMPTY_VALUE && buf2_up[i]!=EMPTY_VALUE && buf1_up[i]!=EMPTY_VALUE && 
          (buf1_down[i+1]!=EMPTY_VALUE || (buf1_up[i+1]==EMPTY_VALUE && buf1_down[i+1]==EMPTY_VALUE) ||
           buf2_down[i+1]!=EMPTY_VALUE || (buf2_up[i+1]==EMPTY_VALUE && buf2_down[i+1]==EMPTY_VALUE) ||
           buf3_down[i+1]!=EMPTY_VALUE || (buf3_up[i+1]==EMPTY_VALUE && buf3_down[i+1]==EMPTY_VALUE) ||
           buf4_down[i+1]!=EMPTY_VALUE || (buf4_up[i+1]==EMPTY_VALUE && buf4_down[i+1]==EMPTY_VALUE)) ){
         ObjectDelete("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0));
         ObjectCreate("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),21,0,Time[i],Low[i]-5*Point);       
         ObjectSetText("tag4_tma"+Symbol()+Period()+DoubleToStr(TF1_Periods1,0)+DoubleToStr(TF2_Periods1,0)+DoubleToStr(i,0),DoubleToStr(Open[i],Digits),Font_Size,"Arial",Text_Color_Long);
         up=true;
         dn=false;
      }      
    }  

   return(0);
  }

