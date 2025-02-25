// Assign 1
// To fill the dataset typeon on the the IRMF property after attachment of dataset on Item Revision



#include"Header.h"

extern DLLAPI int Custom_Exit_sep_register_callbacks()
{
	// After Login successfull Custom_Exit_sep dll will be registered
	CUSTOM_register_exit("Custom_Exit_sep", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);

	CUSTOM_register_exit("Custom_Exit_sep", "USER_exit_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
	return 0;
}

extern DLLAPI int plm_execute_callbacks1(int* decision, va_list argc)
{
	METHOD_id_t method_id;
	// 
	*decision = ALL_CUSTOMIZATIONS;
	TC_write_syslog("\n\n DLL Registered Success for Login");

	// We are getting default method id tag from server
	METHOD_find_method("IMAN_specification", "GRM_create", &method_id);

	// We register our custom function with default method as a pre condition
	// (METHOD_function_t)check_release_status ==> Typecasting in MEHTOD_function_t
	METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)IRMF_after_dataset_attach, NULLTAG);

	return 0;
}

extern DLLAPI int IRMF_after_dataset_attach(METHOD_message_t* msg, va_list args)
{
	int iSecObj = 0;
	int i = 0;

	char* cType = NULL;

	tag_t tPrimary = NULLTAG; //Item Revision tag
	tag_t tSecondary = NULLTAG; // Dataset Tag
	tag_t tRelation = NULLTAG;
	tag_t* tSecObj = NULLTAG;

	tPrimary = va_arg(args, tag_t); //It will give 1st tag from the list
	tSecondary = va_arg(args, tag_t); //It will give 2nd tag from the list

	GRM_find_relation_type("IMAN_master_form_rev", &tRelation); //Retrieves the relation type for the given relation type name.
	GRM_list_secondary_objects_only(tPrimary, tRelation, &iSecObj, &tSecObj);

	WSOM_ask_object_type2(tSecondary, &cType); // from secondary tag(means dataset) we are finding type

	for (i = 0; i < iSecObj; i++)
	{
		AOM_lock(tSecObj[i]);
		AOM_UIF_set_value(tSecObj[i], "user_data_1", cType);
		AOM_save_without_extensions(tSecObj[i]);
		AOM_unlock(tSecObj[i]);
	}
	if (tSecObj)
	{
		MEM_free(tSecObj);
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


