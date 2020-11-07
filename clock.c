#include<GL/glut.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

void Display(void);
void Reshape(int,int);
void Timer(int);

int main(int argc,char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(320,240);
    glutCreateWindow("clock");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutTimerFunc(500,Timer,0);
    glClearColor(0.0,0.0,0.0,1.0);
    glutMainLoop();
    return 0;
}

void Display(void){
    int xc = glutGet(GLUT_WINDOW_WIDTH)/2;
    int yc = glutGet(GLUT_WINDOW_HEIGHT)/2;
    double thetas,thetam,thetah;
    int xs,ys,xm,ym,xh,yh;
    int ls=50;
    int lm = 120;
    int lh = 80; 
    time_t tt;
    struct tm *ts;
    time(&tt);
    ts = localtime(&tt);
    /*
    printf("%d年%d月%d日(",1900+ts->tm_year,
        1+ts->tm_mon,ts->tm_mday);
    switch (ts->tm_wday)
        {
        case 0:
            printf("日");
            break;
        case 1:
            printf("月");
            break;
        case 2:
            printf("火");
            break;
        case 3:
            printf("水");
            break;    
        case 4:
            printf("木");
            break;
        case 5:
            printf("金");
            break;  
        case 6:
            printf("土");
            break;                    
        }
        printf(") %02d時%02d分%02d秒\n",ts->tm_hour,ts->tm_min,ts->tm_sec);
    */
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
    glLineWidth(1.0);
    glColor3ub(255,0,0);
    glBegin(GL_LINES);
    glVertex2i(xc,yc);
    glVertex2i(xs,ys);
    glEnd();
    glLineWidth(5.0);
    glColor3ub(255,255,254);
    glBegin(GL_LINES);
    glVertex2i(xc,yc);
    glVertex2i(xm,ym);
    glEnd();
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2i(xc,yc);
    glVertex2i(xh,yh);
    glEnd();
    glFlush();
}

void Reshape(int w,int h){
    printf("ウィンドウの幅と高さ=%d x %d\n",w,h);
    glViewport(0,0,w,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0,w,0,h);
    glScaled(1,-1,1);
    glTranslated(0,-h,0);
}

void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(500,Timer,0);
}
