<?php
//print_r($_SERVER);
//exit;

$aResult=array("name"=>'',"valid"=>'0',"klasse"=>'');
if ((@$_REQUEST["RF"]!="")){
	  $RFID =	trim($_REQUEST["RF"]);
	  $host_name = 'LOCALHOST';
	  $database = 'id16262414_hogau';
	  $user_name = 'id16262414_admin1';
	  $password = 'HogauTestAdmin1!';
	  
	  $link = new mysqli($host_name, $user_name, $password, $database);
      
	  if ($link->connect_error) {
		die('<p>Verbindung zum MySQL Server fehlgeschlagen: '. $link->connect_error .'</p>');
	  } else {
			$sql = "SELECT `DAYS`, `NAME`,`Klasse`,
			CASE 
				  WHEN  sysdate() between  `VALIDFROM` and  `VALIDUNTIL` THEN 1
				  ELSE 0 
			  END as `intervall` 
					FROM `rfidtags` where ID='".$RFID."'" ;
				//echo $sql;	
			$result = $link->query($sql);
			
			if ($result->num_rows > 0) {
			  while($row = $result->fetch_assoc()) {
				$valid=$row["intervall"];
				$weekdays = explode(',', $row["DAYS"]);
				
				if (!in_array(date("N"),$weekdays)&& $valid==1){
					$valid=0;
				}
				$aResult=array("name"=>$row["NAME"],"valid"=>$valid,"klasse"=>$row["Klasse"]);
				$sql="insert into  `log` (`id`,`CLIENT`,`ANSWER`, `DATE`)values( '".$RFID."','".@$_SERVER['REMOTE_ADDR']."','".$valid."',sysdate())";
				//echo $sql;
				mysqli_query($link, $sql);
				break;
			 }
			} else {
				$sql="insert into  rfidtags (ID)values( '".$RFID."')";
				//echo $sql;
				mysqli_query($link, $sql);
				$sql="insert into  `log` (`id`,`CLIENT`,`ANSWER`, `DATE`)values( '".$RFID."','".@$_SERVER['REMOTE_ADDR']."','0',sysdate())";
		  	    //echo $sql;
				mysqli_query($link, $sql);
	
			}
			mysqli_close($link);
	    }
    }
echo json_encode($aResult);
?>