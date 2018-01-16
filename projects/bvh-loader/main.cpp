
#include <stdio.h>
#include <iostream>

#include <functional>

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "unistd.h"

#include "shader.h"

#include "bvh/bvh.h"

#include "camera.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    std::cout << "e pressed" << std::endl;
  }
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    exit(0);
  }
}

int main() {
  GLFWwindow* window = nullptr;

  if (!glfwInit()) {
    fprintf( stderr, "ERROR: could not start GLFW3\n" );
    return 1;
  }

#if APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(640, 480, "Hello world", nullptr, nullptr);

  if (!window) {
    fprintf( stderr, "ERROR: could not open window with GLFW3\n" );
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetKeyCallback(window, key_callback);

  const GLubyte* renderer;
  const GLubyte* version;
  /* get version info */
  renderer = glGetString( GL_RENDERER ); /* get renderer string */
  version = glGetString( GL_VERSION );	 /* version as a string */
  printf( "Renderer: %s\n", renderer );
  printf( "OpenGL version supported %s\n", version );

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  int width = 800;
  int height = 600;
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
  glm::mat4 View = glm::lookAt(
      glm::vec3(1,1,3), // Camera is at (4,3,3), in World Space
      glm::vec3(0,0,0), // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
      );
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 mvp = Projection * View * Model;

  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  GLushort cube_elements[] = {
    // front
    0, 1, 2 };

  GLuint ibo_cube_elements;
  glGenBuffers(1, &ibo_cube_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // An array of 3 vectors which represents 3 vertices
  static const GLfloat g_vertex_buffer_data[] = {
     -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
  };

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  k::Shader shader;
  shader.loadShader("shader3");
  shader.use();

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  GLuint MatrixID = glGetUniformLocation(shader.getProgram(), "mvp");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp));


  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    shader.use();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  return 0;
}




//glEnable(GL_DEPTH_TEST);
//glDepthFunc(GL_LESS);

//GLuint vao;
//glGenVertexArrays( 1, &vao );
//glBindVertexArray( vao );

//GLushort cube_elements[] = {
//  // front
//  0, 1, 2 };

//GLuint ibo_cube_elements;
//glGenBuffers(1, &ibo_cube_elements);
//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//// An array of 3 vectors which represents 3 vertices
//static const GLfloat g_vertex_buffer_data[] = {
//   -1.0f, -1.0f, 0.0f,
//   1.0f, -1.0f, 0.0f,
//   0.0f,  1.0f, 0.0f,
//};

//GLuint vertexbuffer;
//glGenBuffers(1, &vertexbuffer);
//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

//k::Shader shader;
//shader.loadShader("shader3");
//shader.use();

//glEnableVertexAttribArray(0);
//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//glVertexAttribPointer(
//   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//   3,                  // size
//   GL_FLOAT,           // type
//   GL_FALSE,           // normalized?
//   0,                  // stride
//   BUFFER_OFFSET(0)
//);

//glBindVertexArray(0);


//while (!glfwWindowShouldClose(window)) {
//  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  glBindVertexArray( vao );
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
//  shader.use();
//  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

//  glfwSwapBuffers( window );
//  glfwPollEvents();
//}














//  // Dark blue background
//  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

//  // Enable depth test
//  glEnable(GL_DEPTH_TEST);
//  // Accept fragment if it closer to the camera than the former one
//  glDepthFunc(GL_LESS);

