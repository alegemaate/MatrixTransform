/*
 * TRANSFORMATIONS FOR LIN ALG
 * ALLAN LEGEMAATE
 * 21/11/2016
 * A SIMPLE PROGRAM THAT TRANSFORMS AN IMAGE
 * ACCORDING TO USER PARAMETERS
 */

// Libs
#include <allegro.h>
#include <stdio.h>

// Graphics buffer
BITMAP *buffer;

// Grid view
int grid_h_spacing;
int grid_v_spacing;
int grid_h_scale;
int grid_v_scale;

// A single R2 point
typedef struct{
  float x;
  float y;
} _point;

// A line made of 2 points
typedef struct{
  _point points[2];
} _line;

// A box made of 4 points
typedef struct{
  _point points[4];
} _box;

// Text boxes
typedef struct{
  float x;
  float y;
  float w;
  float h;
  int length;
  char value[10];
  char* label;
  int cursor_loc;
} text_box;

text_box text_boxes[7];
text_box* selected_box;

// Buttons
typedef struct{
  float x;
  float y;
  float w;
  float h;
  char* label;
} button;

button button_run;
button button_reset;
button button_plus;
button button_minus;

// A box to transform
_box my_box;

// Init
void init(){
  // Allegro
  allegro_init();
  install_keyboard();
  install_mouse();

  // Graphics
  set_color_depth( 32);
  set_gfx_mode( GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Spacing
  grid_h_spacing = 40;
  grid_v_spacing = 40;
  grid_h_scale = 40;
  grid_v_scale = 40;

  // Box
  my_box.points[0].x = 1;
  my_box.points[0].y = 1;
  my_box.points[1].x = 1;
  my_box.points[1].y = -1;
  my_box.points[2].x = -1;
  my_box.points[2].y = -1;
  my_box.points[3].x = -1;
  my_box.points[3].y = 1;

  // Text box
  int i;
  for( i = 0; i < 4; i++){
    text_boxes[i].w = 80;
    text_boxes[i].h = 20;
    text_boxes[i].x = (i%2) * 90 + 10;
    text_boxes[i].y = SCREEN_H - 40 - (i < 2) * 40;
    text_boxes[i].length = 10;
    text_boxes[i].cursor_loc = 1;
    itoa(0, text_boxes[i].value, 10);
  }
  selected_box = &text_boxes[0];

  text_boxes[0].label = "A";
  text_boxes[1].label = "B";
  text_boxes[2].label = "C";
  text_boxes[3].label = "D";

  // Buttons
  button_run.x = 200;
  button_run.y = SCREEN_H - 80;
  button_run.w = 60;
  button_run.h = 20;
  button_run.label = "Run";

  button_reset.x = 200;
  button_reset.y = SCREEN_H - 40;
  button_reset.w = 60;
  button_reset.h = 20;
  button_reset.label = "Reset";

  button_plus.x = 300;
  button_plus.y = SCREEN_H - 40;
  button_plus.w = 20;
  button_plus.h = 20;
  button_plus.label = "+";

  button_minus.x = 300;
  button_minus.y = SCREEN_H - 80;
  button_minus.w = 20;
  button_minus.h = 20;
  button_minus.label = "-";
}

// Update
void update(){
  // Buttons
  if( mouse_b & 1){
    // Run
    if( mouse_x > button_run.x && mouse_x < button_run.x + button_run.w &&
        mouse_y > button_run.y && mouse_y < button_run.y + button_run.h){
      // Holds values
      float t_a, t_b, t_c, t_d;

      // Get values from text boxes
      t_a = strtod(text_boxes[0].value,NULL);
      t_b = strtod(text_boxes[1].value,NULL);
      t_c = strtod(text_boxes[2].value,NULL);
      t_d = strtod(text_boxes[3].value,NULL);

      // Transform it
      int i;
      for( i = 0; i < 4; i++){
        float new_x = 0;
        float new_y = 0;

        // Get new position
        new_x = t_a * my_box.points[i].x + t_b * my_box.points[i].y;
        new_y = t_c * my_box.points[i].x + t_d * my_box.points[i].y;

        // Assign new position
        my_box.points[i].x = new_x;
        my_box.points[i].y = new_y;
      }
    }

    // Reset button
    else if( mouse_x > button_reset.x && mouse_x < button_reset.x + button_reset.w &&
        mouse_y > button_reset.y && mouse_y < button_reset.y + button_reset.h){
      // Box
      my_box.points[0].x = 1;
      my_box.points[0].y = 1;
      my_box.points[1].x = 1;
      my_box.points[1].y = -1;
      my_box.points[2].x = -1;
      my_box.points[2].y = -1;
      my_box.points[3].x = -1;
      my_box.points[3].y = 1;
    }

    // Plus button
    else if( mouse_x > button_plus.x && mouse_x < button_plus.x + button_plus.w &&
        mouse_y > button_plus.y && mouse_y < button_plus.y + button_plus.h && grid_h_scale < 80){
      // Box
      grid_h_scale *= 2;
      grid_v_scale = grid_h_spacing = grid_v_spacing = grid_h_scale;
    }

    // Minus button
    else if( mouse_x > button_minus.x && mouse_x < button_minus.x + button_minus.w &&
        mouse_y > button_minus.y && mouse_y < button_minus.y + button_minus.h && grid_h_scale > 5){
      // Box
      grid_h_scale /= 2;
      grid_v_scale = grid_h_spacing = grid_v_spacing = grid_h_scale;
    }

    // Select box
    else{
      int i;
      for( i = 0; i < 4; i++){
        if( mouse_x > text_boxes[i].x && mouse_x < text_boxes[i].x + text_boxes[i].w &&
            mouse_y > text_boxes[i].y && mouse_y < text_boxes[i].y + text_boxes[i].h){
          selected_box = &text_boxes[i];
          break;
        }
      }
    }

    // Wait until released
    while( mouse_b & 1){}
  }

  // Get input
  if( keypressed()){
    int the_key = readkey();
    // Assure input is a number (or decimal/negative)
    if(((the_key >> 8) >= KEY_0 && (the_key >> 8) <= KEY_9) || (the_key >> 8) == KEY_STOP || (the_key >> 8) == KEY_MINUS){
      // Make sure theres room
      if( selected_box -> cursor_loc < selected_box -> length){
        selected_box -> value[selected_box -> cursor_loc] = the_key & 0xff;
        selected_box -> cursor_loc += 1;
      }
    }
    // Back space
    if((the_key >> 8) == KEY_BACKSPACE){
      // Make sure theres items in box
      if( selected_box -> cursor_loc > 0){
        selected_box -> value[selected_box -> cursor_loc - 1] = 0;
        selected_box -> cursor_loc -= 1;
      }
    }
  }
}

// Draw graphics
void draw(){
  // Clear buffer
  clear_to_color( buffer, 0xFFFFFF);

  // Draw grid
  // Vertical
  int i;
  for( i = 0; i < SCREEN_W; i += grid_v_spacing)
    line( buffer, i, 0, i, SCREEN_H, 0xBBBBBB);
  // Horizontal
  for( i = 0; i < SCREEN_H; i += grid_h_spacing)
    line( buffer, 0, i, SCREEN_W, i, 0xBBBBBB);

  // Draw axis
  line( buffer, SCREEN_W/2, 0, SCREEN_W/2, SCREEN_H, 0x000000);
  line( buffer, 0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, 0x000000);

  // Draw the box
  for( i = 0; i < 4; i++){
    line( buffer,
          (my_box.points[ i     ].x * grid_h_scale) + SCREEN_W/2, (my_box.points[ i     ].y * grid_v_scale)*-1 + SCREEN_H/2,
          (my_box.points[(i+1)%4].x * grid_h_scale) + SCREEN_W/2, (my_box.points[(i+1)%4].y * grid_v_scale)*-1 + SCREEN_H/2, 0xFF0000);
    // Point
    textprintf_centre_ex( buffer, font, (my_box.points[ i     ].x * grid_h_scale) + SCREEN_W/2,
                  (my_box.points[ i     ].y * grid_v_scale)*-1 + SCREEN_H/2
                  , 0x000000, -1, "(%.1f,%.1f)", my_box.points[i].x, my_box.points[i].y);
  }

  // Draw a menu
  rectfill( buffer, 0, SCREEN_H - 100, SCREEN_W, SCREEN_H, 0xFFFFFF);
  line( buffer, 0, SCREEN_H - 100, SCREEN_W, SCREEN_H - 100, 0x666666);

  // Draw text boxes
  for( i = 0; i < 4; i++){
    rect( buffer, text_boxes[i].x, text_boxes[i].y, text_boxes[i].x + text_boxes[i].w, text_boxes[i].y + text_boxes[i].h, 0x444444);
    textprintf_ex( buffer, font, text_boxes[i].x + 1, text_boxes[i].y - 10, 0x000000, -1, "%s", text_boxes[i].label);
    textprintf_ex( buffer, font, text_boxes[i].x + 4, text_boxes[i].y + 7, 0x000000, -1, "%s", text_boxes[i].value);
  }

  // Draw border around selected box
  rect( buffer, selected_box -> x, selected_box -> y, selected_box -> x + selected_box -> w, selected_box -> y + selected_box -> h, 0x008800);
  // tiny cursor
  line( buffer, selected_box -> x + selected_box -> cursor_loc * text_length( font, "0") + 5, selected_box -> y + 5,
                selected_box -> x + selected_box -> cursor_loc * text_length( font, "0") + 5, selected_box -> y + selected_box -> h - 5, 0x000000);

  // Draw buttons
  // Run
  rect( buffer, button_run.x, button_run.y, button_run.x + button_run.w, button_run.y + button_run.h, 0x444444);
  textprintf_ex( buffer, font, button_run.x + 4, button_run.y + 7, 0x000000, -1, "%s", button_run.label);
  // Reset
  rect( buffer, button_reset.x, button_reset.y, button_reset.x + button_reset.w, button_reset.y + button_reset.h, 0x444444);
  textprintf_ex( buffer, font, button_reset.x + 4, button_reset.y + 7, 0x000000, -1, "%s", button_reset.label );
  // Plus
  rect( buffer, button_plus.x, button_plus.y, button_plus.x + button_plus.w, button_plus.y + button_plus.h, 0x444444);
  textprintf_ex( buffer, font, button_plus.x + 4, button_plus.y + 7, 0x000000, -1, "%s", button_plus.label );
  // Minus
  rect( buffer, button_minus.x, button_minus.y, button_minus.x + button_minus.w, button_minus.y + button_minus.h, 0x444444);
  textprintf_ex( buffer, font, button_minus.x + 4, button_minus.y + 7, 0x000000, -1, "%s", button_minus.label );

  // Text
  textprintf_ex( buffer, font, SCREEN_W - (text_length( font, "Allan Legemaate") + 20), SCREEN_H - 20, 0x777777, -1, "Allan Legemaate");

  // Draw mouse
  ellipsefill( buffer, mouse_x, mouse_y, 3, 3, 0xFFFFFF);
  ellipse( buffer, mouse_x, mouse_y, 3, 3, 0x000000);

  // Draw buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}

// Main
int main(){
  // INIT PROGRAM
  init();

  // LOOP
  while( !key[KEY_ESC]){
    draw();
    update();
  }

  // END
  return 0;
}
END_OF_MAIN();
