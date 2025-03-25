#include <stdio.h>
#include <string.h>

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

#define TTF_DEFAULT_FILE "../fonts/STIXTwoText_wght.ttf"

FT_Error print_SfntName(FT_Face face, FT_UInt strid) {
  FT_UInt name_count = FT_Get_Sfnt_Name_Count(face);

  for (int i = 0; i < name_count; i++) {
    FT_SfntName aname;

    FT_Error error = FT_Get_Sfnt_Name(face, i, &aname);

    if (error) {
      printf("<unknown>");

      return error;
    } else {
      if (aname.platform_id == TT_PLATFORM_MICROSOFT &&
          aname.encoding_id == TT_MS_ID_UNICODE_CS &&
          aname.name_id == strid) {
        for (int i = 0; i < aname.string_len; i++) {
          if (aname.string[i] > 0) {
            printf("%c", aname.string[i]);
          }
        }

        break;
      }
    }
  }

  return 0;
}

void print_simple_stuff(FT_Face face) {
  printf("    family name: %s\n", face->family_name);
  printf("    style name:  %s\n", face->style_name);

  printf("    num_faces    = %ld\n", face->num_faces);

  FT_Long f_index = face->face_index;

  printf("    face_index   = %ld.%ld\n", f_index >> 16, f_index & 0xFFFF);
  printf("    face_flags   = 0x%05lx\n", face->face_flags);
  printf("    style_flags  = 0x%04lx\n", face->style_flags & 0xFFFF);
  printf("    num_glyphs   = %ld\n", face->num_glyphs);
  printf("    num_charmaps = %d\n", face->num_charmaps);

  printf("\n");

  return;
}

void print_variations(FT_Face face, FT_Library library) {
  FT_MM_Var* amaster;

  FT_Error error = FT_Get_MM_Var(face, &amaster);

  if (error) {
    printf("*** FT_Get_MM_Var failed (%d)\n", error);

    return;
  }

  FT_UInt num_axes = amaster->num_axis;

  FT_Fixed design_coords[num_axes];

  error = FT_Get_Var_Design_Coordinates(face, num_axes, design_coords);

  if (error) {
    printf("*** FT_Get_Var_Design_Coordinates failed (%d)\n", error);

    return;
  }

  FT_Fixed blend_coords[num_axes];

  error = FT_Get_MM_Blend_Coordinates(face, num_axes, blend_coords);

  if (error) {
    printf("*** FT_Get_MM_Blend_Coordinates failed (%d)\n", error);

    return;
  }

  for (int i = 0; i < num_axes; i++) {
    printf("Var axis %d = %ld (%.2f)\n",
           i,
           design_coords[i] >> 16,   // ignore the fractional part
           blend_coords[i]/65536.0);
  }

  printf("\n");

  if (num_axes > 0) {
    printf("Variation %s:\n\n", num_axes == 1 ? "axis" : "axes");

    for (int i = 0; i < num_axes; i++) {
      FT_Var_Axis this_axis = amaster->axis[i];

      printf("Axis %d: %s (", i, this_axis.name);
      print_SfntName(face, this_axis.strid);
      printf(") [%ld, %ld] (default=%ld)\n",
             this_axis.minimum >> 16,
             this_axis.maximum >> 16,
             this_axis.def >> 16);

      // printf("    tag = '%lu'\n", this_axis.tag);
    }

    if (amaster->num_namedstyles > 0) {
      printf("\n");

      FT_Var_Named_Style *named_style = amaster->namedstyle;

      for (int i = 0; i < amaster->num_namedstyles; i++) {
        FT_Var_Named_Style this_style = named_style[i];

        printf("Named Style %d: ", i + 1);

        print_SfntName(face, this_style.strid);

        printf(" [");

        for (int i = 0; i < num_axes; i++) {
          FT_Fixed coords = this_style.coords[i];

          printf("%s=%ld", amaster->axis[i].name, coords >> 16);

          if (i < num_axes - 1) {
            printf(", ");
          }
        }

        printf("]\n");

      }
    }
  }

  FT_Done_MM_Var(library, amaster);

  return;
}

// argv[0] = progname
// argv[1] = ttf_file
// argv[2] = face_idx
// argv[3] = instance_idx

int
main(int argc, char *argv[]) {
  char *ttf_file = argc > 1 ? argv[1] : TTF_DEFAULT_FILE;

  FT_Long face_idx = 0;

  if (argc > 2) {
    face_idx = strtol(argv[2], NULL, 10);
  }

  FT_Long instance_idx = 0;

  if (argc > 3) {
    instance_idx = strtol(argv[3], NULL, 10);
  }

  FT_Library library;

  FT_Error error = FT_Init_FreeType(&library);

  if (error) {
    printf("FT_Init_Freetype failed\n");

    return 1;
  }

  FT_Int amajor, aminor, apatch;

  FT_Library_Version(library, &amajor, &aminor, &apatch);

  printf("FreeType compile version v%d.%d.%d\n", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);

  printf("FreeType v%d.%d.%d\n\n", amajor, aminor, apatch);

  printf("Font file %s:\n\n", ttf_file);

  FT_Face face;

  // FT_Long face_index = (instance_idx << 16) + face_idx;

  FT_Long face_index = face_idx;

  if (error = FT_New_Face(library, ttf_file, face_index, &face)) {
    printf("FT_New_Face (%ld) failed: %d\n", face_index, error);

    return 1;
  }

  FT_Set_Named_Instance(face, instance_idx);

  // FT_Fixed coords[1] = { 550 << 16 };

  // FT_Set_Var_Design_Coordinates(face, 1, coords);

  print_simple_stuff(face);

  print_variations(face, library);

  FT_Done_Face(face);

  FT_Done_FreeType(library);

  return 0;
}
