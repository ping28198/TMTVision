/************************************************************************/
/* 错误列表                                                                     */
/************************************************************************/
#define TMT_NORMAL 0
#define ERR        1000
//文件类错误//////////////////////////////////////////////////////////////
#define FILE_NOT_EXIST			ERR+1
#define CALI_FILE_NOT_EXIST		ERR+2
#define CONFIG_FILE_NOT_EXIST	ERR+3  //设置文件不存在
//相机类错误//////////////////////////////////////////////////////////////
#define CAM_NOT_EXIST			ERR+11
#define CAM_NUM_WRONG			ERR+12 //摄像机数量错误
#define CAM_NAME_UNREAD			ERR+13 //无法读取摄像机名
#define CAM_OPEN_FAILED			ERR+14 //打开相机出错
#define CAM_SEQUENCE_DETECT_FAILED	ERR+15//相机顺序判断失败
//图像类错误/////////////////////////////////////////////////////////////
#define GRAY_VALUE_TOO_BRIGHT	ERR+20 //图像灰度过亮
#define GRAY_VALUE_TOO_DARK		ERR+21 //图像灰度过暗
#define IMAGE_CAPTURE_FAILED	ERR+22 //获取帧失败
#define IMAGE_NOT_EXIST			ERR+23 //图像不存在
#define FOOT_SHAPE_DETECT_FAILED ERR+30 //脚型识别出错