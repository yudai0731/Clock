#include<GL/glut.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<string.h>

//#define LIGHTMODE
#define DARKMODE

// windowのサイズを定義
#define WINDOW_W 320
#define WINDOW_H 320

void Display(void);
void Reshape(int,int);
void Timer(int);
int main(int argc,char **argv){
    // 初期化処理
    glutInit(&argc,argv);
    glutInitWindowSize(WINDOW_W,WINDOW_H);
    glutCreateWindow("clock");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutTimerFunc(500,Timer,0);
    #ifdef DARKMODE
    glClearColor(0.15,0.15,0.15,1.0);
    #else
    glClearColor(0.96,0.96,1.0,1.0);
    #endif
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

    char s[3];
    // 画面サイズ取得
    int xc = glutGet(GLUT_WINDOW_WIDTH)/2;
    int yc = glutGet(GLUT_WINDOW_HEIGHT)/2+30;
    // インデックス描画用
    double theta;
    double l; 
    double x1,x2;
    double y1,y2;
    // 針の角度
    double thetas,thetam,thetah;
    // 針の座標
    int xs,ys,xm,ym,xh,yh;
    // 針の長さ
    int ls=80;
    int lm = 105;
    int lh = 90; 
    // 時間取得
    time_t tt;
    struct tm *ts;
    time(&tt);
    ts = localtime(&tt);

    // 針の角度,座標を計算
    thetas = 2*M_PI*ts->tm_sec/60;
    thetam = 2*M_PI*(60*ts->tm_min+ts->tm_sec)/3600;
    thetah = 2*M_PI*(3600*(ts->tm_hour%12)+60*ts->tm_min+ts->tm_sec)/43200;
    xs = xc+ls*sin(thetas);
    ys = yc-ls*cos(thetas);
    xm = xc+lm*sin(thetam);
    ym = yc-lm*cos(thetam);
    xh = xc+lh*sin(thetah);
    yh = yc-lh*cos(thetah);

    glClear(GL_COLOR_BUFFER_BIT);

    // year,month,dayを表示
    month = ts->tm_mon;
    wday =  ts->tm_wday;
    // 可変文字列生成
    timestr_len+=strlen(month_eg[month])+strlen(wday_eg[wday]);
    timestr = (char *)malloc(timestr_len*sizeof(char));
    sprintf(timestr,"%d %s %02d (%s)",1900+ts->tm_year,month_eg[month],ts->tm_mday,wday_eg[wday]);
    // 文字列表示
    #ifdef DARKMODE
    glColor3ub(0,0,255);
    #else 
    glColor3ub(255,102,0);
    #endif 
    glRasterPos2i(WINDOW_W/2-(18*timestr_len/4),30+1);
    for(i=0;i<timestr_len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,timestr[i]);
    }
    #ifdef DARKMODE
    glColor3ub(0,191,255);
    #else 
    glColor3ub(255,69,0);
    #endif 
    glRasterPos2i(WINDOW_W/2-(18*timestr_len/4),30);
    for(i=0;i<timestr_len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,timestr[i]);
    }
    // 領域解放
    free(timestr);

    //hour,min,secを表示
    // 文字列生成
    timestr = (char *)malloc(9*sizeof(char));
    sprintf(timestr,"%02d:%02d:%02d",ts->tm_hour,ts->tm_min,ts->tm_sec);
    timestr_len = strlen(timestr);
    // 文字列表示
    #ifdef DARKMODE
    glColor3ub(0,255,0);
    #else 
    glColor3ub(0,0,205);
    #endif 
    glRasterPos2i(WINDOW_W/2-(18*timestr_len/4)+1,60);
    for(i=0;i<timestr_len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,timestr[i]);
    }

    #ifdef DARKMODE
    glColor3ub(51,255,102);
    #else 
    glColor3ub(0,0,205);
    #endif 
    glRasterPos2i(WINDOW_W/2-(18*timestr_len/4),60);
    for(i=0;i<timestr_len;i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,timestr[i]);
    }
    // 領域解放
    free(timestr);

    // インデックス描画
    for(i=1;i<=60;i++){
        #ifdef DARKMODE
        glColor3ub(255,255,255);
        #else 
        glColor3ub(0,0,0);
        #endif 
        glLineWidth(1.0);
        l=100; // インデックスの先端を長さ110にする
        if(i%5==0){  // 5の倍数の針は長くする
        l = 90; // インデックスの終端を長さ90にする
        }
        theta = 2*M_PI*i/60;
        x1 = xc+l*sin(theta);
        y1 = yc-l*cos(theta);  
        l = 110;
        x2 = xc+l*sin(theta);
        y2 = yc-l*cos(theta);
        glBegin(GL_LINES);
        glVertex2i(x1,y1);
        glVertex2i(x2,y2);
        glEnd();

        #ifdef DARKMODE
        glColor3ub(255,255,255);
        #else 
        glColor3ub(0,0,0);
        #endif 
        if(i%5==0){ // 5の倍数のとき文字を表示
            sprintf(s,"%d",i/5);
            l =80; // 文字表示位置を80にする
            x2 = xc+l*sin(theta);
            y2 = yc-l*cos(theta);
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


    //時針描画
    #ifdef DARKMODE
    glColor3ub(255,255,255);
    #else 
    glColor3ub(0,0,0);
    #endif 
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glVertex2i(xc,yc);
    glVertex2i(xh,yh);
    glEnd();
    //分針描画
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2i(xc,yc);
    glVertex2i(xm,ym);
    glEnd();
    //秒針描画
    glLineWidth(2.0);
    glColor3ub(255,0,0);
    glBegin(GL_LINES);
    glVertex2i(xc,yc);
    glVertex2i(xs,ys);
    glEnd();

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
