
// Pre condition
// To restrict the user to revise the item revision if it is not released and display custom error msg.


#include"Header.h"

extern DLLAPI int Custom_Exit_sep_register_callback()
{
	CUSTOM_register_exit("Custom_Exit_sep", "USER_init_module",(CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);

	CUSTOM_register_exit("Custom_Exit_sep", "USER_exit_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
	return 0;
}

extern DLLAPI int plm_execute_callbacks1(int* decision, va_list argc)
{
	METHOD_id_t method_id;
	
	
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success");
	TC_write_syslog("\n\n DLL Registered Success for Login");
	
	// METHOD_find_method(const char* type_name, const char* msg_name, METHOD_id_t* method_id)
	METHOD_find_method("ItemRevision", "Item_copy_rev", &method_id);

	// METHOD_add_pre_condition(method_id, your custom function name, TC_argument_list_t* user_args)
	METHOD_add_pre_condition(method_id, (METHOD_function_t)check_release_status, NULLTAG);
	return 0;
}

extern DLLAPI int check_release_status(METHOD_message_t* msg, va_list args)
{
	
	tag_t tsource_rev = NULLTAG;
	char* cValue = NULL;
	char* cName = NULL;
	// pass argument (args) list out of 4 parameter i want to access tag_t type parameter
	tsource_rev = va_arg(args, tag_t);

	AOM_UIF_ask_value(tsource_rev, "release_status_list", &cValue);
	WSOM_ask_name2(tsource_rev, &cName);
	if (tc_strlen(cValue) > 0)
	{
		TC_write_syslog("\n\n Revise Success");
	}
	else
	{ 
		//EMH_store_error(EMH_severity_error, PLM_error);
		
		// Whenever we want to any msg or string from oue code to ue_error file that time we have to use this api 
		EMH_store_error_s1(EMH_severity_error, PLM_error, cName); // %1$ ==> cName ki value pass kr rhe hai woh waha pe print ho jayegi
		return PLM_error;
	}
	
	return 0;
}

extern DLLAPI int plm_execute_callbacks2(int* decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success");
	TC_write_syslog("\n\n DLL Registered Success for Logout");
	return 0;

}




