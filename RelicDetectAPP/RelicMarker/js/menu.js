var gui = require('nw.gui');//引入gui模块

var menubar = new gui.Menu({ type: 'menubar' });//定义菜单栏
var sub1 = new gui.Menu();//定义一个子菜单
var subt_item1 = new gui.MenuItem({//定义一个菜单项
    label: '打开图像目录',
    click: function() {
            var main = require("./js/main");
            main.chooseFile("#openFileDialog");
    }
});

sub1.append(subt_item1);//给子菜单添加一个菜单项
menubar.append(new gui.MenuItem({ label: '文件', submenu: sub1 }));//为菜单栏添加一个菜单项
var win = gui.Window.get();//获取当前窗口
win.menu = menubar;//将当前窗口的菜单赋值为menubar
