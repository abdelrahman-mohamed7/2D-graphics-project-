#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include<cmath>
#include <string>
#include<math.h>
#include<fstream>
#include <sstream>
#include <stack>

// Declaration of needed variables
int command;
int xs,ys,xe,ye;
int x,y,xleft,xright,ytop,ybottom;
int q1 , q2;
COLORREF c;

// intialization of needed variables
int x1=0,z1=0;//start point
int x2=0,z2=0;//End point
int xc=0,yc=0;
int A=0, B=0;
int r=0;
int counter=0;

using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) (CreateSolidBrush(RGB(255,255,255)));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("2d graphics project"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           400,                 /* The programs width */
           400,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/**************** line Algorithms ********************/

void ParametricLine(HDC hdc,int xs, int ys, int xe, int ye,COLORREF color)
{
    double dx = xe-xs;
    double dy = ye-ys;
    for(double t=0;t<=1;t+=0.001)
    {
        int x = xs+dx*t;
        int y = ys+dy*t;
        SetPixel(hdc,x,y,color);
    }
}

void SimpleDDA(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{
    int dx=xe-xs;
    int dy=ye-ys;
    SetPixel(hdc,xs,ys,color);
    if(abs(dx)>=abs(dy))
    {
        int x=xs,xinc= dx>0?1:-1;
        double y=ys,yinc=(double)dy/dx*xinc;
        while(x!=xe)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,x,round(y),color);
            }
    }
    else
    {
        int y=ys,yinc= dy>0?1:-1;
        double x=xs,xinc=(double)dx/dy*yinc;
        while(y!=ye)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,round(x),y,color);
        }
    }
}

void midPoint(HDC hdc,int xs, int ys, int xe, int ye,COLORREF color)
{

	int dx = xe - xs;
	int dy = ye - ys;

	if(dy<=dx){
	int d = dy - (dx/2);
	int x = xs, y = ys;
	SetPixel(hdc,x,y,color);
	while (x < xe)
	{
		x++;

		if (d < 0)
			d = d + dy;

		else
		{
			d += (dy - dx);
			y++;
		}
		SetPixel(hdc,x,y,color);
	}
	}

else if(dx<dy)
{
	// initial value of decision parameter d
	int d = dx - (dy/2);
	int x = xs, y = ys;
	SetPixel(hdc,xs,ys,color);
	while (y < ye)
	{
		y++;

		if (d < 0)
			d = d + dx;

		else
		{
			d += (dx - dy);
			x++;

		}
		SetPixel(hdc,x,y,color);
	}
}
}


/**************** Ellipse Algorithms ********************/
void Draw4Points(HDC hdc,int a,int b,int xc,int yc,COLORREF color)
{
     SetPixel(hdc,xc+a,yc+b,color);
     SetPixel(hdc,xc-a,yc+b,color);
     SetPixel(hdc,xc+a,yc-b,color);
     SetPixel(hdc,xc-a,yc-b,color);
}

void DrawPolarElipse(HDC hdc,int A,int B,int xc,int yc,COLORREF color)
{
   int x=A,y=0;
   Draw4Points(hdc,xc,yc,x,y,color);
    double dtheta = 1.0/max(A,B);
    for(double theta=0;theta<6.28;theta+=dtheta)
    {
        x=A*cos(theta);
        y = B*sin(theta);
        Draw4Points(hdc,xc,yc,x,y,color);
    }
}

void DrawDirectElipse(HDC hdc,int A,int B,int xc,int yc,COLORREF color)
{
    int y=0,x=A;
    Draw4Points(hdc,xc,yc,A,0,color);
    while(y*pow(A,2)<x*pow(B,2))
    {
        y++;
        x=sqrt(pow(A,2)*(1-pow(y,2)/pow(B,2)));
        Draw4Points(hdc,round(x),(y),xc,yc,color);
    }
    y=B,x=0;
    Draw4Points(hdc,xc,yc,0,B,color);
    while(x*pow(B,2)<y*pow(A,2))
    {
        x++;
        y=sqrt(pow(B,2)*(1-pow(x,2)/pow(A,2)));
        Draw4Points(hdc,x,round(y),xc,yc,color);
    }
}




