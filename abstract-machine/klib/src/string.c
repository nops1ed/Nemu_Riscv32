#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  return memcpy (dst, src, strlen(src) + 1);
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  strcpy (dst + strlen (dst), src);
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  /* Avoid potential symbol-related issues. */
  const unsigned char *p1 = (const unsigned char *) s1;
  const unsigned char *p2 = (const unsigned char *) s2;
  unsigned char c1, c2;
  do {
    c1 = (unsigned char) *p1++;
    c2 = (unsigned char) *p2++;
    if (c1 == '\0')   return c1 - c2;
  } while (c1 == c2);
  return c1 - c2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  // TODO()
  long int dstp = (long int)s;
  while (n > 0) {
      ((byte *) dstp)[0] = c;
      dstp += 1;
      n -= 1;
  }
  return s; 
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *dstpp, const void *srcpp, size_t n) {
  unsigned long int dstp = (long int) dstpp;
  unsigned long int srcp = (long int) srcpp;
  /* Just implement byte copy */
  /* Lots of thing to be done here... */
  // TODO()
  BYTE_COPY_FWD(dstp, srcp, n);
  return dstpp;
}


int memcmp(const void *s1, const void *s2, size_t n) {
  long int srcp1 = (long int)s1;
  long int srcp2 = (long int)s2;
  long int res;
  long int a0, b0;
  /* Still lots of thing to do... */
  // TODO()
  while (n != 0) {
      a0 = ((byte *) srcp1)[0];
      b0 = ((byte *) srcp2)[0];
      srcp1 += 1;
      srcp2 += 1;
      res = a0 - b0;
      if (res != 0)  return res;
      n -= 1;
  }
  return 0;
}

#endif
