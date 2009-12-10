// PieChartWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "PieChart.h"
#include "CommTest.h"
#include "PieChartWnd.h"
#include <math.h>
#include <limits>
#define MAX_LABELHEIGHT		25
#define MAX_LABELGAP		18
#define GRADIENTL			120
#define GRADIENTD			10
#define PI			3.14159265
#define MAXINCLINEANGLE		89
#define MAX_PIECHARTPARAMS	150
// CPieChartWnd

IMPLEMENT_DYNAMIC(CPieChartWnd, CWnd)

CPieChartWnd::CPieChartWnd():i_elementIndex(0), d_totalVal(0)
{
	lb_param.hght = 20;
	lb_param.wdth = 50;
	lb_param.xGapLeft =  200;
	lb_param.xGapRight = 20;
	lb_param.yGap = 10;
	lb_param.fStyle =  Gdiplus::FontStyleRegular;
	lb_param.sFont = "";

	lb_info.lbRctHght= 0;
	lb_info.lbIndex = 0;
	lb_info.lbVisibleRectHght = 1000;
	lb_info.lbTextColor = Gdiplus::Color::Black;
	lb_info.lbValueRectColor = Gdiplus::Color::White;
	lb_info.sMaxLenString = "";

	bkg_params.cr_backgrnd = Gdiplus::Color::White;
	bkg_params.cr_GradientD = Gdiplus::Color::White;
	bkg_params.cr_GradientL = Gdiplus::Color::White;
	bkg_params.s_ChartTittle = "";
	bkg_params.f_ColorGradD = 10;
	bkg_params.f_ColorGradL = 200;
	bkg_params.sFont = "";
	bkg_params.fStyle =  Gdiplus::FontStyleRegular;
	bkg_params.i_HorizontalOffset = 0;

	fl_startAngle = 0;
	fl_startAngleIncline = 0;
	fl_InclineAngle = 0;
	f_depth = .5;
	b_SaveFlag = FALSE;
	b_ShowPercentages = TRUE;
	pie_ChartStyle = ThreeDStyle;
	cr_HighlightColor = Gdiplus::Color(107, 230, 255);
	s_saveImage = "";
	i_pieElement_front = -1;
	i_pieElement_last = -1;
}

CPieChartWnd::~CPieChartWnd()
{
	
	
	std::map<int, pie_chart_element*>::iterator iter =  map_pChart.begin();
	for (; iter != map_pChart.end(); ++iter){
		if(iter->second){
			pie_chart_element* ele = iter->second;
			if(ele)
				delete ele;
		}
	}
	map_pChart.clear();
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}


BEGIN_MESSAGE_MAP(CPieChartWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CPieChartWnd message handlers



void CPieChartWnd::OnPaint()
{

	CPaintDC dc(this);
	if (pie_ChartStyle == DoughnutStyle)
		DrawDoughnutStyle(&dc);
	if (pie_ChartStyle == TwoDStyle)
		Draw2DStyle(&dc);
	if (pie_ChartStyle == ThreeDStyle)
		Draw3DStyle(&dc);
}

int CPieChartWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//This is to stop the flickering effects on scrollbar on resizing and painting.
	ModifyStyle(0, WS_CLIPCHILDREN);
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	
	CRect rect;
	GetClientRect(rect);
	m_VscrollBar.Create(WS_CHILD | SBS_VERT, 
		CRect(rect.right -40, rect.top + 10, rect.right -20, rect.bottom - 10), 
		this, 
		1);
	return 0;
}

