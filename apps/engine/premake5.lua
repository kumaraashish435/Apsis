-- premake5.lua
workspace "New Project"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   -- Keep generated Makefiles/project files out of the source tree
   location "build/premake"

   -- Windows needs an explicit architecture for Visual Studio's platform configs.
   -- macOS/Linux are left unset so gmake builds for the host's native arch
   -- (e.g. arm64 on Apple Silicon) instead of being forced to x86_64.
   filter "system:windows"
      architecture "x64"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
   filter {}

-- Architecture is intentionally omitted: it's unset on macOS/Linux so builds
-- target the host's native arch, and including it would leave an empty
-- "system-/config" segment in the output path there.
OutputDir = "%{cfg.system}/%{cfg.buildcfg}"

-- --------------------------------------------------------------------
-- AstroStds (SGP4Prop) vendored library location, resolved per host
-- platform/arch. Shared by Propagate (compiles against it) and by every
-- executable that links Propagate (App, Tests -- needs it discoverable
-- at runtime).
-- --------------------------------------------------------------------
local function astroStdsPlatformDir()
   if os.target() == "windows" then
      return "windows-x64"
   elseif os.target() == "macosx" then
      return "macos-arm64"
   else -- linux
      local arch = os.outputof("uname -m") or ""
      if arch:find("aarch64") or arch:find("arm64") then
         return "linux-arm64"
      end
      return "linux-x64"
   end
end

AstroStdsLibDir = path.getabsolute("Vendor/AstroStds/lib/" .. astroStdsPlatformDir())

-- Called by Propagate's own project (it compiles GpDataLoader.cpp, which
-- reads this macro to set LD_LIBRARY_PATH before the first Sgp4InitSat()).
function AstroStdsCompileDefine()
   defines { 'ASTROSTDS_LIB_DIR="' .. AstroStdsLibDir .. '"' }
end

-- Called by each *executable* project that links Propagate, so the
-- AstroStds libraries it dlopen()s at runtime are discoverable:
--   - macOS: rpath pointing at the vendored lib directory.
--   - Linux: same, plus --disable-new-dtags so the rpath is old-style
--     DT_RPATH (transitively honored when a dlopen()'d library pulls in
--     its own further dependencies, e.g. libgfortran), not DT_RUNPATH
--     (which is not).
--   - Windows: no rpath concept -- copy the DLLs next to the .exe after
--     each build instead, so LoadLibraryA()'s default search order finds
--     them.
function AstroStdsRuntimeConfig()
   filter "system:macosx"
      linkoptions { "-Wl,-rpath," .. AstroStdsLibDir }

   filter "system:linux"
      linkoptions { "-Wl,--disable-new-dtags", "-Wl,-rpath," .. AstroStdsLibDir }

   filter "system:windows"
      postbuildcommands {
         '{COPY} "' .. AstroStdsLibDir .. '" "%{cfg.targetdir}"'
      }

   filter {}
end

group "Core"
	include "Core/Build-Core.lua"
group ""

group "Propagate"
	include "Propagate/Build-Propagate.lua"
group ""

include "App/Build-App.lua"

group "Vendor"
	include "Vendor/Build-GoogleTest.lua"
group ""

include "Tests/Build-Tests.lua"