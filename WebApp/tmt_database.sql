/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50505
Source Host           : localhost:3306
Source Database       : tmt_database

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2016-04-19 17:08:43
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for camera
-- ----------------------------
DROP TABLE IF EXISTS `camera`;
CREATE TABLE `camera` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `cam_num` int(11) DEFAULT NULL,
  `cam_name` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `cam_folder` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `server_ip` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `location_lat` varchar(255) CHARACTER SET utf8 DEFAULT NULL COMMENT '纬度',
  `location_lng` varchar(255) CHARACTER SET utf8 DEFAULT NULL COMMENT '经度',
  `warning_level` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `cam_status` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `algo_mask` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  `algo_status` varchar(255) CHARACTER SET utf8 DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of camera
-- ----------------------------
INSERT INTO `camera` VALUES ('1', '9987', 'Front Gate Cam A', 'C:/a/', '192.168.9.9', '30.999', '180.88', '1', '1', 'default', 'on');
INSERT INTO `camera` VALUES ('2', '893849', 'ioouop', 'D:\\images\\11\\????2', '189.11.22.3', '29.222', '180.332', '1', '1', 'no', 'on');
INSERT INTO `camera` VALUES ('3', null, null, '??', null, null, null, null, null, null, null);

-- ----------------------------
-- Table structure for image
-- ----------------------------
DROP TABLE IF EXISTS `image`;
CREATE TABLE `image` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `image_path` varchar(255) DEFAULT NULL,
  `grab_time` datetime DEFAULT NULL,
  `defects_num` int(11) DEFAULT NULL,
  `defects_pos` varchar(255) DEFAULT NULL,
  `height` double DEFAULT NULL,
  `width` double DEFAULT NULL,
  `cam_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `cam_id` (`cam_id`),
  CONSTRAINT `image_ibfk_1` FOREIGN KEY (`cam_id`) REFERENCES `camera` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of image
-- ----------------------------
INSERT INTO `image` VALUES ('1', 'D:/images/test1.jpg', null, null, null, null, null, '1');
INSERT INTO `image` VALUES ('2', 'D:/images/test2.jpg', null, null, null, null, null, '1');

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL,
  `password` varchar(255) DEFAULT NULL,
  `cam_watch` varchar(255) DEFAULT NULL,
  `wechat_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
