在 Windows 上安装 SQLite


1.请访问 SQLite 下载页面，从 Windows 区下载预编译的二进制文件。

2.您需要下载 sqlite-shell-win32-*.zip 和 sqlite-dll-win32-*.zip 压缩文件。

3.创建文件夹 C:\sqlite，并在此文件夹下解压上面两个压缩文件，将得到 sqlite3.
def、sqlite3.dll 和 sqlite3.exe 文件。

4.添加 C:\sqlite 到 PATH 环境变量，最后在命令提示符下，使用 sqlite3 命令，将显示如下结果。
C:\>sqlite3
SQLite version 3.7.15.2 2013-01-09 11:53:05
Enter ".help" for instructions
Enter SQL statements terminated with a ";"
sqlite>