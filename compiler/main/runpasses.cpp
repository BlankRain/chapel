#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "files.h"
#include "misc.h"
#include "log.h"
#include "runpasses.h"
#include "stringutil.h"
#include "view.h"

void verify();

bool printPasses = false;

struct PassInfo {
  void (*fn)(void);
  char *name;
};

#define FIRST {NULL, NULL}
#define LAST {NULL, NULL}
#define RUN(x) {x, #x}
#include "passlist.h"


static void runPass(char *passName, void (*pass)(void)) {
  struct timeval startTime;
  struct timeval stopTime;
  struct timezone timezone;
  static int heapsize = 0;

  currentTraversal = stringcpy(passName);
  if (fdump_html) {
    gettimeofday(&startTime, &timezone);
  }
  GC_gcollect();
  if (printPasses) {
    fprintf(stderr, "%32s :", passName);
    fflush(stderr);
    gettimeofday(&startTime, &timezone);
    heapsize = GC_get_heap_size();
  }
  (*pass)();
  if (printPasses) {
    gettimeofday(&stopTime, &timezone);
    int newheapsize = GC_get_heap_size();
    fprintf(stderr, "%8.3f seconds,",  
            ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                      (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6);
    fprintf(stderr, " heap = %6dk (+%6dk)\n", newheapsize/1024, 
            (newheapsize - heapsize)/1024);
    heapsize = newheapsize;
  }

  if (fdump_html) {
    gettimeofday(&stopTime, &timezone);
    fprintf(html_index_file, "<TR><TD>");
    fprintf(html_index_file, "%s", passName);
    fprintf(html_index_file, "</TD><TD>");
    html_view();
    fprintf(html_index_file, "</TD></TR>");
    fflush(html_index_file);
  }

  verify();
}


void dump_index_header(FILE* f) {
  fprintf(f, "<HTML>\n");
  fprintf(f, "<HEAD>\n");
  fprintf(f, "<TITLE> Compilation Dump </TITLE>\n");
  fprintf(f, "<SCRIPT SRC=\"%s/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
         system_dir);
  fprintf(f, "<LINK REL=\"stylesheet\" HREF=\"%s/etc/www/mktree.css\">", 
         system_dir);
  fprintf(f, "</HEAD>\n");
  fprintf(f, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(f, "Compilation Dump<br><br></span></big></big>\n");
  fprintf(f, "<div style=\"text-align: left;\">\n\n");
}


void dump_index_footer(FILE* f) {
  fprintf(f, "</HTML>\n");
}


void runPasses(void) {
  if (fdump_html) {
    html_index_file = fopen(stringcat(log_dir, "index.html"), "w");
    dump_index_header(html_index_file);
    fprintf(html_index_file, "<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">");
  }
  PassInfo* pass = passlist+1;  // skip over FIRST
  while (pass->name != NULL) {
    runPass(pass->name, pass->fn);
    check_fatal_errors_encountered();
    pass++;
  }
  if (fdump_html) {
    fprintf(html_index_file, "</TABLE>");
    dump_index_footer(html_index_file);
    fclose(html_index_file);
  }
}
