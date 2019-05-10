#property indicator_chart_window
#property indicator_buffers 2
#property indicator_color1 Red
#property indicator_color2 White

//---- input parameters
extern int RISK=2;
extern int CountBars=300;
extern color Text_Color=Red;
extern int Font_Size=74;

//---- buffers
double val1[];
double val2[];
double Table_value2[];

int init()
  {
//---- indicator line
   IndicatorBuffers(3);
   SetIndexStyle(0,DRAW_ARROW);
   SetIndexArrow(0,234);
   SetIndexStyle(1,DRAW_ARROW);
   SetIndexArrow(1,233);
   SetIndexBuffer(0,val1);
   SetIndexBuffer(1,val2);
   SetIndexBuffer(2,Table_value2);

//----
   return(0);
  }
//+------------------------------------------------------------------+
//| ASCTrend1sig                                                     |
//+------------------------------------------------------------------+
int start()
  {
   int i,shift;
   int Counter,i1,value10,value11;
   double value1,x1,x2;
   double value2,value3;
   double TrueCount,Range,AvgRange,MRO1,MRO2;
   if(CountBars>=Bars) CountBars=Bars;
   SetIndexDrawBegin(0,Bars-CountBars+SSP);
   SetIndexDrawBegin(1,Bars-CountBars+SSP);
   int i,shift;
   int i1,i2,K;
   double Range,AvgRange,smin,smax,SsMax,SsMin,price;
   bool uptrend,old;
//----

   if(Bars<=SSP+1) return(0);
//---- initial zero
/*if( counted_bars < SSP+1 ) {
      for(i=1;i<=SSP;i++) val1[CountBars-i]=0.0;
      for(i=1;i<=SSP;i++) val2[CountBars-i]=0.0;
   }
*/
//----
   int counted_bars = IndicatorCounted();
   if(counted_bars < 0)  return(-1);
   if(counted_bars > 0)   counted_bars--;
   int limit = Bars - counted_bars;
   if(counted_bars==0) limit-=1+SSP;

   
   value10=3+RISK*2;
   x1=67+RISK;
   x2=33-RISK;
   value11=value10;
//----
   shift=CountBars;
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
		{
			if (MathAbs(Open[Counter]-Close[Counter+1])>=Range*2.0) TrueCount=TrueCount+1;
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
				if(Bars>alertBar && shift==0 && (CurTime()-PrevAlertTime>Period()*Alert_Delay_In_Seconds)) 
					{

					Alert("Trendsignal V2.0 ",Period(),""," Mins Timeframe",Symbol()," BUY @",Ask,"");alertBar=Bars;
					if(Enablemail==true) 
						{
							SendMail(subjectDown+" "+Symbol(),textDown+" "+Close[1]+" "+Symbol()); 
						}
					PrevAlertTime=CurTime();
					}
			
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
      shift--;
     }

   return(0);
  }
//+------------------------------------------------------------------+

int deinit()
  {
   return(0);
  }

