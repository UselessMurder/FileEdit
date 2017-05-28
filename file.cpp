#include "file.h"


file::file() {
    path = "";
}

file::file(QString currentPath) : path(currentPath) {}

file::~file() {
    QMap<QString, entity>::const_iterator i = rights.constBegin();
    while (i != rights.constEnd()) {
        rights[i.key()].deny.clear();
        rights[i.key()].grant.clear();
        ++i;
    }
    rights.clear();
}

QString file::getPath() {
    return path;
}

void file::setPath(QString currentPath) {
    path = currentPath;
}

void file::getRights(QMap<QString, entity> &currentRights) {
    currentRights = rights;
}

void file::setRights(QMap<QString, entity> &currentRights) {
    rights = currentRights;
}

unsigned char file::GetFileRights() {
    return getFileRights(path, rights);
}

unsigned char file::SetFileRights() {
    return setFileRights(path, rights);
}

unsigned char file::SetAtributes(QVector<bool> atrs) {
    return setAtributes(path, atrs);
}

unsigned char file::GetAtributes(QVector<bool> &atrs) {
    return getAtributes(path, atrs);
}

unsigned char file::SetTime(QDateTime create, QDateTime access, QDateTime write) {
    return setFileTime(path, create, access, write);
}

unsigned char file::GetTime(QDateTime &create, QDateTime &access, QDateTime &write) {
    return getFileTime(path, create, access, write);
}

unsigned char file::getFileRights(QString path, QMap<QString, entity> &rights) {
    QMap<QString, entity>::const_iterator j = rights.constBegin();
    while (j != rights.constEnd()) {
        rights[j.key()].deny.clear();
        rights[j.key()].grant.clear();
        ++j;
    }
    rights.clear();
    LPWSTR lstr = (wchar_t *)path.utf16();
    PACL dacl = NULL;
    PSECURITY_DESCRIPTOR sd = NULL;
    DWORD len = 0;
    BOOL df, adf;
    if(!GetFileSecurity(lstr,DACL_SECURITY_INFORMATION,sd,len,&len)) {
        sd = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len);
        if(!GetFileSecurity(lstr,DACL_SECURITY_INFORMATION,sd,len,&len))
            return 1;
    }

    if(!GetSecurityDescriptorDacl(sd, &df,&dacl,&adf)) {
        HeapFree(GetProcessHeap(), 0, sd);
        return 2;
    }
    PVOID ace;
    for (WORD i = 0; i < dacl->AceCount; i++) {
       if (!GetAce(dacl, i, &ace)) {
           HeapFree(GetProcessHeap(), 0, sd);
           return 3;
       }
       unsigned char code = parseAce(ace, rights);
       if (code) {
           HeapFree(GetProcessHeap(), 0, sd);
           return 4;
       }
    }
    HeapFree(GetProcessHeap(), 0, sd);
    unsigned char code = addOther(rights);
    if (code)
        return code;
    return 0;
}

unsigned char file::parseAce(PVOID ace, QMap<QString, entity> &rights) {
    switch(((PACE_HEADER)ace)->AceType) {
        case ACCESS_ALLOWED_ACE_TYPE: {
            PSID psid = &(((PACCESS_ALLOWED_ACE)ace)->SidStart);
            QString name;
            unsigned char code = psidToName(psid, name);
            if (code)
                return code;
            if (!rights.contains(name)) {
                entity currentEntity;
                rights[name] = currentEntity;
                rights[name].flags = ((PACE_HEADER)ace)->AceFlags;
                rights[name].valid = true;
                for (int i = 0; i < 14; i++) {
                    rights[name].grant.push_back(false);
                    rights[name].deny.push_back(false);
                }
            }
            parseMask(((PACCESS_ALLOWED_ACE)ace)->Mask, rights, name, true);
            break;
        }
        case ACCESS_DENIED_ACE_TYPE: {
            PSID psid = &(((PACCESS_DENIED_ACE)ace)->SidStart);
            QString name;
            unsigned char code = psidToName(psid, name);
            if (code)
                return code;
            if (!rights.contains(name)) {
                entity currentEntity;
                rights[name] = currentEntity;
                rights[name].flags = ((PACE_HEADER)ace)->AceFlags;
                rights[name].valid = true;
                for (int i = 0; i < 14; i++) {
                    rights[name].grant.push_back(false);
                    rights[name].deny.push_back(false);
                }
            }
            parseMask(((PACCESS_ALLOWED_ACE)ace)->Mask, rights, name, true);
            break;
        }
    }
    return 0;
}

