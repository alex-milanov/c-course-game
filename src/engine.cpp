/* Intro Game Engine
 *
 * Copyright (C) 2016 LibTec
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "readpng.cpp"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

struct Image {
  GLuint id;
  int width;
  int height;
};

struct Window {
  GLFWwindow *internal_window;
};


double rand_double (void)
{
  double result = rand () / ((double) RAND_MAX + 1);
  return result;
}


int rand_range (int min, int max)
{
  int result = ((int) (rand_double () * (max - min))) + min;
  return result;
}


Window * new_window (int width, int height, char *title)
{
  if (!glfwInit ())
    {
      return 0;
    }

  GLFWwindow *internal_window = glfwCreateWindow (width, height, title, 0, 0);

  if (!internal_window)
    {
      glfwTerminate ();
      return 0;
    }

  glfwMakeContextCurrent (internal_window);

  glMatrixMode (GL_PROJECTION);
  glOrtho (0, width, 0, height, 1.0f, -1.0f);

  // glClearColor (0.6f, 0.2f, 0.2f, 1.0f);
  // glClear (GL_COLOR_BUFFER_BIT);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Window *window = (Window *) malloc (sizeof (Window));
  window->internal_window = internal_window;

  return window;
}


int window_should_close (Window *window)
{
  int result = glfwWindowShouldClose (window->internal_window);
  return result;
}


void close_window (Window *window)
{
  free (window);
  glfwTerminate ();
}


void update_window (Window *window) {
  glfwSwapBuffers (window->internal_window);
  glfwPollEvents ();
  // glClear (GL_COLOR_BUFFER_BIT);
}


Image * load_image (char *filepath) {
  Image *image = (Image *) malloc (sizeof(Image));
  unsigned char *bitmap = read_png (filepath, &image->width, &image->height);

  glGenTextures (1, &image->id);
  glBindTexture (GL_TEXTURE_2D, image->id);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8,
                image->width, image->height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

  free (bitmap);

  return image;
}


void draw_image (Image *image,
            float dest_x, float dest_y,
            float width, float height,
            float src_x, float src_y) {
  float half_width = width / 2.0f;
  float half_height = height / 2.0f;

  float x1 = dest_x - half_width;
  float y1 = dest_y - half_height;
  float x2 = dest_x + half_width;
  float y2 = dest_y + half_height;

  float u1 = src_x / image->width;
  float v1 = src_y / image->height;
  float u2 = (src_x + width) / image->width;
  float v2 = (src_y + height) / image->height;

  static int counter = 0;
  if (counter++ < 10)
    {
      printf ("%f %f | %f %f\n", u1, v1, u2, v2);
    }

  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, image->id);

  glBegin (GL_TRIANGLE_STRIP);
  glTexCoord2f (u1, v2);
  glVertex3f (x1, y1, 0.0f);

  glTexCoord2f (u1, v1);
  glVertex3f (x1, y2, 0.0f);

  glTexCoord2f (u2, v2);
  glVertex3f (x2, y1, 0.0f);

  glTexCoord2f (u2, v1);
  glVertex3f (x2, y2, 0.0f);
  glEnd ();
}


void draw_gradient (float x1, float y1,
               float x2, float y2,
               float r1, float g1, float b1,
               float r2, float g2, float b2){
  glDisable (GL_TEXTURE_2D);

  glBegin (GL_TRIANGLE_STRIP);
  glColor3f (r1, g1, b1);
  glVertex2f (x1, y2);
  glVertex2f (x2, y2);
  glColor3f (r2, g2, b2);
  glVertex2f (x1, y1);
  glVertex2f (x2, y1);
  glEnd ();

  glColor3f (1.0f, 1.0f, 1.0f);
}
