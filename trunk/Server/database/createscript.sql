-- phpMyAdmin SQL Dump
-- version 2.11.8.1deb5+lenny1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Oct 19, 2009 at 10:54 PM
-- Server version: 5.0.51
-- PHP Version: 5.2.6-1+lenny3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `lbanet`
--

-- --------------------------------------------------------

--
-- Table structure for table `userinventory`
--

CREATE TABLE IF NOT EXISTS `userinventory` (
  `worldid` int(11) NOT NULL,
  `objectid` int(11) NOT NULL,
  `number` int(11) NOT NULL,
  `InventoryPlace` int(11) NOT NULL,
  PRIMARY KEY  (`worldid`,`objectid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL auto_increment,
  `status` int(1) NOT NULL,
  `username` varchar(20) NOT NULL,
  `password` varchar(40) NOT NULL,
  `email` varchar(50) NOT NULL,
  `activationkey` varchar(100) NOT NULL,
  `creationdate` datetime NOT NULL,
  `lastconnected` datetime default NULL,
  `playedtimemin` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `email` (`email`),
  UNIQUE KEY `activationkey` (`activationkey`),
  UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=6 ;

-- --------------------------------------------------------

--
-- Table structure for table `usertoworldmap`
--

CREATE TABLE IF NOT EXISTS `usertoworldmap` (
  `id` int(11) NOT NULL auto_increment,
  `userid` int(11) NOT NULL,
  `worldname` varchar(50) NOT NULL,
  `lastvisited` datetime default NULL,
  `timeplayedmin` int(11) NOT NULL default '0',
  `lastmap` varchar(50) default NULL,
  `lastposx` float NOT NULL default '0',
  `lastposy` float NOT NULL default '0',
  `lastposz` float NOT NULL default '0',
  `lastrotation` float NOT NULL default '0',
  `InventorySize` int(11) NOT NULL default '30',
  `Shortcuts` varchar(50) NOT NULL default '-1#-1#-1#-1#-1#-1#-1#-1#-1#-1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `userid` (`userid`,`worldname`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;
