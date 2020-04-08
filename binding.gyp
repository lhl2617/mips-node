{
    "targets": [
      {
        "target_name": "mips-parser",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        'xcode_settings': {
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
          'CLANG_CXX_LIBRARY': 'libc++',
          'MACOSX_DEPLOYMENT_TARGET': '10.7',
        },
        'msvs_settings': {
          'VCCLCompilerTool': { 'ExceptionHandling': 1 },
        },
        "sources": [
          "./src/parser/instructionList.cpp",
          "./src/parser/parser.cpp",
          "./src/simulator/src/functionMap.cpp",
          "./src/simulator/src/ISA-I.cpp",
          "./src/simulator/src/ISA-J.cpp",
          "./src/simulator/src/ISA-R.cpp",
          "./src/simulator/src/memory.cpp",
          "./src/simulator/src/simulator.cpp",
          "./src/simulator/src/UI.cpp",
          "./src/main.cpp",
          "./src/index.cpp"
        ],
        "include_dirs": [
          "<!@(node -p \"require('node-addon-api').include\")",
                "C:\\Program Files\\boost\\boost_1_72_0"
        ],
        "libraries": [
          "C:\\Program Files\\boost\\boost_1_72_0\\stage\\lib\\libboost_serialization-vc142-mt-s-x64-1_72.lib"
        ]
      }
    ]
  }