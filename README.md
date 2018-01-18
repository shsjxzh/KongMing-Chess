KongMing Chess
======================================
# 项目简介：
孔明棋（国外称为独立钻石棋）与华容道、魔方一起被称为智力游戏界的三大发明。初始时，你会得到一个已经摆好棋子的棋盘（如图一所示）。该游戏的行棋规则是当某棋子跳过相邻的棋子到空位上，就把被跳过的棋子拿掉。棋子可以沿横、纵方向跳，但不能斜跳。每跳一步，棋子数量会减少一个，**行棋的目标**是只剩下一个棋子在盘中央。

![image](https://github.com/shsjxzh/KongMing-Chess/blob/master/picture/example.JPG)

这个项目的目的是开发一个高效率的孔明棋问题的求解程序。有兴趣的朋友完全可以当成一道复杂一点的ACM赛题来试试。
这个项目采用了最常见的英式棋盘（如下图所示）并给出了对于棋盘坐标的定义。所有的棋子摆放在交叉点上。**注意**英式棋盘的四个角上是不允许棋子走入的。

![image](https://github.com/shsjxzh/KongMing-Chess/blob/master/picture/board.jpg)

这里提供一个在线孔明棋游戏的地址：[孔明棋游戏](http://www.4399.com/flash/86377_4.htm)。
   **如果你有好的想法，Just pull it!**:grin:

# Input Format:
    一个7*7的01矩阵，1和0分别表示在棋盘的这个位置上有还是没有棋子。更多测试样例可以在本项目test_case文件夹中找到。
# Ouput Format:
    每次如何移动棋子。每一步的基本格式为(x1,y1)->(x2,y2)，表示在图二所示的坐标中，(x1,y1)位置的棋子移动到了位置(x2,y2)。
# Sample input:
    0 0 0 0 0 0 0
    0 0 0 1 0 0 0
    0 0 0 1 0 0 0
    0 1 1 1 1 1 0
    0 0 0 1 0 0 0
    0 0 0 1 0 0 0
    0 0 0 0 0 0 0
    （此即图一所示的“大十字”）
# Sample ouput:
    (4 ,3)->(6 ,3)
    (2 ,3)->(4 ,3)
    (3 ,1)->(3 ,3)
    (3 ,3)->(5 ,3)
    (6 ,3)->(4 ,3)
    (3 ,5)->(3 ,3)
    (4 ,3)->(2 ,3)
    (1 ,3)->(3 ,3)