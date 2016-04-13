================================================================================
    MICROSOFT 基础类库 : CameraManager 项目概述
===============================================================================

应用程序向导已为您创建了此 CameraManager 应用程序。此应用程序不仅演示 Microsoft 基础类的基本使用方法，还可作为您编写应用程序的起点。

本文件概要介绍组成 CameraManager 应用程序的每个文件的内容。

CameraManagerApp.h、CameraManagerApp.cpp
    这是应用程序的主头文件。
    其中包括其他项目特定的标头（包括 Resource.h），并声明 CCameraManagerApp 应用程序类。
    这是包含应用程序类 CCameraManagerApp 的主应用程序源文件。

CameraManagerDlg.h、CameraManagerDlg.cpp - 对话框
    这些文件包含 CCameraManagerDlg 类。此类定义应用程序的主对话框的行为。对话框模板包含在 CameraManager.rc 中，该文件可以在 Microsoft Visual C++ 中编辑。

CameraManager.h、CameraManager.cpp
    控制线程。

CameraObject.h、CameraObject.cpp
    相机对象。

ReceiveServer.h、ReceiveServer.cpp
    接受消息线程。
	
SendServer.h、SendServer.cpp
    发送消息线程。

/////////////////////////////////////////////////////////////////////////////

程序架构:

   ╔═══════════════════════════════════╗
   ║ CameraManagerApp                                                     ║
   ║ ┏━━━━━━━━━━━━━━━━━━━━━━━┓   ┏━━━━━━┓║
   ║ ┃CameraManager  ┌       ┐                    ┃   ┃UI:         ┃║
   ║ ┃               │Add()  │                    ┃   ┃CameraManag-┃║
   ║ ┃               │Del()  │<············┃erDlg       ┃║
   ║ ┃ Setting···>│Start()│                    ┃   ┃            ┃║
   ║ ┃               │Stop() │··>[CameraObject*>┃   ┃            ┃║
   ║ ┃               └   ∧  ┘     [             >┃   ┃            ┃║
   ║ ┃                  ┌│──┐   [             >┃   ┃            ┃║
   ║ ┃                  │Task()│   [             >┃   ┃            ┃║
   ║ ┃                  └∧──┘   [             >┃   ┃            ┃║
   ║ ┗━━━━━━━━━━·━━━━━━━━━━━━┛   ┗━━━━━━┛║
   ║                       ····                                       ║
   ║ ┏━━━━━━━━━━━━┓ ·┏━━━━━━━━━━━━━━━━━┓║
   ║ ┃ReceiveServer           ┃ ·┃CameraObject                      ┃║
 ───>:Socket                 ┃ ·┃─────────────────┃║
   ║ ┃  │       [MessageItem]─>·┃┌──────┐┌───────┐┃║
   ║ ┃  │       [           ]┃   ┃│Processor   ││DirWatcher    │┃║
   ║ ┃┌∨──┐ [           ]┃   ┃│            ││    ┌───┐│┃║
   ║ ┃│Task()─>[           ]┃   ┃│            ││    │Task()<────
   ║ ┃└───┘              ┃   ┃│  ┌───┐││    └───┘│┃║
   ║ ┗━━━━━━━━━━━━┛   ┃│┌│Task()<── [        ] │ │┃║
   ║                                ┃││└───┘││[        ] │ │┃║
   ║ ┏━━━━━━━━━━━━┓   ┃││Algori- │││[        ] │ │┃║
   ║ ┃SendServer              ┃ ┌───<thom   │││[        ] │ │┃║
 ───<:Socket                 ┃ │┃│└────┘││[FileItem]<┘ │┃║
   ║ ┃  ∧                    ┃ │┃└──────┘└───────┘┃║
   ║ ┃┌│──┐              ┃ │┗━━━━━━━━━━━━━━━━━┛║
   ║ ┃│Task()<─[           ]┃ │                 :                    ║
   ║ ┃└───┘ [           ]┃ │┏━━━━━━━━━━━━━━━━━┓║
   ║ ┃           [MessageItem]<─┘┃CameraObject                      ┃║
   ║ ┗━━━━━━━━━━━━┛   ┗━━━━━━━━━━━━━━━━━┛║
   ║                                                 :                    ║
   ╚═══════════════════════════════════╝

/////////////////////////////////////////////////////////////////////////////
