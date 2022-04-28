/**
 * SQLite loadable extension providing the plain nested VFS (for testing with no page transcoding)
 */
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1
#include "SQLiteNestedVFS.h"

#ifdef HAS_WEB
#include "web_vfs.h"
#endif
/*************************************************************************************************/

/*
** This routine is called when the extension is loaded.
** Register the new VFS.
*/
#ifdef _WIN32
extern "C" __declspec(dllexport) int sqlite3_nestedvfs_init(sqlite3 *db, char **pzErrMsg,
                                      const sqlite3_api_routines *pApi) {
#else
extern "C" int sqlite3_nestedvfs_init(sqlite3 *db, char **pzErrMsg,
                                      const sqlite3_api_routines *pApi) {
#endif
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);

    #ifdef HAS_WEB
    rc = (new WebVFS::VFS())->Register("web");
    if (rc != SQLITE_OK) {
        return rc;
    }
    #endif
    
    rc = (new SQLiteNested::VFS())->Register("nested");
    if (rc == SQLITE_OK) {
        rc = SQLITE_OK_LOAD_PERMANENTLY;
    }
    return rc;
}
