#include <stdio.h>

#include "reverse.h"

void print_byte_array(char *str, int length);

void print_byte_array(char *str, int length) {
  for (int i = 0; i < length; i++) {
    printf("0x%0.2x ", *(str + i));
  }
  printf("\n");
  printf("\"");
  for (int i = 0; i < length; i++) {
    printf("%c", *(str + i));
  }
  printf("\"\n");
}

int main(int argc, char **argv) {
  printf("Ben Andre Hello, world!\n");

  char str[17] = "This is a string.";
  char numbers[18] = "some NUMmbers12345";
  char does_it_reverse[30] = "Does it reverse \n\0\t correctly?";

  int length = 17;
  printf("Before:\n");
  print_byte_array(str, length);
  reverse(str, length);
  printf("After:\n");
  print_byte_array(str, length);

  length = 18;
  printf("Before:\n");
  print_byte_array(numbers, length);
  reverse(numbers, length);
  printf("After:\n");
  print_byte_array(numbers, length);

  length = 30;
  printf("Before:\n");
  print_byte_array(does_it_reverse, length);
  reverse(does_it_reverse, length);
  printf("After:\n");
  print_byte_array(does_it_reverse, length);

  
  return 0;
}
