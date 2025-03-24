#include <stdio.h>
#include <string.h>

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

#define TTF_DEFAULT_FILE "../STIXTwoText_wght.ttf"

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

// progname ttf_file face_idx instance_idx
//     0       1         2         3

int main(int argc, char *argv[]) {
  // for (int i = 0; i < argc; i++)
  //   printf("%d: %s\n", i, argv[i]);

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

  printf("Freetype v%d.%d.%d\n\n", amajor, aminor, apatch);

  FT_Face face;

  FT_Long face_index = (instance_idx << 16) + face_idx;

  if (error = FT_New_Face(library, ttf_file, face_index, &face)) {
    printf("FT_New_Face failed\n");

    return 1;
  }

  printf("Font file %s:\n", ttf_file);

  printf("    family name: %s\n", face->family_name);
  printf("    style name:  %s\n", face->style_name);

  printf("    num_faces    = %ld\n", face->num_faces);

  FT_Long f_index = face->face_index;

  printf("    face_index   = %ld.%ld\n", f_index >> 16, f_index & 0xFFFF);
  printf("    face_flags   = %ld\n", face->face_flags);
  printf("    style_flags  = %ld\n", face->style_flags);
  printf("    num_glyphs   = %ld\n", face->num_glyphs);
  printf("    num_charmaps = %d\n", face->num_charmaps);

  FT_MM_Var* amaster;

  error = FT_Get_MM_Var(face, &amaster);

  printf("\n");

  if (error)
    printf("*** FT_Get_MM_Var failed\n");
  else {
    if (amaster->num_axis > 0) {
      printf("Variation %s:\n", amaster->num_axis == 1 ? "axis" : "axes");

      for (int i = 0; i < amaster->num_axis; i++) {
        FT_Var_Axis this_axis = amaster->axis[i];

        printf("\nAxis %d\n", i);

        printf("    %s [%ld, %ld]\n",
               this_axis.name,
               this_axis.minimum >> 16,
               this_axis.maximum >> 16);

        printf("    Default = '%ld'\n", this_axis.def >> 16);
        printf("    tag = '%lu'\n", this_axis.tag);
        printf("    strid = '%u'\n", this_axis.strid);
      }

      if (amaster->num_namedstyles > 0) {
        printf("\n");

        FT_Var_Named_Style *named_style = amaster->namedstyle;

        for (int i = 0; i < amaster->num_namedstyles; i++) {
          FT_Var_Named_Style this_style = named_style[i];

          printf("Named Style %d: ", i + 1);

          print_SfntName(face, this_style.strid);

          printf(" [");

          for (int i = 0; i < amaster->num_axis; i++) {
            FT_Fixed coords = this_style.coords[i];

            printf("%s=%ld", amaster->axis[i].name, coords >> 16);

            if (i < amaster->num_axis - 1) {
              printf(", ");
            }
          }

          printf("]\n");

        }
      }
    }

    // FT_Done_MM_Var(library, amaster);

    // free(amaster); // ??
  }

  FT_Done_Face(face);

  FT_Done_FreeType(library);

  return 0;
}
