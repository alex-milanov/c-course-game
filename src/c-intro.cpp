/* C Intro
 *
 * Copyright (C) 2016 LibTec, Alex Milanov
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

#include <math.h>

#include "engine.cpp"

#define GAME_WIDTH 640
#define GAME_HEIGHT 480
#define ENTITIES_COUNT 25

#define FRAMES_IN_ANIM 8
#define VELOCITY_DIALATION 4

enum Direction {
  DIRECTION_UP,
  DIRECTION_LEFT,
  DIRECTION_DOWN,
  DIRECTION_RIGHT
};

struct Entity {
  float x, y;
  int direction;
  int frame;
  int velocity;
  Image *image;
};

void update_entity (Entity *entity, int bouncing) {
  if( entity->frame < FRAMES_IN_ANIM * VELOCITY_DIALATION){
    entity->frame += VELOCITY_DIALATION/entity->velocity;
  } else {
    entity->frame = 0;
  }
  switch ( entity->direction ) {
    case DIRECTION_UP:
      if( entity->y < GAME_HEIGHT ) {
        entity->y += VELOCITY_DIALATION/entity->velocity;
      } else {
        if(bouncing) {
          entity->y = GAME_HEIGHT - 1;
          entity->direction = DIRECTION_DOWN;
        } else {
          entity->y = 0;
        }
      }
      break;
    case DIRECTION_LEFT:
      if( entity->x > 0 ) {
        entity->x -= VELOCITY_DIALATION/entity->velocity;
      } else {
        if(bouncing) {
          entity->x = 0;
          entity->direction = DIRECTION_RIGHT;
        } else {
          entity->x = GAME_WIDTH-1;
        }
      }
      break;
    case DIRECTION_DOWN:
      if( entity->y > 0 ) {
        entity->y -= VELOCITY_DIALATION/entity->velocity;
      } else {
        if(bouncing) {
          entity->y = 0;
          entity->direction = DIRECTION_UP;
        } else {
          entity->y = GAME_HEIGHT-1;
        }
      }
      break;
    case DIRECTION_RIGHT:
      if( entity->x < GAME_WIDTH ) {
        entity->x += VELOCITY_DIALATION/entity->velocity;
      } else {
          if(bouncing) {
            entity->x = GAME_WIDTH - 1;
            entity->direction = DIRECTION_LEFT;
          } else {
            entity->x = 0;
          }
      }
      break;
  }
}

void draw_entity (Entity *entity) {
  draw_image (
    entity->image, entity->x, entity->y,
    64, 64,
    round((float) entity->frame / entity->velocity)*64, entity->direction*64);
}

int main (int argc, char *argv[]) {
  Window *window = new_window (GAME_WIDTH, GAME_HEIGHT, "C Intro");

  Entity entities[ENTITIES_COUNT];
  Entity *hero = new Entity;

  Image *npc_image = load_image ("assets/mage-walking.png");
  Image *hero_image = load_image ("assets/baldric-walking.png");

  for (int entity_index = 0;
       entity_index < ENTITIES_COUNT;
       ++entity_index) {
    Entity *entity = entities + entity_index;
    entity->x = rand_range (0, GAME_WIDTH);
    entity->y = rand_range (0, GAME_HEIGHT);
    entity->direction = rand_range (0, 4);
    entity->frame = 0;
    entity->velocity = VELOCITY_DIALATION;
    entity->image = npc_image;
  }

  hero->x = GAME_WIDTH/2;
  hero->y = GAME_HEIGHT/2;
  hero->direction = DIRECTION_DOWN;
  hero->frame = 0;
  hero->velocity = VELOCITY_DIALATION;
  hero->image = hero_image;

  while (!window_should_close (window)) {
    draw_gradient (0, 0, GAME_WIDTH, GAME_HEIGHT,
                   0.2f, 0.2f, 0.2f,
                   0.4f, 0.4f, 0.2f);

    for (int entity_index = 0;
         entity_index < ENTITIES_COUNT;
         ++entity_index) {
      Entity *entity = entities + entity_index;
      update_entity (entity, 1);
      draw_entity (entity);
    }

    if (glfwGetKey( window->internal_window, GLFW_KEY_F ) == GLFW_PRESS) {
      hero->velocity = VELOCITY_DIALATION/4;
    } else if (glfwGetKey( window->internal_window, GLFW_KEY_S ) == GLFW_PRESS) {
      hero->velocity = VELOCITY_DIALATION;
    } else {
      hero->velocity = VELOCITY_DIALATION/2;
    }

    if (glfwGetKey( window->internal_window, GLFW_KEY_UP ) == GLFW_PRESS){
      hero->direction = DIRECTION_UP;
      update_entity (hero, 0);
    } else if (glfwGetKey( window->internal_window, GLFW_KEY_LEFT ) == GLFW_PRESS){
      hero->direction = DIRECTION_LEFT;
      update_entity (hero, 0);
    } else if (glfwGetKey( window->internal_window, GLFW_KEY_DOWN ) == GLFW_PRESS){
      hero->direction = DIRECTION_DOWN;
      update_entity (hero, 0);
    } else if (glfwGetKey( window->internal_window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
      hero->direction = DIRECTION_RIGHT;
      update_entity (hero, 0);
    }

    draw_entity (hero);

    update_window (window);
  }

  close_window (window);

  return 0;
}
