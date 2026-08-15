project "Tests"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
      "../Core/Source",
      "../Vendor/Eigen",
      "../Vendor/googletest/googletest/include",
      "../Propagate/Source"
   }

   links
   {
      "Core",
      "GoogleTest",
      "GoogleTestMain",
      "Propagate"
   }

   targetdir ("../build/bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../build/obj/" .. OutputDir .. "/%{prj.name}")

   -- AstroStds (SGP4Prop) libraries that Propagate dlopen()s at runtime
   -- need to be discoverable from this executable -- see premake5.lua.
   AstroStdsRuntimeConfig()

   filter "system:windows"
       systemversion "latest"

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
