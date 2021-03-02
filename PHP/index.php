<!DOCTYPE html>
<html>
<body>

<?php
    
    $host_name = 'localhost';
	$database = 'id16262414_hogau';
	$user_name = 'id16262414_admin1';
	$password = 'HogauTestAdmin1!';


	$link = new mysqli($host_name, $user_name, $password, $database);
      
	 
	if ($link->connect_error) {
	die('<p>Verbindung zum MySQL Server fehlgeschlagen: '. $link->connect_error .'</p>');
	}else{
	    
	   echo "Verbindung hergestellt!       ";
	    
	    /*
		$sql = "INSERT INTO rfidtags (Name, Klasse, VALIDFROM, VALIDUNTIL, Days) 
		VALUES ('Jakob Allgayer', 'Q11', '2021-02-26', '2021-03-26', '4,5,6' )";
		
		if($link->query($sql) === TRUE) {
		   echo "Du bist registriert!";
		}
		else{ 
		   echo "Fehler bei der Registrierung!". $link->error;
		}
		*/
	
	
	    /*
		$sql2 = "SELECT * FROM rfidtags";
	    $res = $link->query($sql2);
	
	    if($res->num_rows > 0) {
	    while($i = $res->fetch_assoc()){
	       echo "ID: " . $i["ID"]." Name: ".$i["Name"]."  /  ";
	    }
      	}else{ 
		
	    echo "Fehler bei der Datenausgabe!". $link->error;
      	}
      	*/
	}
	
 
	
	
	
	$link-> close();
?>

 
</body>
</html>