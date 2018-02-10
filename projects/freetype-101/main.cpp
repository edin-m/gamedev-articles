#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "freetype-gl.h"
#include "vertex-buffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

// ------------------------------------------------------- typedef & struct ---
typedef struct {
    float x, y, z;
    float s, t;
    float r, g, b, a;
} vertex_t;

// ------------------------------------------------------- global variables ---
texture_atlas_t* atlas;
vertex_buffer_t* buffer;
k::Shader shader;
glm::mat4 ortho;

// --------------------------------------------------------------- add_text ---
void add_text(vertex_buffer_t* buffer, texture_font_t* font,
              char* text, vec4* color, vec2* pen)
{
  size_t i;
  float r = color->red, g = color->green, b = color->blue, a = color->alpha;
  for (i = 0; i < strlen(text); ++i) {
    texture_glyph_t *glyph = texture_font_get_glyph(font, text + i);
    if (glyph != nullptr) {
      float kerning =  0.0f;
      if (i > 0) {
        kerning = texture_glyph_get_kerning(glyph, text + i - 1);
      }
      pen->x += kerning;
      int x0  = (int)(pen->x + glyph->offset_x);
      int y0  = (int)(pen->y + glyph->offset_y);
      int x1  = (int)(x0 + glyph->width);
      int y1  = (int)(y0 - glyph->height);

      float xdiff = 0.0f;
      float ydiff = 0.0f;

      xdiff = glyph->s1 - glyph->s0;
      ydiff = glyph->t1 - glyph->t0;
      xdiff = xdiff / 20.0f;
      ydiff = ydiff / 25.0f;

      float s0 = glyph->s0;
      float t0 = glyph->t0;
      float s1 = glyph->s1 - xdiff;
      float t1 = glyph->t1 - ydiff;

      GLuint indices[6] = { 0, 1, 2,  0, 2, 3 };
      vertex_t vertices[4] = { { static_cast<float>(x0), static_cast<float>(y0), 0,  s0, t0,  r, g, b, a },
                               { static_cast<float>(x0), static_cast<float>(y1), 0,  s0, t1,  r, g, b, a },
                               { static_cast<float>(x1), static_cast<float>(y1), 0,  s1, t1,  r, g, b, a },
                               { static_cast<float>(x1), static_cast<float>(y0), 0,  s1, t0,  r, g, b, a } };
      vertex_buffer_push_back(buffer, vertices, 4, indices, 6);
      pen->x += glyph->advance_x;
    }
  }
}

void cleanup() {
  glDeleteTextures(1, &atlas->id);
  atlas->id = 0;
  texture_atlas_delete(atlas);
}

// ------------------------------------------------------------------- init ---
void init()
{
    size_t i;
    texture_font_t* font = 0;
    atlas = texture_atlas_new(1024, 1024, 1);
    const char* filename = "data/freetype-101/fonts/Vera.ttf";
    char* text = "A quick brown fox jumps over      the  lazy  dog.";
    buffer = vertex_buffer_new("vertex:3f,tex_coord:2f,color:4f");
    vec2 pen = {{ 0, 900 }};
    vec4 black = {{ 0, 0, 0, 1 }};
    for (i = 7; i < 40; ++i) {
        font = texture_font_new_from_file(atlas, i, filename);
        pen.x = 5;
        pen.y -= font->height;
        texture_font_load_glyphs(font, text);
        add_text(buffer, font, text, &black, &pen);
        texture_font_delete(font);
    }

    glGenTextures(1, &atlas->id);
    glBindTexture(GL_TEXTURE_2D, atlas->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
                 0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);

    shader.loadShader("data/freetype-101/shaders/freetype-101");
}

// ---------------------------------------------------------------- display ---
void display(GLFWwindow* window)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.use();
    {
        glUniform1i(shader.uniform("tex"), 0);
        glUniformMatrix4fv(shader.uniform("mvp"), 1, 0, glm::value_ptr(ortho));
        vertex_buffer_render(buffer, GL_TRIANGLES);
    }
    shader.unuse();

    glfwSwapBuffers(window);
}


// ---------------------------------------------------------------- reshape ---
void reshape(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);

  ortho = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
}

// --------------------------------------------------------------- keyboard ---
void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose( window, GL_TRUE );
  }
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
      exit(0);
  }
}


// --------------------------------------------------------- error-callback ---
void error_callback( int error, const char* description )
{
    fputs( description, stderr );
}

// ------------------------------------------------------------------- main ---
int main(int argc, char **argv)
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit( )) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

#if APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, argv[0], NULL, NULL);

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
        LOG(ERROR) << "Error " << glewGetErrorString(err);
        exit(EXIT_FAILURE);
    }
    LOG(INFO) << "Using GLEW " << glewGetString(GLEW_VERSION);

    const GLubyte* renderer;
    const GLubyte* version;
    int majorVersion;
    int minorVersion;

    renderer = glGetString(GL_RENDERER); /* get renderer string */
    version = glGetString(GL_VERSION);	 /* version as a string */
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);	 /* version as a string */
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);	 /* version as a string */
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    printf("OpenGL version supported %d.%d\n", majorVersion, minorVersion);

    init();

    glfwShowWindow(window);
    reshape(window, 1280, 720);

    while (!glfwWindowShouldClose(window)) {
        display(window);
        glfwPollEvents();
    }

  cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
