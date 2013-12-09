#if !defined(MAIN_HPP)
#define MAIN_HPP

/* Include C++ standard headers */
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <locale>

/* Include C standard headers */
#include <cstdio>
#include <cstring>
//#include <unistd.h>
//#include <GL/glut.h>
//#include <glut.h>

//#include <unistd.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>


/* Include my headers */

//#include "texture.h"
#include "utils/exceptions.hpp"
#include "cg/objparser.hpp"
#include "cg/image.hpp"
#include "cg/Object.hpp"
#include "cg/Transform.hpp"
#include "cg/cg_utils.hpp"
#include "utils/system.hpp"
#include "utils/ASCII_codes.h"

/* Global vars */

/* Macros */

namespace ede
{
int draw_width = 1280;
int draw_height = 720;

bool up_pressed = false, down_pressed = false, left_pressed = false, right_pressed = false,
    W_pressed = false, A_pressed = false, S_pressed = false, D_pressed = false;

typedef std::shared_ptr<obj::object> obj_ptr;

std::vector< obj_ptr > scene;

obj_ptr camera, car_obj, tire_fl, tire_fr, tire_bl, tire_br;

cg::Transform tire_angle;

/* report GL errors, if any, to std::cerr */
//void checkError(const char *functionName)
void checkGlError(const std::string &functionName)
{
    GLenum error;
    while (( error = glGetError() ) != GL_NO_ERROR) {
        std::cerr << "When running function: " << functionName << "\n" ;
        std::cerr << "Detected GL error 0x";
        std::cerr << std::hex << error << std::dec << "\n";
    }
    std::cerr << std::flush;
}


GLuint loadGLTexture(const char *filename)
{
    cg::image::bitmap image = cg::image::loadPPM(filename);

    GLuint textureID = 0; //OpenGL texture ID to be returned

    // Generate ID
    glGenTextures(1, &textureID);

    // Bind to gl state machine
    glBindTexture(GL_TEXTURE_2D, textureID);

    //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Ignore surface color
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //Define the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.raw());
    //Create mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, image.raw());

    // PPMs come in upside down, so correct this.
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScaled(1.0,-1.0,1.0);
    glTranslated(0.0,1.0,0.0);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureID;
}

void draw(void)
{
    for(auto iter = scene.cbegin(); iter != scene.cend(); ++iter)
    {
        iter->get()->draw();
    }
}

void display(void)
{
    glClearColor( 0.0, 0.0, 0.0, 1 );
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glViewport(0,0, draw_width, draw_height);

    draw();
    glFlush();

    glutSwapBuffers();

    checkGlError("display");
}

void idle(void)
{
    //usleep(1000); // SLEEP!
    checkGlError("idle");
}

void reshape(int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double(x)/double(y)), 0.1, 100000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
    glFlush();

    draw_width = x;
    draw_height = y;

    checkGlError("reshape");
}

