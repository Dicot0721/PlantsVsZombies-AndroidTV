<div align="center">

# PlantsVsZombies AndroidTV

A mod of Plants vs. Zombies Android TV version.

[![license](https://img.shields.io/github/license/Dicot0721/PlantsVsZombies-AndroidTV)][GPL-3.0]
[![Android CI](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml/badge.svg)](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml "Android CI")

**English** | **[简体中文](./README.zh-cn.md)**

</div>

## Build

> [!NOTE]
> If you need resource files, please contact the repository author.

Use Android Studio or another IDE that supports Gradle, requiring the following to be installed:

- Android SDK Platform 34
- NDK v27.2.12479018
- CMake v3.20+

Signing configuration is specified in the file `keystore.properties` located in the project root directory (you must create this file yourself). The file content format is as follows:

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

> It is recommended to format the code using the IDE before each commit.

See the `.clang-format` file in the project root directory.

## License

The source code for this project is licensed under the [GPL-3.0][GPL-3.0] license.

This project is not associated with or endorsed by TRANSMENSION, PopCap or Electronic Arts.

[GPL-3.0]: https://www.gnu.org/licenses/gpl-3.0.html "GPL-3.0"
