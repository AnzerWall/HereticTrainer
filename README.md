# HereticTrainer
音乐游戏模拟训练工具
heretictrainer 
V 1.0
    完成了基本的效果和判定。
V 1.1
 1、增加对llp的json格式的支持。
 2、调整触摸区域为较大的不完整圆。
 3、增大perfect的判定难度
 4、不再依赖slist.txt，改为自行扫描sdcard\HereticTrainer\
 5、修复暂停有时候会崩溃的bug

用法：
	在sd卡建立目录，HereticTrainer。
	将slist或者llp的谱丢进去，命名（最好不要有空格和中文日文）。
	slist是X.slist或者是llp的谱是X.json，然后丢个同名的对应音乐文件X.mp3，对应封面是X.png（限定300*300，不然很怪）
	基于cocos2d-x ，自行下载引擎，建立空工程，引入文件即可。
