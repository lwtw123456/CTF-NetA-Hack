#include <windows.h>
#include <filesystem>
#include <string>
#include <vector>

static std::wstring quote_arg(const std::wstring& s)
{
    std::wstring out = L"\"";
    size_t slash = 0;

    for (wchar_t c : s)
    {
        if (c == L'\\')
        {
            ++slash;
        }
        else if (c == L'"')
        {
            out.append(slash * 2 + 1, L'\\');
            out += L'"';
            slash = 0;
        }
        else
        {
            out.append(slash, L'\\');
            slash = 0;
            out += c;
        }
    }

    out.append(slash * 2, L'\\');
    out += L'"';
    return out;
}

int WINAPI wWinMain(
    HINSTANCE,
    HINSTANCE,
    PWSTR,
    int)
{
    wchar_t exe[MAX_PATH];

    if (!GetModuleFileNameW(nullptr, exe, MAX_PATH))
		
        return 1;

    auto exeDir = std::filesystem::path(exe).parent_path();
    auto python = exeDir / L"plugins" / L"python3" / L"python.exe";

    if (!std::filesystem::exists(python))
        return 2;

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

    std::wstring cmd =
        quote_arg(python.wstring()) +
        L" -c " +
        quote_arg(code);

    std::vector<wchar_t> buffer(cmd.begin(), cmd.end());
    buffer.push_back(L'\0');

    STARTUPINFOW si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);

	if (!CreateProcessW(
		python.c_str(),
		buffer.data(),
		nullptr,
		nullptr,
		FALSE,
		CREATE_NO_WINDOW,
		nullptr,
		exeDir.c_str(),
		&si,
		&pi))
	{
		return 3;
	}

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}