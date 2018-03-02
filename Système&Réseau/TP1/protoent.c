/* protoent.c:
 *  Linux Socket Programming by Example - Warren Gray
 * Example getprotoent(3) program:
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
  
int  main(int argc,char **argv) {
  int x;
  struct servent *pp;
  
  for (;;) {

    errno = 0;
    if ( !(pp = getservent()) )
      break;
    
    printf("%s:\n"
	   "\tProtocol: %s\n"
	   "\tAliases:  ",
	   pp->s_name,
	   pp->s_proto);

    for ( x=0; pp->s_aliases[x] != NULL; ++x )
      printf("%s ",pp->s_aliases[x]);
    putchar('\n');
  }
  
  /* Ce qui suit un peu pointu ... pour savoir pourquoi il 
     faudrait regarde dans la reference citee dans le 
     prologue.
     Donc pour l'instant on ignore ce qui suit ...
  */
  if ( errno != 0 && errno != ENOENT ) /* For RH-6.0 */
    fprintf(stderr, "%s: getprotoent(3) %d\n",
	    strerror(errno),errno);
  
  return 0;
 }
