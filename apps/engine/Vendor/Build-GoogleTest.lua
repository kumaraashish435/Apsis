-- Builds GoogleTest from the vendored submodule (Vendor/googletest), mirroring
-- the GTest::gtest / GTest::gtest_main split that the CMake build gets for
-- free via add_subdirectory(Vendor/googletest).

GTestDir = "googletest/googletest"

project "GoogleTest"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files { GTestDir .. "/src/gtest-all.cc" }

   includedirs
   {
      GTestDir .. "/include",
      GTestDir
   }

   targetdir ("../build/bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../build/obj/" .. OutputDir .. "/%{prj.name}")

project "GoogleTestMain"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files { GTestDir .. "/src/gtest_main.cc" }

   includedirs
   {
      GTestDir .. "/include"
   }

   targetdir ("../build/bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../build/obj/" .. OutputDir .. "/%{prj.name}")
