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

	METHOD_find_method("ItemRevision", "ITEM_create_from_rev", &method_id);
	METHOD_add_pre_condition(method_id, (METHOD_function_t)check_userdata_prop, NULLTAG);
	return 0;
}

extern DLLAPI int check_userdata_prop(METHOD_message_t* msg, va_list args)
{
	tag_t tItem = NULLTAG;
	tag_t tItemrev = NULLTAG;
	tag_t tRelation = NULLTAG;
	tag_t* tSecObj = NULLTAG;

	int iSecObj = 0;
	char* cValue = NULL;
	tItem = va_arg(args, tag_t);
	tItemrev = va_arg(args, tag_t);

	GRM_find_relation_type("IMAN_master_form_rev", &tRelation); //Retrieves the relation type for the given relation type name.
	GRM_list_secondary_objects_only(tItemrev, tRelation, &iSecObj, &tSecObj);

	for (int i = 0; i < iSecObj; i++)
	{
		AOM_UIF_ask_value(tSecObj[i], "user_data_1", &cValue);
	}

	if (tc_strlen(cValue) > 0)
	{
		TC_write_syslog("\n\n save as Success");
	}
	else
	{
		EMH_store_error(EMH_severity_error, PLM_error); // %1$ ==> cName ki value pass kr rhe hai woh waha pe print ho jayegi
		return PLM_error;
	}
	return 0;
}

extern DLLAPI int plm_execute_callback2(int* decision, va_list argc)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n DLL Registered Success");
	TC_write_syslog("\n\n DLL Registered Success for Logout");
	return 0;

}