void load_objects(void)
{
    camera.reset(new obj::camera);
    camera->transform = cg::matrix::translate(0,2,0) * cg::matrix::rotate_y(30);

    obj::object::scene_camera = camera;
    obj::null::drawable = false;

    std::shared_ptr<obj::object> tmp;
    GLuint texid = 0;

    //parking lot
    tmp.reset(new obj::geo());
    tmp->set_geo(cg::objparser::parse_file("./geo/ParkingLot.obj"));
    texid = loadGLTexture("./tex/ParkingLot.ppm");
    tmp->set_texid(texid);
    scene.push_back(tmp);

    //car
    tmp.reset(new obj::geo());
    tmp->set_geo(cg::objparser::parse_file("./geo/car.obj"));
    texid = loadGLTexture("./tex/car.ppm");
    tmp->set_texid(texid);
    tmp->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->transform = cg::matrix::translate(-2.9, 0.0, -7.7) * cg::matrix::rotate_y(60.0);
    tmp->transform = cg::matrix::translate(0.0, 0.0, 0.545);
    scene.push_back(tmp);

    car_obj = tmp;

    // Tire data is shared between all instances
    auto tire_geo = cg::objparser::parse_file("./geo/tire.obj");
    texid = loadGLTexture("./tex/tire.ppm");

    //Tire geo, Front Left
    tmp.reset(new obj::geo());
    tmp->set_geo(tire_geo);
    tmp->set_texid(texid);
    tmp->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent->parent = car_obj;
    tmp->parent->parent->transform = cg::matrix::translate(-0.377,0.153,-0.545) * cg::matrix::scale(-0.25, -0.25, 0.25);
    scene.push_back(tmp);
    scene.push_back(tmp->parent);

    tire_fl = tmp;

    //Tire geo, Front Right
    tmp.reset(new obj::geo());
    tmp->set_geo(tire_geo);
    tmp->set_texid(texid);
    tmp->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent->parent = car_obj;
    tmp->parent->parent->transform = cg::matrix::translate(0.377,0.153,-0.545) * cg::matrix::uniform_scale(0.25);
    scene.push_back(tmp);
    scene.push_back(tmp->parent);

    tire_fr = tmp;

    //Tire geo, Back Left
    tmp.reset(new obj::geo());
    tmp->set_geo(tire_geo);
    tmp->set_texid(texid);
    tmp->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent->parent = car_obj;
    tmp->parent->parent->transform = cg::matrix::translate(-0.377,0.153,0.465) * cg::matrix::scale(-0.25, -0.25, 0.25);
    scene.push_back(tmp);
    scene.push_back(tmp->parent);

    tire_bl = tmp;

    //Tire geo, Back Right
    tmp.reset(new obj::geo());
    tmp->set_geo(tire_geo);
    tmp->set_texid(texid);
    tmp->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent = std::shared_ptr<obj::object>(new obj::null());
    tmp->parent->parent->parent = car_obj;
    tmp->parent->parent->transform = cg::matrix::translate(0.377,0.153,0.465) * cg::matrix::uniform_scale(0.25);
    scene.push_back(tmp);
    scene.push_back(tmp->parent);

    tire_br = tmp;

    // Done!
    std::cerr << "Finished loading scene files." << std::endl;
}

void initGL(void)
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);

    load_objects();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 100000);

    checkGlError("initGL");
}

void keyboard(unsigned char key, int x, int y)
{
    const double amt = 0.1, rot = 10;
    switch ( key )
    {
        case ASCII_ESCAPE:
            std::cerr << "Escape key pressed. Exiting program." << std::endl;
            throw err::system_exit();    // then exit the program
            break;
        case 'w':
        case 'W':
            std::cerr << "'W' key pressed." << std::endl;
            camera->transform *= cg::matrix::translate(0,0,-amt);
            break;
        case 'a':
        case 'A':
            std::cerr << "'A' key pressed." << std::endl;
            camera->transform *= cg::matrix::translate(-amt,0,0);
            break;
        case 's':
        case 'S':
            std::cerr << "'S' key pressed." << std::endl;
            camera->transform *= cg::matrix::translate(0,0,amt);
            break;
        case 'd':
        case 'D':
            std::cerr << "'D' key pressed." << std::endl;
            camera->transform *= cg::matrix::translate(amt,0,0);
            break;
        case 'q':
        case 'Q':
            std::cerr << "'Q' key pressed." << std::endl;
            camera->transform *= cg::matrix::rotate_y(amt * rot);
            break;
        case 'e':
        case 'E':
            std::cerr << "'E' key pressed." << std::endl;
            camera->transform *= cg::matrix::rotate_y(-amt * rot);
            break;
        case 'z':
        case 'Z':
            std::cerr << "'Z' key pressed." << std::endl;
            camera->transform *= cg::matrix::rotate_x(amt * rot);
            break;
        case 'c':
        case 'C':
            std::cerr << "'C' key pressed." << std::endl;
            camera->transform *= cg::matrix::rotate_x(-amt * rot);
            break;
        default:
            // Otherwise do nothing
            break;
    }

    display();
}

