# Md5Gen

## 项目简介
开学后的第一个项目是开发一个通过哈希值检查文件夹完整性的程序。

## 安装步骤
1. 克隆仓库：
   ```bash
   git clone https://github.com/opennessgames/Md5Gen.git
2. 进入项目目录 
    ```bash
    cd Md5Gen
    ```
2. 编译
    ```bash
    make
    ```
3.  安装
    ```bash
    sudo make install
    ```
## 用法
1. 生成文件夹的哈希值
```bash
md5gen -md5 md5.json -dir ./FolderPath -mode 1
```
2. 检查文件夹的哈希值一致性
```bash
md5gen -md5 md5.json -dir ./FolderPath -mode 2
```