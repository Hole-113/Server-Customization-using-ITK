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
	METHOD_find_method("Dataset", "AE_delete_dataset", &method_id);

	// We register our custom function with default method as a pre condition
	// (METHOD_function_t)check_release_status ==> Typecasting in MEHTOD_function_t
	METHOD_add_action(method_id,METHOD_pre_action_type,(METHOD_function_t)export_before_delete, NULLTAG);

	return 0;
}

extern DLLAPI int export_before_delete(METHOD_message_t* msg, va_list args)
{
	int iDatasetTypeRefListCount = 0;
	int iRefCount = 0;
	
	char* cFileName = NULL;
	char Originalpath[100] = "c:\\data\\";
	char* cdatasettypeName = NULL;
	char** cDatasetTypeRefNameList = NULL;
	
	tag_t tdatasettype = NULLTAG;
	tag_t tdataset = va_arg(args, tag_t);
	tag_t* tRefList = NULLTAG;

	WSOM_ask_object_type2(tdataset, &cdatasettypeName); // Retrieves the object type(PDF,TEXT) of the specified WorkspaceObject.
	AE_find_datasettype2(cdatasettypeName, &tdatasettype); // we are getting datatsettype from PDF Name
	AE_ask_datasettype_refs(tdatasettype, &iDatasetTypeRefListCount, &cDatasetTypeRefNameList);

	for (int i = 0; i < iDatasetTypeRefListCount; i++)
	{
		printf("\n\n Ref List Name is %s", cDatasetTypeRefNameList[i]);
		AE_ask_all_dataset_named_refs2(tdataset, cDatasetTypeRefNameList[i],&iRefCount,&tRefList);

		for (int j = 0; j < iRefCount; j++)
		{
			TC_write_syslog("\n\n File path is %s", Originalpath);

			IMF_ask_original_file_name2(tRefList[j],&cFileName);
			tc_strcat(Originalpath, cFileName);
			
			TC_write_syslog("\n\n Updated File Path is %s", Originalpath);

			IMF_export_file(tRefList[j], Originalpath);
			tc_strcpy(Originalpath, "c:\\data\\");
			TC_write_syslog("\n\n File path is %s", Originalpath);
		}
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


