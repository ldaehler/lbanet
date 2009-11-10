<?php



	if(isset($_POST['submit']))
	{
		$username = trim(mysql_real_escape_string($_POST['username']));
		$password = $_POST['password'];
		$password2 = $_POST['password2'];
		$email = trim(mysql_real_escape_string($_POST['email']));


		if($username && strlen($username) >= 4 && strlen($username) <= 15)
		{
			$query = mysql_query("SELECT `id` FROM `users` WHERE `username`='".$username."' LIMIT 1");
			if(mysql_num_rows($query))
			{
				$error['userexists'] = ' Username already exists';
			}
		}
		else
		{
			$error['usernameinput'] = ' Username should have between 4 and 15 characters';
		}

		if($email)
		{
			$query = mysql_query("SELECT `id` FROM `users` WHERE `email`='".$email."' LIMIT 1");
			if(mysql_num_rows($query))
			{
				$error['emailexists'] = ' Email already in use';
			}
		}
		else
		{
			$error['emailinput'] = ' Please supply an email address';
		}

		if($password && $password2)
		{
			if($password != $password2)
			{
				$error['passmismatch'] = ' Passwords do not match';
			}

			if(strlen($username) >= 20)
			{
				$error['passwordinput'] = ' Password length should be less than 20 characters';
			}
		}
		else
		{
			$error['passwordinput'] = ' Please enter your password in both fields';
		}

		include("securimage.php");
		$img = new Securimage();
		$valid = $img->check($_POST['code']);
 		if($valid != true)
 		{
			$error['codeinput'] = ' The code entered is incorrect';
 		}


		if(!isset($error))
		{
			$md5pass = md5($password);
			$activationKey =  mt_rand() . mt_rand() . mt_rand() . mt_rand() . mt_rand();
			$sql="INSERT INTO users (username, password, email, activationkey, status, creationdate) VALUES ('$username', '$md5pass', '$email', '$activationKey', '0', UTC_TIMESTAMP())";

			if (!mysql_query($sql))
			{
				die('Error: ' . mysql_error());
			}



			##Send activation Email
			$mail = new PHPMailer ();
			$mail->IsSMTP ();
			$mail->SMTPDebug  = 1;
			$mail->SMTPAuth   = true;
			$mail->SMTPSecure = "tls";
			$mail->Host = "smtp.gmail.com";
			$mail->Port  = 587;
			$mail->Username   = "lbanet.game@googlemail.com";
			$mail->Password   = "lapichonmelba";
			$mail->AddReplyTo ("noreply@lbanet.com", "Lbanet");
			$mail->SetFrom("noreply@lbanet.com", "Lbanet");
			$mail->Subject = "Lbanet Registration";
			$mail->Body = "Welcome to Lbanet!<br /><br />You, or someone using your email address, has registered to Lbanet.<br />Your account information:<br />Login: \"$username\"<br /> Password: \"$password2\" <br /><br />You can complete registration by clicking the following link:<br /> http://sd-1287.dedibox.fr/~lbanetweb/verifyemail.php?$activationKey <br /><br />If this is an error, ignore this email and you will be removed from our database after 24h.<br /><br />Regards,<br /> Lbanet Team";
			$mail->AddAddress ("$email", "$username");
			$mail->IsHTML (true);
			if(!$mail->Send())
			{
				echo "Mailer Error: " . $mail->ErrorInfo;
			}
			else
			{
				die("An email has been sent to $email with an activation key. Please check your mail to complete registration.");
			}
		}
	}


	$query = mysql_query("SELECT COUNT(`id`) FROM `users`");
	$data = mysql_fetch_row($query);
	echo "Registered users: " . $data[0];


	$query2 = mysql_query("SELECT COUNT(`id`) FROM `users` WHERE `connected`='1'");
	$data2 = mysql_fetch_row($query2);
	echo "Connected users: " . $data2[0];

?>

<form name="reg" method="post" enctype="application/x-www-form-urlencoded" action="index.php" style="display: block; padding-left: 20px;">
	<div style="width: 120px; float: left; height: 25px">Username:</div><input type="text" name="username" /><?php if(isset($error['userexists'])){ echo $error['userexists'];} if(isset($error['usernameinput'])){ echo $error['usernameinput'];} ?><br /><br />
	<div style="width: 120px; float: left; height: 25px">Email: </div><input type="text" name="email" /><?php if(isset($error['emailexists'])){ echo $error['emailexists'];} if(isset($error['emailinput'])){ echo $error['emailinput'];} ?><br /><br />
	<div style="width: 120px; float: left; height: 25px">Password: </div><input type="password" name="password" /><?php if(isset($error['passmismatch'])){ echo $error['passmismatch'];} if(isset($error['passwordinput'])){ echo $error['passwordinput'];} ?><br/><br />
	<div style="width: 120px; float: left; height: 25px">Confirm password: </div><input type="password" name="password2" /><br/><br />


	<div style="width: 120px; float: left; height: 55px">Security Code:</div>

	<div style="width: 430px; float: left; height: 55px">
		<img alt="security code" src="securimage_show.php?sid=<?php echo md5(uniqid(time())); ?>" id="siimage" align="left" style="padding-right: 5px; border: 0" />
		<a style="border-style: none" href="#" title="Refresh Image" onclick="document.getElementById('siimage').src = 'securimage_show.php?sid=' + Math.random(); return false"><img src="images/refresh.png" alt="Reload Image" border="0" onclick="this.blur()" align="bottom" /></a>
	</div>
	<br style="clear:both"/>
	<div style="width: 120px; float:left; height: 25px">Verify Code:</div>
	<input type="text" name="code" /><?php if(isset($error['codeinput'])){ echo $error['codeinput'];}?><br/>
	<br/>
	<input type="submit" name="submit" value="Register" />
</form>
