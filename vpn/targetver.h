#pragma once

// Включение SDKDDKVer.h обеспечивает определение самой последней доступной платформы Windows.

// Если требуется выполнить построение приложения для предыдущей версии Windows, включите WinSDKVer.h и
// задайте для макроопределения _WIN32_WINNT значение поддерживаемой платформы перед вхождением SDKDDKVer.h.

#include <SDKDDKVer.h>


// Stringisation.
// (Two steps needed).

#define _STR(x) #x
#define STR(x) _STR(x)

// The following are used as defined quantities in the versioninfo resource:

#define VERMAJ 1
#define VERMIN 0
#define VERFIX	 1
#define MYBUILDNUMBER 7

#define VERMAJSTR	STR( VERMAJ )
#define VERMINSTR	STR( VERMIN )
#define VERFIXSTR	STR( VERFIX )
#define BUILDSTR	STR( MYBUILDNUMBER )
#define VERSIONSTRING	VERMAJSTR "." VERMINSTR "." VERFIXSTR "." BUILDSTR
#define PRODUCTSTRING	VERMAJSTR "." VERMINSTR "." VERFIXSTR "." BUILDSTR
