#include <stdio.h>
#include <limits.h>
#include "itostr.h"

int main(void)
{
  size_t length = 0;

  char buffer[80] = { 0 };
  length = signedtostr(SIGNEDC(USHRT_MAX), sizeof(short), buffer, sizeof(buffer), 10);
  printf("length:%u\tstring:%s\n", (unsigned)length, buffer);

  length = unsignedtostr(UNSIGNEDC(SHRT_MIN), sizeof(unsigned short), buffer, sizeof(buffer), 10);
  printf("length:%u\tstring:%s\n", (unsigned)length, buffer);

  /* OR
  wchar_t wbuffer[80] = { 0 };
  length = signedtowcs(SIGNEDC(USHRT_MAX), sizeof(unsigned short), wbuffer, sizeof(wbuffer) / sizeof(wbuffer[0]), 10);
  wprintf(L"length:%u\tstring:%s\n", (unsigned)length, wbuffer);

  length = unsignedtowcs(UNSIGNEDC(SHRT_MIN), sizeof(short), wbuffer, sizeof(wbuffer) / sizeof(wbuffer[0]), 10);
  wprintf(L"length:%u\tstring:%s\n", (unsigned)length, wbuffer);
  */

  return 0;
}
