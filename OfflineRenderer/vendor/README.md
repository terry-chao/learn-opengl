# Vendor

| 库 | 路径 | 类型 | 用途 |
|---|---|---|---|
| glm | `vendor/glm` | header-only | 数学库（`#include <glm/glm.hpp>`） |
| minifb | `vendor/minifb` | 源码编入工程 | 轻量窗口 / 帧缓冲显示（`#include <MiniFB.h>`） |

## 工程接入说明

- **头文件**：`AdditionalIncludeDirectories` 含 `vendor/glm`、`vendor/minifb/include`、`vendor/minifb/src`
- **源码**：Windows 后端相关 `.c` / `.cpp` 已加入 `OfflineRenderer.vcxproj` 一起编译（GDI，未启用 OpenGL 后端）
- **库**：链接 `winmm.lib`（minifb 定时器需要）

来源：

- https://github.com/g-truc/glm
- https://github.com/emoon/minifb
