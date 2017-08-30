/* 
 * Simplified Assembly Loader v.0.0.1 
 * by gynvael.coldwind//vx
 * http://gynvael.coldwind.pl/
 * 
 * -- License Stuff
 *
 * Copyright (C) 2011 by Gynvael Coldwind
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * -- Usage
 *
 * Usage:
 *   asmloader <file.bin>
 *
 * What this does (in short):
 * 1. It allocated memory for Read/Write/Execute
 * 2. Loads the specified file to it
 * 3. And jumps to it (i.e. executes the binary code that was in the file)
 * +  It provides a minor API set.
 *
 * File.bin should be pure assembly code, without any headers.
 * Your code will be loaded to a random page address + 0x100.
 * In EBX/RBX register there is a pointer to an array of functions:
 *   0 exit
 *   1 putchar
 *   2 getchar
 *   3 printf
 *   4 scanf
 * You have to use the calling convention of your platform, being:
 * Windows/Linux 32-bit: cdecl (arguments to stack, you clean the stack afterwards)
 * Windows       64-bit: fastcall (Windows x64)
 * Linux         64-bit: fastcall (System V AMD64 ABI)
 * See http://en.wikipedia.org/wiki/X86_calling_conventions for details.
 *
 * Whatever you return in EAX will be what the program returns at exit.
 * However, if you decide to return, remember to backup non-volatile registers first ;)
 * Or... use exit function to exit, e.g. (32-bit example):
 *   push dword 0
 *   call [ebx+0]
 * If you neither exit nor return properly, the program will crash.
 *
 * Have fun :)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#  include <windows.h>
#elif defined(__unix__)
#  include <sys/mman.h>
#endif

#define STUB_SIZE 0x100
#define STUB_PAD  1

size_t fsize(FILE *f) {
  size_t pos, len;
  pos = ftell(f);
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, pos, SEEK_SET);
  return len;
}

const char *LoadBinary(void **dst_data, size_t *dst_size, const char *fname) {
  const char *ret = NULL;
  FILE *f = NULL;
  void *data = NULL;
  size_t size = 0;
  size_t alloc_size;
 
  f = fopen(fname, "rb");
  if(!f) {
    ret = "could not open input file";
    goto err;
  }

  size = fsize(f);
  if(size == 0) {
    ret = "file of 0 size";
    goto err;
  }

  alloc_size = STUB_SIZE + size + STUB_PAD;

#ifdef _WIN32
  data = VirtualAlloc(NULL, alloc_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else
  data = mmap(NULL, alloc_size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
#endif

#ifdef _WIN32
  if(!data) {
#else
  if(data == (void*)-1) {
    data = NULL;
#endif
    ret = "could not allocate memory";
    goto err;
  }

  if(fread((uint8_t*)data + STUB_SIZE, 1, size, f) != size) {
    ret = "read error";
    goto err;
  }

  fclose(f); f = NULL;

  memset(data, 0x90, STUB_SIZE);
#define MC(a,b) memcpy(a,b,sizeof(b)-1)

#ifdef __LP64__ 
#  error Not implemented.
#else
  MC(data, 
      "\x8B\x5C\x24\x04" // mov ebx, [esp+4]
    );
#endif

#undef MC

  // Set last byte to int3.
  ((uint8_t*)data)[size + STUB_SIZE] = 0xCC;

  *dst_size = size;
  *dst_data = data;

  return ret;

err:
  if(f) fclose(f);
#ifdef _WIN32
  if(data) VirtualFree(data, 0, MEM_RELEASE);
#else
  if(data) munmap(data, alloc_size);
#endif
  return ret;
}

int main(int argc, char **argv) {
  const char *ret;
  void       *data;
  size_t      size;
#ifdef _WIN32
  typedef int (__cdecl *func_t)(void **);
#elif defined(__unix__)
  typedef int (*func_t)(void **);
#endif
  func_t      func;
  void       *func_arr[] = { exit, putchar, getchar, printf, scanf };
  int         fret;

  puts("Simplified Assembly Loader v.0.0.1 by gynvael.coldwind//vx");

  if(argc != 2) {
    printf("usage: asmloader <file.bin>\n"
           "note : use nasm to compile .asm to .bin\n"
           "     : e.g. nasm file.asm -o file.bin\n"
           "     :      asmloader file.bin\n");
    return 1;
  }

  ret = LoadBinary(&data, &size, argv[1]);
  if(ret != NULL) {
    printf("error: %s\n", ret);
    return 2;
  }

#ifdef __LP64__
#  error Not implemented.
#else
  printf("Code loaded at 0x%.8x (%u bytes)\n",
      (uint32_t)data + STUB_SIZE, (uint32_t)size);
#endif
  fflush(stdout);

  func = (func_t)data;
  fret = func(func_arr);

#ifdef _WIN32
  VirtualFree(data, 0, MEM_RELEASE);
#else
  munmap(data, STUB_SIZE + size + STUB_PAD);
#endif

  return fret;
}
