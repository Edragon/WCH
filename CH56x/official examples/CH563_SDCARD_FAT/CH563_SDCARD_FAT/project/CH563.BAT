SET PATH=F:\keilarm\ARM\ARMCC\bin\;D:\WinAVR\bin;D:\WinAVR\utils\bin;C:\Program Files\Intel\iCLS Client\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\IDM Computer Solutions\UltraEdit-32;D:\Program Files\Microsoft Visual Studio\Common\Tools\WinNT;D:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin;D:\Program Files\Microsoft Visual Studio\Common\Tools;D:\Program Files\Microsoft Visual Studio\VC98\bin;%WPS_HOME%;C:\Program Files\IDM Computer Solutions\UltraCompare;C:\BC\BIN
SET CPU_TYPE=STR912FAW44
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=CH563
SET CPU_CLOCK=0x017D7840
"F:\keilarm\ARM\ARMCC\bin\ArmCC" --Via ".\obj\ch563.__i"
"F:\keilarm\ARM\ARMCC\bin\ArmCC" --Via ".\obj\sd.__i"
"F:\keilarm\ARM\ARMCC\bin\ArmCC" --Via ".\obj\ehci_hcd.__i"
"F:\keilarm\ARM\ARMCC\bin\ArmCC" --Via ".\obj\udisk.__i"
"F:\keilarm\ARM\ARMCC\bin\ArmCC" --Via ".\obj\udisk_fat.__i"
"F:\keilarm\ARM\ARMCC\bin\ArmAsm" --Via ".\obj\startup._ia"
"F:\keilarm\ARM\ARMCC\bin\ArmCC" --Via ".\obj\sysfreq.__i"
"F:\keilarm\ARM\ARMCC\bin\ArmLink" --Via ".\Obj\CH563.lnp"
"F:\keilarm\ARM\ARMCC\bin\fromelf.exe" ".\Obj\CH563.axf" --i32combined --output ".\Obj\CH563.hex"
