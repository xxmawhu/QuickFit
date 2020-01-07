# QuickFit
用RooFit做简单的拟合
* 测试用的ROOT版本，为ROOT-5.14
```bash
source /afs/ihep.ac.cn/soft/CMS/root/rootenv
```
编译方法写入了Makefile，因此可以直接用下面的命令编译。这个Makefile针对各个ROOT版本写了相应的编译方式，原则上可以使用任意的版本进行编译。
```bash
make
```
      
## 特性
* 针对常见的抽取信号数算法，简化了拟合过程。
* 只包含本底和信号两种PDF。信号模型为多个高斯函数，本底为多项式函数
* 方便导出信号形状，作为其他拟合的输入
* 拟合结果保存在ROOT文件里，方便读取作图
* 作图脚本自动生成，方便二次加工

## 使用方法
### 加载
加载lib之后才能调用这个包里定义的QuickFit类，在`ROOT-5`里加载的方法如下
```c++
gSystem->Load("../lib/libSimpleFit.so");
```
在`ROOT-6`里面可以选择如下的方式
```c++
R__LOAD_LIBRARY(../lib/libSimpleFit.so)
```
二者的区别请参考https://root-forum.cern.ch/t/gsystem-load-vs-r-load-library-with-aclic/29685/6
### 类名和初始化
类的初始化需要三个参数，文件名，tree名，branch名
branch不能是类别，只能的单个浮点类型的观察量，比如不变质量
```c++
 QuickFit fit("sigmc.root", "treename", "observe");
```
### 成员方法
* SetSignalPDF("gauss", 2); 设置信号形状为双高斯
* SetBackgroundPDF("None"); 设置本底形状，默认为2阶切比雪夫多项式
* SetCut("");加cut条件
* SetFitRange(-10, 10); 设置拟合范围
* SetSignalArea(-3, 3); 设置信号区为[-3 sigma, 3 sigma]
* SetUnit("MeV"); 设置单位
* SetBins(40);   设置bin数
* SetFigureName(figName); 设置输出图片名
* SetXTitle("M(K_{s})/GeV"); 设置标题
* fit.SaveAs("Example.root", "toyFit"); 设置输出结果，包括信号形状和拟合结果
* fit.FitToData("unbinned");  fit， 总是在最后执行。