void CPieChartWnd::DrawDoughnutStyle(CDC* pDc)
{
	CRect rect;	
	CRect rectBnd;
	Gdiplus::Bitmap* mBtmap = NULL;
	Gdiplus::Graphics* graphics = NULL;
	float totalAngle = fl_startAngle;
	std::map<int, pie_chart_element*>::iterator iter;
	Gdiplus::Pen penOutLine(Gdiplus::Color::Black, 1); 
	Gdiplus::Pen penBkg(Gdiplus::Color::Black, 5);
	GetBoundRect(rectBnd);
	GetClientRect(rect);
	
	//Create the memory bitmap to hold the drawing for the control area.	
	mBtmap = new Gdiplus::Bitmap(rect.Width(), rect.Height());
	graphics = Gdiplus::Graphics::FromImage(mBtmap);

	graphics-> SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	
	DrawBackGround(graphics);
	Gdiplus::LinearGradientBrush brBkgndInner(Gdiplus::PointF((Gdiplus::REAL)rect.left, (Gdiplus::REAL) rect.top), 
								 Gdiplus::PointF((Gdiplus::REAL)rect.right, (Gdiplus::REAL)rect.bottom), 
								 bkg_params.cr_GradientL, 
								 bkg_params.cr_GradientD);
	
	
	Gdiplus::REAL rRatio = (Gdiplus::REAL)rectBnd.Width() / 15;
	Gdiplus::SolidBrush brHighLight(Gdiplus::Color(100, cr_HighlightColor.GetR(), 
									  cr_HighlightColor.GetG(), 
									  cr_HighlightColor.GetB()));
	Gdiplus::Pen pnHighLight(cr_HighlightColor);

	iter = map_pChart.begin();
	penBkg.SetBrush(&brBkgndInner);
	//Draw each pie item on the rectangle area.
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (ele){
			CRect rectDiff;
			rectDiff.CopyRect(rectBnd);
			
			double dx = ele->i_distIndex * cos (PI * (totalAngle + ele->f_angle / 2) / 180);
			double dy = ele->i_distIndex * sin (PI * (totalAngle + ele->f_angle / 2) / 180);
			
			rectDiff.left += (long)dx;
			rectDiff.right += (long)dx;
			rectDiff.top += (long)dy;
			rectDiff.bottom += (long)dy;

			Gdiplus::LinearGradientBrush crGradient(Gdiplus::Point(rectDiff.right/2, rectDiff.top), 
								   Gdiplus::Point(rectDiff.right/2, rectDiff.bottom), 
								   ele->cr_GradientL, 
								   ele->cr_GradientD);

			penOutLine.SetColor(CalculateGradientLight(ele->cr_Base, 160));
		
			graphics->FillPie(&crGradient, 
							  Gdiplus::RectF((Gdiplus::REAL)rectDiff.left, (Gdiplus::REAL)rectDiff.top, (Gdiplus::REAL)rectDiff.Width(), (Gdiplus::REAL)rectDiff.Height()), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
			
			crGradient.SetLinearColors(ele->cr_GradientD, ele->cr_GradientL);
			graphics->FillPie(&crGradient, 
							  Gdiplus::RectF(Gdiplus::REAL(rectDiff.left + rRatio), 
							  Gdiplus::REAL(rectDiff.top + rRatio), 
							  Gdiplus::REAL(rectDiff.Width() - rRatio * 2), 
							  Gdiplus::REAL(rectDiff.Height()- rRatio * 2)), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
			graphics->DrawPie(&penOutLine, 
				              Gdiplus::RectF((Gdiplus::REAL)rectDiff.left, (Gdiplus::REAL)rectDiff.top, (Gdiplus::REAL)rectDiff.Width(), (Gdiplus::REAL)rectDiff.Height()), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
			if (ele->b_select){
					graphics->FillPie(&brHighLight, 
							  Gdiplus::RectF((Gdiplus::REAL)rectDiff.left, (Gdiplus::REAL)rectDiff.top, (Gdiplus::REAL)rectDiff.Width(), (Gdiplus::REAL)rectDiff.Height()), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
					graphics->DrawPie(&pnHighLight, 
							  Gdiplus::RectF((Gdiplus::REAL)rectDiff.left, (Gdiplus::REAL)rectDiff.top, (Gdiplus::REAL)rectDiff.Width(), (Gdiplus::REAL)rectDiff.Height()), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
			}

			graphics->FillPie(&brBkgndInner, 
							  Gdiplus::RectF(Gdiplus::REAL(rectDiff.left +  4.5 * rRatio), 
							  Gdiplus::REAL(rectDiff.top +  4.5 * rRatio), 
							  Gdiplus::REAL(rectDiff.Width() - rRatio * 9), 
							  Gdiplus::REAL(rectDiff.Height()- rRatio * 9)), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);

			graphics->DrawPie(&penBkg, 
				              Gdiplus::RectF(Gdiplus::REAL(rectDiff.left + 4.5 * rRatio), 
							  Gdiplus::REAL(rectDiff.top + 4.5 * rRatio), 
							  Gdiplus::REAL(rectDiff.Width() - rRatio * 9), 
							  Gdiplus::REAL(rectDiff.Height()- rRatio * 9)), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
		
			totalAngle += ele->f_angle;
		}
	}
	
	DrawLabels(graphics);
	
	if(b_ShowPercentages)
		DrawPiechartPecentages(graphics, fl_startAngle);

	//free the graphics object.
	delete graphics;
	graphics = NULL;

	Gdiplus::Graphics gr(pDc->m_hDC);
	//Create the cached bitmap from the memory bitmap
	Gdiplus::CachedBitmap* btmp = new Gdiplus::CachedBitmap(mBtmap, &gr);

	SaveImageInternal(mBtmap);

	if (mBtmap){
		delete mBtmap;
		mBtmap = NULL;
	}
	//Draw the cached bitmap on the graphivs area.
	gr.DrawCachedBitmap(btmp, rect.left, rect.top);	
	//free the cached bitmap after draw.
	if (btmp){
		delete btmp;
		btmp = NULL;
	}
}

PIECHARTITEM CPieChartWnd::InsertItem(CString sElement, CString sLabel, double dValue, Gdiplus::Color crColor)
{
	
	if (sElement.Compare(_T("")) && !HasElement(sElement) && dValue > 0){
		
		pie_chart_element* element = new pie_chart_element;
		element->s_element = sElement;
		element->s_label = sLabel;
		element->d_value = dValue;
		element->f_angle = 0;
		element->f_percentage = 0;
		element->i_ID = i_elementIndex;
		map_pChart[i_elementIndex] = element;
		i_elementIndex++;
		d_totalVal += dValue;
		element->cr_Base = crColor;
		element->f_ColorGradD = GRADIENTD;
		element->f_ColorGradL = GRADIENTL;
		element->cr_GradientL = CalculateGradientLight(crColor, GRADIENTL);
		element->cr_GradientD = CalculateGradientDark(crColor, GRADIENTD);
		element->b_select = FALSE;
		element->i_distIndex = 0;
		element->pie_3d_props.i_alphaVal = 255;
		if (sLabel.GetLength() > lb_info.sMaxLenString.GetLength()){
			lb_info.sMaxLenString = sLabel;
		}

		UpdatePieChart();
		CalculateLabelRect();
		ResetItemScrollBar();
		Invalidate();
		return (PIECHARTITEM)element;
	}
	return NULL;
}

void CPieChartWnd::UpdatePieChart(void)
{
	std::map<int, pie_chart_element*>::iterator iter = map_pChart.begin();	
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (d_totalVal > 0){
			ele->f_percentage = float(ele->d_value / d_totalVal);
			ele->f_angle = 360 * ele->f_percentage;
			ele->pie_3d_props.f_InclineAngle = ele->f_angle;
		}
	}
}

Gdiplus::Color CPieChartWnd::CalculateGradientLight(Gdiplus::Color crBase, float fGradVal)
{

	BYTE r = crBase.GetR();
	BYTE g = crBase.GetG();
	BYTE b = crBase.GetB();

	float fact = 255.0f;
	float rGrad = (255 - r) / fact;
	float gGrad = (255 - g) / fact;
	float bGrad = (255 - b) / fact;
	
	r =  BYTE(min(r + rGrad * fGradVal, 255));
	b =  BYTE(min(b + bGrad * fGradVal, 255));
	g =  BYTE(min(g + gGrad * fGradVal, 255));

	return Gdiplus::Color(r, g, b);
}

Gdiplus::Color CPieChartWnd::CalculateGradientDark(Gdiplus::Color crBase, float fGradVal)
{

	BYTE r = crBase.GetR();
	BYTE g = crBase.GetG();
	BYTE b = crBase.GetB();

	float fact = 255.0f;
	float rGrad = r / fact;
	float gGrad = g / fact;
	float bGrad = b / fact;

	r = BYTE(max(r - rGrad * fGradVal, 0));
	b = BYTE(max(b - bGrad * fGradVal, 0));
	g = BYTE(max(g - gGrad * fGradVal, 0));

	return Gdiplus::Color(r, g, b);
}

BOOL CPieChartWnd::HasElement(CString sElement){
	
	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.begin();
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (!ele->s_element.Compare(sElement))
			return TRUE;
	}
	return FALSE;
}
int CPieChartWnd::GetElementID(CString sElement)
{

	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.begin();
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (!ele->s_element.Compare(sElement))
			return ele->i_ID;
	}
	return -1;
}

BOOL CPieChartWnd::RemoveItem(CString sElement)
{

	std::map<int, pie_chart_element*>::iterator iter;
	
	iter = map_pChart.begin();
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (!ele->s_element.Compare(sElement)){
			d_totalVal -= ele->d_value;
			delete ele;
			ele = NULL;
			map_pChart.erase(iter);
			i_elementIndex--;
			ReArrangeElementMap();
			UpdatePieChart();
			CalculateLabelRect();
			GetVerticalScrollPos(m_VscrollBar.GetScrollPos(), false);
			ResetItemScrollBar();
			Invalidate();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPieChartWnd::RemoveItem(int iElementID){
	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.find(iElementID);
	if (iter != map_pChart.end()){
		pie_chart_element* ele = iter->second;
		d_totalVal -= ele->d_value;
		delete ele;
		ele = NULL;
		map_pChart.erase(iter);
		i_elementIndex--;
		ReArrangeElementMap();
		UpdatePieChart();
		CalculateLabelRect();
		GetVerticalScrollPos(m_VscrollBar.GetScrollPos(), false);
		ResetItemScrollBar();
		Invalidate();
		return TRUE;
	}
	return FALSE;		
}

PIECHARTITEM CPieChartWnd::GetPieChartItem(CString sElement){
	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.begin();
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (!ele->s_element.Compare(sElement))
			return (PIECHARTITEM)ele;
	}
	return NULL;
}

BOOL CPieChartWnd::UpdateItemLabel(PIECHARTITEM pItem, CString sLabel){
	if (pItem){
		((pie_chart_element*)pItem)->s_label = sLabel;
		//Reset the maximum length label string
		lb_info.sMaxLenString = "";
		std::map<int, pie_chart_element*>::iterator iter;
		iter = map_pChart.begin();
		for(;iter != map_pChart.end(); ++iter){
			pie_chart_element* ele = iter->second;
			if (ele->s_label.GetLength() > lb_info.sMaxLenString.GetLength()){
				lb_info.sMaxLenString = ele->s_label;
			}
		}
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

BOOL CPieChartWnd::UpdateItemValue(PIECHARTITEM pItem, double dVal){
	if (pItem && dVal != 0){
		d_totalVal -= ((pie_chart_element*)pItem)->d_value;
		((pie_chart_element*)pItem)->d_value = dVal;
		d_totalVal += dVal;
		UpdatePieChart();
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

BOOL CPieChartWnd::UpdateItemColor(PIECHARTITEM pItem, Gdiplus::Color crItem){
	if (pItem){
		((pie_chart_element*)pItem)->cr_Base = crItem;
		((pie_chart_element*)pItem)->cr_GradientD = CalculateGradientDark(((pie_chart_element*)pItem)->cr_Base,   
																		   ((pie_chart_element*)pItem)->f_ColorGradD);
		
		((pie_chart_element*)pItem)->cr_GradientL = CalculateGradientLight(((pie_chart_element*)pItem)->cr_Base,
																			((pie_chart_element*)pItem)->f_ColorGradL);

		Invalidate();
		return TRUE;
	}
	return FALSE;
}

void CPieChartWnd::GetBoundRect(LPRECT rect){
	CRect rectWn;
	GetClientRect(rectWn);
	rectWn.right = lb_param.xGapLeft;
	rectWn.top += 100;
	rectWn.left += 30;
	rectWn.bottom -= 30;

	if (rectWn.Height() > rectWn.Width()){
		rectWn.right = max(rectWn.right, (rectWn.left + MAX_PIECHARTPARAMS));
		rectWn.bottom = rectWn.top +  rectWn.Width();
	}
	else{
		rectWn.bottom = max(rectWn.bottom, (rectWn.top + MAX_PIECHARTPARAMS));
		rectWn.right = rectWn.left + rectWn.Height();
	}

	if (rectWn.Height() % 2 != 0){			//This is added to preserve the symmetry of the pie
		rectWn.bottom += 1;					//Much important on larger inclinations
		rectWn.right += 1;
		lb_param.xGapLeft += 1;
	}
	rectWn.left += bkg_params.i_HorizontalOffset;
	rectWn.right += bkg_params.i_HorizontalOffset;
	CopyRect(rect, &rectWn);
}

void CPieChartWnd::DrawLabels(Gdiplus::Graphics* graphics){
	CRect rect;
	CRect rectBnd;
	Gdiplus::StringFormat strFormat;
	Gdiplus::REAL iValueRectPos = 0;
	Gdiplus::REAL iTextBoundLen = 0;
	Gdiplus::RectF labelTextRect;
	Gdiplus::RectF textArea;
	std::map<int, pie_chart_element*>::iterator iter;	
	Gdiplus::SolidBrush brRect(Gdiplus::Color::Black);;
	Gdiplus::Pen pnRect(Gdiplus::Color::Black, 2);
	Gdiplus::SolidBrush brText(lb_info.lbTextColor); 
	Gdiplus::SolidBrush brValueRect(lb_info.lbValueRectColor); 
	Gdiplus::Color crRectLight;
	
	GetBoundRect(rectBnd);
	GetClientRect(rect);
	//Gdiplus::Color to paint the seperators in % value boxes
	Gdiplus::Color crLight = CalculateGradientLight(lb_info.lbValueRectColor, 200);	
	Gdiplus::Pen pnVal(crLight, 1);	
	
	rect.top = rectBnd.top;
	rect.bottom = rectBnd.bottom;

	strFormat.SetFormatFlags(Gdiplus::StringFormatFlagsLineLimit);
	
	int iFontSize = min(rectBnd.Height() / 25, 11);
	Gdiplus::REAL iRectOffset = 0;
	Gdiplus::Font textF(lb_param.sFont, (Gdiplus::REAL)iFontSize, lb_param.fStyle, Gdiplus::UnitPoint, NULL);
	
	int iEleCount = (int)map_pChart.size();
	int iStartHght = max(rectBnd.CenterPoint().y - ((lb_param.hght + lb_param.yGap) * iEleCount)/2, //The starting height calculated by deviding elements to 
							  rectBnd.top);															//upper and lower halfs taken from the center point.
					
	
	int iLabelColorBoxSt = rectBnd.right + rect.Width() / 20;
	
	int iLabelTextSt = iLabelColorBoxSt + lb_param.wdth +  rect.Width() / 20;
	graphics->MeasureString(lb_info.sMaxLenString,							//Measure the string length for the longest label 
							lb_info.sMaxLenString.GetLength(),				//string from the starting point of string labels
							&textF, 
							Gdiplus::RectF((Gdiplus::REAL)iLabelTextSt,0, 0, 0), 
							&labelTextRect); 
		
	if (labelTextRect.GetRight() > rect.right - lb_param.xGapRight)
		labelTextRect.Width =  rect.right - (labelTextRect.X + lb_param.xGapRight);

	iValueRectPos =  labelTextRect.GetRight() + rect.Width() / 20;		//Set the percentage rectangle start position using the string lenghth offset


	graphics->MeasureString(L"100.00%", (INT)strlen("100.00%"),		//Calculate the percentage string length
							&textF, Gdiplus::RectF(iValueRectPos, 
							0, 
							0, 
							(Gdiplus::REAL)lb_param.hght), 
							&textArea); 
	
	iTextBoundLen = textArea.Width;			
	
	if (textArea.GetRight() > rect.right - lb_param.xGapRight)
		iTextBoundLen =  (Gdiplus::REAL)rect.right - ((Gdiplus::REAL)lb_param.xGapRight + iValueRectPos);
	else{
		iRectOffset = rect.right - (lb_param.xGapRight + iValueRectPos + iTextBoundLen + 10);
	}
	
	iter = map_pChart.find(lb_info.lbIndex);

	graphics->FillRectangle(&brValueRect,		//Fill the value showing rectangle area
							Gdiplus::RectF(iValueRectPos + iRectOffset, 
								  (Gdiplus::REAL)iStartHght, 
								  iTextBoundLen, 
								  (Gdiplus::REAL)(min(lb_info.lbRctHght, lb_info.lbVisibleRectHght))
								  ));
	int iStartH = iStartHght;
	while (iStartH + lb_param.hght <= rectBnd.bottom && iter != map_pChart.end()){
		pie_chart_element* ele = iter->second;
		if (ele) {
			brRect.SetColor(ele->cr_Base);
			crRectLight = CalculateGradientLight(ele->cr_Base, 200);
			pnRect.SetColor(crRectLight);
			graphics->FillRectangle(&brRect, 
				Gdiplus::RectF(Gdiplus::REAL(max(iLabelColorBoxSt + iRectOffset, rectBnd.right)), 
									(Gdiplus::REAL)iStartH, 
									(Gdiplus::REAL)lb_param.wdth, 
									(Gdiplus::REAL)lb_param.hght));
			graphics->DrawRectangle(&pnRect, 
									Gdiplus::RectF(Gdiplus::REAL(max(iLabelColorBoxSt + iRectOffset, rectBnd.right)), 
									(Gdiplus::REAL)iStartH, 
									(Gdiplus::REAL)lb_param.wdth, 
									(Gdiplus::REAL)lb_param.hght));
			
			graphics->DrawString(ele->s_label, 
								 ele->s_label.GetLength(), 
								 &textF, 
								 Gdiplus::RectF(Gdiplus::REAL(iLabelTextSt + iRectOffset), 
								 (Gdiplus::REAL)(iStartH), 
									    labelTextRect.Width, 
										(Gdiplus::REAL)lb_param.hght), 
								 &strFormat, 
								 &brText);
			
			Gdiplus::REAL yPos = (Gdiplus::REAL)iStartH + lb_param.hght + Gdiplus::REAL(lb_param.yGap / 2);
		
			Gdiplus::PointF ptStart(Gdiplus::REAL(iValueRectPos + iRectOffset), yPos);
			Gdiplus::PointF ptEnd(Gdiplus::REAL(iValueRectPos + iTextBoundLen + iRectOffset), yPos);
			
			if (yPos - iStartHght < lb_info.lbVisibleRectHght && iTextBoundLen > 0)
				graphics->DrawLine(&pnVal, ptStart, ptEnd);
			
			CString sValue;
			sValue.Format(L"%.2f", ele->f_percentage * 100);
			sValue.Append(L"%");

			if (ele->f_percentage * 100 < 10)
				sValue = L"0" + sValue; 

			if(iTextBoundLen > 0)
				graphics->DrawString(sValue, 
									 sValue.GetLength(), 
									 &textF, 
									 Gdiplus::RectF(Gdiplus::REAL(iValueRectPos + iRectOffset), 
										   (Gdiplus::REAL)iStartH, 
										   (Gdiplus::REAL)iTextBoundLen, 
										   (Gdiplus::REAL)textArea.Height), 
									 &strFormat, 
									 &brText);
			iStartH += (lb_param.hght + lb_param.yGap);	
			iter++;
		}
	}
	
	
}
void CPieChartWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	CRect rectBnd;
	GetClientRect(rect);
	lb_param.xGapLeft =  max(2 * rect.Width() / 3, MAX_PIECHARTPARAMS + 30);
	lb_param.xGapRight = rect.Width() / 20;

	GetBoundRect(rectBnd);
	//Reset the label sizes on sizing the client area
	lb_param.hght = min(rectBnd.Height() / 15, MAX_LABELHEIGHT);
	lb_param.wdth = min(rectBnd.Width() , MAX_LABELHEIGHT * 15) / 8;
	lb_param.yGap = min (rectBnd.Height() / 20, MAX_LABELGAP);
	

	m_VscrollBar.MoveWindow(CRect(rect.right - lb_param.xGapRight, 
								  rectBnd.top, 
								  rect.right - lb_param.xGapRight + 20, 
								  rectBnd.bottom));
	//Reset the label rectangle area
	CalculateLabelRect();
	//Reset the item scrollbar
	ResetItemScrollBar();
	GetVerticalScrollPos(m_VscrollBar.GetScrollPos(), false);
}

void CPieChartWnd::ResetItemScrollBar(void)
{
	CRect rect;
	GetBoundRect(rect);
	SCROLLINFO	siV;
	siV.cbSize = sizeof(SCROLLINFO);
	siV.fMask  = SIF_PAGE | SIF_RANGE;
	siV.nPage  = rect.Height();
	siV.nMax   = lb_info.lbRctHght;
	siV.nMin   = 0 ;

	m_VscrollBar.SetScrollInfo(&siV,TRUE);
		
	if (lb_info.lbRctHght > rect.Height())
		m_VscrollBar.ShowScrollBar();
	else
		m_VscrollBar.ShowScrollBar(FALSE);

	
}

void CPieChartWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int iScrollBarPos = m_VscrollBar.GetScrollPos();
	int iScrollmt = m_VscrollBar.GetScrollLimit();
	CRect rFrame;
	GetBoundRect(rFrame);

	switch(nSBCode)
	{
		case SB_LINEUP:
			iScrollBarPos = iScrollBarPos - (lb_param.hght + lb_param.yGap);
			if(lb_info.lbIndex == 0)
				break;
			lb_info.lbIndex--;
			if (lb_info.lbIndex == 0){
				iScrollBarPos = 0;
				lb_info.lbIndex = 0;
			}

		break;

		case SB_LINEDOWN: 
			{
				if (lb_info.lbIndex == i_elementIndex)
					break;
				iScrollBarPos = iScrollBarPos + lb_param.hght + lb_param.yGap;
				int i = (rFrame.Height()- lb_param.hght)/ (lb_param.hght + lb_param.yGap) + 1;
				i  = i_elementIndex - i;
				if (i == lb_info.lbIndex)
				break;
				lb_info.lbIndex++;
		
				if (lb_info.lbIndex == i){
				iScrollBarPos = iScrollmt;
				lb_info.lbIndex = i;
				}
				break;
			}
		case SB_PAGEUP:
			iScrollBarPos = max(iScrollBarPos - rFrame.Height(), 0);
			GetVerticalScrollPos(iScrollBarPos, false);
		break;

		case SB_PAGEDOWN:
			iScrollBarPos = min(iScrollBarPos + rFrame.Height(), m_VscrollBar.GetScrollLimit());
			GetVerticalScrollPos(iScrollBarPos, false);
		break;

		case SB_THUMBTRACK:

		case SB_THUMBPOSITION:
		{
			SCROLLINFO si;

			ZeroMemory(&si, sizeof(SCROLLINFO));

			si.cbSize	= sizeof(SCROLLINFO);
			si.fMask	= SIF_TRACKPOS;

			if (m_VscrollBar.GetScrollInfo(&si, SIF_TRACKPOS)){
				
				iScrollBarPos = si.nTrackPos;
				GetVerticalScrollPos(iScrollBarPos, true);
			}
			else
				iScrollBarPos = (UINT)nPos;
			break;
		}
		break;
	}		
	
	Invalidate();
	m_VscrollBar.SetScrollPos(iScrollBarPos, 1);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPieChartWnd::CalculateLabelRect(void)
{
	//This method reset the rectangle height for the % value showing area
	CRect rect;
	GetBoundRect(rect);
	int iStartHght = 0;
	std::map<int, pie_chart_element*>::iterator iter = map_pChart.begin();
	for (; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (ele) {
			iStartHght += (lb_param.hght + lb_param.yGap);
			if (rect.Width() >= iStartHght - lb_param.yGap)
				lb_info.lbVisibleRectHght = iStartHght - lb_param.yGap;
		}
	}
	lb_info.lbRctHght = iStartHght  - lb_param.yGap;
}

int CPieChartWnd::GetVerticalScrollPos(int iScrollPos, BOOL bDrag){
	//This method finds the element (index) which should start on drawing the labels according to the scroll position. 
	CRect rect;
	GetBoundRect(rect);
	if ((m_VscrollBar.GetScrollLimit() == 0) && bDrag)
		return lb_info.lbIndex;

	if ((m_VscrollBar.GetScrollLimit() - m_VscrollBar.GetScrollPos() < lb_param.hght) && bDrag) {
		
		int rectHght = rect.Height();
		rectHght -= lb_param.hght;
		int itmCount  = 1 + (rectHght) / (lb_param.hght + lb_param.yGap);
		lb_info.lbIndex =  i_elementIndex - itmCount;
		m_VscrollBar.SetScrollPos( m_VscrollBar.GetScrollLimit());
	}
	else if (iScrollPos > 0){
		lb_info.lbIndex = iScrollPos / (lb_param.hght + lb_param.yGap);
	}
	else if (iScrollPos == 0){
		lb_info.lbIndex = 0;
	}
	return lb_info.lbIndex;
}


void CPieChartWnd::SetLabelFont(LOGFONT lf)
{
	lb_param.sFont.Format(L"%s", lf.lfFaceName);

	lb_param.fStyle = lf.lfWeight ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;
	lb_param.fStyle	|= lf.lfItalic ? Gdiplus::FontStyleItalic : lb_param.fStyle;
	lb_param.fStyle |= lf.lfUnderline ? Gdiplus::FontStyleUnderline :lb_param.fStyle ;
	lb_param.fStyle	|= lf.lfStrikeOut ? Gdiplus::FontStyleStrikeout : lb_param.fStyle;
	Invalidate();
}

void CPieChartWnd::SetLabelBoxColor(Gdiplus::Color crTextColor)
{
	lb_info.lbValueRectColor = crTextColor;
	Invalidate();
}

void CPieChartWnd::SetBackgrndColor(Gdiplus::Color crBkgColor)
{
	bkg_params.cr_backgrnd = crBkgColor;
	bkg_params.cr_GradientD = CalculateGradientDark(crBkgColor, bkg_params.f_ColorGradD);
	bkg_params.cr_GradientL = CalculateGradientLight(crBkgColor, bkg_params.f_ColorGradL);
	Invalidate();
}
BOOL CPieChartWnd::SetPieElementGradientLight(PIECHARTITEM pItem, float flGradientVal){
	if (pItem) {
		CRect rect;
		GetBoundRect(rect);
		((pie_chart_element*)pItem)->f_ColorGradL = flGradientVal;
		((pie_chart_element*)pItem)->cr_GradientL = CalculateGradientLight(((pie_chart_element*)pItem)->cr_Base,
																			flGradientVal);
		Invalidate();
		return true;
	}
	return false;
}	
BOOL CPieChartWnd::SetPieElementGradientDark(PIECHARTITEM pItem, float flGradientVal){
	if (pItem) {
		CRect rect;
		GetBoundRect(rect);
		((pie_chart_element*)pItem)->f_ColorGradD = flGradientVal;
		((pie_chart_element*)pItem)->cr_GradientD = CalculateGradientDark(((pie_chart_element*)pItem)->cr_Base,
																			flGradientVal);
		Invalidate();
		return true;
	}
	return false;
}

BOOL CPieChartWnd::SetBackGroundGradientLight(float flGradientVal){
	CRect rect;
	GetClientRect(rect);
	bkg_params.f_ColorGradL = flGradientVal;
	bkg_params.cr_GradientL = CalculateGradientLight(bkg_params.cr_backgrnd, flGradientVal);
	Invalidate();
	return false;
}
BOOL CPieChartWnd::SetBackGroundGradientDark(float flGradientVal){
	CRect rect;
	GetClientRect(rect);
	bkg_params.f_ColorGradD = flGradientVal;
	bkg_params.cr_GradientD = CalculateGradientDark(bkg_params.cr_backgrnd, flGradientVal);
	Invalidate();
	return false;
}
BOOL CPieChartWnd::SetPieElementGradientDarkAll(float flGradientVal){

	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.begin();
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		ele->f_ColorGradD = flGradientVal;
		ele->cr_GradientD = CalculateGradientDark(ele->cr_Base, flGradientVal);		
	}
	Invalidate();
	return true;
}
BOOL CPieChartWnd::SetPieElementGradientLightAll(float flGradientVal){

	std::map<int, pie_chart_element*>::iterator iter;
	CRect rectBnd;
	iter = map_pChart.begin();
	GetBoundRect(rectBnd);
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		ele->cr_GradientL = CalculateGradientLight(ele->cr_Base, flGradientVal);
	}
	Invalidate();
	return true;
}
void CPieChartWnd::SetStartAngle(float flAngle)
{
	while (flAngle >= 360){
		flAngle -= 360;
	}
	fl_startAngle = fl_startAngleIncline = flAngle;
	Invalidate();
}

void CPieChartWnd::SetPieChartTitle(CString sTittle)
{
	bkg_params.s_ChartTittle = sTittle;
	Invalidate();
}

void CPieChartWnd::SetLabelColor(Gdiplus::Color crLbColor)
{
	lb_info.lbTextColor = crLbColor;
	Invalidate();
}

void CPieChartWnd::DrawPiechartPecentages(Gdiplus::Graphics* pGraphics, float flStartAngleS)
{
	//Draw the % values on top of the pie chart elements
	CRect rectBnd, rectTop, rectBtm;
	GetBoundRect(rectBnd);
	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.begin();

	int iFontSize = min(rectBnd.Height() / 25, 11);
	iFontSize = max(iFontSize, 7);
	float flStartIncline = fl_startAngleIncline;
	float flStartAngle = fl_startAngle;
	int iDistance = rectBnd.Width();
	Gdiplus::RectF textArea;
	Gdiplus::SolidBrush textBr(lb_info.lbTextColor);
	Gdiplus::Font textF(lb_param.sFont, (Gdiplus::REAL)iFontSize, lb_param.fStyle, Gdiplus::UnitPoint, NULL);

	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (ele){
			Gdiplus::REAL xPoint = 0;
			Gdiplus::REAL yPoint = 0;
			if (pie_ChartStyle == ThreeDStyle){	//If the style is 3-D
				Gdiplus::GraphicsPath path;
				Gdiplus::PointF ptInPie;
				Get3DBounds(rectTop, rectBtm);	
				Gdiplus::REAL flDisplace = flStartIncline + Gdiplus::REAL(ele->pie_3d_props.f_InclineAngle / 2);
				
				double dx_rect = ele->i_distIndex * cos (PI * (flDisplace) / 180);
				double dy_rect = ele->i_distIndex * sin (PI * (flDisplace) / 180);
				
				rectTop.left += (long)dx_rect;
				rectTop.top += (long)dy_rect;
				rectTop.right += (long)dx_rect;
				rectTop.bottom += (long)dy_rect;

				Gdiplus::REAL rOffsetX = Gdiplus::REAL(rectTop.Width() / 10);
				Gdiplus::REAL rOffSetY = Gdiplus::REAL(rectTop.Height() / 10);

				path.AddPie(Gdiplus::RectF(Gdiplus::REAL(rectTop.left + rOffsetX), Gdiplus::REAL(rectTop.top + rOffSetY),	//Add pie to the path andget the correct location point
					Gdiplus::REAL(rectTop.Width()- rOffsetX * 2), Gdiplus::REAL(rectTop.Height() - rOffSetY * 2)),
					Gdiplus::REAL(flStartIncline), Gdiplus::REAL(ele->pie_3d_props.f_InclineAngle / 2));
				path.GetLastPoint(&ptInPie);
				path.Reset();

				xPoint =  ptInPie.X;
				yPoint = ptInPie.Y;
				flStartIncline += ele->pie_3d_props.f_InclineAngle;
			}
			else{
				yPoint = Gdiplus::REAL(rectBnd.CenterPoint().y + 
					(2 * (iDistance / 5 + ele->i_distIndex / 2)) * sin(PI * (flStartAngle + ele->f_angle / 2) / 180));
				xPoint = Gdiplus::REAL(rectBnd.CenterPoint().x + 
				(2 * (iDistance / 5 + ele->i_distIndex / 2)) * cos(PI * (flStartAngle  + ele->f_angle / 2) / 180));
			}
			
			CString sVal;
			sVal.Format(L"%.2f", ele->f_percentage * 100);
			sVal.Append(L"%");
		
			pGraphics->MeasureString(sVal, (INT)sVal.GetLength(),
							&textF, Gdiplus::RectF(0, 
							0, 
							0, 
							0), 
							&textArea);	
		  	xPoint -= textArea.Width / 2;
			yPoint-= textArea.Height / 2;  

			pGraphics->DrawString(sVal, sVal.GetLength(), &textF, Gdiplus::PointF(xPoint, yPoint), &textBr); 
			flStartAngle += ele->f_angle;
		}
	}
}

BOOL CPieChartWnd::ReArrangeElementMap(void)
{
	//This methos rearrange the element std::map and reset the longest label string for labels
	//This method needed when elements are removed from the chart
	std::map<int, pie_chart_element*> map_temp;
	std::map<int, pie_chart_element*>::iterator iter;
	int iElementIndex = 0;
	
	lb_info.sMaxLenString = "";
	map_temp.swap(map_pChart);
	iter = map_temp.begin();
	for(;iter != map_temp.end(); ++iter){
		map_pChart[iElementIndex] = iter->second;
		iElementIndex++;
		pie_chart_element* ele = iter->second;
		if (ele->s_label.GetLength() > lb_info.sMaxLenString.GetLength()){
			lb_info.sMaxLenString = ele->s_label;
		}
	}
	map_temp.clear();
	return true;
}

void CPieChartWnd::sortPiechartElements(BOOL bClockWise)
{
	//Elements are sorted by the value
	std::map<int, pie_chart_element*> map_temp;
	std::map<int, pie_chart_element*>::iterator iter;
	std::map<int, pie_chart_element*>::iterator iter_next;
	double dVal = 0;
	int index = 0;
	pie_chart_element* element = NULL;
	
	map_temp.swap(map_pChart);


	while(!map_temp.empty()){
		iter = map_temp.begin();
		if (iter != map_temp.end())
			dVal = ((pie_chart_element*)iter->second)->d_value;
		for(; iter != map_temp.end(); ++iter){
			pie_chart_element* ele = iter->second;
			if (dVal >= ele->d_value && !bClockWise){
				dVal = ele->d_value;
				iter_next = iter;
			}
			else if (dVal <= ele->d_value && bClockWise){
				dVal = ele->d_value;
				iter_next = iter;
			}
		}
		if (iter_next != map_temp.end()){
			map_pChart[index] = iter_next->second;
			map_temp.erase(iter_next);
			index++;
		}
	}
	Invalidate();
}

void CPieChartWnd::Draw2DStyle(CDC* pDc)
{
	CRect rect;	
	CRect rectBnd;
	Gdiplus::Bitmap* mBtmap;
	Gdiplus::Graphics* graphics;
	float totalAngle = fl_startAngle;
	std::map<int, pie_chart_element*>::iterator iter;
	
	GetBoundRect(rectBnd);
	GetClientRect(rect);
	
	mBtmap = new Gdiplus::Bitmap(rect.Width(), rect.Height());
	graphics = Gdiplus::Graphics::FromImage(mBtmap);

	graphics-> SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gdiplus::REAL penWidth = Gdiplus::REAL(lb_param.xGapLeft / 150);
	DrawBackGround(graphics);

	Gdiplus::REAL rRatio = (Gdiplus::REAL)rectBnd.Width() / 25;

	Gdiplus::SolidBrush brHighLight(Gdiplus::Color(100, cr_HighlightColor.GetR(), 
									  cr_HighlightColor.GetG(), 
									  cr_HighlightColor.GetB()));

	Gdiplus::Pen pnHighLight(cr_HighlightColor);

	iter = map_pChart.begin();


	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (ele){

			CRect rectDiff;
			rectDiff.CopyRect(rectBnd);
			//Find the correct distance from the pie chart center point
			double dx = ele->i_distIndex * cos (PI * (totalAngle + ele->f_angle / 2) / 180);
			double dy = ele->i_distIndex * sin (PI * (totalAngle + ele->f_angle / 2) / 180);
			
			rectDiff.left += (long)dx;
			rectDiff.right += (long)dx;
			rectDiff.top += (long)dy;
			rectDiff.bottom += (long)dy;

			Gdiplus::LinearGradientBrush crGradient(Gdiplus::Point(rectDiff.right/2, rectDiff.top), 
								   Gdiplus::Point(rectDiff.right/2, rectDiff.bottom), 
								   ele->cr_GradientD, 
								   ele->cr_Base);  

			graphics->FillPie(&crGradient, 
							  Gdiplus::RectF((Gdiplus::REAL)rectDiff.left, 
									(Gdiplus::REAL)rectDiff.top, 
									(Gdiplus::REAL)rectDiff.Width(), 
									(Gdiplus::REAL)rectDiff.Height()), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
			
			crGradient.SetLinearColors(ele->cr_Base, ele->cr_GradientL);
			graphics->FillPie(&crGradient, 
							  Gdiplus::RectF(Gdiplus::REAL(rectDiff.left + rRatio), 
							  Gdiplus::REAL(rectDiff.top + rRatio), 
							  Gdiplus::REAL(rectDiff.Width() - rRatio * 2), 
							  Gdiplus::REAL(rectDiff.Height()- rRatio * 2)), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);

			if (ele->b_select){
				graphics->FillPie(&brHighLight, 
							  Gdiplus::RectF((Gdiplus::REAL)rectDiff.left, (Gdiplus::REAL)rectDiff.top, (Gdiplus::REAL)rectDiff.Width(), (Gdiplus::REAL)rectDiff.Height()), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
				graphics->DrawPie(&pnHighLight, 
							  Gdiplus::RectF((Gdiplus::REAL)rectDiff.left, (Gdiplus::REAL)rectDiff.top, (Gdiplus::REAL)rectDiff.Width(), (Gdiplus::REAL)rectDiff.Height()), 
							  (Gdiplus::REAL)totalAngle, 
							  (Gdiplus::REAL)ele->f_angle);
			}
		
			totalAngle += ele->f_angle;
		}
	}
	
	DrawLabels(graphics);
	if(b_ShowPercentages)
		DrawPiechartPecentages(graphics, fl_startAngle);

	delete graphics;
	graphics = NULL;

	Gdiplus::Graphics gr(pDc->m_hDC);
	Gdiplus::CachedBitmap* btmp = new Gdiplus::CachedBitmap(mBtmap, &gr);

	SaveImageInternal(mBtmap);

	if (mBtmap){
		delete mBtmap;
		mBtmap = NULL;
	}
	gr.DrawCachedBitmap(btmp, rect.left, rect.top);	
	if (btmp){
		delete btmp;
		btmp = NULL;
	}

}

void CPieChartWnd::Draw3DStyle(CDC* pDc)
{
	CRect rect;	
	Gdiplus::Bitmap* mBtmap;
	Gdiplus::Graphics* graphics;
	float totalAngle = fl_startAngleIncline;
	std::map<int, pie_chart_element*>::iterator iter;
	pie_chart_element* ele = NULL;
	Gdiplus::REAL totalRight = 0;
	Gdiplus::REAL totalLeft = 0;
	pie_chart_element* pLast = NULL;
	pie_chart_element* pFront= NULL;
	bool bRightAngled = false;
	bool bLeftAngled = false;
	int  ielementLeft = 0;
	int  ielementRight = 0;
	
	GetClientRect(rect);
	UpdatePiechartPoints();

	mBtmap = new Gdiplus::Bitmap(rect.Width(), rect.Height());
	graphics = Gdiplus::Graphics::FromImage(mBtmap);

	graphics-> SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	DrawBackGround(graphics);
	
	iter = map_pChart.begin();
	totalAngle = fl_startAngleIncline;

	for(; iter != map_pChart.end(); ++iter){
		ele = iter->second;
		if (ele){
			
			if (totalAngle >= 360)
				totalAngle -= 360;			
	
			if ((totalAngle <= 90 && totalAngle+ ele->pie_3d_props.f_InclineAngle > 90) ||
				totalAngle >= 90 && (totalAngle+ ele->pie_3d_props.f_InclineAngle - 360) > 90){
					pFront = ele;
					i_pieElement_front = iter->first;	//save the index of the front element which will be drawn
			}

			if (totalAngle <= 270 && totalAngle+ ele->pie_3d_props.f_InclineAngle > 270 || 
				totalAngle >= 270 && (totalAngle+ ele->pie_3d_props.f_InclineAngle - 360) > 270){
				ielementLeft = iter->first;
				ielementRight = iter->first;
				i_pieElement_last = iter->first;				//save the index of the last element will be drawn
				totalRight = totalAngle + ele->pie_3d_props.f_InclineAngle;
				totalLeft = totalAngle;
				pLast = iter->second;
			}
			totalAngle += ele->pie_3d_props.f_InclineAngle;
		}
	}

	if (pLast == pFront){
		if (totalLeft < 90 || totalLeft > 270)
			bRightAngled = true;	//The elements are on the right hand side!
		else if(totalLeft > 90 && totalLeft < 270)
			bLeftAngled = true;		//The elements are on the right hand side!
	}
	else{
		bLeftAngled = true;			//Elements are on both sides!
		bRightAngled = true;		//
	}

	if(pLast){
		if (pLast == pFront && bRightAngled)
			Construct3DElementSpecific(graphics, pLast, (Gdiplus::REAL)totalLeft, PIE_LAST_RIGHT, Gdiplus::PointF(0, 0));		//Draw the pie element which is the front and last 
		else if(pLast == pFront && bLeftAngled)												//element. First draw only the latter part of the element.
			Construct3DElementSpecific(graphics, pLast, (Gdiplus::REAL)totalLeft, PIE_LAST_LEFT, Gdiplus::PointF(0, 0));
		else
			Construct3DElement(graphics, pLast, (Gdiplus::REAL)totalLeft, PIE_LAST, Gdiplus::PointF(0, 0));						//Draw the normal element
	}

	Gdiplus::SolidBrush brtest(Gdiplus::Color(255,25,25));
	ele = NULL;
	while(pFront != ele && bLeftAngled){
		ielementLeft--;
		iter = map_pChart.find(ielementLeft);
		if(iter!= map_pChart.end()){
			ele = iter->second;
			if (ele != pFront){
				totalLeft -= ele->pie_3d_props.f_InclineAngle;
				Construct3DElement(graphics, ele, (Gdiplus::REAL)totalLeft, LEFT_ANGELD, Gdiplus::PointF(0, 0));
			}
		}else{
			ielementLeft = i_elementIndex;
		}
	}

	ele = NULL;
	while(pFront != ele && bRightAngled){
		ielementRight++;
			iter = map_pChart.find(ielementRight);
			if(iter!= map_pChart.end()){
				if (totalRight >= 360)
					totalRight -= 360;
				ele = iter->second;
				if (ele != pFront){
					
					Construct3DElement(graphics, ele, (Gdiplus::REAL)totalRight, RIGHT_ANGLED, Gdiplus::PointF(0, 0));
					totalRight += ele->pie_3d_props.f_InclineAngle;
				}
			}else{
				ielementRight = -1;		
			}
	}

	if(pFront){
		if (pLast == pFront && bRightAngled)						//Draw the pie element which is the front and last
			Construct3DElementSpecific(graphics, pFront, totalRight, PIE_FRONT_RIGHT, Gdiplus::PointF(0, 0));	//element. First draw only the front part of the element.
		else if(pLast == pFront && bLeftAngled)
			Construct3DElementSpecific(graphics, pFront, 
			Gdiplus::REAL(totalRight - pFront->pie_3d_props.f_InclineAngle), PIE_FRONT_LEFT, Gdiplus::PointF(0, 0));
		else
			Construct3DElement(graphics, pFront, (Gdiplus::REAL)totalRight, PIE_FRONT, Gdiplus::PointF(0, 0));
	}

	DrawLabels(graphics);

	if(b_ShowPercentages)
		DrawPiechartPecentages(graphics, fl_startAngleIncline);

	delete graphics;
	graphics = NULL;

	Gdiplus::Graphics gr(pDc->m_hDC);
	gr. SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gdiplus::CachedBitmap* btmp = new Gdiplus::CachedBitmap(mBtmap, &gr);

	SaveImageInternal(mBtmap);

	if (mBtmap){
		delete mBtmap;
		mBtmap = NULL;
	}
	gr.DrawCachedBitmap(btmp, rect.left, rect.top);	
	if (btmp){
		delete btmp;
		btmp = NULL;
	}

}
void CPieChartWnd::DrawBackGround(Gdiplus::Graphics* pGraphics)
{

	CRect rect;	
	Gdiplus::Color crWindowClr;
	Gdiplus::GraphicsPath ellipsePath;
	Gdiplus::SolidBrush brBkgndOuter(Gdiplus::Color::Black);
	Gdiplus::RectF textArea;
	Gdiplus::StringFormat strFormat;

	GetClientRect(rect);
	Gdiplus::SolidBrush brText(lb_info.lbTextColor); 
	crWindowClr.SetFromCOLORREF(cr_parentClr);
	brBkgndOuter.SetColor(crWindowClr);
	
	Gdiplus::LinearGradientBrush brBkgndInner(Gdiplus::PointF((Gdiplus::REAL)rect.left, (Gdiplus::REAL) rect.top), 
								 Gdiplus::PointF((Gdiplus::REAL)rect.right, (Gdiplus::REAL)rect.bottom), 
								 bkg_params.cr_GradientL, 
								 bkg_params.cr_GradientD);
	
	pGraphics->FillRectangle(&brBkgndOuter, 
							Gdiplus::RectF((Gdiplus::REAL)rect.left, 
								  (Gdiplus::REAL)rect.top, 
								  (Gdiplus::REAL)rect.Width(), 
								  (Gdiplus::REAL)rect.Height()));


	rect.right = max(rect.right, MAX_PIECHARTPARAMS * 2);
	rect.bottom = max(rect.bottom, MAX_PIECHARTPARAMS);

	ellipsePath.AddLine(Gdiplus::PointF(Gdiplus::REAL(rect.left + 30), Gdiplus::REAL(rect.top)), 
						Gdiplus::PointF(Gdiplus::REAL(rect.right -30), 
						(Gdiplus::REAL)rect.top));
	ellipsePath.AddArc(Gdiplus::RectF(Gdiplus::REAL(rect.right - 60), (Gdiplus::REAL)rect.top, 60, 60), 270, 90);
	ellipsePath.AddLine(Gdiplus::PointF((Gdiplus::REAL)rect.right, Gdiplus::REAL(rect.top + 30)),
								Gdiplus::PointF((Gdiplus::REAL)rect.right, 
								Gdiplus::REAL(rect.bottom)));
	ellipsePath.AddArc(Gdiplus::RectF(Gdiplus::REAL(rect.right - 60), Gdiplus::REAL(rect.bottom - 60), 60, 60), 0, 90);
	ellipsePath.AddLine(Gdiplus::PointF(Gdiplus::REAL(rect.right - 30), Gdiplus::REAL(rect.bottom)), 
						Gdiplus::PointF(Gdiplus::REAL(rect.left + 30), 
						Gdiplus::REAL(rect.bottom)));
	ellipsePath.AddArc(Gdiplus::RectF((Gdiplus::REAL)rect.left, Gdiplus::REAL(rect.bottom - 60), 60, 60), 90, 90);
	ellipsePath.AddLine(Gdiplus::PointF((Gdiplus::REAL)rect.left, 
								Gdiplus::REAL(rect.bottom)), 
								Gdiplus::PointF((Gdiplus::REAL)rect.left, 
								Gdiplus::REAL(rect.top + 30)));
	ellipsePath.AddArc(Gdiplus::RectF((Gdiplus::REAL)rect.left, (Gdiplus::REAL)rect.top, 60, 60), 180, 90);

	pGraphics->FillPath(&brBkgndInner, &ellipsePath);

	int iFontSize = min(rect.Height() / 10, 20);	

	Gdiplus::Font textF(bkg_params.sFont, (Gdiplus::REAL)iFontSize, bkg_params.fStyle, Gdiplus::UnitPoint, NULL);


	pGraphics->MeasureString(bkg_params.s_ChartTittle, 
							bkg_params.s_ChartTittle.GetLength(),
							&textF, 
							Gdiplus::RectF(0,0,0,0), 
						    &textArea); 

	pGraphics->DrawString(bkg_params.s_ChartTittle, 
								 bkg_params.s_ChartTittle.GetLength(), 
								 &textF, 
								 Gdiplus::RectF(Gdiplus::REAL(rect.CenterPoint().x - textArea.Width / 2), 
								 Gdiplus::REAL(rect.top + 30), 
								 textArea.Width, 
								 textArea.Height), 
								 &strFormat, 
								 &brText);

}

void CPieChartWnd::Get3DBounds(LPRECT rectTop, LPRECT rectBtm)
{
	//This method retuns the upper and lower faces rects for the 3-D style drawing
	CRect rect;
	GetBoundRect(rect);
	//Clip area calculated using the inclination angle set
	long rectClip = long(fl_InclineAngle * rect.Height() / 180);

	rect.top += rectClip;
	rect.bottom -= rectClip;

	CopyRect(rectBtm, &rect);
	CopyRect(rectTop, &rect);
	rectTop->top -= long(rectClip * f_depth / 2);
	rectTop->bottom -= long(rectClip * f_depth / 2);
	rectBtm->top += long(rectClip * f_depth / 2);
	rectBtm->bottom += long(rectClip * f_depth / 2);
}

void CPieChartWnd::SetInclineAngle(float flAngle)
{	
	fl_InclineAngle = min(flAngle, MAXINCLINEANGLE);
	fl_InclineAngle = max(fl_InclineAngle, 0);
	Invalidate();
}

BOOL CPieChartWnd::SetDepth(float fDepth)
{
	CRect rect;
	GetBoundRect(rect);
	if(fDepth >= 0){
		long depthMax = long(MAXINCLINEANGLE * rect.Height() / 180);
		long rectClip = long(fl_InclineAngle * rect.Height() / 180);
		f_depth = float(fDepth) / float(depthMax);
		Invalidate();
		return true;
	}
	return false;
}

void CPieChartWnd::CalcuatePieElemetPoints(void)
{
	//Calculate the elements points on pie. This is the end angle point calculated and saved under the 3-D properties
	//This calculated angles then adjusted according the incline angle.
	CRect rectBnd;
	GetBoundRect(rectBnd);

	float flStart = fl_startAngle;
	std::map<int, pie_chart_element*>::iterator iter = map_pChart.begin();

	for (; iter != map_pChart.end(); iter++){
		pie_chart_element* ele = iter->second;
		Gdiplus::REAL xPoint = Gdiplus::REAL(rectBnd.CenterPoint().x + 
				(rectBnd.Width() / 2) * cos(PI * (flStart + ele->f_angle) / 180));
		Gdiplus::REAL yPoint = Gdiplus::REAL(rectBnd.CenterPoint().y + 
				(rectBnd.Height() / 2) * sin(PI * (flStart + ele->f_angle) / 180));

			ele->pie_3d_props.pt_End.X = xPoint;
			ele->pie_3d_props.pt_End.Y = yPoint;
			flStart += ele->f_angle;
	}
}

void CPieChartWnd::UpdatePiechartPoints(void)
{
	//The calculated pie element points are relocated according to the incline angle and the resulting formations of angles were calculated and set.
	CRect rectBnd;
	GetBoundRect(rectBnd);
	//Calculate (set) the original locations for the points prior to the circle it bounds.
	CalcuatePieElemetPoints();
	
	float flStart = fl_startAngle;
	float flStartIncline = 0;
	Gdiplus::PointF ptStart;
	CRect rectBtm, rectTop;
	Get3DBounds(rectTop, rectBtm);

	long rectClip = long(fl_InclineAngle * rectBnd.Height() / 180);

	rectTop.top += long(rectClip * f_depth / 2);
	rectTop.bottom += long(rectClip * f_depth / 2);

	Gdiplus::REAL xPoint = Gdiplus::REAL(rectBnd.CenterPoint().x + 
				(rectBnd.Width() / 2) * cos(PI * (flStart)/ 180));
	Gdiplus::REAL yPoint = Gdiplus::REAL(rectBnd.CenterPoint().y + 
				(rectBnd.Height() / 2) * sin(PI * (flStart)/ 180));

	ptStart.X = xPoint;
	//Relocate the start angle y cordinate according to the inclination
	ptStart.Y = yPoint - Gdiplus::REAL(rectClip * sin((flStart) * PI / 180));

	flStartIncline = CacluateInclineAngle(ptStart, rectTop);
	fl_startAngleIncline = flStartIncline;
	
	std::map<int, pie_chart_element*>::iterator iter = map_pChart.begin();
	pie_chart_element* pPrev = NULL;
	pie_chart_element* ele = NULL;
	//Relocate the y coordinates according to the incline angle and recalculate the angles for elements
	for (; iter != map_pChart.end(); iter++){
		ele = iter->second;
		ele->pie_3d_props.pt_End.Y -= Gdiplus::REAL(rectClip * sin((flStart + ele->f_angle) * PI / 180));
		float inClineAngle = CacluateInclineAngle(ele->pie_3d_props.pt_End, rectTop);
	
		if(inClineAngle - flStartIncline < 0.001 && ele->f_angle == 360)	//+ changed the comparison since floats are not equal all the time
				inClineAngle = 360;	// Two points lies in the same location and the angle is 360
		else if (inClineAngle >= flStartIncline)
				inClineAngle -= flStartIncline;
		else
			inClineAngle += (360 - flStartIncline);
	
		ele->pie_3d_props.f_InclineAngle = inClineAngle;
	 	ele->pie_3d_props.pt_End.Y -= Gdiplus::REAL(rectClip * f_depth / 2);
		
		if (pPrev)	//Set the previous element's last point to this element's start point
			ele->pie_3d_props.pt_Start = pPrev->pie_3d_props.pt_End;	//Set the start point of the pie element.

		pPrev = ele;
		flStartIncline += inClineAngle;
		flStart += ele->f_angle;	
	}
	if (ele){
		iter = map_pChart.begin();
		pie_chart_element* pFirst = iter->second;		
		pFirst->pie_3d_props.pt_Start = ele->pie_3d_props.pt_End;	//Set the starting point of first element using the last
	}																//element and point
}

float CPieChartWnd::CacluateInclineAngle(Gdiplus::PointF ptElement, CRect rectTop)
{
	double yDist = double(ptElement.Y - (Gdiplus::REAL)rectTop.CenterPoint().y);
	double xDist = double(ptElement.X - (Gdiplus::REAL)rectTop.CenterPoint().x);

	float angle = float(180 * atan(yDist / xDist) / PI);

	if (xDist < 0)
		angle = 180 + angle;
	if(xDist >= 0 && yDist < 0)
		angle = 360 + angle;

	return  angle;
}

BOOL CPieChartWnd::SaveImage(CString sPath)
{
		s_saveImage = sPath;
		b_SaveFlag = TRUE;
		Invalidate();
		return true;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	//Function from msdn//
   UINT  num = 0;          
   UINT  size = 0;         
   Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
   Gdiplus::GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  
   pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1; 
   GetImageEncoders(num, size, pImageCodecInfo);
   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  
      }    
   }
   free(pImageCodecInfo);
   return -1;  
}
BOOL CPieChartWnd::SaveImageInternal(Gdiplus::Bitmap* btImage)
{
	//Some error regarding image save on vista
	if (b_SaveFlag){
		  CLSID imgClsid;
		  int iPos = s_saveImage.Find(L".");
		  iPos++;
		  CString sFileExt = s_saveImage.Right(s_saveImage.GetLength() - iPos);
		  CString sFormat("image/");
		  sFormat.Append(sFileExt);
		  GetEncoderClsid(sFormat, &imgClsid);
		  btImage->Save(s_saveImage, &imgClsid);
		  b_SaveFlag = FALSE;
		  return true;
	}
	return false;
}

void CPieChartWnd::SetPieChartStyle(pie_chart_style pieChartStyle)
{
	pie_ChartStyle = pieChartStyle;
	Invalidate();
}

BOOL CPieChartWnd::Create(LPCTSTR lpCaption, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	bkg_params.s_ChartTittle = lpCaption;
	return CWnd::Create(NULL, NULL, WS_VISIBLE, rect, pParentWnd, nID, NULL);
}

void CPieChartWnd::ShowPercentagesOnPie(BOOL bShow)
{
	b_ShowPercentages = bShow;
	Invalidate();
}

int CPieChartWnd::SetSelectedPieElement(CPoint point)
{
	//This method used in finding the mouse clicked element. Used the graphics path and IsVisible method to find the corresponding element
	//Can be easily used in right click too
	CRect rectBnd;
	Gdiplus::GraphicsPath elementPath;
	GetBoundRect(rectBnd);	
	if (pie_ChartStyle != ThreeDStyle){
		float flStart = fl_startAngle;
		Gdiplus::REAL rRatio = (Gdiplus::REAL)rectBnd.Width() / 15;
		Gdiplus::PointF ptClicked((Gdiplus::REAL)point.x, (Gdiplus::REAL)point.y);
		std::map<int, pie_chart_element*>::iterator iter = map_pChart.begin();
		for (; iter != map_pChart.end(); ++iter){
			pie_chart_element* ele = iter->second;
			GetBoundRect(rectBnd);	
			
			Gdiplus::REAL flDisplace = flStart + Gdiplus::REAL(ele->f_angle / 2);
			double dx_rect = ele->i_distIndex * cos (PI * (flDisplace) / 180);
			double dy_rect = ele->i_distIndex * sin (PI * (flDisplace) / 180);
			
			rectBnd.left += (long)dx_rect;
			rectBnd.right += (long)dx_rect;
			rectBnd.top += (long)dy_rect;
			rectBnd.bottom += (long)dy_rect;

			elementPath.Reset();
			elementPath.AddPie(Gdiplus::RectF((Gdiplus::REAL)rectBnd.left, 
									(Gdiplus::REAL)rectBnd.top, 
									(Gdiplus::REAL)rectBnd.Width(), 
									(Gdiplus::REAL)rectBnd.Height()), 
							  (Gdiplus::REAL)flStart, 
							  (Gdiplus::REAL)ele->f_angle);
		
			if (pie_ChartStyle == DoughnutStyle)
				elementPath.AddPie(Gdiplus::RectF( Gdiplus::REAL(rectBnd.CenterPoint().x - rRatio * 3),
										Gdiplus::REAL(rectBnd.CenterPoint().y - rRatio * 3),
										rRatio * 6, rRatio * 6),
								  (Gdiplus::REAL)flStart, 
								  (Gdiplus::REAL)ele->f_angle);

			if(elementPath.IsVisible(ptClicked)){
				ele->b_select = TRUE;
				return 0;
			}
			flStart += ele->f_angle;
		}
	}
	else{
		Gdiplus::PointF ptClicked((Gdiplus::REAL)point.x, (Gdiplus::REAL)point.y);
		if (FindClicked3DElement(ptClicked))
			return 0;
	}
	return -1;
}

void CPieChartWnd::OnLButtonDown(UINT nFlags, CPoint point)
{

	std::map<int, pie_chart_element*>::iterator iter;	
	iter = map_pChart.begin();
	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		ele->b_select = FALSE;
	}
	
	SetSelectedPieElement(point);
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);	
}

