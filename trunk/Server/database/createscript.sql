-- phpMyAdmin SQL Dump
-- version 3.2.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Oct 11, 2009 at 04:53 PM
-- Server version: 5.1.37
-- PHP Version: 5.3.0

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `lbanet`
--

-- --------------------------------------------------------

--
-- Table structure for table `inventoryobjects`
--

CREATE TABLE IF NOT EXISTS `inventoryobjects` (
  `id` int(11) NOT NULL,
  `name` varchar(30) NOT NULL,
  `type` int(11) NOT NULL,
  `description` text NOT NULL,
  `effect` varchar(255) NOT NULL,
  `maxnumber` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `userinventory`
--

CREATE TABLE IF NOT EXISTS `userinventory` (
  `worldid` int(11) NOT NULL,
  `objectid` int(11) NOT NULL,
  `numer` int(11) NOT NULL,
  PRIMARY KEY (`worldid`,`objectid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `status` int(1) NOT NULL,
  `username` varchar(20) NOT NULL,
  `password` varchar(40) NOT NULL,
  `email` varchar(50) NOT NULL,
  `activationkey` varchar(100) NOT NULL,
  `creationdate` datetime NOT NULL,
  `lastconnected` datetime DEFAULT NULL,
  `playedtimemin` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `email` (`email`),
  UNIQUE KEY `activationkey` (`activationkey`),
  UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=6 ;

-- --------------------------------------------------------

--
-- Table structure for table `usertoworldmap`
--

CREATE TABLE IF NOT EXISTS `usertoworldmap` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `userid` int(11) NOT NULL,
  `worldname` varchar(50) NOT NULL,
  `lastvisited` datetime DEFAULT NULL,
  `timeplayedmin` int(11) NOT NULL DEFAULT '0',
  `lastmap` varchar(50) DEFAULT NULL,
  `lastposx` float NOT NULL DEFAULT '0',
  `lastposy` float NOT NULL DEFAULT '0',
  `lastposz` float NOT NULL DEFAULT '0',
  `lastrotation` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `userid` (`userid`,`worldname`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=11 ;
