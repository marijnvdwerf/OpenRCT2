
@import Foundation;
#include "platform.h"
#include "../util/util.h"


bool platform_check_steam_overlay_attached() {
    STUB();
    return false;
}

bool platform_open_common_file_dialog(utf8 *outFilename, file_dialog_desc *desc) {
    STUB();
    return false;
}

void platform_get_exe_path(utf8 *outPath)
{
    strcpy(outPath, "/sdcard/openrct2");
}

void platform_posix_sub_user_data_path(char *buffer, const char *homedir, const char *separator) {
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = paths.firstObject;
    
    strcpy(buffer, [basePath UTF8String]);
}

void platform_show_messagebox(char *message)
{
    STUB();
}

utf8 *platform_open_directory_browser(utf8 *title)
{
    log_info(title);
    STUB();
    return "/sdcard/rct2";
}

bool platform_get_font_path(TTFFontDescriptor *font, utf8 *buffer)
{
    STUB();
    return false;
}

void platform_posix_sub_resolve_openrct_data_path(utf8 *out) {
    strcpy(out, "/sdcard/openrct2");
}

float platform_get_default_scale() {
    return 1;
}