void CPieChartWnd::SortToOrginalOrder(void)
{
	//Rearrange the items to its added order
	std::map<int, pie_chart_element*> map_temp;
	std::map<int, pie_chart_element*>::iterator iter;
	std::map<int, pie_chart_element*>::iterator iter_next;
	double dVal = 0;
	int index = 0;
	pie_chart_element* element = NULL;
	
	map_temp.swap(map_pChart);


	while(!map_temp.empty()){
		iter = map_temp.begin();
		if (iter != map_temp.end())
			dVal = ((pie_chart_element*)iter->second)->i_ID;
		for(; iter != map_temp.end(); ++iter){
			pie_chart_element* ele = iter->second;
			if (dVal >= ele->i_ID){
				dVal = ele->i_ID;
				iter_next = iter;
			}
		}
		if (iter_next != map_temp.end()){
			map_pChart[index] = iter_next->second;
			map_temp.erase(iter_next);
			index++;
		}
	}
	Invalidate();
}

void CPieChartWnd::SetTitleFont(LOGFONT lf)
{
	bkg_params.sFont.Format(L"%s", lf.lfFaceName);
	bkg_params.fStyle |= lf.lfWeight ? Gdiplus::FontStyleBold :  Gdiplus::FontStyleRegular;
	bkg_params.fStyle |= lf.lfItalic ? Gdiplus::FontStyleItalic : bkg_params.fStyle;
	bkg_params.fStyle |= lf.lfUnderline ? (Gdiplus::FontStyleUnderline | bkg_params.fStyle) :bkg_params.fStyle ;
	bkg_params.fStyle |= lf.lfStrikeOut ? (Gdiplus::FontStyleStrikeout | bkg_params.fStyle) : bkg_params.fStyle;

	Invalidate();
}

