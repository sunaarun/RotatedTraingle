/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Dr Anton Gerdelan, Trinity College Dublin, Ireland.                          |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Matrices and Vectors                                                         |
| Note: code discussed in previous tutorials is moved into gl_utils file       |
| On Apple don't forget to uncomment the version number hint in start_gl()     |
\******************************************************************************/
#include "gl_utils.h"   // utility functions discussed in earlier tutorials
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 640;
GLFWwindow* g_window = NULL;

int main() {
    restart_gl_log();
    // all the GLFW and GLEW start-up code is moved to here in gl_utils.cpp
    start_gl();
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"

    /* OTHER STUFF GOES HERE NEXT */
    GLfloat points[] = { 0.0f, 0.5f, 0.0f,   0.5f, -0.5f, 0.0f,   -0.5f, -0.5f, 0.0f };
    GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

    GLfloat Lpoints[] = {-0.75f,-0.75f,0.0, -0.75f,0.0f,0.0f, -0.75f,0.75f,0.0f};
    GLfloat Lcolours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

    GLfloat RTpoints[] = { 0.8f,-0.5f,0.0,   0.5f,0.5f,0.0f,    0.8f,0.5f,0.0f };
    GLfloat RTcolours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    //-------------------------------

    GLuint points_vbo;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

    GLuint colours_vbo;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colours, GL_STATIC_DRAW);

    GLuint L_points_vbo;
    glGenBuffers(1, &L_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, L_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9* sizeof(GLfloat), Lpoints, GL_STATIC_DRAW);

    GLuint Lcolours_vbo;
    glGenBuffers(1, &Lcolours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Lcolours_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), Lcolours, GL_STATIC_DRAW);


    GLuint RT_points_vbo;
    glGenBuffers(1, &RT_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, RT_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), RTpoints, GL_STATIC_DRAW);

    GLuint RTcolours_vbo;
    glGenBuffers(1, &RTcolours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, RTcolours_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), RTcolours, GL_STATIC_DRAW);
    //-------------------------------------------------------------------------------------------------
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint Lvao;
    glGenVertexArrays(1, &Lvao);
    glBindVertexArray(Lvao);
    glBindBuffer(GL_ARRAY_BUFFER, L_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, Lcolours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint RTvao;
    glGenVertexArrays(1, &RTvao);
    glBindVertexArray(RTvao);
    glBindBuffer(GL_ARRAY_BUFFER, RT_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, RTcolours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //-------------------------------------------------------------------
    char vertex_shader[1024 * 256];
    char fragment_shader[1024 * 256];
    parse_file_into_str("test_vs.glsl", vertex_shader, 1024 * 256);
    parse_file_into_str("test_fs.glsl", fragment_shader, 1024 * 256);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* p = (const GLchar*)vertex_shader;
    glShaderSource(vs, 1, &p, NULL);
    glCompileShader(vs);

    //----- line
    char lvertex_shader[1024 * 256];
    char lfragment_shader[1024];
    parse_file_into_str("l_test_vs.glsl", lvertex_shader, 1024 * 256);
    parse_file_into_str("l_test_fs.glsl", lfragment_shader, 1024);

    GLuint lvs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* lp = (const GLchar*)lvertex_shader;
    glShaderSource(lvs, 1, &lp, NULL);
    glCompileShader(lvs);

    // right triangle 
    char RTvertex_shader[1024];
    char RTfragment_shader[1024];
    parse_file_into_str("test_vs_rt.glsl", RTvertex_shader, 1024 );
    parse_file_into_str("test_fs_rt.glsl", RTfragment_shader, 1024);

    GLuint RTvs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* RTp = (const GLchar*)RTvertex_shader;
    glShaderSource(RTvs, 1, &RTp, NULL);
    glCompileShader(RTvs);

    //--------------------------------------------------------------------------
    // check for compile errors
    int params = -1;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
        print_shader_info_log(vs);
        return 1; // or exit or something
    }



    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    p = (const GLchar*)fragment_shader;
    glShaderSource(fs, 1, &p, NULL);
    glCompileShader(fs);

    // check for compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
        print_shader_info_log(fs);
        return 1; // or exit or something
    }

    GLuint lfs = glCreateShader(GL_FRAGMENT_SHADER);
    lp = (const GLchar*)lfragment_shader;
    glShaderSource(lfs, 1, &lp, NULL);
    glCompileShader(lfs);
    
    // right triangle
    GLuint RTfs = glCreateShader(GL_FRAGMENT_SHADER);
    RTp = (const GLchar*)RTfragment_shader;
    glShaderSource(RTfs, 1, &RTp, NULL);
    glCompileShader(RTfs);


    //----------------------------------------------------------------------------------------

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: could not link shader programme GL index %i\n", shader_programme);
        print_programme_info_log(shader_programme);
        return false;
    }
    GLuint lshader_programme = glCreateProgram();
    glAttachShader(lshader_programme, lfs);
    glAttachShader(lshader_programme, lvs);
    glLinkProgram(lshader_programme);

    //right triangle
    GLuint RTshader_programme = glCreateProgram();
    glAttachShader(RTshader_programme, RTfs);
    glAttachShader(RTshader_programme, RTvs);
    glLinkProgram(RTshader_programme);

    //-----------------------------------------------------------------------

    GLfloat matrix[] = {
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      0.0f, 0.0f, 0.0f, 1.0f  // fourth column
      //  0.5f, 0.0f, 0.0f, 1.0f  // fourth column
    };
    GLfloat matT1[] = {
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      -0.0f, -0.0f, -0.0f, 1.0f  // fourth column
    };
    GLfloat matT2[] = {
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      0.0f, 0.0f, 0.0f, 1.0f  // fourth column
    };
    /*
    You can write it like these
     GLfloat matrix[] = {
      1.0f,
      0.0f,
      0.0f,
      0.0f, // first column
          0.0f,
          1.0f,
          0.0f,
          0.0f, // second column
              0.0f,
              0.0f,
              1.0f,
              0.0f, // third column
                   0.5f,
                   0.0f,
                   0.0f,
                   1.0f  // fourth column
    };
    */
    int matrix_location = glGetUniformLocation(shader_programme, "matrix");
    int matT1_location = glGetUniformLocation(shader_programme, "matT1");
    int matT2_location = glGetUniformLocation(shader_programme, "matT2");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);// update the matrix , send the update martix to the card
    glUniformMatrix4fv(matT1_location, 1, GL_FALSE, matT1);
    glUniformMatrix4fv(matT2_location, 1, GL_FALSE, matT2);
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK);    // cull back face
    glFrontFace(GL_CW);     // GL_CCW for counter clock-wise

    //-------------------------------- right triangle

    GLfloat matrix2[] = {
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      0.0f, 0.0f, 0.0f, 1.0f  // fourth column
      //  0.5f, 0.0f, 0.0f, 1.0f  // fourth column
    };
    GLfloat matT12[] = { //translation to origin 
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      -0.75f, -0.0f, -0.0f, 1.0f  // fourth column
    };
    GLfloat matT22[] = { //
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      0.75f, 0.0f, 0.0f, 1.0f  // fourth column
    };

    int Rmatrix_location = glGetUniformLocation(RTshader_programme, "matrix2");
    int RmatT1_location = glGetUniformLocation(RTshader_programme, "matT12");
    int RmatT2_location = glGetUniformLocation(RTshader_programme, "matT22");
    glUseProgram(RTshader_programme);
    glUniformMatrix4fv(Rmatrix_location, 1, GL_FALSE, matrix2);// update the matrix , send the update martix to the card
    glUniformMatrix4fv(RmatT1_location, 1, GL_FALSE, matT12);
    glUniformMatrix4fv(RmatT2_location, 1, GL_FALSE, matT22);
   // glEnable(GL_CULL_FACE); // cull face
    //glCullFace(GL_BACK);    // cull back face
    //glFrontFace(GL_CW);     // GL_CCW for counter clock-wise

    //---------------------------------
    float speed = 1.0f; // move at 1 unit per second - increase the steps to speed up
    float last_position = 0.0f;
    while (!glfwWindowShouldClose(g_window)) {
        // add a timer for doing animation
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        _update_fps_counter(g_window);
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, g_gl_width, g_gl_height);

        glUseProgram(shader_programme);
        //glUseProgram(RTshader_programme);

        // update the matrix
        // - you could simplify this by just using sin(current_seconds)
       // matrix[12]    = elapsed_seconds * speed + last_position;//moving in x cooridinate
        /*
        matrix[0] =cos( elapsed_seconds * speed + last_position);
        matrix[1] =- sin(elapsed_seconds * speed + last_position);
        matrix[2] = 0;//sin(elapsed_seconds * speed + last_position);
        matrix[3] = 0; //-sin(elapsed_seconds * speed + last_position);

        matrix[4] = sin(elapsed_seconds * speed + last_position);
        matrix[5]= cos(elapsed_seconds * speed + last_position);
        matrix[6] = 0; //cos(elapsed_seconds * speed + last_position);
        matrix[7] = 0;

        matrix[8] = 0;
        matrix[9] = 0;
        matrix[10] = 1;
        matrix[11] = 0;

        matrix[12] = 0;
        */
        //-----------------------------
    /* matrix[0] = cos(elapsed_seconds * speed + last_position);
        matrix[1] = sin(elapsed_seconds * speed + last_position);
  
        matrix[4] = -sin(elapsed_seconds * speed + last_position);
        matrix[5] = cos(elapsed_seconds * speed + last_position);
        matrix[10] = 1;
        matrix[11] = 0;
        
       // matrix[12] =  elapsed_seconds* speed + last_position;
      
        matrix[15] = 1;
        matrix[10]=sin( elapsed_seconds * speed + last_position);
        */
       //  matrix[13] = elapsed_seconds * speed + last_position;
       // 
  

        matrix[0] = cos(glfwGetTime() * -2);
        matrix[1] = sin(glfwGetTime() * -2);

        matrix[4] = -sin(glfwGetTime() * -2);
        matrix[5] = cos(glfwGetTime() * -2);

        // matrix2[10] = 1;
  //        matrix2[12] =  glfwGetTime()*10;

        matrix[15] = 1;
        matrix[10] = sin(glfwGetTime() * -2);

       // --------------------------------

        matrix2[0] = cos(glfwGetTime()*2);
        matrix2[1] = sin(glfwGetTime()*2);
    
        matrix2[4] = -sin(glfwGetTime()*2);
        matrix2[5] = cos(glfwGetTime()*2);
  
       // matrix2[10] = 1;
 //        matrix2[12] =  glfwGetTime()*10;
 
        matrix2[15] = 1;
        matrix2[10] = sin(glfwGetTime()*2);
       
        //--------------------------------------------------
         // if you change to 13, it will move vertical in y coordinate
        last_position = elapsed_seconds * speed + last_position;
        if (fabs(last_position) > 1.0) { speed = -speed; }// moving from righ to left or left to right
   
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);


        glUseProgram(RTshader_programme);
        glUniformMatrix4fv(Rmatrix_location, 1, GL_FALSE, matrix2);

        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
     


        glUseProgram(RTshader_programme);
        glBindVertexArray(RTvao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(lshader_programme);
        glBindVertexArray(Lvao);
        /* draw points 0-3 from the currently bound VAO with current in-use shader */
        glDrawArrays(GL_LINES, 0, 3);

        glfwPollEvents();
        if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(g_window, 1); }
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(g_window);
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
