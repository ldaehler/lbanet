<?PHP session_start();

require_once('class.phpmailer.php');

mysql_connect("localhost", "lbanetuser", "lapichonmelba") or die(mysql_error());
mysql_select_db("lbanet") or die(mysql_error());
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
 <title>LBANet - Account registration</title>
 <style type="text/css">
 html,body {
       background: black url(images/background.png) repeat-x;
       color: white;
       height: 100%;
       padding:0px;
       margin:0px;
       font-family: tahoma, sans serif;
       text-shadow:1px 1px 2px #000000;
   }
   a:link, a:visited{
        color: #C0C0C0;
        font-size: 14px;
        font-family: tahoma, sans serif;
        text-decoration: none;
        background: none;
        margin:0;
        text-shadow:1px 1px 2px #000000;
   }
   a:hover{
        text-decoration: underline;
        text-shadow:1px 1px 20px #C5DCF7;
   }
   .usercp{
        position: absolute;
        left: 50%;
        margin: 20px 0 0 260px;
        height: 188px;
        width: 170px;
        background: transparent url(images/login-box.png) no-repeat;
        z-index: 3;
        opacity:0.9;
   }
   .logo {
       display: block;
       margin: 0 auto;
       border: 0px;
   }
   .content{
       margin: -282px auto;
       width: 870px;
       background: transparent url(images/background-content.png) repeat-x;
  	   min-height:100%;
       height:auto !important; /* moderne Browser */
       height:100%; /* IE */
       z-index: 2;
   }
   .header {
       z-index: 1;
       display: block;
       margin: 0 auto;
   }
   .navi {
       width: 880px;
       height: 75px;
       background: transparent url(images/navi.png) no-repeat;
       margin: -65px 0px 0px -15px;
       line-height: 75px;
       vertical-align: middle;
       font-size: 14px;
       padding-left: 20px;
   }
   .navilinks {
       z-index: 3;

   }
   .sidebar {
       margin-left: -10px;
       padding: 15px 15px 10px 15px;
       width:150px;
       height:168px;
       background: transparent url(images/sidebar.png) no-repeat;
       font-size: 12px;
       float: right;
       line-height: 150%;
       opacity: 0.8;
   }
   .news {
       margin-left: 10px;
       padding: 10px 40px 10px 20px;
       width:639px;
       min-height: 168px;
       height:auto !important; /* moderne Browser */
       height:168px; /* IE */
       background: transparent url(images/newsbg.png) no-repeat;
       font-size: 14px;
   }
   .newsf {
       margin-left: 10px;
       width:699px;
       height:9px;
       background: transparent url(images/newsf.png) no-repeat;
   }
   .contentbox {
       margin-left: 10px;
       padding: 10px 30px 10px 20px;
       width:801px;
       min-height: 168px;
       height:auto !important; /* moderne Browser */
       height:168px; /* IE */
       background: transparent url(images/contentbg.png) no-repeat;
       font-size: 14px;
       text-align: justify;
   }
   .contentboxf {
       margin-left: 10px;
       width:851px;
       height:20px;
       background: transparent url(images/contentf.png) no-repeat;
   }
 </style>

</head>
<body>
<div class="usercp"></div>
<img class="header" src="images/header1.png" alt="Header" />
<div class="content">
<a href="index.php" ><img class="logo" src="images/logo.png" alt="LBANet" /></a>
 <div class="navi">
   <div class="navilinks">
   <a class="navi" href="index.php" >News</a>
<!--   <a class="navi" href="index.php" >Game Info</a>
   <a class="navi" href="index.php" >Downloads</a>
   <a class="navi" href="index.php" >Support</a>
   <a class="navi" href="index.php" >Credits</a> -->
   <a class="navi" href="http://forum.magicball.net/forumdisplay.php?f=59" >Forum</a>
   <a class="navi" href="http://forum.magicball.net/showpost.php?p=388603&postcount=2" >Download</a>
    <div style="float: right; margin-right: 20px; font-size: 9px;">
    LBANet  v0.7
    </div>
   </div>
 </div>

 <div class="sidebar"><br /><br />
 <?PHP $query2 = mysql_query("SELECT COUNT(`id`) FROM `users` WHERE `connected`='1'");
	$data2 = mysql_fetch_row($query2);
	echo "Connected users: " . $data2[0] . "<br />";
  $query = mysql_query("SELECT COUNT(`id`) FROM `users`");
	$data = mysql_fetch_row($query);
	echo "Registered users: " . $data[0] . "<br />"; ?>
 </div>

 <div class="news"><img src="images/inv.png" style="float:right; margin-right: 10px;" alt="inventory" />
14/11/2009:<span style="color: #3483EC"> New Release v0.7 + new website!</span> <br />

<span style="color: #A7FAFA">Hi guys. I would like to welcome you to our new website!<br /> So this is our first big release since a while implementing the inventory system together with a few more features.
</span><br /><span style="color: #3483EC;">&nbsp;&nbsp;&nbsp;&nbsp; <a href="news.php">... More details</a></span>
 </div>
     <div class="newsf">
     </div>
<br />
 <div class="contentbox">
 <span style="color: #A7FAFA; font-size: 18px">Account creation:</span><br /><br />
 <?PHP
 include 'register.php'  ;
 ?>
 </div>

<div class="contentboxf" ></div>
<br /> <br />
</div>
</body>
</html>