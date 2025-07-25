# PlantsVsZombies-AndroidTV

[![许可协议](https://img.shields.io/github/license/Dicot0721/PlantsVsZombies-AndroidTV)
](https://www.gnu.org/licenses/gpl-3.0.html)

*一个基于植物大战僵尸 TV 版的改版*

## 待办事项

- [x] 将函数的声明移至头文件中 (短期)
- [x] 将 PvZ 中的成员函数移至对应的类内 (中期)
- [x] 对于无需修改的 PvZ 成员函数, 在函数体中调用符号地址, 而不是将符号地址赋值给函数指针 (中期)
- [ ] 重构修改器功能 (长期)

## 构建

> [!NOTE]
> 需要资源文件请联系仓库作者

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

## 编码风格

### 命名

- 类: `ClassName`
- 变量: `varName`
- 命名空间: `name_space_name`
- 宏/常量/枚举量: `MACRO_NAME`

### 其他

见项目根目录下的 `.clang-format` 文件.

> 建议在每次提交更改前先用 IDE 对代码进行格式化.

## 许可协议

```
Copyright (C) 2023-2025  PvZ TV Touch Team

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
```
