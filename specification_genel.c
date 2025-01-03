/*********************************************************
 ** File  specification_genel.h
 ** Some Ansys Scade File Specifications Here
 ** This code takes the same name with your project name.
 *********************************************************/

#include "specification_genel.h"
#include "specification_imported_functions.h"
#include "sgl_simu.h"
#include "modbus_comm.h" //Modbus functions header. This will be included by user manually.


#include <stdlib.h>
#include <string.h>

specification_typ_genel * specification_genel_alloc(void)
{
  /* Function to allocate memory for the program context. */
}

void specification_genel_free(specification_typ_genel *pContext)
{
  /* Function to free allocated memory when the work done. */
}


void specification_genel_init(specification_typ_genel *pContext)
{
  /* Init function to initialize all placeholders and elements*/
}

void specification_genel_predraw(specification_typ_genel *pContext)
{
  /* Init function to draw all placeholders and elements*/
}

void specification_genel_draw(specification_typ_genel *pContext, SGLint32 pPriority)
{
  /* Your program runs here and redraws at every milisecınds regarded with your processing speed. */
  /* Update modbus function should called here and the rest work done by modbus_comm.c functions. */
  update_modbus_values(pContext);  //Update modbus function with sending only the changed bits.
  update_modbus_values_all(pContext);  //Update modbus function with sending all bits at once.
}


/*********************************************************
 ** End of file
 ** End of generation: 2025-01-02T14:57:37
 *********************************************************/

