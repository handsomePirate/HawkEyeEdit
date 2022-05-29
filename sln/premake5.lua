workspace "HawkEyeEdit"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "HawkEyeEdit"
	location ""
	
	flags
	{
		"MultiProcessorCompile"
	}
	
include "../dependencies.lua"
	
include "../proj/HawkEyeEdit"