<?php
//print_r($_SERVER);
//exit;

$aResult=array("name"=>'',"valid"=>'0',"klasse"=>'',"reason"=>'0');
if ((@$_REQUEST["RF"]!="")){
	
	  $RFID1 = trim($_REQUEST["RF"]);
	  $RFID = preg_replace( '/[^\pL\d\s]+/u', '' , $RFID1);
	  
	  $host_name = 'LOCALHOST';
	  $database = 'id16262414_hogau';
	  $user_name = 'id16262414_admin1';
	  $password = 'HogauTestAdmin1!';
	  
	  $link = new mysqli($host_name, $user_name, $password, $database);
      
	  if ($link->connect_error) {
		die('<p>Verbindung zum MySQL Server fehlgeschlagen: '. $link->connect_error .'</p>');
	  } else {
		     
			
		    //Funktion Einer ID die für Testzwecke den lastUse-Wert aller Karten zurücksetzt
		    if($RFID == "1234"){
				
			$sql3="UPDATE `rfidtags` SET `lastUse` = (sysdate()-(4*60*60)) WHERE `lastUse` > (sysdate()-(3*60*60))";
			echo $sql3;
            mysqli_query($link, $sql3);
            echo "LastUse wurde für alle Karten in einen gültigen Bereich geschoben";
		    exit;
			}
		    
			
			$sql = "SELECT `DAYS`, `NAME`,`Klasse`,`lastUse`,
			CASE 
				  WHEN  sysdate() between  `VALIDFROM` and  `VALIDUNTIL` THEN 1
				  ELSE 0 
			  END as `intervall` 
					FROM `rfidtags` where ID='".$RFID."'" ;
				//echo $sql;	
			$result = $link->query($sql);
			
			//Variable in der gespeichert wird Warum man nicht essen darf  
			$reason = 0;
			
			if ($result->num_rows > 0) {
			    while($row = $result->fetch_assoc()) {
				   
				//Test ob Validfrom and Validuntil wahr ist -> reason = 1
				if($row["intervall"] == 0){
				$reason = 1;
				}
				
				$valid=$row["intervall"];
				$weekdays = explode(',', $row["DAYS"]);
				
				if (!in_array(date("N"),$weekdays)&& $valid==1){
					$valid=0;
				//Nicht für den jeweiligen Wochentag zugelassen	
					$reason=2;
				}
				
				//Sperre das die Karte nicht doppelt verwendet werden kann mit z.B. 3h Puffer
				$lastUse3h = strtotime($row["lastUse"]) + (3 * 60 * 60);
				if($lastUse3h > time() && $valid == 1 ){
				    $valid=0;
					$reason=3;					
				}
				
				//Falls nicht der richtige Wochentag soll ausgegeben werden wann er essen darf
				if($reason !== 2){	
				  $aResult=array("name"=>$row["NAME"],"valid"=>$valid,"klasse"=>$row["Klasse"],"reason"=>$reason);
				}else{
				  $aResult=array("name"=>$row["NAME"],"valid"=>$valid,"klasse"=>$row["Klasse"],"reason"=>$reason,"weekdaysValid"=>$row["DAYS"]);
				}
				
				//lastUse wird geupdatet nach einer neuen gültigen Benutzung
				if($valid == 1){
				$sql="UPDATE `rfidtags` SET `lastUse` = sysdate() WHERE `rfidtags`.`ID` ='".$RFID."'";
				mysqli_query($link, $sql);
				}
				
				$sql="insert into  `log` (`id`,`CLIENT`,`ANSWER`, `DATE`)values( '".$RFID."','".@$_SERVER['REMOTE_ADDR']."','".$valid."',sysdate())";
				//echo $sql;
				mysqli_query($link, $sql);
				break;
			    }
			} else {
				//Wenn es diese ID nicht gibt > Rückmeldung
				$reason=4;
				$aResult=array("name"=>null,"valid"=>0,"klasse"=>null,"reason"=>$reason);
				
				//$sql="insert into  rfidtags (ID)values( '".$RFID."')";
				//echo $sql;
				//mysqli_query($link, $sql);
				
				$sql="insert into  `log` (`id`,`CLIENT`,`ANSWER`, `DATE`)values( '".$RFID."','".@$_SERVER['REMOTE_ADDR']."','0',sysdate())";
		  	    //echo $sql;
				mysqli_query($link, $sql);
	
			}
			mysqli_close($link);
	    }
    }
echo json_encode($aResult);
?>