project "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",

	  -- Include Core
	  "../Core/Source",
	  "../Vendor/Eigen",

	  -- Include Propagate (GpDataLoader / SatelliteTracker)
	  "../Propagate/Source"
   }

   links
   {
      "Core",
      "Propagate"
   }

   targetdir ("../build/bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../build/obj/" .. OutputDir .. "/%{prj.name}")

   -- AstroStds (SGP4Prop) libraries that Propagate dlopen()s at runtime
   -- need to be discoverable from this executable -- see premake5.lua.
   AstroStdsRuntimeConfig()

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