PIECHARTITEM CPieChartWnd::GetSelectedElement(void)
{
	std::map<int, pie_chart_element*>::iterator iter = map_pChart.begin();
		for (; iter != map_pChart.end(); ++iter){
			pie_chart_element* ele = iter->second;
			if (ele->b_select)
				return (PIECHARTITEM)ele;
		}

	return NULL;
}

BOOL CPieChartWnd::RemoveItem(PIECHARTITEM pItem)
{
	return RemoveItem(((pie_chart_element*)pItem)->s_element);
}

float CPieChartWnd::GetBackGroungGrLight(void)
{
	return bkg_params.f_ColorGradL;
}

float CPieChartWnd::GetBackgroundGrDark(void)
{
	return bkg_params.f_ColorGradD;
}

float CPieChartWnd::GetElementGrLight(PIECHARTITEM pItem)
{
	if(pItem)
		return ((pie_chart_element*)pItem)->f_ColorGradL;
	else
		return -1;
}

float CPieChartWnd::GetElementGrDark(PIECHARTITEM pItem)
{
	if(pItem)
		return ((pie_chart_element*)pItem)->f_ColorGradD;
	else
		return -1;
}

CString CPieChartWnd::GetElementName(PIECHARTITEM pItem)
{
	return ((pie_chart_element*)pItem)->s_element;
}

