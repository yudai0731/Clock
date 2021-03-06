// 17406 金澤雄大 11/17
#include<GL/glut.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<string.h>

// windowのサイズを定義
#define WINDOW_W 320
#define WINDOW_H 320

int dispMode =0; // 0 : LIGHTMODE 1 : DARKMODE
double loop1=0; //use for design rotation
double loop2=0; //use for design rotation
void Display(void);
void Reshape(int,int);
void Timer(int);
void Printstr(int,int,char *,int);
void calPosition(int *,int *,int,int,int,double);
void drawLine(int,int,int,int);
void drawDesign(int,int,int,int,int,double);
void Mouse(int,int,int,int);

int main(int argc,char **argv){
// 初期化処理
    // 引数処理
    glutInit(&argc,argv);
    // 初期Windowサイズ設定
    glutInitWindowSize(WINDOW_W,WINDOW_H);
    // 新規Window作成
    glutCreateWindow("clock");
    // 関数登録
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutTimerFunc(500,Timer,0);
    // display初期化
    glutInitDisplayMode(GLUT_RGBA);
    glClearColor(0.96,0.96,1.0,1.0);
    
    // メインループ
    glutMainLoop();
    return 0;
}

void Display(void){
    int i; //ループ用
    char *timestr; // 時間情報表示用文字列
    int month,wday; // 月の番号,曜日の番号取得用
    int timestr_len = 12; //year+space+space+month+space+(+)+\0
    // 月の名前を定義
    char month_eg[12][10] = {{"January"},{"February"},{"March"},{"April"},{"May"},
    {"June"},{"July"},{"August"},{"Septermber"},{"October"},{"November"},{"December"}};
    // 曜日の名前を定義
    char wday_eg[7][10] = {{"Sunday"},{"Monday"},{"Tuesday"},{"Wednesday"},{"Thursday"},{"Friday"},{"Saturday"}};
    
    // 画面サイズ取得
    int xc = glutGet(GLUT_WINDOW_WIDTH)/2;
    int yc = glutGet(GLUT_WINDOW_HEIGHT)/2+30; // y軸方向の中心は30ずらす.
    
    // インデックス描画用
    double l,theta; 
    int x1,x2,y1,y2;
    char s[3];

    // 針の角度
    double thetas,thetam,thetah;
    // 針の座標
    int xs,ys,xm,ym,xh,yh;
    // 針の長さ
    int ls=80;
    int lm = 105;
    int lh = 90; 

    // 描画クリア
    glClear(GL_COLOR_BUFFER_BIT);
    
// 時間取得
    time_t tt;
    struct tm *ts;
    time(&tt);
    ts = localtime(&tt);

// 針の角度,座標を計算
    thetas = 2*M_PI*ts->tm_sec/60;
    thetam = 2*M_PI*(60*ts->tm_min+ts->tm_sec)/3600;
    thetah = 2*M_PI*(3600*(ts->tm_hour%12)+60*ts->tm_min+ts->tm_sec)/43200;
    calPosition(&xs,&ys,xc,yc,ls,thetas);
    calPosition(&xm,&ym,xc,yc,lm,thetam);
    calPosition(&xh,&yh,xc,yc,lh,thetah);

// デザイン描画
    l=128;
    if(dispMode){
    glColor3ub(218,112,214);
    }else{
    glColor3ub(127,255,0);
    }
    drawDesign(xc,yc,0,130,l,loop1);
    l=124;
    if(dispMode){
    glColor3ub(38,38,38);
    }else{
    glColor3ub(245,245,255);
    }
    drawDesign(xc,yc,-1,131,l,loop1);



    l=128;
    if(dispMode){
    glColor3ub(218,112,214);
    }else{
    glColor3ub(127,255,0);
    }
    drawDesign(xc,yc,160,270,l,loop1);
    l=124;
    if(dispMode){
    glColor3ub(38,38,38);
    }else{
    glColor3ub(245,245,255);
    }
    drawDesign(xc,yc,159,271,l,loop1);





    l=121;
    if(dispMode){
    glColor3ub(153,50,204);
    }else{
    glColor3ub(0,250,154);
    }
    drawDesign(xc,yc,70,200,l,loop2);
    l=118;
    if(dispMode){
    glColor3ub(38,38,38);
    }else{
    glColor3ub(245,245,255);
    }
    drawDesign(xc,yc,69,201,l,loop2);



    l=121;
    if(dispMode){
    glColor3ub(153,50,204);
    }else{
    glColor3ub(0,250,154);
    }
    drawDesign(xc,yc,240,380,l,loop2);
    l=118;
    if(dispMode){
    glColor3ub(38,38,38);
    }else{
    glColor3ub(245,245,255);
    }
    drawDesign(xc,yc,239,381,l,loop2);

// デザイン回転
    loop1+=0.05;
    loop2-=0.1;
    if(loop1>=2*M_PI){
        loop1=0;
    }
    if(loop2<=-2*M_PI){
        loop2=0;
    }

// year,month,dayを表示
    month = ts->tm_mon; // 月の番号を取得
    wday =  ts->tm_wday; // 曜日の番号を取得 
    // 可変文字列生成
    timestr_len+=strlen(month_eg[month])+strlen(wday_eg[wday]); // 文字数計算
    timestr = (char *)malloc(timestr_len*sizeof(char)); // 配列確保
    sprintf(timestr,"%d %s %02d (%s)",1900+ts->tm_year,month_eg[month],ts->tm_mday,wday_eg[wday]); // timestrに文字列書き込み
    // 文字列表示
    if(dispMode){
    glColor3ub(0,191,255);
    }else{
    glColor3ub(255,102,0);
    }
    Printstr(xc-(18*timestr_len/4),30+1,timestr,timestr_len); // 文字列表示
    // 領域解放
    free(timestr);

//hour,min,secを表示
    // 文字列生成
    timestr = (char *)malloc(9*sizeof(char)); // 配列確保
    sprintf(timestr,"%02d:%02d:%02d",ts->tm_hour,ts->tm_min,ts->tm_sec); // timestrに文字列書き込み
    timestr_len = strlen(timestr); // 文字列の長さを取得
    // 文字列表示
    if(dispMode){
    glColor3ub(0,255,0);
    }else{
    glColor3ub(0,0,205);
    }
    Printstr(xc-(18*timestr_len/4)+1,60,timestr,timestr_len); // 文字列表示(太文字用)
    if(dispMode){
    glColor3ub(51,255,102);
    }else{
    glColor3ub(0,0,205);
    }
    Printstr(xc-(18*timestr_len/4),60,timestr,timestr_len); // 文字列表示
    // 領域解放
    free(timestr);

// インデックス描画
    for(i=1;i<=60;i++){
        if(dispMode){
        glColor3ub(255,255,255);
        }else{
        glColor3ub(0,0,0);
        }
        glLineWidth(2.0);
        l=100; // インデックスの先端を長さ110にする
        if(i%5==0){  // 5の倍数の針は長くする
        l = 90; // インデックスの終端を長さ90にする
        }
        theta = 2*M_PI*i/60;
        calPosition(&x1,&y1,xc,yc,l,theta);
        l = 110;
        calPosition(&x2,&y2,xc,yc,l,theta);
        drawLine(x1,y1,x2,y2);

        if(i%5==0){ // 5の倍数のとき文字を表示
            sprintf(s,"%d",i/5);
            l =80; // 文字表示位置を80にする
            calPosition(&x2,&y2,xc,yc,l,theta);
            if(i/5<10){ // 一桁表示用
                glRasterPos2i(x2-5,y2+5);
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[0]);

            }else{ // 二桁表示用
                glRasterPos2i(x2-14,y2+5);
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[0]);
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[1]);
            }
        }
    }

