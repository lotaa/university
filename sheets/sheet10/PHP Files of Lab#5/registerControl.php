<?php
//require "pdo.php";
require_once "../myPadoFiles/pdo.php";
if(isset($_POST['stName'])&& isset($_POST['stEmail'])){
    $sql = "INSERT INTO students (name, id, email,gpa,deptId) VALUES (:name, :id,:email,:gpa,:deptId)";
    echo("<pre>\n".$sql."\n</pre>\n");
    $stmt = $pdo->prepare($sql);
    $AnAssociativeArray = array(
        ':name' => $_POST['stName'],
        ':email' => $_POST['stEmail'],
        ':gpa' => $_POST['stGpa'],
        ':id' => $_POST['stId'],
        ':deptId' => $_POST['stDept']
            );
    $stmt->execute($AnAssociativeArray);

}
?>

<a href="http://localhost//formRgister.php"> back to form register</a>
<a href="http://localhost//displayData.php"> display data</a>