void special(int key, int x, int y)
{
    const double amt = 1.0;
    cg::Transform tmp(tire_angle);
    switch ( key )
    {
        case GLUT_KEY_UP:
            std::cerr << "Up key pressed." << std::endl;
            tire_fl->transform *= cg::matrix::rotate_x(amt * 32);
            tire_bl->transform *= cg::matrix::rotate_x(amt * 32);
            tire_fr->transform *= cg::matrix::rotate_x(-amt * 32);
            tire_br->transform *= cg::matrix::rotate_x(-amt * 32);

            tmp.ry *= 0.08;
            car_obj->parent->transform *= cg::matrix::translate(0, 0, -amt * 0.12) * cg::Mat4x4(tmp);
            break;
        case GLUT_KEY_DOWN:
            std::cerr << "Down key pressed." << std::endl;
            tire_fl->transform *= cg::matrix::rotate_x(-amt * 8);
            tire_bl->transform *= cg::matrix::rotate_x(-amt * 8);
            tire_fr->transform *= cg::matrix::rotate_x(amt * 8);
            tire_br->transform *= cg::matrix::rotate_x(amt * 8);

            tmp.ry *= 0.04;
            car_obj->parent->transform *= cg::matrix::translate(0, 0, amt * 0.03) * cg::matrix::inverted(cg::Mat4x4(tmp));
            break;
        case GLUT_KEY_LEFT:
            std::cerr << "Left key pressed." << std::endl;
            tire_angle.ry += amt * 2;
            tire_angle.ry = cg::utils::clamp(tire_angle.ry, -45.0, 45.0);
            tire_fl->parent->transform = cg::matrix::inverted(cg::Mat4x4(tire_angle));
            tire_fr->parent->transform = cg::Mat4x4(tire_angle);
            break;
        case GLUT_KEY_RIGHT:
            std::cerr << "Right key pressed." << std::endl;
            tire_angle.ry -= amt * 2;
            tire_angle.ry = cg::utils::clamp(tire_angle.ry, -45.0, 45.0);
            tire_fl->parent->transform = cg::matrix::inverted(cg::Mat4x4(tire_angle));
            tire_fr->parent->transform = cg::Mat4x4(tire_angle);
            break;
        default:
            // Otherwise do nothing
            break;
    }

    display();
}

void flush_all(void)
{
    std::cout.flush();
    std::clog.flush();
    std::cerr.flush();
}

void set_locale(void)
{
    // Attempt to force locale to US English, UTF-8.
    try
    {
        std::locale::global(std::locale("en_US.utf8"));
    }
    catch (std::runtime_error)
    {
        throw err::system_exit("The required locale is not installed. "
                                 "Please install: en_US.utf8", 8);
    }

    // Success!
    // Force the only pre-existing iostream objects to follow suit. All new 
    // iostream objects will simply inherit the global locale at instantiation.
    std::cout.imbue(std::locale());
    std::cerr.imbue(std::locale());
    std::clog.imbue(std::locale());
    std::cin.imbue(std::locale());
}

// Since we are inside a namespace the compiler/linker will not complain
// about the function being named main.
int main(int argc, char **argv)
{
    std::atexit(flush_all);
    set_locale();

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

    //glutInitContextVersion (3, 1);
    //glutInitContextFlags (GLUT_COMPATIBILITY_PROFILE | GLUT_DEBUG);
    //glutInitContextFlags (GLUT_FORWARD_COMPATIBLE | GLUT_CORE_PROFILE | GLUT_DEBUG);

    glutInitWindowSize (draw_width, draw_height); 
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("CS455 Viewport transforms");

    initGL();

    // Register GLUT functions
    glutDisplayFunc(display); 
    glutIdleFunc(idle); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    // Start GLUT main loop
    glutMainLoop();
    return EXIT_SUCCESS;
}

}

#endif // end include guard
