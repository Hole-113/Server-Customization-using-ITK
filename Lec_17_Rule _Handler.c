/*check number of Named Reference files on particulae type of dataset before initate the
workflow on ItemRevision.*/
// 1.Datatype must be pdf
// 2.Check count of named reference file must be 1
// 3.If condition is not matched then display the custom error message 






#include <stdio.h>
#include <tcinit/tcinit.h>
#include <tccore/custom.h>
#include <tc/tc_startup.h>
#include <tc/tc_util.h>
#include <tccore/method.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
#include <tc/emh.h>
#include <tccore/grm.h>
#include <ae/dataset.h>
#include <epm/epm.h>
#include <pom/pom/pom.h>

int iFail = ITK_ok;

#define DLLAPI _declspec(dllexport)
#define CUSTOM_error (EMH_USER_error_base +4)
#define Namedref_error (EMH_USER_error_base +3)

extern DLLAPI int Handler_Practice_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);
EPM_decision_t chk_named_ref(EPM_rule_message_t msg);







#include "Header.h"

extern DLLAPI int Handler_Practice_register_callbacks()
{
	CUSTOM_register_exit("Handler_Practice", "USER_init_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks1);
	CUSTOM_register_exit("Handler_Practice", "USER_exit_module", (CUSTOM_EXIT_ftn_t)plm_execute_callbacks2);
	return iFail;
}

extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	// Registers a rule handler for a given handler name
	EPM_register_rule_handler("EPLM_chk_named_ref", "Custom Rule Handler", (EPM_rule_handler_t)chk_named_ref);
	return iFail;
}

EPM_decision_t chk_named_ref(EPM_rule_message_t msg)
{
	EPM_decision_t decision = EPM_nogo;

	int k = 0;
	int iTargetCount = 0;
	int iSecObjeCount = 0;
	int iNamedRefCount = 0;

	char* cClassName = NULL;
	char* cTargetObjName = NULL;
	char* cDatasetType = NULL;
	char* cSecObjName = NULL;

	tag_t tTask = NULLTAG;
	tag_t tRootTask = NULLTAG;
	tag_t* tTargetList = NULLTAG;
	tag_t tClassID = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t* tSecObjList = NULLTAG;
	tag_t* tNamedRefList = NULLTAG;

	tTask = msg.task;

	EPM_ask_root_task(tTask, &tRootTask);
	if (tRootTask != NULLTAG)
	{
		EPM_ask_attachments(tRootTask, EPM_target_attachment, &iTargetCount, &tTargetList);
		if (iTargetCount > 0)
		{
			for (int i = 0; i < iTargetCount; i++)
			{
				POM_class_of_instance(tTargetList[i], &tClassID);
				POM_name_of_class(tClassID, &cClassName);
				if (tc_strcmp(cClassName, "ItemRevision") == 0)
				{
					GRM_find_relation_type("IMAN_specification", &tRelationType);
					GRM_list_secondary_objects_only(tTargetList[i], tRelationType, &iSecObjeCount, &tSecObjList);
					if (tSecObjList > 0)
					{
						for (int j = 0; j < iSecObjeCount; j++)
						{
							WSOM_ask_object_type2(tSecObjList[j], &cDatasetType);
							if (tc_strcmp(cDatasetType, "PDF") == 0)
							{
								k++;
								AE_ask_dataset_named_refs(tSecObjList[j], &iNamedRefCount, &tNamedRefList);
								if (iNamedRefCount == 1)
								{
									decision = EPM_go;
								}
								else
								{
									EMH_store_error(EMH_severity_error, Namedref_error);
									return Namedref_error;
								}
							}
						}
						if (k == 0)
						{
							EMH_store_error(EMH_severity_error, CUSTOM_error);
							return CUSTOM_error;
						}
					}
					else
					{
						EMH_store_error(EMH_severity_error, CUSTOM_error);
						return CUSTOM_error;
					}
				}
			}
		}
	}
	if (tTargetList)
	{
		MEM_free(tTargetList);
	}
	if (tSecObjList)
	{
		MEM_free(tSecObjList);
	}
	if (tNamedRefList)
	{
		MEM_free(tNamedRefList);
	}
	return decision;
}


extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	TC_write_syslog("\n\nDLL register success for logout");
	return iFail;
}
