/*********************************************************
 ** File  specification_genel.h
 ** Some Ansys Scade File Specifications Here
 *********************************************************/

#ifndef SPECIFICATION_GENEL_H
#define SPECIFICATION_GENEL_H

/* ------------------------- Some included libraries --------------------------- */
#include "sgl_types.h"
#include "specification_RoundedRectangle.h"
#include "specification_PushButton.h"
#include "specification_ToggleButton.h"

/* Accessors */
/* ----------------------- General accessor definitions ----------------------- */

/* Context type */
/* ------------------------- This is the context we --------------------------- */
/* -------------------------   will use to parse    --------------------------- */
/* ------------------------- our variables to modbus -------------------------- */

typedef struct specification_typ_genel_ {
  /* All of these variables will go to the init_mappings function located in modbus_comm.c  */
  /* After making mappings properly, your project can read adress values in config.ini file.*/
  
  /* ------------------------- inputs --------------------------- */
  SGLbool out_RT01_Accept;
  SGLbool out_RT01_Reject;
  SGLbool out_RT01_RejectAck;
  SGLbool out_RT01_Request;
  SGLbool out_RT01_Reserve;
  /* ------------------------- outputs -------------------------- */
  SGLbool in_RT01_RejectAck;
  SGLbool in_RT01_Request;
  SGLbool in_TC03_I_Occupied_hws;
  SGLbool in_RT02_RejectAck;
  SGLbool in_RT02_Request;

  /* ------------------------- subcontexts ---------------------- */
  /* ------------------- not needed for mapping ----------------- */
  specification_typ_PushButton _SW01_norreq__;
  specification_typ_PushButton _SW01_revreq__;
  specification_typ_PushButton _SW01_errack__;
  specification_typ_PushButton _SW01_rejack__;
  specification_typ_ToggleButton _TC01_free_occ__;

} specification_typ_genel; // This will go to CONTEXT_STRUCT_NAME definition in modbus_comm.h

/* Associated functions */
/* ------------------------- Some associates functions --------------------------- */

#endif /* SPECIFICATION_GENEL_H */

/*********************************************************
 ** Some Ansys Scade File Specifications Here
 *********************************************************/

