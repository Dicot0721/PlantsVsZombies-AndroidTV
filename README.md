<div align="center">

# PlantsVsZombies AndroidTV

**English** | **[简体中文](./README.zh-cn.md)**

[![license](https://img.shields.io/github/license/Dicot0721/PlantsVsZombies-AndroidTV)][GPL-3.0]
[![Android CI](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml/badge.svg)](https://github.com/Dicot0721/PlantsVsZombies-AndroidTV/actions/workflows/android.yml "Android CI")

A mod of _Plants vs. Zombies_ Android TV version.

</div>

## Build

- Ensure the following is installed:
    * Android SDK Platform 34
    * NDK v27.2.12479018 (r27c)
    * CMake v3.20+

- Clone the repository.
    ```shell
    git clone https://github.com/Dicot0721/PlantsVsZombies-AndroidTV.git
    ```

- Copy assets files to the path `PlantsVsZombies-AndroidTV/app/src/main/assets/`.
    > If you need resource files, please contact the repository author.

- Build with:
    * Android Studio: Click on the build button.
    * Command line: Run the following command:
        ```shell
        cd PlantsVsZombies-AndroidTV
        ./gradlew assembleDebug
        ```

- If release, configure signing using the file `keystore.properties` located in the project root directory (you must
  create this file yourself). The file content format is as follows:
    ```properties
    storePassword=myStorePassword
    keyPassword=mykeyPassword
    keyAlias=myKeyAlias
    storeFile=myStoreFileLocation
    ```

## Contributing

### Coding Style (C++)

**Name Convention**

- Function / Type / Concept: `PascalCase`
- Variable: `camelCase`
- Namespace: `snake_case`
- Macro / Constant / Enumerator / Non-type template parameter: `UPPER_CASE`

**Format**

See the file `.clang-format` in the project root directory.

> It is recommended to format the code using the IDE before each commit.

### Commit

See _[AngularJS Commit Message Guidelines](https://github.com/angular/angular/blob/20.0.x/CONTRIBUTING.md#-commit-message-guidelines)_.

### Pull Request (PR)

Send the PR to branch `dev`.

## License

The source code for this project is licensed under the [GPL-3.0][GPL-3.0] license.

This project is not associated with or endorsed by TRANSMENSION, PopCap or Electronic Arts.

[GPL-3.0]: https://www.gnu.org/licenses/gpl-3.0.html "GPL-3.0"