//  const GLuint NumVertices = 6;
//  GLuint vaoObj;
//  glGenVertexArrays(1, &vaoObj);
//  glBindVertexArray(vaoObj);
//  GLfloat vertices[NumVertices][3] = {
//    { -2.90, -2.90, 5.0 }, // Triangle 1
//    { 2.85, -2.90, 5.0 },
//    { -2.90, 2.85, 5.0 },
//    { 2.90, -2.85, 5.0 }, // Triangle 2
//    { 2.90, 2.90, 5.0 },
//    { -2.85, 2.90, 5.0 }
//  };
//  static const GLfloat g_vertex_buffer_data[] = {
//          -1.0f,-1.0f,-1.0f,
//          -1.0f,-1.0f, 1.0f,
//          -1.0f, 1.0f, 1.0f,
//           1.0f, 1.0f,-1.0f,
//          -1.0f,-1.0f,-1.0f,
//          -1.0f, 1.0f,-1.0f,
//           1.0f,-1.0f, 1.0f,
//          -1.0f,-1.0f,-1.0f,
//           1.0f,-1.0f,-1.0f,
//           1.0f, 1.0f,-1.0f,
//           1.0f,-1.0f,-1.0f,
//          -1.0f,-1.0f,-1.0f,
//          -1.0f,-1.0f,-1.0f,
//          -1.0f, 1.0f, 1.0f,
//          -1.0f, 1.0f,-1.0f,
//           1.0f,-1.0f, 1.0f,
//          -1.0f,-1.0f, 1.0f,
//          -1.0f,-1.0f,-1.0f,
//          -1.0f, 1.0f, 1.0f,
//          -1.0f,-1.0f, 1.0f,
//           1.0f,-1.0f, 1.0f,
//           1.0f, 1.0f, 1.0f,
//           1.0f,-1.0f,-1.0f,
//           1.0f, 1.0f,-1.0f,
//           1.0f,-1.0f,-1.0f,
//           1.0f, 1.0f, 1.0f,
//           1.0f,-1.0f, 1.0f,
//           1.0f, 1.0f, 1.0f,
//           1.0f, 1.0f,-1.0f,
//          -1.0f, 1.0f,-1.0f,
//           1.0f, 1.0f, 1.0f,
//          -1.0f, 1.0f,-1.0f,
//          -1.0f, 1.0f, 1.0f,
//           1.0f, 1.0f, 1.0f,
//          -1.0f, 1.0f, 1.0f,
//           1.0f,-1.0f, 1.0f
//      };

//  GLfloat cube_vertices[] = {
//    // front
//    -1.0, -1.0,  1.0,
//     1.0, -1.0,  1.0,
//     1.0,  1.0,  1.0,
//    -1.0,  1.0,  1.0,
//    // back
//    -1.0, -1.0, -1.0,
//     1.0, -1.0, -1.0,
//     1.0,  1.0, -1.0,
//    -1.0,  1.0, -1.0,
//  };

//  /* init_resources */
//      GLushort cube_elements[] = {
//          // front
//          0, 1, 2,
//          2, 3, 0,
//          // top
//          1, 5, 6,
//          6, 2, 1,
//          // back
//          7, 6, 5,
//          5, 4, 7,
//          // bottom
//          4, 0, 3,
//          3, 7, 4,
//          // left
//          4, 5, 1,
//          1, 0, 4,
//          // right
//          3, 2, 6,
//          6, 7, 3,
//      };

//      GLuint ibo_cube_elements;
//      glGenBuffers(1, &ibo_cube_elements);
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
//      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

//  GLuint vboObj;
//  glGenBuffers(1, &vboObj);
//  glBindBuffer(GL_ARRAY_BUFFER, vboObj);
////  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

//  k::Shader shader;
//  shader.loadShader("shader3");
//  shader.use();
//  GLuint vPosition = 0;
//  GLuint mvpPos = 1;
//  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//  glEnableVertexAttribArray(vPosition);
//  glm::mat4 M = glm::mat4(1.0);
//  glm::mat4 V = glm::lookAt(
//        glm::vec3(4, 3, -3),
//        glm::vec3(0, 0, 0),
//        glm::vec3(0, 1, 0));
//  glm::mat4 P = glm::perspective(glm::radians(45.f), 4.f / 3.f, 0.1f, 1000.0f);
//  glm::mat4 mat = P * V * M;
//  glUniformMatrix4fv(mvpPos, 1, GL_FALSE, glm::value_ptr(mat));

//  while ( !glfwWindowShouldClose( window ) ) {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glBindVertexArray(vaoObj);
//    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//    glEnableVertexAttribArray(vPosition);
////    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//    glDrawArrays(GL_TRIANGLES, 0, 12*3);

