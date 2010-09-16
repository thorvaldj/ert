#include <ecl_kw.h>
#include <stdlib.h>
#include <ecl_sum.h>
#include <util.h>
#include <string.h>
#include <signal.h>
#include <stringlist.h>


int main(int argc , char ** argv) {
  const char * data_file = argv[1];
  
  ecl_sum_type * ecl_sum = ecl_sum_fread_alloc_case( data_file , ":");
  if (ecl_sum != NULL) {
    stringlist_type * keys = stringlist_alloc_new();

    if (argc == 2)
      ecl_sum_select_matching_general_var_list( ecl_sum , "*" , keys);
    else {
      for (int iarg = 2; iarg < argc; iarg++) {
        printf("Matchging:%s \n",argv[iarg]);
        ecl_sum_select_matching_general_var_list( ecl_sum , argv[iarg] , keys);
      }
    }
    
    stringlist_sort( keys , NULL );
    {
      int i;
      for (i=0; i < stringlist_get_size( keys );  i++)
        printf("%s \n",stringlist_iget( keys , i ));
    }
    
    stringlist_free( keys );
    ecl_sum_free(ecl_sum);
  } else 
    fprintf(stderr,"key_list.x: No summary data found for case:%s\n", data_file );
}
