#include <stdio.h>
#include <string.h>

#include <hb.h>

#define TTF_DEFAULT_FILE "../fonts/STIXTwoText_wght.ttf"

// argv[0] = progname
// argv[1] = ttf_file
// argv[2] = face_idx
// argv[3] = instance_idx

int
main(int argc, char *argv[]) {
  printf("HarfBuzz compile version v%d.%d.%d\n", HB_VERSION_MAJOR, HB_VERSION_MINOR, HB_VERSION_MICRO);

  unsigned int major, minor, micro;

  hb_version(&major, &minor, &micro);

  printf("HarfBuzz runtime version v%d.%d.%d\n\n", major, minor, micro);

  // char *ttf_file = argc > 1 ? argv[1] : TTF_DEFAULT_FILE;

  // hb_blob_t *blob = hb_blob_create_from_file(ttf_file);
  // 
  // if (blob)
  //   printf("Successful?\n");
  // else
  //   printf("Not successful?\n");

  return 0;
}
