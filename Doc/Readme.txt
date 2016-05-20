html                              说明文档输出目录
Doxyfile                          说明文档生成配置
doxygen-1.8.11-setup              说明文档生成软件
2014-01-16_graphviz-2.37.20140115 说明图生成软件
HTMLHelpWorkshop chm              说明书生成软件
Visual Assist X                   VS帮助插件
Logo                              Logo图标
cpp.tpl                           doxygen配置文件

1.安装doxygen,graphviz,解压HTMLHelpWorkshop;
2.使用doxygen打开Doxyfile;
3.修改Expert->HTML->HHC_LOCATION至../HTML Help Workshop/hhc.exe所在目录;
4.修改Expert->Dot->DOT_PATH至../Graphviz2.37/bin所在目录;
5.已设置好Source code directory
          Destination directory
6.Run doxygen

doxygen安装后会添加插件在VS2015->VAssistX->Insert VA Snippet..中添加功能;
也可以在VS2015->VAssistX->Insert VA Snippet->Edit VA Snippet..中编辑;
备份并覆盖文件:C:\Users\**\AppData\Roaming\VisualAssist\Autotext\cpp.tpl加入doxygen标准的注释功能.