void CPieChartWnd::SetItemHighlightColor(Gdiplus::Color crHighlightColor)
{
	cr_HighlightColor = crHighlightColor;
	Invalidate();
}

void CPieChartWnd::SetParentWindowColor(COLORREF crParent)
{
	cr_parentClr = crParent;
	Invalidate();
}

BOOL CPieChartWnd::Construct3DElement(Gdiplus::Graphics* pGraphics, pie_chart_element* pElement, 
								  Gdiplus::REAL flStart, pie_drawn_direction drawCase, Gdiplus::PointF ptClicked)
{

	CRect rectTop;
	CRect rectBtm;
	pie_chart_element pieElm;
	BOOL bClicked = FALSE;
	Get3DBounds(rectTop, rectBtm);
	Gdiplus::RectF rectFtop, rectFbtm;
	Gdiplus::REAL flDisplace = flStart + Gdiplus::REAL(pElement->pie_3d_props.f_InclineAngle / 2);
	double dx_rect = pElement->i_distIndex * cos (PI * (flDisplace) / 180);
	double dy_rect = pElement->i_distIndex * sin (PI * (flDisplace) / 180);

	Gdiplus::REAL flTotalAngle = flStart + (Gdiplus::REAL)pElement->pie_3d_props.f_InclineAngle;
	
	Gdiplus::PointF ptPieSt(pElement->pie_3d_props.pt_Start.X + long(dx_rect), 
		pElement->pie_3d_props.pt_Start.Y + long(dy_rect));
	
	Gdiplus::PointF ptPieEnd(pElement->pie_3d_props.pt_End.X + long(dx_rect), 
		pElement->pie_3d_props.pt_End.Y + long(dy_rect));
	
	rectFtop.X = Gdiplus::REAL(rectTop.left + (long)dx_rect);
	rectFtop.Y = Gdiplus::REAL(rectTop.top + (long)dy_rect);
	rectFtop.Width =  (Gdiplus::REAL)rectTop.Width();
	rectFtop.Height =  (Gdiplus::REAL)rectTop.Height();
	
	rectFbtm.X =  Gdiplus::REAL(rectBtm.left + (long)dx_rect);
	rectFbtm.Y =  Gdiplus::REAL(rectBtm.top + (long)dy_rect);
	rectFbtm.Width =  (Gdiplus::REAL)rectBtm.Width();
	rectFbtm.Height =  (Gdiplus::REAL)rectBtm.Height();

	Gdiplus::PointF ptTopS(Gdiplus::REAL(rectFtop.GetRight()), Gdiplus::REAL((rectFtop.Y + rectFtop.Height / 2)));	//The point at the 0 of top face
	Gdiplus::PointF ptTopF(Gdiplus::REAL(rectFtop.X), Gdiplus::REAL((rectFtop.Y + rectFtop.Height / 2)));	//The point at the 180 of the top face
	Gdiplus::PointF ptTopCenter(Gdiplus::REAL((rectFtop.X + rectFtop.Width / 2)), 
		Gdiplus::REAL((rectFtop.Y + rectFtop.Height / 2))); //Center point

	//Set the temporary element used todraw transformed parts of pie elements
	pieElm.cr_GradientD = pElement->cr_GradientD;
	pieElm.cr_GradientL = pElement->cr_GradientL;
	pieElm.pie_3d_props.i_alphaVal = pElement->pie_3d_props.i_alphaVal;
	pieElm.i_distIndex = pElement->i_distIndex;
	pieElm.b_select = pElement->b_select;
	pieElm.cr_Base = pElement->cr_Base;

	if ((flStart > 180 && flStart < 360) && (flTotalAngle > 360) && (flTotalAngle - 360 < 180)){	//Element cross over 0 th angle
		bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);

		if (drawCase == PIE_LAST) {//If last element(

			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(360 - flStart);
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopS;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm,flDisplace,  flStart, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);

			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(flTotalAngle - 360);
			pieElm.pie_3d_props.pt_End = ptTopS;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked |ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = bClicked |ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
		else{
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);

			pieElm.pie_3d_props.f_InclineAngle =  Gdiplus::REAL(360 - flStart);
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopS;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm,  flDisplace, flStart, ptClicked);
		
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);

			pieElm.pie_3d_props.f_InclineAngle =  Gdiplus::REAL(flTotalAngle - 360);
			pieElm.pie_3d_props.pt_End = ptTopS;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);
		}
		bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
	}
	else if ((flStart >= 0 && flStart < 180) && (flTotalAngle > 180) && (flTotalAngle < 360)){	//Element cross over 180 th angle
		bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
		
		if(drawCase == PIE_LAST){//Last element
			pieElm.pie_3d_props.f_InclineAngle =  Gdiplus::REAL(flTotalAngle - 180);
			pieElm.pie_3d_props.pt_End = ptTopF;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(180 - flStart);
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopF;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}

		else{
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);

			pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(flTotalAngle - 180);
			pieElm.pie_3d_props.pt_End = ptTopF;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
		
			pieElm.pie_3d_props.pt_End = ptPieSt;	
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
			pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(180 - flStart);
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopF;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
		}
		bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
	}
	else if((flStart > 0 && flStart < 180) && (flTotalAngle >= 360)){	//Element cross over both 0th and 180th and facing the front
		bClicked = ConstructArcFace(pGraphics, pElement,  flStart, rectFbtm, ptClicked);
		
		pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(180);
		pieElm.pie_3d_props.pt_End = ptTopF;
		pieElm.pie_3d_props.pt_Start = ptTopS;
		bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
		
		pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(180 - flStart);
		pieElm.pie_3d_props.pt_End = ptPieSt;
		pieElm.pie_3d_props.pt_Start = ptTopF;
		bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);	
		
		pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(flTotalAngle - 360);
		pieElm.pie_3d_props.pt_End = ptTopS;
		pieElm.pie_3d_props.pt_Start = ptPieEnd;
		bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);
		
		pieElm.pie_3d_props.pt_End = ptPieSt;	
		ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
		pieElm.pie_3d_props.pt_End = ptPieEnd;	
		bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
		bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
	}
	else if((flStart >= 180 && flStart < 360) && (flTotalAngle - 360 > 180)){ //Element cross over both 0th and 180th and facing behind
		bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
		
		pieElm.pie_3d_props.pt_End = ptPieSt;	
		bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
		pieElm.pie_3d_props.pt_End = ptPieEnd;
		bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
		pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(360 - flStart);
		pieElm.pie_3d_props.pt_End = ptPieSt;
		pieElm.pie_3d_props.pt_Start = ptTopS;
		bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
	
		pieElm.pie_3d_props.f_InclineAngle =   Gdiplus::REAL(flTotalAngle - 540);
		pieElm.pie_3d_props.pt_End = ptTopF;
		pieElm.pie_3d_props.pt_Start = ptPieEnd;
		bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
	
		pieElm.pie_3d_props.f_InclineAngle =   180;
		pieElm.pie_3d_props.pt_End = ptTopS;
		pieElm.pie_3d_props.pt_Start = ptTopF;
		bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);	
		
		bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
	}
	else{
		bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
		if (drawCase == RIGHT_ANGLED){			//Right angled
			pieElm.pie_3d_props.pt_End = ptPieSt;	
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle =   pElement->pie_3d_props.f_InclineAngle;
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
		else if(drawCase == LEFT_ANGELD){		//Left angled
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle =   pElement->pie_3d_props.f_InclineAngle;
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
		if (drawCase == PIE_LAST){		//Last element
			pieElm.pie_3d_props.f_InclineAngle =   pElement->pie_3d_props.f_InclineAngle;
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
		else if(drawCase == PIE_FRONT){		//Front element
			pieElm.pie_3d_props.pt_End = ptPieEnd;		
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
			pieElm.pie_3d_props.pt_End = ptPieSt;	
	    	bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
			pieElm.pie_3d_props.f_InclineAngle =   pElement->pie_3d_props.f_InclineAngle;
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
		}
		bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
	}
	return bClicked;
}

BOOL CPieChartWnd::ConstructRectangularFace(Gdiplus::Graphics* pGraphics, pie_chart_element pieElement, 
									   Gdiplus::PointF ptCenterTop, Gdiplus::PointF ptClicked)
{
	Gdiplus::GraphicsPath path;
	CRect rectTop;
	CRect rectBtm;
	
	Get3DBounds(rectTop, rectBtm);
	Gdiplus::PointF ptCenterBtm(ptCenterTop.X, ptCenterTop.Y + Gdiplus::REAL(rectBtm.top - rectTop.top));		//Center point of the bottom
	Gdiplus::PointF ptBtmInPie(pieElement.pie_3d_props.pt_End.X, pieElement.pie_3d_props.pt_End.Y 
		+ Gdiplus::REAL(rectBtm.top - rectTop.top));
		
	path.AddLine(pieElement.pie_3d_props.pt_End, ptBtmInPie);
	path.AddLine(ptBtmInPie, ptCenterBtm);
	path.AddLine(ptCenterBtm, ptCenterTop);
	path.AddLine(ptCenterTop, pieElement.pie_3d_props.pt_End);

	if (pGraphics) {
		Gdiplus::Color crGradientFace = CalculateGradientLight(pieElement.cr_Base, GRADIENTL);
		BYTE Alpha = pieElement.pie_3d_props.i_alphaVal;
		BYTE Red = crGradientFace.GetR();
		BYTE Green = crGradientFace.GetG();
		BYTE Blue = crGradientFace.GetB();
		Gdiplus::Color crGradientL(Alpha, Red, Green, Blue);
		
		Red = pieElement.cr_GradientD.GetR();
		Green = pieElement.cr_GradientD.GetG();
		Blue = pieElement.cr_GradientD.GetB();
		Gdiplus::Color crGradientD(Alpha, Red, Green, Blue);
		
		Gdiplus::SolidBrush brHighLight(Gdiplus::Color(100, cr_HighlightColor.GetR(), 
										  cr_HighlightColor.GetG(), 
										  cr_HighlightColor.GetB()));
		Gdiplus::Pen pnHighLight(cr_HighlightColor);
		Gdiplus::SolidBrush crSolid(crGradientL);
		
		Gdiplus::Pen penOutLine(pieElement.cr_Base, (Gdiplus::REAL).8);
		pGraphics->FillPath(&crSolid, &path);
		pGraphics->DrawPath(&penOutLine, &path);
		
		if(pieElement.b_select){
			pGraphics->FillPath(&brHighLight, &path);
			pGraphics->DrawPath(&pnHighLight, &path);
		}

	}
	return path.IsVisible(ptClicked);
}

BOOL CPieChartWnd::ConstructArcFace(Gdiplus::Graphics* pGraphics, pie_chart_element* pElement, Gdiplus::REAL flStart, Gdiplus::RectF rect, 
							   Gdiplus::PointF ptClicked)
{
	Gdiplus::GraphicsPath path;

	path.AddPie(rect, 
					flStart, 
					Gdiplus::REAL(pElement->pie_3d_props.f_InclineAngle));

	if (pGraphics) {
		BYTE Alpha = pElement->pie_3d_props.i_alphaVal;
		BYTE Red = pElement->cr_GradientL.GetR();
		BYTE Green = pElement->cr_GradientL.GetG();
		BYTE Blue = pElement->cr_GradientL.GetB();
		Gdiplus::Color crGradientL(Alpha, Red, Green, Blue);

		Red = pElement->cr_GradientD.GetR();
		Green = pElement->cr_GradientD.GetG();
		Blue = pElement->cr_GradientD.GetB();
		Gdiplus::Color crGradientD(Alpha, Red, Green, Blue);

		Gdiplus::LinearGradientBrush crGradientBtm(Gdiplus::PointF((rect.GetRight())/2, rect.Y), 
			Gdiplus::PointF((rect.GetRight())/2, rect.GetBottom()), 
									   crGradientD, 
									   crGradientL);

		Gdiplus::Pen penOutLine(pElement->cr_Base, (Gdiplus::REAL).8);

		Gdiplus::SolidBrush brHighLight(Gdiplus::Color(100, cr_HighlightColor.GetR(), 
										  cr_HighlightColor.GetG(), 
										  cr_HighlightColor.GetB()));
		Gdiplus::Pen pnHighLight(cr_HighlightColor);
	
		pGraphics->FillPath(&crGradientBtm, &path);
		pGraphics->DrawPath(&penOutLine, &path);
		
		if (pElement->b_select){
			pGraphics->FillPath(&brHighLight, &path);
			pGraphics->DrawPath(&pnHighLight, &path);
		}
	}	
	return path.IsVisible(ptClicked);
}

BOOL CPieChartWnd::ConstructSideFace(Gdiplus::Graphics* pGraphics, pie_chart_element pElement, Gdiplus::REAL flDisplace, 
											  Gdiplus::REAL flStart,Gdiplus::PointF ptClicked)
{
	Gdiplus::GraphicsPath path;
	CRect rectTop;
	CRect rectBtm;
	Gdiplus::RectF rectFtop, rectFbtm;
	Get3DBounds(rectTop, rectBtm);
	
	double dx_rect = pElement.i_distIndex * cos (PI * (flDisplace) / 180);
	double dy_rect = pElement.i_distIndex * sin (PI * (flDisplace) / 180);

	rectFtop.X = Gdiplus::REAL(rectTop.left + dx_rect);
	rectFtop.Y  = Gdiplus::REAL(rectTop.top + dy_rect);
	rectFtop.Width  = (Gdiplus::REAL)rectTop.Width();
	rectFtop.Height  = (Gdiplus::REAL)rectTop.Height();

	rectFbtm.X = Gdiplus::REAL(rectBtm.left + dx_rect);
	rectFbtm.Y  = Gdiplus::REAL(rectBtm.top + dy_rect);
	rectFbtm.Width  = (Gdiplus::REAL)rectBtm.Width();
	rectFbtm.Height  = (Gdiplus::REAL)rectBtm.Height();

	Gdiplus::PointF ptBtmStart(pElement.pie_3d_props.pt_End.X, 
		pElement.pie_3d_props.pt_End.Y + long(rectFbtm.Y - rectFtop.Y));
	Gdiplus::PointF ptBtmEnd(pElement.pie_3d_props.pt_Start.X, 
		pElement.pie_3d_props.pt_Start.Y + long(rectFbtm.Y - rectFtop.Y));

	path.AddLine(pElement.pie_3d_props.pt_End, ptBtmStart);
    path.AddArc(rectFbtm, 
					flStart, 
					Gdiplus::REAL(pElement.pie_3d_props.f_InclineAngle));
	
	
	path.AddLine(ptBtmEnd, pElement.pie_3d_props.pt_Start);

	path.AddArc(rectFtop, 
		Gdiplus::REAL(flStart + pElement.pie_3d_props.f_InclineAngle), 
		- (Gdiplus::REAL)pElement.pie_3d_props.f_InclineAngle);

	if (pGraphics) {
		Gdiplus::Pen penOutLine(pElement.cr_Base, (Gdiplus::REAL).8);
		BYTE Alpha = pElement.pie_3d_props.i_alphaVal;
		BYTE Red = pElement.cr_GradientL.GetR();
		BYTE Green = pElement.cr_GradientL.GetG();
		BYTE Blue = pElement.cr_GradientL.GetB();
		Gdiplus::Color crGradientL(Alpha, Red, Green, Blue);
		
		Red = pElement.cr_GradientD.GetR();
		Green = pElement.cr_GradientD.GetG();
		Blue = pElement.cr_GradientD.GetB();
		Gdiplus::Color crGradientD(Alpha, Red, Green, Blue);

		Gdiplus::LinearGradientBrush crGradientBtm(Gdiplus::PointF(rectFtop.GetRight() / 2, rectFtop.Y), 
									   Gdiplus::PointF(rectFtop.GetRight() / 2, rectFbtm.GetBottom()), 
									   crGradientL, 
									   crGradientD);
		
		Gdiplus::SolidBrush brHighLight(Gdiplus::Color(100, cr_HighlightColor.GetR(), 
										  cr_HighlightColor.GetG(), 
										  cr_HighlightColor.GetB()));
		Gdiplus::Pen pnHighLight(cr_HighlightColor);
		pGraphics->FillPath(&crGradientBtm, &path);
		pGraphics->DrawPath(&penOutLine, &path);

		if(pElement.b_select){
			pGraphics->FillPath(&brHighLight, &path);
			pGraphics->DrawPath(&pnHighLight, &path);
		}
	}
	return path.IsVisible(ptClicked);
}


BOOL CPieChartWnd::Construct3DElementSpecific(Gdiplus::Graphics* pGraphics, pie_chart_element* pElement, 
								  Gdiplus::REAL flStart,  pie_drawn_direction drawCase, Gdiplus::PointF ptClicked)
{

	CRect rectTop;
	CRect rectBtm;
	Gdiplus::RectF rectFtop, rectFbtm;
	pie_chart_element pieElm;
	BOOL bClicked = FALSE;
	Get3DBounds(rectTop, rectBtm);
	Gdiplus::REAL flDisplace = flStart + Gdiplus::REAL(pElement->pie_3d_props.f_InclineAngle / 2);
	double dx_rect = pElement->i_distIndex * cos (PI * (flDisplace) / 180);
	double dy_rect = pElement->i_distIndex * sin (PI * (flDisplace) / 180);

	Gdiplus::REAL flTotalAngle = flStart + Gdiplus::REAL(pElement->pie_3d_props.f_InclineAngle);
	Gdiplus::PointF ptPieSt(pElement->pie_3d_props.pt_Start.X + Gdiplus::REAL(dx_rect), 
		pElement->pie_3d_props.pt_Start.Y + Gdiplus::REAL(dy_rect));
	Gdiplus::PointF ptPieEnd(pElement->pie_3d_props.pt_End.X + Gdiplus::REAL(dx_rect), 
		pElement->pie_3d_props.pt_End.Y + Gdiplus::REAL(dy_rect));
	

	
	rectFtop.X = Gdiplus::REAL(rectTop.left + dx_rect);
	rectFtop.Y = Gdiplus::REAL(rectTop.top + dy_rect);
	rectFtop.Width =  (Gdiplus::REAL)rectTop.Width();
	rectFtop.Height =  (Gdiplus::REAL)rectTop.Height();
	
	rectFbtm.X =  Gdiplus::REAL(rectBtm.left + dx_rect);
	rectFbtm.Y =  Gdiplus::REAL(rectBtm.top + dy_rect);
	rectFbtm.Width =  (Gdiplus::REAL)rectBtm.Width();
	rectFbtm.Height =  (Gdiplus::REAL)rectBtm.Height();

	Gdiplus::PointF ptTopS(Gdiplus::REAL(rectFtop.GetRight()), Gdiplus::REAL((rectFtop.Y + rectFtop.Height / 2)));	//The point at the 0 of top face
	Gdiplus::PointF ptTopF(Gdiplus::REAL(rectFtop.X), Gdiplus::REAL((rectFtop.Y + rectFtop.Height / 2)));	//The point at the 180 of the top face
	Gdiplus::PointF ptTopCenter(Gdiplus::REAL((rectFtop.X + rectFtop.Width / 2)), 
		Gdiplus::REAL((rectFtop.Y + rectFtop.Height / 2))); //Center point


	pieElm.cr_GradientD = pElement->cr_GradientD;
	pieElm.cr_GradientL = pElement->cr_GradientL;
	pieElm.pie_3d_props.i_alphaVal = pElement->pie_3d_props.i_alphaVal;
	pieElm.i_distIndex = pElement->i_distIndex;
	pieElm.b_select = pElement->b_select;
	pieElm.cr_Base = pElement->cr_Base;

	if(drawCase == PIE_LAST_RIGHT) { //Elements are right angled drawn, Draw the latter part of the pie element
		if(flTotalAngle - 360 > 270){ //Both start and end points in 270 < x < 360
			bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = (flTotalAngle - 540);
			pieElm.pie_3d_props.pt_End = ptTopF;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
		else if(flTotalAngle > 360){ //Both start and end points in 0 < x < 90
			bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = 180;
			pieElm.pie_3d_props.pt_End = ptTopF;
			pieElm.pie_3d_props.pt_Start = ptTopS;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = (flTotalAngle - 360);
			pieElm.pie_3d_props.pt_End = ptTopS;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
		else if(flTotalAngle <= 360){	//Start point is 0 < x < 90 and end point is 270 < x < 360
			bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = (flTotalAngle - 180);
			pieElm.pie_3d_props.pt_End = ptTopF;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
	}
	else if(drawCase == PIE_LAST_LEFT){ //Elements are left angled drawn, Draw the latter part of the pie element
		if(flStart > 90 && flStart < 180){	//Start point between 90 < x < 180
			bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(180);
			pieElm.pie_3d_props.pt_End = ptTopF;
			pieElm.pie_3d_props.pt_Start = ptTopS;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180,ptClicked);
		
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL((180 - flStart));
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopF;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);	
		}
		else if (flStart > 180){	//Start point between 180 < x < 270
			bClicked = ConstructArcFace(pGraphics, pElement, flStart, rectFbtm, ptClicked);
		
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(360 - flStart);
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopS;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
			
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = bClicked | ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		}
	}
	else if(drawCase == PIE_FRONT_RIGHT){	//Elements are right angled drawn, Draw the front part of the pie element
	 	if(flStart < 360 && flStart > 180){	//Start point between 270 < x < 360
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(360 - flStart);
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopS;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
		
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(180);
			pieElm.pie_3d_props.pt_End = ptTopS;
			pieElm.pie_3d_props.pt_Start = ptTopF;
			ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);
			bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
		
	}
	else if(flStart >= 0 && flStart < 90){	//Start point between 0 < x < 90
			pieElm.pie_3d_props.pt_End = ptPieSt;
			bClicked = ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
		
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(180 - flStart);
			pieElm.pie_3d_props.pt_End = ptPieSt;
			pieElm.pie_3d_props.pt_Start = ptTopF;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, flStart, ptClicked);
			
			bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
		} 
	 }
	else if(drawCase == PIE_FRONT_LEFT){ //Elements are left angled drawn, Draw the front part of the pie element
		if(flTotalAngle - 360 > 90 && (flTotalAngle - 360 < 180)){ //End point is between 90 < x < 180
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(flTotalAngle - 360);
			pieElm.pie_3d_props.pt_End = ptTopS;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);
			
			bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
		}
		else if (flTotalAngle - 360 >= 180){	//End point between 180 < x < 270
			pieElm.pie_3d_props.pt_End = ptPieEnd;
			bClicked = ConstructRectangularFace(pGraphics, pieElm, ptTopCenter, ptClicked);
			
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(flTotalAngle - 540);
			pieElm.pie_3d_props.pt_End = ptTopF;
			pieElm.pie_3d_props.pt_Start = ptPieEnd;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 180, ptClicked);
		
			pieElm.pie_3d_props.f_InclineAngle = Gdiplus::REAL(180);
			pieElm.pie_3d_props.pt_End = ptTopS;
			pieElm.pie_3d_props.pt_Start = ptTopF;
			bClicked = bClicked | ConstructSideFace(pGraphics, pieElm, flDisplace, 0, ptClicked);
			
			bClicked = bClicked | ConstructArcFace(pGraphics, pElement, flStart, rectFtop, ptClicked);
		}
	}


	return bClicked;
}

