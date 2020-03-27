<?php
require_once "../myPadoFiles//pdo.php";
$sqlQueryString="select * from students";
$Data =  $pdo->query($sqlQueryString);
?>
<table border='1'>
<?php  

$i=1;
    
    while ( $row = $Data->fetch(PDO::FETCH_ASSOC) ) {
       
       if($i==1){ //print table header
            echo "<tr bgcolor='#f4f4f4'>";
              foreach($row as $key=>$value)
                   echo "<td>".$key."</td>";
            echo "</tr>";

        }
            
        if($i%2==1)
            echo "<tr bgcolor='yellow'> ";
        else 
            echo "<tr > ";
        
        

        foreach($row as $key=>$value)
          echo "<td>". $value ."</td>"; 
        echo "</tr>";
        $i++;
    }
    
?>
</table>