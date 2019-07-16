#include "module.h"

StringHashMap<ke::AutoPtr<IEntDataEntry>> *g_entityData = nullptr;
int fwdSave = 0;

void OnAmxxAttach()
{
    MF_AddNatives(g_natives);
    g_entityData = new StringHashMap<ke::AutoPtr<IEntDataEntry>>;
}

void OnAmxxDetach()
{
	g_entityData->clear();
	delete g_entityData;
}

void OnPluginsLoaded()
{
	fwdSave = MF_RegisterForward("PDS_Save", ET_IGNORE, FP_DONE);
}

void OnPluginsUnloading()
{
	g_entityData->clear();
	MF_ExecuteForward(fwdSave);
}