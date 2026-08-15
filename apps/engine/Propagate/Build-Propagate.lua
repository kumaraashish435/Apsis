project "Propagate"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
      "../Vendor/AstroStds/include",
      "../Vendor/AstroStds/include/wrappers",
      "../Vendor/AstroStds/include/services"
   }

   -- The SGP4 engine's Sgp4InitSat() looks for SGP4_Open_License.txt via
   -- LD_LIBRARY_PATH at call time (GpDataLoader.cpp sets that env var
   -- itself using this macro), so this is needed at compile time.
   AstroStdsCompileDefine()

   targetdir ("../build/bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../build/obj/" .. OutputDir .. "/%{prj.name}")

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
