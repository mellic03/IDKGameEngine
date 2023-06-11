#include "IDK_ui.h"




void
idk::core_modules::UIModule::_rect(int x, int y, int w, int h)
{
    y = _height - h - y;

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    my = _height - my;

    glBegin(GL_QUADS);

        if (mx > x && mx < x+w && my > y && my < y+h)
            glColor3f(1, 1, 0);
        else
            glColor3f(1, 0, 0);

        glVertex2f( x,    y   );
        glVertex2f( x,    y+h );
        glVertex2f( x+w,  y+h );
        glVertex2f( x+w,  y   );

    glEnd();
}



void
idk::core_modules::UIModule::stage_C(idk::Engine &engine)
{
    if (engine.mouseCaptured())
        return;

    _width = engine.rengine().width();
    _height = engine.rengine().height();

    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);


    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, _width, 0, _height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    for (int i=0; i<_width; i+=_width/9)
    {
        for (int j=0; j<_height; j+=_height/9)
        {
            UIModule::_rect(i+_width/18, j+_height/18, 25, 25);
        }
    }


    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}


