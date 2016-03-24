一、在 Windows 上安装 SQLite


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


二、Database_Connection.hpp 使用说明
    1. 通过初始化一个result_obtainer类对象可获得想要的数据，初始化时请注意参数：
	第一个参数string：起始城市；
	第二个参数int：最早出发时间。（无冒号，如7:00为700，最大四位）；
	第三个参数string：偏好。（只有“time”和“price”两个选择，分别代表最早到达和最低价格）；
	第四个参数string：旅行方式。有默认值，默认值为ALL（从所有三种方式中选择），可输入参数为
			（”CAR”/“TRAIN”/“FLIGHT”)。
	
    2. result_obtainer类对象包含Get_Result()方法，其用来获得满足条件的结果，并将结果保存在
	调用其的对象public域中的all_result里面；
    3. 关于all_result:
	all_result是一个头指针，其本身不保存任何数据。数据（如果有的话）从all_result的nextptr开始
	保存，每一个节点包含一条完整的记录（含有字段：交通工具编号NUMBER, 起始城市START, 到达城市END, 交通工具METHOD, 出发时间DEPARTURE_TIME, 到达时间ARRIVAL_TIME, 旅行时间DURATION, 价格PRICE），所有字段均为string。

三、注意事项
1.如无法正确打开The_Map.db请检查路径设置；
2.sqlite3.h和sqlite3.c是Sqlite数据库头文件和定义；
3.Small_Funs.cpp/hpp仅包含一些浅显易懂小函数；