// 針を描画
    //時針描画
    if(dispMode){
    glColor3ub(255,255,255);
    }else{
    glColor3ub(0,0,0);
    }
    glLineWidth(5.0);
    drawLine(xc,yc,xh,yh);
    //分針描画
    glLineWidth(3.0);
    drawLine(xc,yc,xm,ym);
    //秒針描画
    glLineWidth(2.0);
    glColor3ub(255,0,0);
    drawLine(xc,yc,xs,ys);

    glFlush();
}


// windowサイズが変更されたときの処理
void Reshape(int w,int h){
    //printf("ウィンドウの幅と高さ=%d x %d\n",w,h);
    glViewport(0,0,w,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0,w,0,h);
    glScaled(1,-1,1);
    glTranslated(0,-h,0);

    //windowサイズ固定 
    glutReshapeWindow(WINDOW_W,WINDOW_H);

}

// タイマーの処理
void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(500,Timer,0);
}

// 文字列描画
//座標(x,y)を初期位置として文字列str(文字列の長さstrlen)を画面に描画
void Printstr(int x,int y,char *str,int strlen){
    int i;
    glRasterPos2i(x,y);
    for(i=0;i<strlen;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
}

// 極座標と直交座標を変換
//極座標(r,theta)を(xc,yc)を原点とした直交座標(x,y)に変換
void calPosition(int *x,int *y,int xc,int yc,int r,double theta){
    *x = xc+r*sin(theta);
    *y = yc-r*cos(theta);
}

// lineを描画
//(x1,y1)と(x2,y2)を結ぶ直線を描画
void drawLine(int x1,int y1,int x2,int y2){
    glBegin(GL_LINES);
    glVertex2i(x1,y1);
    glVertex2i(x2,y2);
    glEnd();
};

// デザインを描画
// (xc,yc)を原点とする極座標をとる.
// この極座標でthetaStartからtheraEndまで,長さlの扇形を描画する.
// loopは扇形を回転させる変数.
void drawDesign(int xc,int yc,int thetaStart,int thetaEnd,int l,double loop){
    int x,y,i;
    double theta;
    glBegin(GL_POLYGON);
    for(i=thetaStart;i<=thetaEnd;i++){
        theta= 2*M_PI*i/360;
        calPosition(&x,&y,xc,yc,l,theta+loop);
        glVertex2i(x,y);
    }
    glEnd();
}

// 左クリック時にmodeを切り替える
void Mouse(int b,int s,int x,int y){
    if(b==GLUT_LEFT_BUTTON){
        if(s==GLUT_UP){
            if(dispMode==1){
                dispMode=0;
            }else{
                dispMode=1;
            }
        }
        if(dispMode){
            glClearColor(0.15,0.15,0.15,1.0);
        }else{
            glClearColor(0.96,0.96,1.0,1.0);
        }
    }
}
