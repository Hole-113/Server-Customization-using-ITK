#include "Header.h"

extern DLLAPI int Custom_Exit_sep_register_callbacks()
{
	CUSTOM_register_exit("Custom_Exit_sep", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callback1);
	CUSTOM_register_exit("Custom_Exit_sep", "USER_exit_module", (CUSTOM_EXIT_ftn_t)plm_execute_callback2);
	return 0;
}

extern DLLAPI int plm_execute_callback1(int* decision, va_list args)
{
	METHOD_id_t method_id;
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success");

	METHOD_find_method("ItemRevision", "ITEM_copy_rev", &method_id);
	METHOD_add_pre_condition(method_id, (METHOD_function_t)check_target_attachments_status, NULLTAG);
	return 0;
}

extern DLLAPI int check_target_attachments_status(METHOD_message_t* msg, va_list args)
{

	tag_t tsource_rev = NULLTAG;
	char* cValue = NULL;
	tsource_rev = va_arg(args, tag_t);
	AOM_UIF_ask_value(tsource_rev, "find()AllWorkflows", &cValue);
	if (tc_strlen(cValue) > 0)
	{
		printf("\n\n Revise success");
	}
	else
	{
		EMH_store_error(EMH_severity_error, PLM_error); // %1$ ==> cName ki value pass kr rhe hai woh waha pe print ho jayegi
		return PLM_error;

	}
	return 0;
}

extern DLLAPI int plm_execute_callback2(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success");
	TC_write_syslog("\n\n DLL Registered Success for Logout");
	return 0;

}
