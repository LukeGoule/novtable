**noVTable**

This is a simple PoC for Garry's Mod to show that you do not actually need vtables to hack a game efficiently. Detours does most of the work.

To compile, change pch.h to include detours.h and add detours.lib using
`#pragma comment(lib, "path/to/detours.lib")`


Credits to MarkHC for some of the CSGOSimple base stuff.