//    glfwSwapBuffers( window );
//    glfwPollEvents();
//  }
//  return 0;
//}
//  GLuint vao;
//  GLuint vbo;
//  /* geometry to use. these are 3 xyz points (9 floats total) to make a triangle */
//  GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
//  /* these are the strings of code for the shaders
//          the vertex shader positions each vertex point */
////  const char *vertex_shader = "#version 410\n"
////                              "in vec3 vp;"
////                              "void main () {"
////                              "	gl_Position = vec4 (vp, 1.0);"
////                              "}";
////  /* the fragment shader colours each fragment (pixel-sized area of the
////          triangle) */
////  const char *fragment_shader = "#version 410\n"
////                                "out vec4 frag_colour;"
////                                "void main () {"
////                                "	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
////                                "}";
//  /* GL shader objects for vertex and fragment shader [components] */
////  GLuint vert_shader, frag_shader;
//  /* GL shader programme object [combined, to link] */
////  GLuint shader_programme;

//  k::Shader shader;
//  int res = shader.loadShader("shader3");
//  std::cout << "Shader load result " << res << std::endl;

//  /* tell GL to only draw onto a pixel if the shape is closer to the viewer
//      than anything already drawn at that pixel */
//  glEnable( GL_DEPTH_TEST ); /* enable depth-testing */
//  /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
//  glDepthFunc( GL_LESS );
//  /* a vertex buffer object (VBO) is created here. this stores an array of
//      data on the graphics adapter's memory. in our case - the vertex points */
//  glGenBuffers( 1, &vbo );
//  glBindBuffer( GL_ARRAY_BUFFER, vbo );
//  glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( GLfloat ), points, GL_STATIC_DRAW );

//  /* the vertex array object (VAO) is a little descriptor that defines which
//      data from vertex buffer objects should be used as input variables to vertex
//      shaders. in our case - use our only VBO, and say 'every three floats is a
//      variable' */
//  glGenVertexArrays( 1, &vao );
//  glBindVertexArray( vao );
//  // "attribute #0 should be enabled when this vao is bound"
//  glEnableVertexAttribArray( 0 );
//  // this VBO is already bound, but it's a good habit to explicitly specify which
//  // VBO's data the following
//  // vertex attribute pointer refers to
//  glBindBuffer( GL_ARRAY_BUFFER, vbo );
//  // "attribute #0 is created from every 3 variables in the above buffer, of type
//  // float (i.e. make me vec3s)"
//  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

//  /* here we copy the shader strings into GL shaders, and compile them. we
//      then create an executable shader 'program' and attach both of the compiled
//              shaders. we link this, which matches the outputs of the vertex shader to
//      the inputs of the fragment shader, etc. and it is then ready to use */
////  vert_shader = glCreateShader( GL_VERTEX_SHADER );
////  glShaderSource( vert_shader, 1, &vertex_shader, NULL );
////  glCompileShader( vert_shader );
////  frag_shader = glCreateShader( GL_FRAGMENT_SHADER );
////  glShaderSource( frag_shader, 1, &fragment_shader, NULL );
////  glCompileShader( frag_shader );
////  shader_programme = glCreateProgram();
////  glAttachShader( shader_programme, frag_shader );
////  glAttachShader( shader_programme, vert_shader );
////  glLinkProgram( shader_programme );

//  /* this loop clears the drawing surface, then draws the geometry described
//              by the VAO onto the drawing surface. we 'poll events' to see if the window
//      was closed, etc. finally, we 'swap the buffers' which displays our drawing
//              surface onto the view area. we use a double-buffering system which means
//              that we have a 'currently displayed' surface, and 'currently being drawn'
//              surface. hence the 'swap' idea. in a single-buffering system we would see
//              stuff being drawn one-after-the-other */
//  while ( !glfwWindowShouldClose( window ) ) {
//    /* wipe the drawing surface clear */
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
////    glUseProgram( shader_programme );
//    shader.use();
//    glBindVertexArray( vao );
//    /* draw points 0-3 from the currently bound VAO with current in-use shader */
//    glDrawArrays( GL_TRIANGLES, 0, 3 );
//    /* update other events like input handling */
//    glfwPollEvents();
//    /* put the stuff we've been drawing onto the display */
//    glfwSwapBuffers( window );
//  }

//  /* close GL context and any other GLFW resources */
//  glfwTerminate();
//  return 0;
//}
