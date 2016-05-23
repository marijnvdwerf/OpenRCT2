
@import UIKit;
@import Foundation;
#include <mach-o/dyld.h>
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
    char exePath[MAX_PATH];
    uint32_t size = MAX_PATH;
    int result = _NSGetExecutablePath(exePath, &size);
    if (result != 0) {
        log_fatal("failed to get path");
    }
    exePath[MAX_PATH - 1] = '\0';
    char *exeDelimiter = strrchr(exePath, platform_get_path_separator());
    if (exeDelimiter == NULL)
    {
        log_error("should never happen here");
        outPath[0] = '\0';
        return;
    }
    int exeDelimiterIndex = (int)(exeDelimiter - exePath);
    
    safe_strcpy(outPath, exePath, exeDelimiterIndex + 1);
    outPath[exeDelimiterIndex] = '\0';
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
    @autoreleasepool
    {
        NSBundle *bundle = [NSBundle mainBundle];
        if (bundle)
        {
            const utf8 *resources = bundle.resourcePath.UTF8String;
            if (platform_directory_exists(resources))
            {
                out[0] = '\0';
                safe_strcpy(out, resources, MAX_PATH);
                return;
            }
        }
    }
}

float platform_get_default_scale() {
    return 1;
}