/**************** circle Algorithms ********************/


void Draw8points(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);

    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc+y,yc-x,color);
    SetPixel(hdc,xc-y,yc-x,color);
}

void DrawCircleDirect(HDC hdc,int xc,int yc,int r,COLORREF color)
{
    int x=0,y=r;
    int r2=r*r;
    Draw8points(hdc,x,y,xc,yc,color);
    while(x<y)
    {
        x++;
        y = round(sqrt((double)(r2-x*x)));
        Draw8points(hdc,x,y,xc,yc, color);

    }
}


void DrawCirclePolar(HDC hdc,int xc,int yc,int r,COLORREF color)
{
    double dtheta=1.0/r;
    for(double theta = 0;theta<6.28;theta+=dtheta)
    {
        int x = r*cos(theta);
        int y = r*sin(theta);
        Draw8points(hdc,round(x),round(y),xc,yc,color);

    }

}




void DrawCircleIterative(HDC hdc,int xc,int yc,int r,COLORREF color)
{
    double x=r,y=0;
    double dtheta=1.0/r;
    double cdtheta = cos(dtheta) , sdtheta = sin(dtheta);
    Draw8points(hdc,xc,yc,r,0,color);
    while(x>y)
    {
        double xp=x*cdtheta-y*sdtheta;
        y =x*sdtheta+y*cdtheta;
        x=xp;

        Draw8points(hdc,round(x),round(y),xc,yc,color);


    }
}

void DrawCircleMidPoint(HDC hdc,int xc,int yc, int r,COLORREF color)
{
    int x=0;
    int y=r;
    double d = 1-r;
    Draw8points(hdc,x,y,xc,yc,color);
    while(x<y)
    {
        if(d<=0)
        {
            d+= 2*x+3;

        }
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        Draw8points(hdc,x,y,xc,yc,color);
    }
}

void DrawCircleMidPoint2(HDC hdc,int xc,int yc, int r,COLORREF color)
{
    int xm=0,ym=r;
    int dm=1-r;
    int c1=3,c2=5-2*r;
    Draw8points(hdc,xm,ym,xc,yc,color);
    while(xm<ym)
    {
        if(dm<0)
        {
           dm+=c1;
           c2+=2;
        }
        else
        {
            dm+=c2;
            c2+=4;
            ym--;
        }
        c1+=2;
        xm++;
        Draw8points(hdc,xm,ym,xc,yc,color);
    }
}


/**************** clipping Algorithms ********************/

union Out
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};

Out GetOut(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    Out outp;
    outp.All=0;
    if(x<xleft)
        outp.left=1;
    if(x>xright)
        outp.right=1;
    if(y<ytop)
        outp.top=1;
   if(y>ybottom)
        outp.bottom=1;
    return outp;
}

void VIntersect(int xs,int ys,int xe,int ye,int x, double*xi,double*yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}

