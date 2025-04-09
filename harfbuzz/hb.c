#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hb.h>
#include <hb-ot.h>

#define TTF_DEFAULT_FILE "../fonts/STIXTwoText_wght.ttf"

void print_harfbuzz_version() {
  printf("HarfBuzz compile-time v%d.%d.%d, runtime v%s\n\n",
         HB_VERSION_MAJOR, HB_VERSION_MINOR, HB_VERSION_MICRO,
         hb_version_string());

  return;
}

void print_face_name(hb_face_t *face, hb_ot_name_id_t name_id) {
  unsigned int text_size = 256;

  char text[text_size];

  unsigned len = hb_ot_name_get_utf8(face,
                                     name_id,
                                     HB_LANGUAGE_INVALID,
                                     &text_size,
                                     text);

  if (len > 0)
    printf("%s", text);

  return;
}

void print_variation_data(hb_face_t *face) {
  if (! hb_ot_var_has_data(face))
    return;

  unsigned num_axes = hb_ot_var_get_axis_count(face);

  printf("Variation axes: %u\n", num_axes);

  hb_ot_var_axis_info_t axes_array[num_axes];

  hb_ot_var_get_axis_infos(face, 0, &num_axes, axes_array);

  for (int i = 0; i < num_axes; i++) {
    hb_ot_var_axis_info_t axis = axes_array[i];

    printf("    Axis %u: ", axis.axis_index);
    print_face_name(face, axis.name_id);
    printf(" [%.f, %.f] (default=%.f)\n", axis.min_value, axis.max_value, axis.default_value);
  }

  printf("\n");

  unsigned num_instances = hb_ot_var_get_named_instance_count(face);

  printf("Named instances: %u\n", num_instances);

  for (int i = 0; i < num_instances; i++) {
    printf("    Named Style %d: ", i);

    hb_ot_name_id_t name_id = hb_ot_var_named_instance_get_subfamily_name_id(face, i);

    print_face_name(face, name_id);

    printf(" [");

    unsigned coords_length = num_axes;

    float coords[coords_length];

    hb_ot_var_named_instance_get_design_coords(face, i, &coords_length, coords);

    for (int i = 0; i < coords_length; i++) {
      print_face_name(face, axes_array[i].name_id);

      printf("=%.f", coords[i]);

      if (i < coords_length - 1)
        printf(", ");
    }

    printf("]");
    
    printf("\n");
  }

  printf("\n");

  return;
}

// argv[0] = progname
// argv[1] = ttf_file
// argv[2] = face_idx
// argv[3] = instance_idx

int
main(int argc, char *argv[]) {
  print_harfbuzz_version();

  char *ttf_file    = argc > 1 ? argv[1] : TTF_DEFAULT_FILE;

  unsigned face_idx = argc > 2 ? strtol(argv[2], NULL, 10) : 0;

  hb_blob_t *blob = hb_blob_create_from_file_or_fail(ttf_file);

  if (! blob) {
    printf("hb_blob_create_from_file_or_fail() failed\n");

    return 1;
  }

  unsigned num_faces = hb_face_count(blob);

  printf("Font file %s [num_faces=%u]\n\n", ttf_file, num_faces);

  if (face_idx > num_faces - 1) {
    printf("You requested face %d, but there are only %d faces\n", face_idx, num_faces);
    printf("Resetting face_idx to %d\n", face_idx = num_faces - 1);
  }

  hb_face_t *face = hb_face_create_or_fail(blob, face_idx);

  if (! face) {
    printf("hb_face_create_or_fail() failed\n");

    return 1;
  }

  printf("Font family: ");
  print_face_name(face, HB_OT_NAME_ID_FONT_FAMILY);
  printf("\n");

  printf("Font subfamily: ");
  print_face_name(face, HB_OT_NAME_ID_FONT_SUBFAMILY);
  printf("\n");

  printf("\n");

  print_variation_data(face);

  hb_font_t *font = hb_font_create(face);

  if (! font) {
    printf("hb_font_create() failed\n");

    return 1;
  }

  hb_font_destroy(font);

  hb_face_destroy(face);

  hb_blob_destroy(blob);

  return 0;
}
