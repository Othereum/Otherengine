# Configuration Files

[한국어](../한국어/환경설정%20파일.md)

Otherengine provides a configuration system that allows you to quickly change various settings without code modification or rebuild. In this guide, we will explore some of the features and use of the Otherengine's configuration system.

## Basic Syntax

The configuration file is written in JSON, and the file extension is `.json` or `.jsonc`. The file name will be the configuration name.

```jsonc
// Display.json
/* Comments available */
{
    "Display": 0,
    "DisplayMode": 0,
    "Fullscreen": true
}
```

```jsonc
// Input.json
{
    "AxisConfig": {
        "CtrlAxis.leftx": {
            "DeadZone": 0.25,
            "Sensitivity": 1.0
        }
    }
}
```

## File Path

Configuration files can be located in the paths below:

1. `{gamePath}`/Engine/Config
1. `{gamePath}`/Config
1. `{userDataPath}`/Config

`{gamePath}`: Game project path  
`{userDataPath}`: Path where user-specific data is stored. Note table below:

Build＼OS | **Windows** | **Linux/OS X**
---: | --- | ---
**Shipping** | `{userDocumentsPath}\{gameName}` | `~/.{gameName}`
**Debug/Development** | `{gamePath}\Saved` | `{gamePath}/Saved`

`{userDocumentsPath}`: User 'Documents' path. Default is `%USERPROFILE%\Documents`  
`{gameName}`: Game name defined in game module `DEFINE_GAME_MODULE(game_name)`

While the engine is initializing, the configuration files will be loaded in the order above. This allows you to modify the previously loaded configuration from the loaded file later.

## Add/Modify/Remove

Rather than completely overwriting the configuration, you may want to modify it only slightly. To do this, Otherengine supports the following syntax:

```jsonc
{
    // Add/override ActionMap
    "ActionMap": {
        "Flash": [
            {
                "Code": "Keycode.F"
            }
        ]
    },

    // Add/modify AxisMap
    "+AxisMap": {
        // Add/modify LookUp to/in AxisMap
        "+LookUp": [
            // Add to AxisMap.LookUp
            {
                "Code": "MouseAxis.Y",
                "Scale": 1.0
            }
        ],

        // Remove [0], [2] from AxisMap.MoveRight
        "-MoveRight": [0, 2]
    },

    // Remove MoveForward from AxisMap
    "-AxisMap": ["MoveForward"],
}
```

- The removal operation precedes any operation.
- If the target you want to remove does not exist, a warning is logged.

## Accessing Configurations

To use configurations in code:

```cpp
auto& cfg_sys = ConfigSystem::Get();

// MyConfig.json
const Name cfg_name = u8"MyConfig"sv;
auto& my_cfg = cfg_sys(cfg_name);

// Get "MyInt" property value from MyConfig.json
const auto my_int = cfg_name.at("MyInt").get<int>();

// You can also modify it.
cfg_name["MyFloat"] = 3.14_f;

// See the nlohmann-json document for detailed syntax (link below)

// You can also save it. Saved in path {userDataPath}/Config
cfg_sys.Save(cfg_name);
```

nlohmann-json usage examples: <https://github.com/nlohmann/json#examples>
