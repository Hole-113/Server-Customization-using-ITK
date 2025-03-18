// Action Handler
// Assign project to target business objects through workflow process


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
#include <tccore/project.h>

int iFail = ITK_ok;

#define DLLAPI _declspec(dllexport)
#define CUSTOM_error (EMH_USER_error_base +4)
#define Namedref_error (EMH_USER_error_base +3)

extern DLLAPI int Handler_Practice_register_callbacks();
extern DLLAPI int plm_execute_callbacks1(int* decision, va_list args);
extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args);
extern DLLAPI int Assign_project(EPM_action_message_t msg);





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
	EPM_register_action_handler("EPLM_assign_project", "Custom Action Handler", (EPM_action_handler_t)Assign_project);
	return iFail;
}

extern DLLAPI int Assign_project(EPM_action_message_t msg)
{
	EPM_decision_t decision = EPM_nogo;

	int iTargetCount = 0;
	int iNo_Arguments = 0;

	char* cClassName = NULL;
	char* cArgValue = NULL;
	char* cProjectID = NULL;
	char* cArgName = NULL;

	tag_t tTask = NULLTAG;
	tag_t tRootTask = NULLTAG;
	tag_t* tTargetList = NULLTAG;
	tag_t tClassID = NULLTAG;
	tag_t tProject = NULLTAG;

	tTask = msg.task;
	
	EPM_ask_root_task(tTask, &tRootTask);
	if (tRootTask != NULLTAG)
	{
		EPM_ask_attachments(tRootTask, EPM_target_attachment, &iTargetCount, &tTargetList);
		if (iTargetCount > 0)
		{
			// 
			iNo_Arguments = TC_number_of_arguments(msg.arguments);
			for (int i = 0; i < iNo_Arguments; i++)
			{
				// It will argument pair means Argument Name & Value one by one
				ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgName, &cArgValue);
				if (tc_strcmp(cArgName, "project_id") == 0)
				{
					cProjectID = (char*)MEM_alloc(100);
					tc_strcpy(cProjectID, "");
					tc_strcpy(cProjectID, cArgValue);
				}
			}
			for (int j = 0; j < iTargetCount; j++)
			{
				POM_class_of_instance(tTargetList[j], &tClassID);
				POM_name_of_class(tClassID, &cClassName);
				if (tc_strcmp(cClassName, "ItemRevision")==0)
				{
					PROJ_find(cProjectID, &tProject);
					if (tProject != NULLTAG)
					{
						PROJ_assign_objects(1, &tProject, 1, &tTargetList[j]);
						printf("\n\n Project Assigned to object");
					}
				}
			}
		}
	}
	return iFail;
}


extern DLLAPI int plm_execute_callbacks2(int* decision, va_list args)
{
	*decision = ALL_CUSTOMIZATIONS;
	TC_write_syslog("\n\nDLL register success for logout");
	return iFail;
}
