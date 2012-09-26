/*
   Copyright (C) 2011  Statoil ASA, Norway. 
    
   The file 'misfit_table.h' is part of ERT - Ensemble based Reservoir Tool. 
    
   ERT is free software: you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation, either version 3 of the License, or 
   (at your option) any later version. 
    
   ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE.   
    
   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
   for more details. 
*/

#ifndef __MISFIT_ENSEMBLE_H__
#define __MISFIT_ENSEMBLE_H__

#include <stdbool.h>

#include <enkf_obs.h>
#include <ensemble_config.h>
#include <enkf_fs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MISFIT_DEFAULT_RANKING_KEY "DEFAULT"
#include <misfit_ensemble_typedef.h>

typedef struct misfit_member_struct    misfit_member_type;
typedef struct misfit_ts_struct        misfit_ts_type;

  void                misfit_ensemble_clear( misfit_ensemble_type * table);
  misfit_ensemble_type * misfit_ensemble_alloc( );
  void                misfit_ensemble_free( misfit_ensemble_type * table );
  void                misfit_ensemble_fwrite( const misfit_ensemble_type * misfit_ensemble , FILE * stream);
  bool                misfit_ensemble_initialized( const misfit_ensemble_type * misfit_ensemble );
  void                misfit_ensemble_update( misfit_ensemble_type * misfit_ensemble , const ensemble_config_type * ensemble_config , const enkf_obs_type * enkf_obs , enkf_fs_type * fs , int ens_size , int history_length);
  void                misfit_ensemble_set_ens_size( misfit_ensemble_type * misfit_ensemble , int ens_size);
  int                 misfit_ensemble_get_ens_size( const misfit_ensemble_type * misfit_ensemble );

  misfit_member_type * misfit_ensemble_iget_member( const misfit_ensemble_type * table , int iens);
  misfit_ts_type     * misfit_member_get_ts( const misfit_member_type * node , const char * obs_key );
  bool                 misfit_member_has_ts( const misfit_member_type * node , const char * obs_key );
  double               misfit_ts_eval( const misfit_ts_type * ts , int step1 , int step2 );
  
  
#ifdef __cplusplus
}
#endif

#endif