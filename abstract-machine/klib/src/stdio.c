#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static int int_to_str(int num, char *str) {  
	char tmp[32];
  int i = 0;  
  while (num != 0) {  
	// We consider it as overflow and just stop converting;
		if (i > 30) break;
      tmp[i++] = '0' + num % 10;
      num /= 10; 
  }  
  for (int j = 0 ; j < i; j++)	str[j] = tmp[i - 1 - j];
	str[i++] = '\0'; 
	return i;
}

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

/* Trivial implement , We didn`t call _vsprintf_internal 
 * The behavior of copy we implement is just byte copy 
 */
int sprintf(char *out, const char *fmt, ...) {
  va_list arg_list;  
  va_start(arg_list, fmt);  

  uint32_t offset = 0, len;

  for (const char *p = fmt; *p != '\0'; p++) {
		/* Here we got a if-else structure
		* Branch 'then' processes format control
		* Branch 'else' processes the behavior of printing infomation
		*/
    if (*p == '%') {  
      p++;  
			if (*p == 'd') {
				int num = va_arg(arg_list, int);  
				/* This should be enough , or we consider it as overflow */
				char buf[64];
				len = int_to_str(num , buf);
        /* Just byte copy */
        memcpy(out + offset, buf, len);
        offset += len;
			}
			else if(*p == 's') {
        char *buf = va_arg(arg_list, char *);  
				len = strlen(buf);
        memcpy(out + offset, buf, len);
        offset += len;
			}
			else if(*p == 'c') {
        char buf[8];
        *buf = (char)va_arg(arg_list, int);  
				len = 1;
        memcpy(out + offset, buf, len);
        offset += len;
		  }
			else {
				char *buf = "%%";
				len = 2;	
        memcpy(out + offset, buf, len);
        offset += len;
		  }
    }  
		else {  
			char buf[32];
			buf[0] = *p;
			len = 1;	
      memcpy(out + offset, buf, len);
      offset += len;
    }  
  }  
  va_end(arg_list);  
  return offset;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
