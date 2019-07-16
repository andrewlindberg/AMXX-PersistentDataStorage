#include "module.h"

//native CED_SetCell(entity, const key[], any:data);
static cell Native_SetCell(AMX *amx, cell *params)
{
    int dummyLen;
    const char *key = MF_GetAmxString(amx, params[1], 0, &dummyLen);

    g_entityData->replace(key, ke::AutoPtr<IEntDataEntry>(new EntDataCell(params[2])));

    return 0;
}

//native CED_SetArray(entity, const key[], const any:data[], size);
static cell Native_SetArray(AMX *amx, cell *params)
{
    int dummyLen;
    const char *key = MF_GetAmxString(amx, params[1], 0, &dummyLen);
    
    ke::AutoPtr<cell[]> data(new cell[params[3]]);
    MF_CopyAmxMemory(data.get(), MF_GetAmxAddr(amx, params[2]), params[3]);

    g_entityData->replace(key, ke::AutoPtr<IEntDataEntry>(new EntDataArray(ke::Move(data), params[3])));

    return 0;
}

// //native CED_SetString(entity, const key[], const buffer[]);
static cell Native_SetString(AMX *amx, cell *params)
{
    int dummyLen;
    const char *key = MF_GetAmxString(amx, params[1], 0, &dummyLen);

    g_entityData->replace(key, ke::AutoPtr<IEntDataEntry>(new EntDataString(MF_GetAmxString(amx, params[2], 1, &dummyLen))));

    return 0;
}

//native bool:CED_GetCell(entity, const key[], &any:data);
static cell Native_GetCell(AMX *amx, cell *params)
{
    int dummyLen;
    const char *key = MF_GetAmxString(amx, params[1], 0, &dummyLen);

    ke::AutoPtr<IEntDataEntry> *ptrData;
    if(g_entityData->retrieve(key, &ptrData))
    {
        EntDataCell *data = static_cast<EntDataCell *>(ptrData->get());
        if(data->GetEntryType() != IEntDataEntry::EntryType::Cell)
            return 0;

        *(MF_GetAmxAddr(amx, params[2])) = data->Data;
        return 1;
    }

    return 0;
}

//native bool:CED_GetArray(entity, const key[], any:data[], size);
static cell Native_GetArray(AMX *amx, cell *params)
{
    int dummyLen;
    const char *key = MF_GetAmxString(amx, params[1], 0, &dummyLen);

    ke::AutoPtr<IEntDataEntry> *ptrData;
    if(g_entityData->retrieve(key, &ptrData))
    {
        EntDataArray *data = static_cast<EntDataArray *>(ptrData->get());
        if(data->GetEntryType() != IEntDataEntry::EntryType::Array)
            return 0;

        size_t size = static_cast<size_t>(params[3]);
        if(size > data->DataSize)
            size = data->DataSize;

        cell *outAddr = MF_GetAmxAddr(amx, params[2]);
        MF_CopyAmxMemory(outAddr, data->Data.get(), size);
        return 1;
    }

    return 0;
}

// //native bool:CED_GetString(entity, const key[], buffer[], maxLength);
static cell Native_GetString(AMX *amx, cell *params)
{
    int dummyLen;
    const char *key = MF_GetAmxString(amx, params[1], 0, &dummyLen);

    ke::AutoPtr<IEntDataEntry> *ptrData;
    if(g_entityData->retrieve(key, &ptrData))
    {
        EntDataString *data = static_cast<EntDataString *>(ptrData->get());
        if(data->GetEntryType() != IEntDataEntry::EntryType::String)
            return 0;

        MF_SetAmxString(amx, params[2], data->Data.chars(), params[3]);
        return 1;
    }
    
    return 0;
}

AMX_NATIVE_INFO g_natives[] =
{
    {"PDS_SetCell", Native_SetCell},
    {"PDS_SetArray", Native_SetArray},
    {"PDS_SetString", Native_SetString},

    {"PDS_GetCell", Native_GetCell},
    {"PDS_GetArray", Native_GetArray},
    {"PDS_GetString", Native_GetString},

    {nullptr, nullptr}
};