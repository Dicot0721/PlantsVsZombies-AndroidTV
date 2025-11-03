<div align="center">

# PlantsVsZombies-AndroidTV

_A mod of Plants vs. Zombies Android TV version._

[![license](https://img.shields.io/github/license/Dicot0721/PlantsVsZombies-AndroidTV)](https://www.gnu.org/licenses/gpl-3.0.html)
[![Android CI](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml/badge.svg)](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml)

**English** | **[简体中文](./README.zh-cn.md)**

</div>

## Build

> [!NOTE]
> If you require resource files, please contact the repository author.

Use Android Studio or another IDE that supports Gradle, requiring the following to be installed:

- Android SDK Platform 34
- NDK v27.2.12479018
- CMake v3.20+

Signing configuration is specified in the `keystore.properties` file located in the project root directory (you must create this file yourself). The file content format is as follows:

```properties
storePassword=myStorePassword
keyPassword=mykeyPassword
keyAlias=myKeyAlias
storeFile=myStoreFileLocation
```

## Coding Style (C++)

### Name Convention

- Function / Type / Enum / Concept: `PascalCase`
- Variable: `camelCase`
- Namespace: `snake_case`
- Macro / Constant / Enumerator / Non-type template parameter: `UPPER_CASE`

### Format

See the `.clang-format` file in the project's root directory.

> It is recommended to format your code using an IDE before each commit.

## License

This project is licensed under the [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.html) license.
