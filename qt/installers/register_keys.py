import sys
import os
import ctypes

def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False

if not is_admin():
    # Relaunch as admin
    params = " ".join([f'"{arg}"' for arg in sys.argv])
    ctypes.windll.shell32.ShellExecuteW(
        None, "runas", sys.executable, params, None, 1
    )
    sys.exit(0)

# --- Elevated code below ---
import winreg  # can now access HKLM etc.

print("Running with administrator privileges!")
install_prefix = sys.argv[1].replace("/", "\\")

if install_prefix.endswith("\\") or install_prefix.endswith("\""):
    install_prefix = install_prefix[:-1]

with open("debug.txt", "w") as f:
    version = f.write(install_prefix)

key = winreg.CreateKeyEx(
    winreg.HKEY_CLASSES_ROOT,
    r"qtsample",
    0,
    winreg.KEY_WRITE | winreg.KEY_WOW64_64KEY,
)
winreg.SetValueEx(key, None, 0, winreg.REG_SZ, f"URL:qtsample protocol")
winreg.CloseKey(key)

key = winreg.CreateKeyEx(
    winreg.HKEY_CLASSES_ROOT,
    r"qtsample",
    0,
    winreg.KEY_WRITE | winreg.KEY_WOW64_64KEY,
)
winreg.SetValueEx(key, "URL Protocol", 0, winreg.REG_SZ, f"")
winreg.CloseKey(key)

key = winreg.CreateKeyEx(
    winreg.HKEY_CLASSES_ROOT,
    r"qtsample\\Shell\\Open\\Command",
    0,
    winreg.KEY_WRITE | winreg.KEY_WOW64_64KEY,
)
winreg.SetValueEx(key, None, 0, winreg.REG_SZ, f"\"{install_prefix}\\bin\\qtsample.exe\" \"%1\"")
winreg.CloseKey(key)

print("Registry value written successfully.")