DataDisplay(glbPrice_Bid,"Bid");


//DataDisplay
//***************************************************************************************************************************************************
void DataDisplay(const string strAsk,const double dblAsk,const string strBid,const double dblBid,const string strCurrent,const double dblCurrent)
//void DataDisplay()
   {
		string text = "";
		int xAxis = (int)(ChartGetInteger(ChartID(),CHART_WIDTH_IN_PIXELS,0) - glb_right_edge_shift);
		int yAxis = glb_upper_edge_shift;

		text = "***************************************************";
		createObject(1,OBJ_LABEL,0,xAxis,yAxis,text);
		
		text= strAsk + " = "+DoubleToString(dblAsk ,5);
		createObject(2,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift,text);
		
		text = "***************************************************";
		createObject(3,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 2,text);

		text= strBid + " = "+DoubleToString(dblBid ,5);
		createObject(4,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 3,text);
		
		text = "***************************************************";
		createObject(5,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 4,text);

		text= strCurrent + " = "+DoubleToString(dblCurrent ,5);
		createObject(6,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 5,text);
		
		text = "***************************************************";
		createObject(7,OBJ_LABEL,0,xAxis,yAxis + glb_upper_edge_shift * 6,text);				
					
						
   }