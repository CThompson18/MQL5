//+------------------------------------------------------------------+
//|                                                   new_system.mq4 |
//|                                                                  |
//+------------------------------------------------------------------+
// property indicator
//********************************************************************
#property indicator_chart_window
#property indicator_buffers 2
#property indicator_color1 Aqua
#property indicator_width1 2
#property indicator_color2 clrMediumSpringGreen
#property indicator_width2 2
//+------------------------------------------------------------------+
//| Global variables                                                 |
//+------------------------------------------------------------------+
//Multi-function variables
//*********************************************************************
//---- input parameters
extern double glbRisk = 3.025;
extern int glbCountBars = 300;
extern color glbText_Color = Red;
extern int glbFont_Size = 10;
extern string StartDate = "2017.03.31 00:00";

//BuySell orders.
//*********************************************************************
extern int glbSlippage = 1;
extern double glbLot_Size = .10;
string com = "chucknjill";

//---- buffers
//*********************************************************************
double glbSellArray[];
double glbBuyArray[];

//DataDisplay function global variables
//*********************************************************************
extern int glb_right_edge_shift = 175;
extern int glb_upper_edge_shift = 30;
extern int glbMagic_Number = 1234;
extern double glbLot_Size_Percent = .75;
string glb_ea_name = "ChucknJill Financials";
double last_p = 0;

 

int init()

  {
//---- indicator line
   IndicatorBuffers(2);
   SetIndexStyle(0,DRAW_ARROW);
   SetIndexArrow(0,234);
   SetIndexStyle(1,DRAW_ARROW);
   SetIndexArrow(1,233);
   SetIndexBuffer(0,glbSellArray);
   SetIndexBuffer(1,glbBuyArray);

//----
   return(0);
  }
//+------------------------------------------------------------------+
//| ASCTrend1sig                                                     |
//+------------------------------------------------------------------+
int start()
    {
		// if (glbCountBars>=1000) glbCountBars=950;
		SetIndexDrawBegin(0,Bars-glbCountBars+11+1);
		SetIndexDrawBegin(1,Bars-glbCountBars+11+1);
		int i,shift,counted_bars=IndicatorCounted(),Counter,i1,value10,value11;
		double x1,x2,value2,value3,TrueCount,Range,AvgRange,MRO1,MRO2;
		double Table_value2[5000];		
		
		value10 = 3 + glbRisk * 2;
		x1 = 67 + glbRisk;
		x2 = 33 - glbRisk;
		value11 = value10;

		//----
		if(Bars <= 11 + 1) return(0);
		//---- initial zero

		if(counted_bars < 11 + 1)

			{
				for(i = 1;i <= 0;i++) glbSellArray[glbCountBars - i] = 0.0;
				for(i = 1;i <= 0;i++) glbBuyArray[glbCountBars - i] = 0.0;
			}
		//----
		shift = glbCountBars - 11 - 1;
		while(shift >= 0)
			{
				Counter = shift;
				Range = 0.0;
				AvgRange = 0.0;

				for (Counter = shift; Counter <= shift + 9; Counter++) 
					AvgRange = AvgRange + MathAbs(High[Counter] - Low[Counter]);
					Range = AvgRange / 10;
					Counter = shift;
					TrueCount = 0;

				while (Counter<shift+9 && TrueCount<1)

					{
						if (MathAbs(Open[Counter] - Close[Counter + 1]) >= Range * 2.0) 
						TrueCount = TrueCount + 1;
						Counter = Counter + 1;
					}

				if (TrueCount >= 1) 
					{
						MRO1 = Counter;
					} 
					else 
					{
						MRO1 = -1;
					}
					
				Counter = shift;
				TrueCount = 0;
				
				while (Counter<shift+6 && TrueCount<1)

					{
						if (MathAbs(Close[Counter + 3] - Close[Counter]) >= Range * 4.6) 
							TrueCount = TrueCount + 1;
							Counter = Counter + 1;
					}
				if (TrueCount >= 1) 
					{
						MRO2 = Counter;
					} 
					else 
					{
						MRO2 = -1;
					}
				if (MRO1>-1) 
					{
						value11 = 3;
					} 
					else 
					{
						value11 = value10;
					}
				if (MRO2>-1) 
					{
						value11 = 4;
					} 
					else 
					{
						value11 = value10;
					}
				value2 = 100 - MathAbs(iWPR(NULL,0,value11,shift)); // PercentR(value11=9)
				Table_value2[shift] = value2;
				glbSellArray[shift] = 0;
				glbBuyArray[shift] = 0;
				value3 = 0;

				if (value2 < x2)
					{
						i1 = 1;

						while (Table_value2[shift + i1] >= x2 && Table_value2[shift + i1] <= x1)
							{
								i1++;
							}
						if (Table_value2[shift + i1] > x1) 
							{
								value3 = High[shift] + Range * 0.5;
								glbSellArray[shift] = value3;
								//DataDisplay(value3,"Sell");
							} 
					}

				if (value2 > x1)
					{
						i1=1;
						while (Table_value2[shift + i1] >= x2 && Table_value2[shift + i1] <= x1)
							{
								i1++;
							}
						if (Table_value2[shift + i1] < x2) 
							{
								value3 = Low[shift] - Range * 0.5;
								glbBuyArray[shift] = value3;
								//DataDisplay(value3,"Buy");
							}
					}      
					shift--;
/*			      if (glbSellArray[shift+1]!=0)
					   {
						   ObjectDelete("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0));
						   ObjectCreate("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0),21,0,Time[shift],Low[shift]-5*Point);       
						   ObjectSetText("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0),DoubleToStr(Open[shift],Digits),10,"TimesNewRoman",Aqua);
					   }	
			      if (glbBuyArray[shift+1]!=0)
					   {
						   ObjectDelete("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0));
						   ObjectCreate("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0),21,0,Time[shift],Low[shift]-5*Point);       
						   ObjectSetText("tag2"+Symbol()+Period()+DoubleToStr(glbRisk,0)+DoubleToStr(shift,0),DoubleToStr(Open[shift],Digits),10,"TimesNewRoman",clrMediumSpringGreen);
					   }					
*/				   				
			}
			//CommentTest();
			return(0);
    }

