KongMing Chess
======================================
## 项目简介：
孔明棋（国外称为独粒钻石棋）与华容道、魔方一起被称为智力游戏界的三大发明。初始时，你会得到一个已经摆好棋子的棋盘（如图一所示）。该游戏的行棋规则是当某棋子跳过相邻的棋子到空位上，就把被跳过的棋子拿掉。棋子可以沿横、纵方向跳，但不能斜跳。每跳一步，棋子数量会减少一个，**行棋的目标**是只剩下一个棋子在盘中央。

![image](https://github.com/shsjxzh/KongMing-Chess/blob/master/picture/example.JPG)

这个项目的目的是开发一个高效率的孔明棋问题的求解程序。有兴趣的朋友完全可以当成一道复杂一点的ACM赛题来试试。
这个项目采用了最常见的英式棋盘（如下图所示）并给出了对于棋盘坐标的定义。所有的棋子摆放在交叉点上。**注意**英式棋盘的四个角上是不允许棋子走入的。

![image](https://github.com/shsjxzh/KongMing-Chess/blob/master/picture/board.jpg)

**如果你有好的的想法，Just pull it!**:grin:

## 为什么会做这样一个项目？
在我拥有的第一部手机上，内置了一个解孔明棋的游戏。从此，我便迷上了这样一个小游戏，并萌生了做一个这种游戏的解法生成器的想法。开始时我的想法是寻找孔明棋的特殊的数学规律，像玩魔方那样寻找出解决问题的公式，然而并没有什么收获。进入计算机专业之后，随着编程技能的成熟和许多专业知识的积累，我开始了采用暴力搜索来解决这一问题的尝试。于是，就有了现在这个项目。

## Input Format:
    一个7*7的01矩阵，1和0分别表示在棋盘的这个位置上有还是没有棋子。更多测试样例可以在本项目test_case文件夹中找到。
## Ouput Format:
    每次如何移动棋子。每一步的基本格式为(x1,y1)->(x2,y2)，表示在图二所示的坐标中，(x1,y1)位置的棋子移动到了位置(x2,y2)。
## Sample input:
    0 0 0 0 0 0 0
    0 0 0 1 0 0 0
    0 0 0 1 0 0 0
    0 1 1 1 1 1 0
    0 0 0 1 0 0 0
    0 0 0 1 0 0 0
    0 0 0 0 0 0 0
    （此即图一所示的“大十字”）
## Sample ouput:
    (4 ,3)->(6 ,3)
    (2 ,3)->(4 ,3)
    (3 ,1)->(3 ,3)
    (3 ,3)->(5 ,3)
    (6 ,3)->(4 ,3)
    (3 ,5)->(3 ,3)
    (4 ,3)->(2 ,3)
    (1 ,3)->(3 ,3)
    
## 目前所做到的：
一开始考虑暴力搜索算法就会非常吃力。所以采用了模拟退火算法，但效果还不是很好。（见Simulated_Annealing文件夹）<br>
接着回归了暴力搜索，但是进行了如下优化：<br>
1. 采用了类似字符串哈希的方法将每个局面哈希成为一个长整数。由于棋局情况有限，所以可以做到无冲突的哈希。
2. 采取了一些小技巧，使得通过哈希就可以将大部分经过旋转或翻折之后重复的棋局去掉。
3. 将问题的解答步骤存成了一个动态的链表，最大限度去除存储答案时的冗余。<br>
经过以上优化之后，该算法在Intel(R) Core(TM) i7-6700HQ，8G内存，win10操作系统上运行，以64位编译器运行。在最复杂的棋盘情况（即test_case文件夹下test_difficult中最后一个）下在16分钟内运行完成。

## 参考文献&致谢：
[独粒钻石棋的化学反应优化解法](https://github.com/shsjxzh/KongMing-Chess/blob/master/%E7%8B%AC%E7%B2%92%E9%92%BB%E7%9F%B3%E6%A3%8B%E7%9A%84%E5%8C%96%E5%AD%A6%E5%8F%8D%E5%BA%94%E4%BC%98%E5%8C%96%E8%A7%A3%E6%B3%95.pdf) <br>
[手把手教会你模拟退火算法](http://www.cnblogs.com/CsOH/p/6049117.html)<br>
感谢刘予希同学对于字符串哈希的讲解，感谢王天哲同学提供的关于如何去重的哈希技巧讲解，以及冯思远同学对程序内存优化的一系列指导。感谢你们的帮助，使我圆了这一个的小梦想:grin:！

## 孔明棋游戏：
这里提供一个孔明棋游戏的链接，方便大家直观感受这个游戏：
[孔明棋游戏](http://www.4399.com/flash/86377_4.htm)