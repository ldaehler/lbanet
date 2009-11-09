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
   .news {
       margin-left: 10px;
       padding: 10px 30px 10px 20px;
       width:649px;
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
   <a class="navi" href="index.php" >Game Info</a>
   <a class="navi" href="index.php" >Downloads</a>
   <a class="navi" href="index.php" >Support</a>
   <a class="navi" href="index.php" >Credits</a>
    <div style="float: right; margin-right: 20px; font-size: 9px;">
    LBANet  v0.5.7 
    </div>
   </div>
 </div> 
 
 <div class="news">
14/11/2009:<span style="color: #3483EC"> New Release v0.7 + new website!</span> <br />
<span style="color: #A7FAFA">Hi guys. I would like to welcome you to our new website!<br /> So this is our first big release since a while implementing the inventory system together with a few more features.
</span><br />
<br />  Here is a complete list of the new features:<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Persistent storage system on server side. Players will now need to create a game account where all their data will be saved in each world they will visit (player position in world, inventory, etc.)<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; New game website:&nbsp; you can find our new game website at this address:&nbsp; <a href="http://sd-1287.dedibox.fr/%7Evdelage/" target="_blank">sd-1287.dedibox.fr/~vdelage/</a>. The site is still under construction and our great designer and webmaster leoboe is still working on it. However, you can already register a game account there to be able to play the game.<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Inventory system:&nbsp; the player will get an independent inventory in each world. A new GUI is available in game were the player can visualize his current inventory.<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Container system: the player can open container in the maps. Then he will be able to take object from the containers or add object from his own inventory into the container.<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Shortcut bar: a new in game shortcut bar as appear. You can drag and drop items from your inventory into it.<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; New whisper functionality: player can now whisper to each other. To do so you can either tape /w playername message in your chatbox or choose whisper in the channel menu.<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Friend functionality: you can now add people to your friend list in the community window.&nbsp; You will then be able to easily see which or your friends are online. You can also directly whisper to your friend s by right clicking on their name in the community window.<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Faster map loading: some optimizations have been made server side to decrease the loading time of maps.<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Added a map created by Polaris in tippet island<br /><br />  -&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Few bugs fixes.<br /><br />  &nbsp;<br /><br />  Finally you will notice that you cannot anymore ride the horse, dino-fly or use the protopack in the lbaexpanded world. Indeed, the corresponding items have been hidden all over the world. It is now your task to find them out&nbsp;&nbsp; ;-)<br /><br />    
 </div>
     <div class="newsf">
     </div>
<br />
 <div class="contentbox">
 <?PHP 
 include 'register.php'  ;
 ?>
 </div>
 
<div class="contentboxf" ></div>
 
</div>
</body>
</html>