#include <windows.h>
#include <MinHook.h>
#include <jni.h>

typedef void *(*define_class0_t)(JNIEnv *, jclass, jobject, jclass, jstring, jbyteArray, jint, jint, jobject, jboolean, jint, jobject);
define_class0_t original_define = nullptr;

void write_class_to_file(const char *name, const char *data, size_t length)
{
    const char *output_dir = "classes";
    CreateDirectoryA(output_dir, NULL);

    char filename[256];
    sprintf(filename, "%s/%s.class", output_dir, name);

    char dir_path[256];
    strcpy(dir_path, filename);

    char *last_slash = strrchr(dir_path, '/');
    if (last_slash)
    {
        *last_slash = '\0';
        for (char *p = dir_path; *p; p++)
        {
            if (*p == '/')
            {
                *p = '\0';
                CreateDirectoryA(dir_path, NULL);
                *p = '/';
            }
        }
        CreateDirectoryA(dir_path, NULL);
    }

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        printf("Failed to create file: %s\n", filename);
        return;
    }

    fwrite(data, 1, length, file);
    fclose(file);
}

// Java_java_lang_ClassLoader_defineClass0
void *define_class0(JNIEnv *env, jclass cls, jobject loader, jclass lookup, jstring name, jbyteArray data, jint offset, jint length, jobject pd, jboolean initialize, jint flags, jobject classData)
{
    char *name_str = (char *)env->GetStringUTFChars(name, nullptr);
    printf("class loading: %s\n", name_str);

    auto *byte_array = env->GetByteArrayElements(data, nullptr);
    write_class_to_file(name_str, (char *)byte_array, length);

    // clean up
    env->ReleaseByteArrayElements(data, byte_array, 0);
    env->ReleaseStringUTFChars(name, name_str);

    return original_define(env, cls, loader, lookup, name, data, offset, length, pd, initialize, flags, classData);
}

bool init_hooks()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    if (MH_Initialize() != MH_OK)
    {
        return false;
    }

    auto java_handle = GetModuleHandleA("java.dll");

    if (!java_handle)
    {
        printf("Failed to get handle for java.dll\n");
        return false;
    }

    auto def_address = (void *)GetProcAddress(java_handle, "Java_java_lang_ClassLoader_defineClass0");

    if (!def_address)
    {
        printf("Failed to get address for Java_java_lang_ClassLoader_defineClass0\n");
        return false;
    }

    printf("hooking: %p...\n", def_address);

    MH_CreateHook(
        (void **)def_address,
        &define_class0,
        (void **)&original_define);

    MH_EnableHook(MH_ALL_HOOKS);

    printf("hooks initialized\n");
    return true;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID)
{

    if (reason == DLL_PROCESS_ATTACH)
    {
        if (!init_hooks())
            return FALSE;
    }

    return TRUE;
}