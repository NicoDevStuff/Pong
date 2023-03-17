workspace "Pong"
   configurations { "Debug", "Release" }

project "Pong"
   kind "ConsoleApp"
   language "C"
   targetdir "./build/%{cfg.buildcfg}"

   files {
      "src/*.c",
   }

   links {
      "raylib",
      "GL",
      "m",
      "pthread",
      "dl",
      "rt",
      "X11",
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "Full"
   filter { "system:linux", "action:gmake" }
      buildoptions { "-DBX_CONFIG_DEBUG=1" }
