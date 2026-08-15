-- Root premake5.lua: thin wrapper so `premake5` can be run from the repo
-- root and still build apps/engine. apps/engine/premake5.lua remains the
-- single source of truth for the actual workspace/project configuration.
include "apps/engine"
