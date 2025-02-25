#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tccore/workspaceobject.h>
#include<tccore/workspaceobject.h>
#include<ae/datasettype.h>
#include<ae/dataset.h>
#include<sa/tcfile.h>
#include<tccore/aom.h>
#include<tcinit/grm.h>


// syntax for DLL code
#define DLLAPI _declspec(dllexport)
//#define PLM_error (EMH_USER_error_base +2)

extern DLLAPI int Custom_Exit_sep_register_callbacks();

extern DLLAPI int plm_execute_callbacks1(int * decision,va_list argc);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list argc);
extern DLLAPI int check_release_status(METHOD_message_t* msg, va_list args);

extern DLLAPI int export_before_delete(METHOD_message_t* msg, va_list args);

extern DLLAPI int IRMF_after_dataset_attach(METHOD_message_t* msg, va_list args);
