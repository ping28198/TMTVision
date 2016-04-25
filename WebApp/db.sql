-- MySQL dump 10.13  Distrib 5.6.30, for Win64 (x86_64)
--
-- Host: localhost    Database: tmt_database
-- ------------------------------------------------------
-- Server version	5.6.30

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `cam1_image_201604`
--

DROP TABLE IF EXISTS `cam1_image_201604`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cam1_image_201604` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `image_path` varchar(255) DEFAULT NULL,
  `grab_time` datetime DEFAULT NULL,
  `defects_num` int(11) DEFAULT NULL,
  `defects_pos` varchar(255) DEFAULT NULL,
  `width` double DEFAULT NULL,
  `height` double DEFAULT NULL,
  `cam_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cam1_image_201604`
--

LOCK TABLES `cam1_image_201604` WRITE;
/*!40000 ALTER TABLE `cam1_image_201604` DISABLE KEYS */;
/*!40000 ALTER TABLE `cam1_image_201604` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cam2_image_201604`
--

DROP TABLE IF EXISTS `cam2_image_201604`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cam2_image_201604` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `image_path` varchar(255) DEFAULT NULL,
  `grab_time` datetime DEFAULT NULL,
  `defects_num` int(11) DEFAULT NULL,
  `defects_pos` varchar(255) DEFAULT NULL,
  `width` double DEFAULT NULL,
  `height` double DEFAULT NULL,
  `cam_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cam2_image_201604`
--

LOCK TABLES `cam2_image_201604` WRITE;
/*!40000 ALTER TABLE `cam2_image_201604` DISABLE KEYS */;
INSERT INTO `cam2_image_201604` VALUES (1,'d:','2016-02-03 12:23:20',1,'100,200,300,400,',1920,1080,2),(2,'d:','2016-02-03 12:23:20',1,'100,200,300,400,',1920,1080,2),(3,'d:','2016-02-03 12:23:20',1,'100,200,300,400,',1920,1080,2),(4,'d:','2016-02-03 12:23:20',1,'100,200,300,400,',1920,1080,2),(5,'d:a','2016-02-03 12:23:20',1,'100,200,300,400,',1920,1080,2),(6,'d:a','2016-02-03 12:23:20',1,'100,200,300,400,',1920,1080,2),(7,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(8,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(9,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(10,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(11,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(12,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(13,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(14,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(15,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(16,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(17,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(18,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(19,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(20,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(21,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(22,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(23,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(24,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(25,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(26,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(27,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(28,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2),(29,'d:a','2016-04-20 12:23:20',1,'100,200,300,400,',1920,1080,2);
/*!40000 ALTER TABLE `cam2_image_201604` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `camera`
--

DROP TABLE IF EXISTS `camera`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `camera` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `cam_num` int(11) DEFAULT NULL,
  `cam_name` varchar(255) DEFAULT NULL,
  `cam_folder` varchar(255) DEFAULT NULL,
  `server_ip` varchar(255) DEFAULT NULL,
  `location_lat` varchar(255) DEFAULT NULL COMMENT '绾?害',
  `location_lng` varchar(255) DEFAULT NULL COMMENT '缁忓害',
  `warning_level` varchar(255) DEFAULT NULL,
  `cam_status` varchar(255) DEFAULT NULL,
  `algo_mask` varchar(255) DEFAULT NULL,
  `algo_status` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `camera`
--

LOCK TABLES `camera` WRITE;
/*!40000 ALTER TABLE `camera` DISABLE KEYS */;
INSERT INTO `camera` VALUES (1,9987,'Front Gate Cam A','C:/a/','192.168.9.9','30.999','180.88','1','1','default','on'),(2,893849,'name0','d:aB','192.168.1.1','11.000000','22.000000','0','300','d:a','0'),(4,9987,'name','d:\\a','192.168.0.1','11','22','0','300','d:\\a\\b','0'),(5,9987,'name','d:\\a','192.168.0.1','11','22','0','300','d:\\a\\b','0'),(6,9987,'name','d:\\a','192.168.0.1','11','22','0','300','d:\\a\\b','0'),(7,9987,'name','d:\\a','192.168.0.1','11','22','0','300','d:\\a\\b','0'),(8,9987,'name','d:\\a','192.168.0.1','11','22','0','300','d:\\a\\b','0'),(9,9987,'name0','d:\\aB','192.168.1.1','11','22','0','300','d:\\a\\b','0'),(10,3,'name0','d:aB','192.168.1.1','11','22','0','300','d:a','0');
/*!40000 ALTER TABLE `camera` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `client_info`
--

DROP TABLE IF EXISTS `client_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip_address` varchar(18) NOT NULL,
  `port` int(6) NOT NULL,
  `status` int(2) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `client_info`
--

LOCK TABLES `client_info` WRITE;
/*!40000 ALTER TABLE `client_info` DISABLE KEYS */;
INSERT INTO `client_info` VALUES (1,'192.168.0.1',5012,0);
/*!40000 ALTER TABLE `client_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `image`
--

DROP TABLE IF EXISTS `image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
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
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `image`
--

LOCK TABLES `image` WRITE;
/*!40000 ALTER TABLE `image` DISABLE KEYS */;
INSERT INTO `image` VALUES (1,'D:/images/test1.jpg',NULL,NULL,NULL,NULL,NULL,1),(2,'D:/images/test2.jpg',NULL,NULL,NULL,NULL,NULL,1);
/*!40000 ALTER TABLE `image` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  `password` varchar(32) NOT NULL,
  `cam_watch` varchar(255) DEFAULT NULL,
  `we_chat` varchar(64) DEFAULT NULL,
  `authority_level` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-04-25 14:31:34
