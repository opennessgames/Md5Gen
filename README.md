# Md5Gen

## 项目简介
开学后的第一个项目是开发一个通过哈希值检查文件夹完整性的程序。该项目是基于之前的Python项目进行改造的。之所以选择做这个项目，是因为在管理大量文件夹时，常常会遇到文件丢失等意外情况。我寻思着整这个项目，以确保文件夹的完整性和安全性。
## 安装步骤
1. 安装依赖项
   ```bash
    sudo apt-get install make cmake libssl-dev
   ```
2. 克隆仓库：
   ```bash
   git clone https://github.com/opennessgames/Md5Gen.git
   ```
3. 进入项目目录 
    ```bash
    cd Md5Gen
    ```
4. 编译
    ```bash
    make
    ```
5.  安装
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
## 友情提示
1.喜欢的话记得点个⭐️支持一下~ 谢谢喔<br>
2.如果你有任何改进建议或反馈，请随时告诉我们，我们很乐意听取你的意见。<br>
3.我是新手,大佬轻喷~