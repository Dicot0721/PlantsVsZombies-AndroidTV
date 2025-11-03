<div align="center">

# PlantsVsZombies-AndroidTV

_一个基于植物大战僵尸 TV 版的改版._

[![license](https://img.shields.io/github/license/Dicot0721/PlantsVsZombies-AndroidTV)](https://www.gnu.org/licenses/gpl-3.0.html)
[![Android CI](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml/badge.svg)](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml)

**[English](./README.md)** | **简体中文**

</div>

## 构建

> [!NOTE]
> 需要资源文件请联系仓库作者.

使用 Android Studio 或其他支持 gradle 的 IDE, 要求已安装:

- Android SDK Platform 34
- NDK v27.2.12479018
- CMake v3.20+

签名配置位于项目根目录的 `keystore.properties` 文件中 (需要自行创建), 文件内容样式如下:

```properties
storePassword=myStorePassword
keyPassword=mykeyPassword
keyAlias=myKeyAlias
storeFile=myStoreFileLocation
```

## 编码风格 (C++)

### 命名约定

- 函数/类型/枚举/概念: `PascalCase`
- 变量: `camelCase`
- 命名空间: `snake_case`
- 宏/常量/枚举值/非类型模板参数: `UPPER_CASE`

### 格式

见项目根目录下的 `.clang-format` 文件.

> 建议在每次提交更改前先用 IDE 对代码进行格式化.

## 许可协议

本项目使用 [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.html) 许可进行授权。
