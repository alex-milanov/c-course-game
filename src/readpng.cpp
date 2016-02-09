/* Read PNG
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


#include <png.h>
#include <stdio.h>
#include <stdlib.h>


unsigned char *
read_png (char *filename, int *width, int *height)
{
  FILE *png_file = fopen (filename, "rb");

  if (!png_file)
    {
      return 0;
    }

  png_byte header[8];
  fread (header, 1, sizeof (header), png_file);

  if (png_sig_cmp (header, 0, 8))
    {
      fprintf (stderr, "error: Invalid PNG file.\n");
      return 0;
    }

  png_structp png_struct_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop png_info_ptr = png_create_info_struct (png_struct_ptr);

  if (setjmp (png_jmpbuf (png_struct_ptr)))
    {
      fprintf (stderr, "error: Error during PNG read.\n");
      return 0;
    }

  png_init_io (png_struct_ptr, png_file);
  png_set_sig_bytes (png_struct_ptr, 8);
  png_read_info (png_struct_ptr, png_info_ptr);

  png_uint_32 rows = png_get_image_height (png_struct_ptr, png_info_ptr);
  png_byte color_type = png_get_color_type (png_struct_ptr, png_info_ptr);
  png_byte bit_depth = png_get_bit_depth (png_struct_ptr, png_info_ptr);

  if (color_type != PNG_COLOR_TYPE_RGB_ALPHA || bit_depth != 8)
    {
      fprintf (stderr, "error: Unsupported PNG format.\n");
      return 0;
    }

  *width = png_get_image_width (png_struct_ptr, png_info_ptr);
  *height = rows;

  png_uint_32 rowbytes = png_get_rowbytes (png_struct_ptr, png_info_ptr);
  unsigned char *bitmap = (unsigned char *) malloc (rowbytes * rows);
  unsigned char *bitmap_p = bitmap;
  png_bytep image_rows[rows];

  {
    png_uint_32 row_index;
    for (row_index = 0;
         row_index < rows;
         ++row_index)
      {
        image_rows[row_index] = bitmap_p;
        bitmap_p += rowbytes;
      }
  }

  png_read_image (png_struct_ptr, image_rows);
  fclose (png_file);

  return bitmap;
}
