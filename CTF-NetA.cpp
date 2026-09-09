#include <windows.h>
#include <string>
#include <cstdint>
#include <filesystem>

template<class T>
T get(HMODULE h, const char* n)
{
    return reinterpret_cast<T>(GetProcAddress(h, n));
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	wchar_t exe[MAX_PATH];
	GetModuleFileNameW(nullptr, exe, MAX_PATH);

	auto exeDir = std::filesystem::path(exe).parent_path();

	auto python = (exeDir / L"plugins" / L"python3" / L"python.exe").wstring();

	auto qt = LoadLibraryW(
		(exeDir / L"_internal" / L"PyQt6" / L"Qt6" / L"bin" / L"Qt6Core.dll").c_str()
	);

    std::wstring code = LR"PY(
import sys
import module.core as core
import module.getVersion as getVersion

def no_update(self):
    return False, self.current_version, ""

getVersion.UpdateChecker.check_update = no_update

def patched_checkregister(self):
    self.money = 99999
    self.comment = "SuperVip"
    self.activation = True
    return True
    
core.MyMainForm.checkregister = patched_checkregister

window, app1 = core.main()

window.show()
window.run()

sys.exit(app1.exec())
)PY";

    std::wstring packed = std::wstring(L"-c\0", 3) + code;

    using QString = void* (__fastcall*)(void*, const wchar_t*, int64_t);
    using Split   = void* (__fastcall*)(void*, void*, wchar_t, int, int);
    using Start   = bool  (__fastcall*)(const void*, const void*, const void*, int64_t*);

    auto QString_new = get<QString>(qt,
        "??0QString@@QEAA@PEBVQChar@@_J@Z");

    auto split = get<Split>(qt,
        "?split@QString@@QEBA?AV?$QList@VQString@@@@VQChar@@V?$QFlags@W4SplitBehaviorFlags@Qt@@@@W4CaseSensitivity@Qt@@@Z");

    auto startDetached = get<Start>(qt,
        "?startDetached@QProcess@@SA_NAEBVQString@@AEBV?$QList@VQString@@@@0PEA_J@Z");

    void* qPython[3]{};
    void* qPacked[3]{};
    void* qArgs[3]{};
    void* qWorkingDir[3]{};

    QString_new(qPython, python.data(), python.size());
    QString_new(qPacked, packed.data(), packed.size());

    split(qPacked, qArgs, 0, 0, 1);

    return startDetached(
        qPython,
        qArgs,
        qWorkingDir,
        nullptr
    ) ? 0 : 1;
}