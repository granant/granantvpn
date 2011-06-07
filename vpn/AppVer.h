
// Stringisation.
// (Two steps needed).

#define _STR(x) #x
#define STR(x) _STR(x)

// The following are used as defined quantities in the versioninfo resource:

#define VERMAJ 1
#define VERMIN 0
#define VERFIX	 0
#define BUILDNUMBER 8

#define VERMAJSTR	STR( VERMAJ )
#define VERMINSTR	STR( VERMIN )
#define VERFIXSTR	STR( VERFIX )
#define BUILDSTR	STR( BUILDNUMBER )
#define VERSIONSTRING	VERMAJSTR "." VERMINSTR "." VERFIXSTR "." BUILDSTR
#define PRODUCTSTRING	VERMAJSTR "." VERMINSTR "." VERFIXSTR "." BUILDSTR