//+------------------------------------------------------------------+
int deinit()
	{
		return(0);
	}
	
	
//DataDisplay
//***************************************************************************************************************************************************
void DataDisplay(const double dvalue2,const string BuySell)
//void DataDisplay()
   {
		string text = "";
		int xAxis = (int)(ChartGetInteger(ChartID(),CHART_WIDTH_IN_PIXELS,0) - glb_right_edge_shift);
		int yAxis = glb_upper_edge_shift;

		text = "***************************************************";
		createObject(1,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift,text);
		
		//text="BuySell = "+DoubleToString(dvalue2 ,5);
		text= BuySell + " = "+DoubleToString(dvalue2 ,5);
		createObject(2,OBJ_LABEL,0,xAxis,yAxis,text);
		
		text = "***************************************************";
		createObject(3,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift,text);				
   }
   
//createObject
//***************************************************************************************************************************************************
void createObject(int st_ID,ENUM_OBJECT obj,int window,int xA,int yA,string txt="")
	{
		color text_color = Red;
		int text_size = 10;
		ObjectCreate(glb_ea_name + "_" + IntegerToString(st_ID),obj,window,xA,yA);
		ObjectSet(glb_ea_name + "_" + IntegerToString(st_ID),OBJPROP_XDISTANCE,xA);
		ObjectSet(glb_ea_name + "_" + IntegerToString(st_ID),OBJPROP_YDISTANCE,yA);
		ObjectSetText(glb_ea_name + "_" + IntegerToString(st_ID),txt,14,"TimesNewRoman",clrGreenYellow);
	} 
//Test
//***************************************************************************************************************************************************
  void CommentTest()
   {
      		double Ask,Bid;
            int Spread;
            Ask=SymbolInfoDouble(Symbol(),SYMBOL_ASK);
            Bid=SymbolInfoDouble(Symbol(),SYMBOL_BID);
            Spread=SymbolInfoInteger(Symbol(),SYMBOL_SPREAD);
         //--- Output values in three lines
            Comment(StringFormat("Show prices\nAsk = %G\nBid = %G\nSpread = %d",Ask,Bid,Spread));
   }           
            
 