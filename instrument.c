/********************************************************************
 * File: instrument.c
 *
 * Instrumentation source -- link this_function with your application, and
 *  then execute to build trace data file (trace.txt).
 *
 * Author: M. Tim Jones <mtj@mtjones.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
/* Function prototypes with attributes */
void main_constructor( void )
	__attribute__ ((no_instrument_function, constructor));

void main_destructor( void )
	__attribute__ ((no_instrument_function, destructor));

void __cyg_profile_func_enter( void *, void * ) 
	__attribute__ ((no_instrument_function));

void __cyg_profile_func_exit( void *, void * )
	__attribute__ ((no_instrument_function));

#ifdef __cplusplus
}
#endif

static FILE *fp;


void main_constructor( void )
{
  fp = fopen( "trace.txt", "w" );
  if (fp == NULL) exit(-1);
}


void main_deconstructor( void )
{
  fclose( fp );
}


void __cyg_profile_func_enter( void *this_function, void *callsite )
{
  char command_callee[100];

  sprintf(command_callee, "addr2line -e main %p", (void*)this_function);

  FILE *fp_callee = popen(command_callee, "r");

  if (fp_callee == NULL){
     perror("popen failed!");
     exit(1);
  }

  char path_linenumber_callee[200];
  fgets(path_linenumber_callee, 200, fp_callee);

  if ( strstr( path_linenumber_callee, "/usr/" ) == NULL )
     fprintf(fp, "E%p\n", (int *)this_function);
}


void __cyg_profile_func_exit( void *this_function, void *callsite )
{
  char command_callee[100];

  sprintf(command_callee, "addr2line -e main %p", (void*)this_function);

  FILE *fp_callee = popen(command_callee, "r");

  if (fp_callee == NULL){
     perror("popen failed!");
     exit(1);
  }

  char path_linenumber_callee[200];
  fgets(path_linenumber_callee, 200, fp_callee);

  if ( strstr( path_linenumber_callee, "/usr/" ) == NULL )
     fprintf(fp, "X%p\n", (int *)this_function);
}