void HIntersect(int xs,int ys,int xe,int ye,int y,double*xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void line_clip(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    Out outp1=GetOut(x1,y1,xleft,ytop,xright,ybottom);
    Out outp2=GetOut(x2,y2,xleft,ytop,xright,ybottom);
    while( (outp1.All ||outp2.All) && !(outp1.All & outp2.All))
    {
        double xi,yi;
        if(outp1.All)
        {
            if(outp1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(outp1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(outp1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            outp1=GetOut(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(outp2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(outp2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(outp2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            outp2=GetOut(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!outp1.All && !outp2.All)
    {

        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
    }
}




void PointClip(HDC hdc,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
        SetPixel(hdc,x,y,color);
}
int X_start,X_end,Y_start,Y_end,X_left,Y_top,X_right,Y_bottom;



/**************** save and load ********************/
void save(HDC h, int x , int y){

    ofstream fout;
    ifstream fin;
    fin.open("2d.txt");
    fout.open ("2d.txt");
    for (int i = 0 ; i < x ; i++ ){
        for(int j = 0 ; j < y ; j++){
             if(fin.is_open())
          fout<< GetPixel(h,i,j) << endl;
        }
    }
    fin.close();
    fout.close();

}

void load(HDC h, int x , int y){

  ifstream MyReadFile("2d.txt");
    string myText;
    int c;

     for (int i = 0 ; i < x ; i++ ){
       for(int j = 0 ; j < y ; j++){
         getline (MyReadFile, myText);
            stringstream ss;
            ss << myText;
            ss >> c;
            SetPixel(h,i,j,c);

        }
}
}


void change_back(HDC h){


     for (int x = 0 ; x < 400 ; x++ ){
        for(int y = 0 ; y < 400 ; y++){

            SetPixel(h,x,y,RGB(120,130,150));
            }
        }
}

/**************** quarter filling ********************/
void draw_p(HDC hdc ,int x ,int y ,int xc,int yc , int q,COLORREF color )

{

    //fourth
    if(q == 4){
    SetPixel(hdc,xc+x,yc+y,color );
    SimpleDDA( hdc ,xc,yc,xc+x,yc+y,color );
    SetPixel(hdc,xc+y,yc+x,color );
    SimpleDDA( hdc ,xc,yc,xc+y,yc+x,color );}

    //third
    else if(q == 3){
    SetPixel(hdc,xc-x,yc+y,color);
    SimpleDDA( hdc ,xc,yc,xc-x,yc+y,color );

    SetPixel(hdc,xc-y,yc+x,color );
    SimpleDDA( hdc ,xc,yc,xc-y,yc+x,color );}

    //first
    else if(q == 1){
    SetPixel(hdc,xc+x,yc-y,color);
    SimpleDDA( hdc ,xc,yc,xc+x,yc-y,color);
    SetPixel(hdc,xc+y,yc-x,color);
    SimpleDDA( hdc ,xc,yc,xc+y,yc-x,color );}

    //second
    else if(q == 2){
    SetPixel(hdc,xc-y,yc-x,color );
    SimpleDDA( hdc ,xc,yc,xc-y,yc-x,color );

     SetPixel(hdc,xc-x,yc-y,color );
    SimpleDDA( hdc ,xc,yc,xc-x,yc-y,color );
    }
    }


void draw_c(HDC hdc,int xc ,int yc,int q1 ,int q2, int r,COLORREF color)
{
    int q ;
     int x=0;
     int y=r;
    double d=1-r;

       if(q1 >= xc && q2 <= yc   ){
            q  = 1;
        }
         else if(q1 <= xc && q2 <= yc ){
            q  = 2;
        }
       else if(q1 >= xc && q2 >=yc  ){
            q  = 4;
        }
        else if(q1 <= xc && q2 >= yc){
            q  = 3;
        }
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
            {
            d+=2*(x-y)+5;
            y--;

        }
        x++;

        draw_p(hdc,x,y,xc,yc,q,color);

    }
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message)
    {
        case WM_COMMAND:
        switch(wParam)
        {
        case 1:
            command =1;
            save(hdc,400,400);
            cout<<" The file is saved! "<<endl;
            break;
        case 2:
            command=2;
            load(hdc,400,400);
            cout<<"the file loaded"<<endl;
            break;
        case 3:
            command=3;
            InvalidateRect(hwnd, NULL, TRUE);
            cout<<"the screen is clear now"<<endl;
            break;
        case 4:
            command=4;
            cout<<"Parametric line"<<endl;
            break;
        case 5:
            command=5;
            cout<<"DDA line"<<endl;
            break;
        case 6:
            command=6;
            cout<<"Bresenham line"<<endl;
            break;
        case 7:
            command=7;
            cout<<"Direct circle"<<endl;
            break;
        case 8:
            command=8;
            cout<<"polar circle "<<endl;
            break;

        case 9:
            command=9;
            cout<<"iterative circle "<<endl;
            break;

        case 10:
            command=10;
            cout<<"midpoint1 circle "<<endl;
            break;

        case 11:
            command=11;
            cout<<"midpoint2 circle "<<endl;
            break;
        case 12:
            command=12;
            cout<<"Direct ellipse "<<endl;
            break;
         case 13:
            command=13;
            cout<<"polar ellipse "<<endl;
            break;



        case 14:
             c = RGB(0,255,0);
            break;

        case 15:
            c = RGB(255,255,0);
            break;

        case 16:
             c = RGB(255,0,0);
            break;
        case 17:
             c = RGB(128,0,128);
            break;

        case 18:
             command = 18;
             cout<<"clip point"<<endl;
             break;
        case 19:
             command = 19;
             cout<<"clip line"<<endl;
            break;

         case 20:
             command = 20;
             cout<<"filling a quarter in circle"<<endl;
            break;
        }
    case WM_CREATE:
    {
        HMENU hmenu=CreateMenu();
        HMENU FILE=CreateMenu();
        HMENU CIRCLE=CreateMenu();
        HMENU LINE=CreateMenu();
        HMENU ELLIPSE=CreateMenu();
        HMENU COLORS=CreateMenu();
        HMENU CLIPPING=CreateMenu();
        HMENU filling = CreateMenu();

        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)FILE,"File");
        AppendMenu (FILE,MF_STRING,1, "Save");
        AppendMenu (FILE,MF_STRING,2, "Load");
        AppendMenu (FILE,MF_STRING,3, "Clear");


        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)LINE,"Line");
        AppendMenu(LINE,MF_STRING,4,"Parametric");
        AppendMenu(LINE,MF_STRING,5," DDA");
        AppendMenu(LINE,MF_STRING,6,"Bresenham");

        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)CIRCLE,"Circle");
        AppendMenu(CIRCLE,MF_STRING,7,"Direct");
        AppendMenu(CIRCLE,MF_STRING,8,"Polar");
        AppendMenu(CIRCLE,MF_STRING,9,"iterative");
        AppendMenu(CIRCLE,MF_STRING,10,"midpoint1");
        AppendMenu(CIRCLE,MF_STRING,11,"midpoint2");

        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)ELLIPSE,"Ellipse");
        AppendMenu(ELLIPSE,MF_STRING,12,"Direct");
        AppendMenu(ELLIPSE,MF_STRING,13,"polar");


        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)COLORS,"Colors");
        AppendMenu(COLORS,MF_STRING,14,"green");
        AppendMenu(COLORS,MF_STRING,15,"yellow");
        AppendMenu(COLORS,MF_STRING,16,"red");
        AppendMenu(COLORS,MF_STRING,17,"purple");


        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)CLIPPING,"Clipping");
        AppendMenu(CLIPPING,MF_STRING,18,"point");
        AppendMenu(CLIPPING,MF_STRING,19,"line");

        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)filling,"filling");
        AppendMenu(filling,MF_STRING,20,"quarter filling");


        SetMenu(hwnd,hmenu);
        break;
    }
      case WM_LBUTTONDOWN:

        if (command == 4){
            if(counter == 0 ){
              xs=LOWORD(lParam);
              ys=HIWORD(lParam);
              counter++;
            }
            else if(counter == 1){
                xe=LOWORD(lParam);
                ye=HIWORD(lParam);
                ParametricLine(hdc,xs,ys,xe,ye,c);

                counter = 0;
            }
        }


         if (command == 5){
            if(counter == 0 ){
              xs=LOWORD(lParam);
              ys=HIWORD(lParam);
              counter++;
            }
            else if(counter == 1){
                xe=LOWORD(lParam);
                ye=HIWORD(lParam);
                SimpleDDA(hdc,xs,ys,xe,ye,c);
                counter = 0;
            }
        }


         if (command == 6){
            if(counter == 0 ){
              xs=LOWORD(lParam);
              ys=HIWORD(lParam);
              counter++;
            }
            else if(counter == 1){
                xe=LOWORD(lParam);
                ye=HIWORD(lParam);
                midPoint(hdc,xs,ys,xe,ye,c);
                counter = 0;
            }
        }


           if(command==7)
        {
            if (counter == 0)
            {   x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;}
            else if (counter == 1)
            {
                x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                r = sqrt(pow((x2-x1),2)+pow((z2-z1),2));
                DrawCircleDirect(hdc,xc,yc,r,c);
                counter = 0;
            }
        }

        if(command==8)
        {
            if (counter == 0)
            {   x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;}
            else if (counter == 1)
            {
                x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                r = sqrt(pow((x2-x1),2)+pow((z2-z1),2));
                DrawCirclePolar(hdc,xc,yc,r,c);
                counter = 0;
            }
        }

            if(command==9)
        {
            if (counter == 0)
            {   x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;}
            else if (counter == 1)
            {
                x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                r = sqrt(pow((x2-x1),2)+pow((z2-z1),2));
                DrawCircleIterative(hdc,xc,yc,r,c);
                counter = 0;
            }
        }

            if(command==10)
        {
            if (counter == 0)
            {   x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;}
            else if (counter == 1)
            {
                x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                r = sqrt(pow((x2-x1),2)+pow((z2-z1),2));
                DrawCircleMidPoint(hdc,xc,yc,r,c);
                counter = 0;
            }
        }

             if(command==11)
        {
            if (counter == 0)
            {   x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;}
            else if (counter == 1)
            {
                x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                r = sqrt(pow((x2-x1),2)+pow((z2-z1),2));
                DrawCircleMidPoint2(hdc,xc,yc,r,c);
                counter = 0;
            }
        }


        if(command==12)
        {



            if (counter == 0)
            {

                x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;
            }
            else if (counter == 1)
            {
                x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                A=x2-xc;
                B=z2-yc;
                DrawDirectElipse(hdc,A,B,xc,yc,c);
                counter = 0;
            }
        }

          if(command==13)
        {



            if (counter == 0)
            {

                x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;
            }
            else if (counter == 1)
            {
                x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                A=x2-xc;
                B=z2-yc;
                DrawPolarElipse(hdc,A,B,xc,yc,c);
                counter = 0;
            }
        }





        if(command==18)
        {

        if(counter==0)
        {
            xleft=LOWORD(lParam);
            ytop=HIWORD(lParam);
            counter++;
        }
        else if (counter==1)
        {
            xright=LOWORD(lParam);
            ybottom=HIWORD(lParam);
            Rectangle(hdc,xleft,ytop,xright,ybottom);
            counter++;

        }
        else if(counter==2)
        {
            x=LOWORD(lParam);
            y=HIWORD(lParam);
            PointClip(hdc,x,y,xleft,ytop,xright,ybottom,RGB(250,0,0));
            counter=0;
        }
        }

        if(command==19)
        {

        if(counter==0)
        {
            X_left=LOWORD(lParam);
            Y_top=HIWORD(lParam);
            counter++;
        }
        else if (counter==1)
        {
            X_right=LOWORD(lParam);
            Y_bottom=HIWORD(lParam);
            Rectangle(hdc, X_left,Y_top,X_right,Y_bottom);
            counter++;
        }
        else if(counter==2)
        {
            X_start=LOWORD(lParam);
            Y_start=HIWORD(lParam);
            counter++;
        }
        else if (counter==3)
        {
            X_end=LOWORD(lParam);
            Y_end=HIWORD(lParam);
            line_clip(hdc,X_start,Y_start,X_end,Y_end,X_left,Y_top,X_right,Y_bottom);
            counter= 0;
        }
        }


         if(command==20)
        {


        if(counter==0)
        {
            x1=LOWORD(lParam);
                z1=HIWORD(lParam);
                xc=x1;
                yc=z1;
                counter++;}
        else if (counter==1)
        {
            x2=LOWORD(lParam);
                z2=HIWORD(lParam);
                r = sqrt(pow((x2-x1),2)+pow((z2-z1),2));
                DrawCircleMidPoint(hdc,xc,yc,r,c);
                counter ++;
        }
        else if (counter==2)
        {
            q1=LOWORD(lParam);
            q2=HIWORD(lParam);
            draw_c(hdc,x1,z1,q1,q2,r,c);
            counter = 0;
        }


        }




        break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
