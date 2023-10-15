#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define MAX_IBUF 64

enum {
  NUM_DEC = 10, NUM_OCT = 8,  NUM_HEX = 16,
};

/* Functions below implement writing integer or string 
* to certain buffer 
*/

/* Write an interger into buffer according to type we wanna convert */
static int _writeI(char *out, const int num, size_t *n, uint32_t type) {
  long int _num = num;
  /* This should be enough, or we consider it as overflow and cut it down */
  char buf[MAX_IBUF];
  uint32_t offset = 0;  
  if (_num == 0) buf[offset++] = '0';
  else 
    while(_num) {
      buf[offset++] = _num % type > 9? 'a' + _num % type - 10: _num % type;
      _num /= type;
    }     
  int i;
  for(i = offset - 1; i > 0 && *n > 0; i--, (*n)--)  
    *(out + offset - 1 - i) = buf[i];
  return i - offset - 1; 
}

/* Write a string to buffer */
static int _writeS(char *out, const char *buffer, size_t *n, const int len) {
  uint32_t offset;
  for (offset = 0; offset < len && *n > 0; offset++, (*n)--) {
    *(out + offset) = *(buffer + offset);
  }
  return offset;
}


int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return vsnprintf(out, -1, fmt, ap);
}

/* Trivial implement , We didn`t call _vsprintf_internal 
 * The behavior of copy we implement is just byte copy 
 */
int sprintf(char *out, const char *fmt, ...) {
  va_list ap;  
  va_start(ap, fmt);
  int ret = vsprintf(out, fmt, ap);

  va_end(ap);
  return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  uint32_t offset = 0, len;
  for (const char *p = fmt; *p != '\0'; p++) {
    if (*p == '%') {  
      p++;  
			if (*p == 'd') {
        /* Some functions should be implemented here 
        * which returns a value: len, it symbolizes the length we write into 
        * the buffer
        */
        offset += _writeI(out, va_arg(ap, int), &n, NUM_DEC);
			}
			else if(*p == 's') {
        char *buf = va_arg(ap, char *);  
				len = strlen(buf);
        offset += _writeS(out, buf, &n, len);
			}
			else if(*p == 'c') {
        char buf[8];
        *buf = (char)va_arg(ap, int);  
        offset += _writeS(out, buf, &n, 1);
		  }
			else {
				char *buf = "%%";
        offset += _writeS(out, buf, &n, 2);
		  }
    }  
		else {  
			char buf[32];
			buf[0] = *p;
      offset += _writeS(out, buf, &n, 1);
    }  
  }  
  va_end(ap);
  return offset;
}

#endif
