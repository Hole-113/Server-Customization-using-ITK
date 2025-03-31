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



// syntax for DLL code
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +2)

extern DLLAPI int Custom_Exit_sep_register_callbacks();
extern DLLAPI int plm_execute_callback1(int* decision, va_list args);
extern DLLAPI int check_userdata_prop(METHOD_message_t* msg, va_list args);
extern DLLAPI int plm_execute_callback2(int* decision, va_list args);