unsigned char file::psidToName(PSID psid, QString &name){
    wchar_t *tmp = NULL;
    DWORD sName = 0;
    DWORD cchDomainName = 0;
    WCHAR * wszDomainName = NULL;
    SID_NAME_USE eSidType;
    if(LookupAccountSid(NULL, psid, NULL, &sName, wszDomainName, &cchDomainName, &eSidType) || GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        return 1;

    tmp = (wchar_t *)LocalAlloc(GMEM_FIXED, sName * 2);
    wszDomainName = (WCHAR *)LocalAlloc(GMEM_FIXED, cchDomainName * 2);

    if (!LookupAccountSid(NULL, psid, tmp, &sName, wszDomainName, &cchDomainName, &eSidType))
    {
        LocalFree(tmp);
        LocalFree(wszDomainName);
        return 2;
    }

    name = QString::fromWCharArray(tmp);

    LocalFree(tmp);
    LocalFree(wszDomainName);

    return 0;
}

void file::parseMask(DWORD mask, QMap<QString, entity> &rights, QString key, bool access) {
    if ((mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS) {
        if (access) {
            rights[key].grant[0] = true;
        } else {
            rights[key].deny[0] = true;
        }
    }
    if ((mask & FILE_EXECUTE) == FILE_EXECUTE) {
        if (access) {
            rights[key].grant[1] = true;
        } else {
            rights[key].deny[1] = true;
        }
    }
    if ((mask & FILE_READ_DATA) == FILE_READ_DATA) {
        if (access) {
            rights[key].grant[2] = true;
        } else {
            rights[key].deny[2] = true;
        }
    }
    if ((mask & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES) {
        if (access) {
            rights[key].grant[3] = true;
        } else {
            rights[key].deny[3] = true;
        }
    }
    if ((mask & FILE_READ_EA) == FILE_READ_EA) {
        if (access) {
            rights[key].grant[4] = true;
        } else {
            rights[key].deny[4] = true;
        }
    }
    if ((mask & FILE_WRITE_DATA) == FILE_WRITE_DATA) {
        if (access) {
            rights[key].grant[5] = true;
        } else {
            rights[key].deny[5] = true;
        }
    }
    if ((mask & FILE_APPEND_DATA) == FILE_APPEND_DATA) {
        if (access) {
            rights[key].grant[6] = true;
        } else {
            rights[key].deny[6] = true;
        }
    }
    if ((mask & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES) {
        if (access) {
            rights[key].grant[7] = true;
        } else {
            rights[key].deny[7] = true;
        }
    }
    if ((mask & FILE_WRITE_EA) == FILE_WRITE_EA) {
        if (access) {
            rights[key].grant[8] = true;
        } else {
            rights[key].deny[8] = true;
        }
    }
    if ((mask & DELETE) == DELETE) {
        if (access) {
            rights[key].grant[9] = true;
        } else {
            rights[key].deny[9] = true;
        }
    }
    if ((mask & READ_CONTROL) == READ_CONTROL) {
        if (access) {
            rights[key].grant[10] = true;
        } else {
            rights[key].deny[10] = true;
        }
    }
    if ((mask & WRITE_DAC) == WRITE_DAC) {
        if (access) {
            rights[key].grant[11] = true;
        } else {
            rights[key].deny[11] = true;
        }
    }
    if ((mask & WRITE_OWNER) == WRITE_OWNER) {
        if (access) {
            rights[key].grant[12] = true;
        } else {
            rights[key].deny[12] = true;
        }
    }
    if ((mask & SYNCHRONIZE) == SYNCHRONIZE) {
        if (access) {
            rights[key].grant[13] = true;
        } else {
            rights[key].deny[13] = true;
        }
    }
}

unsigned char file::addOther(QMap<QString, entity> &rights) {
      QVector<QString> other;
      unsigned char code = 0;

      code = getUsers(other);
      if (code) {
          other.clear();
          return code;
      }

      code = getGroups(other);
      if (code) {
          other.clear();
          return code;
      }

    for (int i = 0; i < other.size(); i++) {
        if(!rights.contains(other[i])) {
            entity currentEntity;
            rights[other[i]] = currentEntity;
            rights[other[i]].flags = 0;
            rights[other[i]].valid = false;
            for (int j = 0; j < 14; j++) {
                rights[other[i]].grant.push_back(false);
                rights[other[i]].deny.push_back(false);
            }
        }
    }
    other.clear();
    return code;
}

unsigned char file::getGroups(QVector<QString> &groups) {
    DWORD dwTotalEntries = 0;
    DWORD dwEntriesRead = 0;
    GROUP_INFO_0 * theEntries = NULL;
    if (!NetLocalGroupEnum(NULL, 0, (LPBYTE*)&theEntries, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, NULL))
    {
        for (DWORD i = 0; i < dwEntriesRead; i++)
            groups.push_back(QString::fromWCharArray(theEntries[i].grpi0_name));
        NetApiBufferFree(theEntries);
    }
    else
        return 1;

    return 0;
}

unsigned char file::getUsers(QVector<QString> &users) {
    DWORD dwTotalEntries = 0;
    DWORD dwEntriesRead = 0;

    USER_INFO_0 * theEntries = NULL;
    if (!NetUserEnum(NULL, 0, NULL, (LPBYTE*)&theEntries, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, NULL))
    {
        for (DWORD i = 0; i < dwEntriesRead; i++)
            users.push_back(QString::fromWCharArray(theEntries[i].usri0_name));
        NetApiBufferFree(theEntries);
    }
    else
        return 1;
    return 0;
}

unsigned char file::setFileRights(QString path, QMap<QString, entity> &rights) {
    PACL newDacl = NULL;
    QVector<PVOID> aces;
    LPWSTR lstr = (wchar_t *)path.utf16();
    QMap<QString, entity>::const_iterator i = rights.constBegin();
    while (i != rights.constEnd()) {
        if (rights[i.key()].valid) {
            unsigned char code = getAces(aces, i.key(), i.value(), false);
            if (code) {
                for (int i = 0; i < aces.size(); i++) {
                    HeapFree(GetProcessHeap(), 0, aces[i]);
                }
                aces.clear();
                return code;
            }
        }
        ++i;
    }
    i = rights.constBegin();
    while (i != rights.constEnd()) {
        if (rights[i.key()].valid) {
            unsigned char code = getAces(aces, i.key(), i.value(), true);
            if (code) {
                for (int i = 0; i < aces.size(); i++) {
                    HeapFree(GetProcessHeap(), 0, aces[i]);
                }
                aces.clear();
                return code;
            }
        }
        ++i;
    }
    DWORD size = 0;
    for (int i = 0; i < aces.size(); i++)
        size += ((PACCESS_ALLOWED_ACE)aces[i])->Header.AceSize;
    size += sizeof(ACL);
    newDacl = (PACL)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);

    if(!InitializeAcl(newDacl, size, ACL_REVISION)) {
        HeapFree(GetProcessHeap(),0, newDacl);
        for (int i = 0; i < aces.size(); i++) {
            HeapFree(GetProcessHeap(), 0, aces[i]);
        }
        aces.clear();
        return 2;
    }
    for (int i = 0; i < aces.size(); i++) {
        if (!AddAce( newDacl, ACL_REVISION, MAXDWORD, aces[i], ((PACCESS_ALLOWED_ACE)aces[i])->Header.AceSize)) {
            HeapFree(GetProcessHeap(), 0, newDacl);
            for (int i = 0; i < aces.size(); i++) {
                HeapFree(GetProcessHeap(), 0, aces[i]);
            }
            aces.clear();
            return 3;
        }
    }
    PSECURITY_DESCRIPTOR sd = NULL;
    sd = (PSECURITY_DESCRIPTOR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, SECURITY_DESCRIPTOR_MIN_LENGTH);
    if (!InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION)) {
        HeapFree(GetProcessHeap(), 0, sd);
        HeapFree(GetProcessHeap(), 0, newDacl);
        for (int i = 0; i < aces.size(); i++) {
            HeapFree(GetProcessHeap(), 0, aces[i]);
        }
        aces.clear();
        return 4;
    }

    if (!SetSecurityDescriptorDacl(sd, TRUE, newDacl, FALSE)) {
        HeapFree(GetProcessHeap(), 0, sd);
        HeapFree(GetProcessHeap(), 0, newDacl);
        for (int i = 0; i < aces.size(); i++) {
            HeapFree(GetProcessHeap(), 0, aces[i]);
        }
        aces.clear();
        return 5;
    }
    if (!SetFileSecurity(lstr,DACL_SECURITY_INFORMATION, sd)) {
        HeapFree(GetProcessHeap(), 0, sd);
        HeapFree(GetProcessHeap(), 0, newDacl);
        for (int i = 0; i < aces.size(); i++) {
            HeapFree(GetProcessHeap(), 0, aces[i]);
        }
        aces.clear();
        return 6;
    }
    HeapFree(GetProcessHeap(), 0, sd);
    HeapFree(GetProcessHeap(), 0, newDacl);
    for (int i = 0; i < aces.size(); i++) {
        HeapFree(GetProcessHeap(), 0, aces[i]);
    }
    aces.clear();
    return 0;
}

unsigned char file::getAces(QVector<PVOID> &aces, QString name, entity parameters, bool access) {
    if (access && checkFlags(parameters.grant)) {
        PSID psid;
        DWORD mask;
        unsigned char code = nameToPsid(psid, name);
        if (code)
            return code;
        parseFlags(mask, parameters.grant);
        PACCESS_ALLOWED_ACE pace = (ACCESS_ALLOWED_ACE *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(psid) - sizeof(DWORD));
        pace->Header.AceType  = ACCESS_ALLOWED_ACE_TYPE;
        pace->Header.AceFlags = parameters.flags ;
        pace->Header.AceSize  = LOWORD(sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(psid) - sizeof(DWORD));
        pace->Mask = mask;
        if (!CopySid(GetLengthSid(psid), &pace->SidStart, psid)) {
             HeapFree(GetProcessHeap(), 0, psid);
             HeapFree(GetProcessHeap(), 0 , pace);
             return 9;
        }
        HeapFree(GetProcessHeap(), 0, psid);
        aces.push_back((PVOID)pace);
    }

    if (!access && checkFlags(parameters.deny)) {
        PSID psid;
        DWORD mask;
        unsigned char code = nameToPsid(psid, name);
        if (code)
            return code;
        parseFlags(mask, parameters.deny);
        PACCESS_DENIED_ACE pace = (ACCESS_DENIED_ACE *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ACCESS_DENIED_ACE) + GetLengthSid(psid) - sizeof(DWORD));
        pace->Header.AceType  = ACCESS_DENIED_ACE_TYPE;
        pace->Header.AceFlags = parameters.flags ;
        pace->Header.AceSize  = LOWORD(sizeof(ACCESS_DENIED_ACE) + GetLengthSid(psid) - sizeof(DWORD));
        pace->Mask = mask;
        if (!CopySid(GetLengthSid(psid), &pace->SidStart, psid)) {
             HeapFree(GetProcessHeap(), 0, psid);
             HeapFree(GetProcessHeap(), 0 , pace);
             return 9;
        }
        HeapFree(GetProcessHeap(), 0, psid);
        aces.push_back((PVOID)pace);
    }
    return 0;
}

bool file::checkFlags(QVector<bool> flags) {
    for (int i = 0; i < flags.size(); i++) {
        if (flags[i] != false)
            return true;
    }
    return false;
}

unsigned char file::nameToPsid(PSID &psid, QString name) {
    DWORD sSid = 0;
    DWORD cchDomainName = 0;
    WCHAR * wszDomainName = NULL;
    SID_NAME_USE eSidType;
    if (LookupAccountName(NULL, (wchar_t *)name.utf16(), NULL, &sSid, wszDomainName, &cchDomainName, &eSidType) || GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        return 7;

    PSID sid = NULL;
    sid = LocalAlloc(GMEM_FIXED, sSid * 2);
    wszDomainName = (WCHAR *)LocalAlloc(GMEM_FIXED, cchDomainName * 2);

    if (!LookupAccountName(NULL, (wchar_t *)name.utf16(), sid, &sSid, wszDomainName, &cchDomainName, &eSidType))
    {
        LocalFree(sid);
        LocalFree(wszDomainName);
        return 8;
    }

    LocalFree(wszDomainName);

    DWORD need = GetLengthSid((PTOKEN_USER)sid);
    psid = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, need);

    if (!CopySid(need, psid, sid))
    {
        LocalFree(sid);
        HeapFree(GetProcessHeap(), 0, psid);
        return 9;
    }

    LocalFree(sid);

    return 0;
}

void file::parseFlags(DWORD &mask, QVector<bool> flags) {
    mask = 0;
    if (flags[0]) {
       mask = mask | FILE_ALL_ACCESS;
    }
    if (flags[1]) {
        mask = mask | FILE_EXECUTE;
    }
    if (flags[2]) {
        mask = mask | FILE_READ_DATA;
    }
    if (flags[3]) {
       mask = mask | FILE_READ_ATTRIBUTES;
    }
    if (flags[4]) {
        mask = mask | FILE_READ_EA;
    }
    if (flags[5]) {
        mask = mask | FILE_WRITE_DATA;
    }
    if (flags[6]) {
       mask = mask | FILE_APPEND_DATA;
    }
    if (flags[7]) {
       mask = mask | FILE_WRITE_ATTRIBUTES;
    }
    if (flags[8]) {
        mask = mask | FILE_WRITE_EA;
    }
    if (flags[9]) {
        mask = mask | DELETE;
    }
    if (flags[10]) {
        mask = mask | READ_CONTROL;
    }
    if (flags[11]) {
       mask = mask | WRITE_DAC;
    }
    if (flags[12]) {
       mask = mask | WRITE_OWNER;
    }
    if (flags[13]) {
       mask = mask | SYNCHRONIZE;
    }
}



unsigned char file::getAtributes(QString path, QVector<bool> &atrs) {
    unsigned char code = 0;
    LPWSTR lstr = (wchar_t *)path.utf16();
    DWORD datrs = GetFileAttributes(lstr);
    if (GetLastError() != 0) {
        code = 1;
        return code;
    }
    atrs.clear();
    for (int i = 0; i < 7; i++) {
        atrs.push_back(false);
    }
    if ((datrs & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE) {
        atrs[0] = true;
    }
    if ((datrs & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) {
        atrs[1] = true;
    }
    if ((datrs & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) == FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) {
        atrs[2] = true;
    }
    if ((datrs & FILE_ATTRIBUTE_OFFLINE) == FILE_ATTRIBUTE_OFFLINE) {
        atrs[3] = true;
    }
    if ((datrs & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY) {
        atrs[4] = true;
    }
    if ((datrs & FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM) {
        atrs[5] = true;
    }
    if ((datrs & FILE_ATTRIBUTE_TEMPORARY) == FILE_ATTRIBUTE_TEMPORARY) {
        atrs[6] = true;
    }
    return code;
}

unsigned char file::setAtributes(QString path, QVector<bool> atrs) {
     unsigned char code = 1;
     if (atrs.size() != 7) {
        code = 2;
        return code;
    }
    DWORD datrs = 0;
    if (atrs[0]) {
        datrs = datrs | FILE_ATTRIBUTE_ARCHIVE;
    }
    if (atrs[1]) {
        datrs = datrs | FILE_ATTRIBUTE_HIDDEN;
    }
    if (atrs[2]) {
        datrs = datrs | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
    }
    if (atrs[3]) {
        datrs = datrs | FILE_ATTRIBUTE_OFFLINE;
    }
    if (atrs[4]) {
        datrs = datrs | FILE_ATTRIBUTE_READONLY;
    }
    if (atrs[5]) {
        datrs = datrs | FILE_ATTRIBUTE_SYSTEM;
    }
    if (atrs[6]) {
        datrs = datrs | FILE_ATTRIBUTE_TEMPORARY;
    }
    LPWSTR lstr = (wchar_t *)path.utf16();
    BOOL result = SetFileAttributes(lstr, datrs);
    if (result) {
        code = 0;
        return code;
    }
    return code;
}

unsigned char file::setFileTime(QString path, QDateTime create, QDateTime access, QDateTime write) {
     unsigned char code = 1;
     BOOL result = FALSE;
     LPWSTR lstr = (wchar_t *)path.utf16();
     SYSTEMTIME create_time, read_time, write_time;
     file::QDTtoSYST(create, create_time);
     file::QDTtoSYST(access, read_time);
     file::QDTtoSYST(write, write_time);
     HANDLE hFile = CreateFile(lstr, FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
     if (hFile)
     {
         FILETIME file_create_time, file_read_time, file_write_time;
         result = SystemTimeToFileTime(&create_time, &file_create_time);
         if (result)
             result = SystemTimeToFileTime(&read_time, &file_read_time);
         if (result)
             result = SystemTimeToFileTime(&write_time, &file_write_time);
         if (result)
             result = SetFileTime(hFile, &file_create_time, &file_read_time, &file_write_time);
         CloseHandle(hFile);
     }
     if (result) {
         code = 0;
         return code;
     }
     return code;
}

unsigned char file::getFileTime(QString path, QDateTime &create, QDateTime &access, QDateTime &write) {
    unsigned char code = 1;
    BOOL result = FALSE;
    LPWSTR lstr = (wchar_t *)path.utf16();
    HANDLE hFile = CreateFile(lstr, FILE_READ_ATTRIBUTES | FILE_READ_EA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile)
    {
        FILETIME file_create_time, file_read_time, file_write_time;
        SYSTEMTIME create_time, read_time, write_time;
        result = GetFileTime(hFile, &file_create_time, &file_read_time, &file_write_time);
        if (result) {
            result = FileTimeToSystemTime(&file_create_time, &create_time);
            if (result)
                file::SYSTtoQDT(create_time, create);
        }
        if (result) {
            result = FileTimeToSystemTime(&file_read_time, &read_time);
            if (result)
                file::SYSTtoQDT(read_time, access);
        }
        if (result) {
            result = FileTimeToSystemTime(&file_write_time, &write_time);
            if (result)
                file::SYSTtoQDT(write_time, write);
        }
        CloseHandle(hFile);
    }

    if (result) {
        code = 0;
        return code;
    }
    return code;
}

void file::QDTtoSYST(QDateTime qdt, SYSTEMTIME &syst) {
    syst.wYear = qdt.date().year();
    syst.wMonth = qdt.date().month();
    syst.wDay = qdt.date().day();
    syst.wDayOfWeek = 0;
    syst.wHour = qdt.time().hour();
    syst.wMinute = qdt.time().minute();
    syst.wSecond = qdt.time().second();
    syst.wMilliseconds = 0;
}

void file::SYSTtoQDT(SYSTEMTIME syst, QDateTime &qdt) {
    QDate date;
    QTime time;
    date.setDate(syst.wYear, syst.wMonth, syst.wDay);
    time.setHMS(syst.wHour, syst.wMinute, syst.wSecond, syst.wMilliseconds);
    qdt.setDate(date);
    qdt.setTime(time);
}


