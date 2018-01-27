#include <iostream>

#include <functional>

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "shader.h"

#include "freetype-gl.h"
#include "demos/mat4.h"
#include "demos/shader.h"
#include "vertex-buffer.h"
#include "utf8-utils.h"
//#include "demos/screenshot-util.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "glcheck.h"

k::Shader myshader;
texture_atlas_t* atlas;
vertex_buffer_t* buffer;
mat4 model, view, projection;

typedef struct {
    float x, y, z;
    float s, t;
    float r, g, b, a;
} vertex_t;

void add_text( vertex_buffer_t * buffer, texture_font_t * font,
               const char * text, vec4 * color, vec2 * pen )
{
    size_t i;
    float r = color->red, g = color->green, b = color->blue, a = color->alpha;
    for( i = 0; i < strlen(text); ++i )
    {
        texture_glyph_t *glyph = texture_font_get_glyph( font, text + i );
        if( glyph != NULL )
        {
            float kerning =  0.0f;
            if( i > 0)
            {
                kerning = texture_glyph_get_kerning( glyph, text + i - 1 );
            }
            pen->x += kerning;
            float x0  = ( pen->x + glyph->offset_x );
            float y0  = ( pen->y + glyph->offset_y );
            float x1  = ( x0 + glyph->width );
            float y1  = ( y0 - glyph->height );
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;
            GLuint indices[6] = {0,1,2, 0,2,3};
            vertex_t vertices[4] = { { (float)x0,(float)y0,0,  s0,t0,  r,g,b,a },
                                     { (float)x0,(float)y1,0,  s0,t1,  r,g,b,a },
                                     { (float)x1,(float)y1,0,  s1,t1,  r,g,b,a },
                                     { (float)x1,(float)y0,0,  s1,t0,  r,g,b,a } };
            vertex_buffer_push_back( buffer, vertices, 4, indices, 6 );
            pen->x += glyph->advance_x;
        }
    }
}

void init(void) {
  atlas = texture_atlas_new(512, 512, 1);
  const char* filename = "../data/fonts/Vera.ttf";
  const char* cache = " !\"#$%&'()*+,-./0123456789:;<=>?"
                       "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                       "`abcdefghijklmnopqrstuvwxyz{|}~";
  size_t minsize = 8, maxsize = 26;
  size_t count = maxsize - minsize;
  size_t i, missed = 0;

  texture_font_t* font15 = 0;
  atlas = texture_atlas_new(512, 512, 1);
  const char * text = "A Quick Brown Fox Jumps Over The Lazy Dog 0123456789";
  buffer = vertex_buffer_new( "vertex:3f,tex_coord:2f,color:4f" );
  vec2 pen = {{5,800}};
  vec4 black = {{0,0,0,1}};
  for (i = minsize; i < maxsize; ++i) {
    texture_font_t* font = texture_font_new_from_file(atlas, i, filename);
    pen.x = 5;
    pen.y -= font->height;
    texture_font_load_glyphs(font, cache);
    if (i == 15) {
      font15 = font;
    } else {
      texture_font_delete(font);
    }
  }
  add_text(buffer, font15, text, &black, &pen);
  texture_font_delete(font15);

  glGenTextures(1, &atlas->id);
  glBindTexture(GL_TEXTURE_2D, atlas->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
                0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);

  myshader.loadShader("data/freetype-101/shaders/v3f-t2f-c4f");
  mat4_set_identity( &projection );
  mat4_set_identity( &model );
  mat4_set_identity( &view );
}

void display(GLFWwindow* window) {
  glClearColor( 1, 1, 1, 1 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    myshader.use();
    {
      GLint texLoc = myshader.uniform("texture");
      GLint modelLoc = myshader.uniform("model");
      GLint viewLoc = myshader.uniform("view");
      GLint projLoc = myshader.uniform("projection");

      glUniform1i(texLoc, 0);
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data);
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data);
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.data);

      vertex_buffer_render(buffer, GL_TRIANGLES);
    }
    myshader.unuse();
    glfwSwapBuffers(window);
}

void reshape(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    mat4_set_orthographic(&projection, 0, width, 0, height, -1, 1);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void error_callback(int error, const char* description) {
    LOG(ERROR) << error << description;
}

int main(int argc, char **argv)
{
    START_EASYLOGGINGPP(argc, argv);
    GLFWwindow* window;
    char* screenshot_path = NULL;

    if (argc > 1) {
        if (argc == 3 && 0 == strcmp("--screenshot", argv[1])) {
            screenshot_path = argv[2];
        } else {
            LOG(ERROR) << "Unknown or incomplete parameters given";
            exit(EXIT_FAILURE);
        }
    }

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(512, 512, argv[0], NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetWindowRefreshCallback(window, display);
    glfwSetKeyCallback(window, keyboard);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        LOG(ERROR) << "Error: " << glewGetErrorString(err);
        exit(EXIT_FAILURE);
    }
    LOG(INFO) << "Using GLEW " << glewGetString(GLEW_VERSION);

    init();

    glfwShowWindow(window);
    reshape(window, 512, 512);

    while (!glfwWindowShouldClose(window))
    {
        display(window);
        glfwPollEvents();

//        if (screenshot_path)
//        {
//            screenshot(window, screenshot_path);
//            glfwSetWindowShouldClose(window, 1);
//        }
    }

    glDeleteTextures(1, &atlas->id);
    atlas->id = 0;
    texture_atlas_delete(atlas);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
