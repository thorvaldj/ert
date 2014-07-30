#include <ert/enkf/ert_log.h>
#include <ert/enkf/enkf_defaults.h>


log_type             * logh;               /* Handle to an open log file. */

void ert_log_init_log( int log_level,const char * log_file_name,const char * user_log_file_name,bool verbose){
  logh = log_open( NULL , DEFAULT_LOG_LEVEL );

  log_set_level(logh, log_level);

  if (log_file_name && log_file_name[0] != '\0')
    log_reopen( logh , log_file_name);
  else {
    char * log_file = util_alloc_filename(NULL , user_log_file_name , DEFAULT_LOG_FILE);
    log_reopen( logh , log_file );
    free( log_file );
  }
  if (verbose)
    printf("Activity will be logged to ..............: %s \n",log_get_filename( logh ));
  log_add_message(logh , 1 , NULL , "ert configuration loaded" , false);
}

void ert_log_add_message(int message_level , FILE * dup_stream , char* message, bool free_message) {
    if(logh==NULL)
        ert_log_init_log(1,NULL,NULL,true);
    log_add_message(logh, message_level, dup_stream, message, free_message);
}

void ert_log_add_fmt_message(int message_level , FILE * dup_stream , const char * fmt , ...) {
    if (log_include_message(logh,message_level)) {
      char * message;
      va_list ap;
      va_start(ap , fmt);
      message = util_alloc_sprintf_va( fmt , ap );
      log_add_message( logh , message_level , dup_stream , message , true);
      va_end(ap);
    }
}

void ert_log_close(){
    if (log_is_open( logh ))
      log_add_message( logh , false , NULL , "Exiting ert application normally - all is fine(?)" , false);
    log_close( logh );
}

bool ert_log_is_open(){
    if(logh==NULL)
        return false;
    return log_is_open(logh);
}

void ert_log_set_log_level(int log_level){
    if(logh==NULL)
        ert_log_init_log(1,NULL,NULL,true);
    log_set_level(logh, log_level);
}

int ert_log_get_log_level(){
    if(logh==NULL)
        ert_log_init_log(1,NULL,NULL,true);
    return log_get_level(logh);
}

char * ert_log_get_filename(){
    if(logh==NULL)
        ert_log_init_log(1,NULL,NULL,true);
    return log_get_filename(logh);
}

log_type * ert_log_get_logh() {
  if(logh==NULL)
      ert_log_init_log(1,NULL,NULL,true);
  return logh;
}
