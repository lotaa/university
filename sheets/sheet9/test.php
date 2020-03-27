<html>
<head>
<title>Create Database. </title>
</head>
<body>
		<?php
		$con = mysql_connect("localhost","root","");
		if(!$con)
		{
		die("not opened");
		}
		echo "Connection open"."</br>";
		$db = mysql_select_db("studinfo",$con);
		if(!$db)
		{
		die("Database not found".mysql_error());
		}
		echo "Database is selected"."</br>";
		$query = "create table computer(id INT not null,name varchar(50),branch
		varchar(50))";
		$crtb = mysql_query($query,$con);
		if(!$crtb)
		{
		die(" table not created. .!".mysql_error());
		}
		echo "table created.. !"."</br>";
		?>
</body>
</html>