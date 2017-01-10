/*
Navicat MySQL Data Transfer

Source Server         : localhost3307
Source Server Version : 50716
Source Host           : localhost:3307
Source Database       : hotel

Target Server Type    : MYSQL
Target Server Version : 50716
File Encoding         : 65001

Date: 2017-01-11 02:03:47
*/

SET FOREIGN_KEY_CHECKS=0;
DROP DATABASE IF EXISTS `hotel`;
CREATE DATABASE `hotel`;
USE hotel;

-- ----------------------------
-- Table structure for guest
-- ----------------------------
DROP TABLE IF EXISTS `guest`;
CREATE TABLE `guest` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `sex` varchar(6) NOT NULL,
  `years` varchar(11) NOT NULL,
  `idcard_type` varchar(255) NOT NULL,
  `idcard_id` varchar(255) NOT NULL,
  `phone` varchar(20) DEFAULT NULL,
  `address` varchar(255) DEFAULT NULL,
  `companyname` varchar(255) DEFAULT NULL,
  `companyaddr` varchar(255) DEFAULT NULL,
  `checkintime` datetime NOT NULL,
  `checkouttime` datetime NOT NULL,
  `deposit` decimal(11,2) DEFAULT '0.00',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of guest
-- ----------------------------
INSERT INTO `guest` VALUES ('0', '未入住', '男', '0', '0', '0', '0', '0', '0', '0', '2017-01-10 21:04:28', '2017-01-10 21:04:28', '0.00');
INSERT INTO `guest` VALUES ('1', '张三', '男', '19', '身份证', '371202199705068989', '13569874563', '淄博', '山东理工大学', null, '2017-01-10 15:23:36', '2017-01-15 01:00:18', '100.00');
INSERT INTO `guest` VALUES ('2', '李四', '男', '21', '军人证', '123456789', '12345678900', '北京', '中央警卫团', '卫戍区', '2016-12-31 01:00:19', '2017-01-12 01:00:19', '150.00');
INSERT INTO `guest` VALUES ('3', '王五', '女', '27', '护照', '123456789123456', '13256987456', '美国', '白宫', '华盛顿特区', '2016-12-31 01:00:22', '2017-01-10 01:00:22', '100.00');
INSERT INTO `guest` VALUES ('4', '赵六', '男', '55', '港澳同胞证', '123456798456132', '12345678955', '香港', '花旗银行', null, '2016-12-31 01:00:25', '2017-01-20 01:00:25', '50.00');

-- ----------------------------
-- Table structure for login
-- ----------------------------
DROP TABLE IF EXISTS `login`;
CREATE TABLE `login` (
  `name` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of login
-- ----------------------------
INSERT INTO `login` VALUES ('admin', 'admin');

-- ----------------------------
-- Table structure for room
-- ----------------------------
DROP TABLE IF EXISTS `room`;
CREATE TABLE `room` (
  `id` int(11) NOT NULL,
  `typeid` int(11) NOT NULL,
  `guestid` int(11) DEFAULT NULL,
  `dirty` tinyint(1) DEFAULT NULL,
  `checkintime` datetime DEFAULT NULL,
  `checkouttime` datetime DEFAULT NULL,
  `unitprice` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `ck_guest` (`guestid`),
  KEY `ck_type` (`typeid`),
  CONSTRAINT `ck_guest` FOREIGN KEY (`guestid`) REFERENCES `guest` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `ck_type` FOREIGN KEY (`typeid`) REFERENCES `room_type` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of room
-- ----------------------------
INSERT INTO `room` VALUES ('101', '1', '1', '1', '2017-01-10 15:23:36', '2017-01-14 15:22:43', '100.00');
INSERT INTO `room` VALUES ('102', '2', '2', '1', '2016-12-31 01:00:19', '2017-01-12 01:00:19', '200.00');
INSERT INTO `room` VALUES ('103', '4', '3', '0', '2017-01-11 00:22:38', '2017-01-11 00:22:38', '500.00');
INSERT INTO `room` VALUES ('104', '1', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('105', '5', '4', '0', '2016-12-31 01:00:25', '2017-01-20 01:00:25', '400.00');
INSERT INTO `room` VALUES ('106', '6', '1', '0', '2016-12-31 01:00:18', '2017-01-15 01:00:18', '250.00');
INSERT INTO `room` VALUES ('107', '1', '1', '0', '2017-01-11 00:31:09', '2017-01-11 00:31:09', '100.00');
INSERT INTO `room` VALUES ('108', '2', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('201', '1', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('202', '3', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('203', '4', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('204', '5', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('205', '6', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('206', '3', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('207', '4', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('208', '5', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('301', '1', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('302', '2', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('303', '3', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('304', '4', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('305', '5', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('306', '6', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('307', '2', '0', '0', null, null, '0.00');
INSERT INTO `room` VALUES ('308', '4', '0', '0', null, null, '0.00');

-- ----------------------------
-- Table structure for room_type
-- ----------------------------
DROP TABLE IF EXISTS `room_type`;
CREATE TABLE `room_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `peoplenum` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of room_type
-- ----------------------------
INSERT INTO `room_type` VALUES ('1', '标单', '100.00', '1');
INSERT INTO `room_type` VALUES ('2', '标双', '200.00', '2');
INSERT INTO `room_type` VALUES ('3', '豪单', '300.00', '1');
INSERT INTO `room_type` VALUES ('4', '豪华套房', '500.00', '2');
INSERT INTO `room_type` VALUES ('5', '麻将房', '400.00', '4');
INSERT INTO `room_type` VALUES ('6', '情侣房', '250.00', '2');
SET FOREIGN_KEY_CHECKS=1;
