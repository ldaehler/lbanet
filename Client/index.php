<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php echo $this->language; ?>" lang="<?php echo $this->language; ?>" >
<head>
<jdoc:include type="head" />
<link rel="stylesheet" href="<?php echo $this->baseurl ?>/templates/<?php echo $this->template;?>/css/template.css" type="text/css" />
</head>
<body>
<img class="header" src="<?php echo $this->baseurl ?>/templates/<?php echo $this->template;?>/images/header1.png" alt="Header" />
<div id="container">
	<img class="logo" src="<?php echo $this->baseurl ?>/templates/<?php echo $this->template;?>/images/logo.png" alt="LBANet" />

    	<div class="navi">
       	<div class="navilinks" id="nolistmenu">

	    		<div style="float: right; margin-right: 20px; font-size: 9px;">
    	   			LBANet  v0.9 <jdoc:include type="modules" name="user4" />
	    		</div>

	   		<jdoc:include type="modules" name="user3" />
       	</div>
    	</div>

    	<div id="sidebar_left" class="float"><jdoc:include type="modules" name="left"  style="rounded"/></div>
    	<div id="content" class="float">
		<img class="contentimage" src="<?php echo $this->baseurl ?>/templates/<?php echo $this->template;?>/images/contenttop.png" alt="Content" />
		<div id="contentmiddleimage">
			<div id="contentmiddle">
				<jdoc:include type="message" />
				<jdoc:include type="component" />
			</div>
		</div>
		<img class="contentimage" src="<?php echo $this->baseurl ?>/templates/<?php echo $this->template;?>/images/contentf.png" alt="Content" />
	</div>
    	<div id="sidebar_right"class="float"><jdoc:include type="modules" name="right" /></div>
</div>
<script type="text/javascript" src="<?php echo $this->baseurl ?>/templates/<?php echo $this->template;?>/hideseparator.js"></script>
</body>
</html>