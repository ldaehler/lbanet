<?php
require_once('class.phpmailer.php');

mysql_connect("localhost", "vdelage", "lapichonmelba") or die(mysql_error());
mysql_select_db("lbanet") or die(mysql_error());


##User isn't registering, check verify code and change activation code to null, status to activated on success

$queryString = $_SERVER['QUERY_STRING'];

$query = "SELECT * FROM users";

$result = mysql_query($query) or die(mysql_error());

while($row = mysql_fetch_array($result))
{

	if ($queryString == $row["activationkey"])
	{
		$login =  $row["username"];
		$pass =  $row["password"];
		$to =  $row["email"];

		echo "Congratulations! $login is now the proud new owner of an Lbanet account. <br />";
		echo "You will receive shortly a confirmation email with a summary of your account information.";

		$sql="UPDATE users SET activationkey = '$row[id]', status='1' WHERE (id = $row[id])";

		if (!mysql_query($sql))
		{
			die('Error: ' . mysql_error());
		}


		##Send confirmation Email
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
		$mail->Subject = "Lbanet account confirmation";
		$mail->Body = "Welcome to Lbanet!<br /><br />You have successfully registered to Lbanet. Your account information:<br />Login: \"$login\"<br /> Password: \"$pass\"<br /><br />Regards,<br /> Lbanet Team";
		$mail->AddAddress ("$to", "$login");
		$mail->IsHTML (true);
		if(!$mail->Send())
		{
			echo "Mailer Error: " . $mail->ErrorInfo;
		}
	}

}

?>