BOOL CPieChartWnd::SetDistanceIndex(PIECHARTITEM pItem, int iDistIndex)
{
	CRect rectTop;
	CRect rectBtm;
	Get3DBounds(rectTop, rectBtm);
	if(pItem){
		((pie_chart_element*)pItem)->i_distIndex = rectTop.Width() * iDistIndex / 100;
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

BOOL CPieChartWnd::SetDistanceIndexAll(int iDistIndex)
{

	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.begin();
	CRect rectTop;
	CRect rectBtm;
	Get3DBounds(rectTop, rectBtm);

	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		ele->i_distIndex = rectTop.Width() * iDistIndex / 100;;	
	}
	Invalidate();
	return TRUE;
}

BOOL CPieChartWnd::SetElementTransparency(PIECHARTITEM pItem, float flPercentage)
{
	if(pItem){
		if(flPercentage > 100)
			flPercentage = 100;
		if(flPercentage < 0)
			flPercentage = 0;
		((pie_chart_element*)pItem)->pie_3d_props.i_alphaVal = BYTE(255 * (1 - flPercentage / 100));
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

BOOL CPieChartWnd::SetElementTransparencyAll(float flPercentage)
{
	std::map<int, pie_chart_element*>::iterator iter;
	iter = map_pChart.begin();
	CRect rectTop;
	CRect rectBtm;
	Get3DBounds(rectTop, rectBtm);

	if(flPercentage > 100)
			flPercentage = 100;
		if(flPercentage < 0)
			flPercentage = 0;

	for(; iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if (ele)
			ele->pie_3d_props.i_alphaVal = BYTE(255 * (1 - flPercentage / 100));
	}
	Invalidate();
	return TRUE;
}

BOOL CPieChartWnd::FindClicked3DElement(Gdiplus::PointF ptClicked)
{
	CRect rectTop;
	CRect rectBtm;
	std::map<int, pie_chart_element*>::iterator iter = map_pChart.begin();
	pie_chart_element* pElementLast = NULL;
	pie_chart_element* pElementFront = NULL;
	pie_chart_element* pElement = NULL;
	int iRindex = i_pieElement_front;
	int iLindex = i_pieElement_front;
	Gdiplus::REAL flStartLeft = fl_startAngleIncline;
	Gdiplus::REAL flStartRight = fl_startAngleIncline;
	BOOL bRightAngled = FALSE;
	BOOL bLeftAngled = FALSE;
	
	iter = map_pChart.find(i_pieElement_last);
	if (iter != map_pChart.end())
		pElementLast = iter->second;
		
	iter = map_pChart.find(i_pieElement_front);
	if (iter !=  map_pChart.end())
		pElementFront = iter->second;

	for(iter = map_pChart.begin(); iter != map_pChart.end(); ++iter){
		pie_chart_element* ele = iter->second;
		if(i_pieElement_front != iter->first){
			flStartLeft += ele->pie_3d_props.f_InclineAngle;
			flStartRight = flStartLeft;
		}else{
			flStartLeft += ele->pie_3d_props.f_InclineAngle;
			break;
		}
	}

	if (i_pieElement_front == i_pieElement_last){
		if (flStartRight < 90 || flStartRight > 270)
			bRightAngled = TRUE;	//The elements are on the right hand side!
		else if(flStartRight > 90 && flStartRight < 270)
			bLeftAngled = TRUE;		//The elements are on the right hand side!
	}
	else{
		bLeftAngled = TRUE;			//Elements are on both sides!
		bRightAngled = TRUE;		//
	}

	if(pElementFront){
		if (i_pieElement_front == i_pieElement_last && bRightAngled){
			if (Construct3DElementSpecific(NULL, pElementFront, flStartRight, PIE_FRONT_RIGHT, ptClicked)){		//Draw the pie element which is the front and last 
				pElementFront->b_select = TRUE;
				return TRUE;
			}
		}
		else if(i_pieElement_front == i_pieElement_last && bLeftAngled)	{											//element. First draw only the latter part of the element.
			if (Construct3DElementSpecific(NULL, pElementFront, flStartRight, PIE_FRONT_LEFT, ptClicked)){
				pElementFront->b_select = TRUE;
				return TRUE;
			}
		}
		else {
			if (Construct3DElement(NULL, pElementFront, flStartRight, PIE_FRONT, ptClicked)){						//Draw the normal element
				pElementFront->b_select = TRUE;
				return TRUE;
			}
		}
	}
	
	while(pElement != pElementLast && bLeftAngled){	//Iterate through from front element to last element drawn in left clockwise
		if(flStartLeft >= 360)			//This preserves the drawn order and identify the correct element clicked when the
			flStartLeft -= 360;			//elements overlap.
		
		iLindex++;
		iter = map_pChart.find(iLindex);
		if(iter != map_pChart.end()){
			pElement = iter->second;
			if (pElement == pElementLast)
				break;

			if (Construct3DElement(NULL, pElement, flStartLeft, LEFT_ANGELD, ptClicked)){
				pElement->b_select = TRUE;
				return TRUE;
			}
			flStartLeft += pElement->pie_3d_props.f_InclineAngle;
		}else{
			iLindex = -1;
		}	
	}
	
	pElement= NULL;
	
	while(pElement != pElementLast && bRightAngled){	//Iterate through from front element to last element drawn in left anti-clockwise
		if(flStartRight < 0)							//This preserves the drawn order and identify the correct element clicked when the
				flStartRight += 360;					//elements overlap.					
		Gdiplus::GraphicsPath path;
		path.Reset();
	
		iRindex--;
		iter = map_pChart.find(iRindex);
		if(iter != map_pChart.end()){
			pElement = iter->second;
			
			if (pElement == pElementLast)
				break;
			flStartRight -= pElement->pie_3d_props.f_InclineAngle;
			if (Construct3DElement(NULL, pElement, flStartRight, RIGHT_ANGLED, ptClicked)){
				pElement->b_select = TRUE;
				return TRUE;
			}
			
		}else{
			iRindex = i_elementIndex;
		}	
	}

	if(pElementLast){
		if (i_pieElement_front == i_pieElement_last && bRightAngled){											//Draw the pie element which is the front and last
			if(Construct3DElementSpecific(NULL, pElementLast, (flStartLeft - pElementFront->pie_3d_props.f_InclineAngle), PIE_LAST_RIGHT, ptClicked)){	//element. First draw only the front part of the element.
				pElementLast->b_select = TRUE;
				return TRUE;
			}
		}
		else if(i_pieElement_front == i_pieElement_last && bLeftAngled){
			if (Construct3DElementSpecific(NULL, pElementLast, flStartRight, PIE_LAST_LEFT, ptClicked)){
				pElementLast->b_select = TRUE;
				return TRUE;
			}
		}
		else{
			if (Construct3DElement(NULL, pElementLast, flStartLeft, PIE_LAST, ptClicked)){
				pElementLast->b_select = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}




BOOL CPieChartWnd::SetHorizontalOffset(int iHrzOffset){
	if (bkg_params.i_HorizontalOffset != iHrzOffset){
		bkg_params.i_HorizontalOffset = iHrzOffset;
		Invalidate();
		return TRUE;
	}
	return FALSE;
}
